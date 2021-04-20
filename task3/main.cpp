#include <stdio.h>
#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include "objects.hpp"
#include "functions.inl"

#define width   768
#define height  768
#define Vw      1
#define Vh      1
#define C2Vd    1



inline float ComputeLighting(const sf::Vector3f& point, const sf::Vector3f& normal, const sf::Vector3f& direction, Material prop, const LightManager& lights)
{
    float intensity = 0;

    for(size_t i = 0; i < lights.size(); i ++)
        if (lights[i].m_type == Light::Type::AMBIENT)
            intensity += lights[i].m_intensity;
        else
        {   
            sf::Vector3f L = {};

            if (lights[i].m_type == Light::Type::POINT)
                L = lights[i].m_position - point;
            else
                L = -lights[i].m_direction;

            //diffuse
            float cos_alpha = dot(L, normal) / (length(L) * length(normal));
            if(cos_alpha > 0)
                intensity += lights[i].m_intensity * cos_alpha;  

            //specular
            if( prop.specular != -1)
            {
                sf::Vector3f R = 2.f * normal * dot(normal, L) - L;
                float cos_beta = dot(normalize(R), normalize(direction));
                if(cos_beta > 0)
                    intensity += lights[i].m_intensity * pow(cos_beta , static_cast<float>(prop.specular));
            }
        }
    return intensity;
}




inline void setPixel(sf::Uint8* framebuffer, unsigned int x, unsigned int y, Color pixel)
{
    framebuffer[(y * width + x) * 4]     = (unsigned char) (255 * pixel.r);
    framebuffer[(y * width + x) * 4 + 1] = (unsigned char) (255 * pixel.g);
    framebuffer[(y * width + x) * 4 + 2] = (unsigned char) (255 * pixel.b);
    framebuffer[(y * width + x) * 4 + 3] = (unsigned char) (255);
}

inline sf::Vector3f CanvasToViewPort(unsigned int x, unsigned int y)
{
    return sf::Vector3f (((float)x * (float)(Vw) / width - 0.5f), (-(float)y * (float)(Vh) / height + 0.5f), C2Vd);
}

inline Color ray_cast( const sf::Vector3f& origin, const sf::Vector3f& direction, const ObjectManager& objects, const LightManager& lights)
{
    float min_dist = _INFINITY;
    const Drawable* closest_obj = nullptr;


    for(size_t i = 0; i < objects.size(); i++)
    {
        sf::Vector3f solutions = objects[i].ray_intersect(origin, direction);
        
        if(solutions.x < _EPS)
            continue;
        float t1 = solutions.y;
        float t2 = solutions.z;

        if(t1 < min_dist && t1 > 1)
        {
            min_dist = t1;
            closest_obj = &objects[i];
        }

        if(t2 < min_dist && t2 > 1)
        {
            min_dist = t2;
            closest_obj = &objects[i];
        }
    }

    if(!closest_obj)
        return sf::Vector3f(0.2f, 0.7f, 0.8f);

    sf::Vector3f point = origin + min_dist * direction;
    sf::Vector3f normal = point - closest_obj->getPosition();

    normal = normalize(normal);
    
    return closest_obj->getColor() * ComputeLighting(point, normal, -direction, closest_obj->m_properties, lights);
}



inline sf::Uint8* renderer(const ObjectManager& objects, const LightManager& lights)
{
    // const double fov = M_PI / 6;

    static sf::Uint8 framebuffer [width * height * 4];
    
    for (size_t line = 0; line < height; line++)
        for(size_t column = 0; column < width; column++)
        {
            #if 0
            if(line % 2 == 0 && column % 2 == 0)
            {
                sf::Vector3f dir  = CanvasToViewPort(column, line);
                //fprintf(stderr, "direction is %lf %lf %lf\n", dir.x, dir.y, dir.z);
                sf::Vector3f res = ray_cast(sf::Vector3f(0,0,0), dir, objects, lights); 
                setPixel(framebuffer, column, line, res);
                setPixel(framebuffer, column + 1, line, res);
                setPixel(framebuffer, column, line + 1, res);
                setPixel(framebuffer, column + 1, line + 1, res);
            }
            #endif

            sf::Vector3f dir  = CanvasToViewPort(column, line);
            //fprintf(stderr, "direction is %lf %lf %lf\n", dir.x, dir.y, dir.z);
            sf::Vector3f res = ray_cast(sf::Vector3f(0,0,0), dir, objects, lights);
            
            setPixel(framebuffer, column, line, res);
        }
    
    for (size_t line = 0; line < height; line++)
        setPixel(framebuffer, width / 2, line, sf::Vector3f(0, 0,0));
    for (size_t column = 0; column < width; column++)
        setPixel(framebuffer, column, height / 2, sf::Vector3f(0, 0,0));
    return framebuffer;
}





int main()
{
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!");
    window.setFramerateLimit(60);

    sf::Texture texture;
    sf::Sprite sprite;
    
    texture.create(width, height);

    ObjectManager& objects = ObjectManager::createManager();
    LightManager lights = {};

    
    //objects.add(new Sphere {sf::Vector3f(0, 0, 15), 5, sf::Color::Red} );
    objects.add(new Sphere {sf::Vector3f(0, -1, 3), 1, {500}, sf::Color::Red} );
    objects.add(new Sphere {sf::Vector3f(2,  0, 4), 1, {500}, sf::Color::Blue} );
    objects.add(new Sphere {sf::Vector3f(-2, 0, 4), 1, {10}, sf::Color::Green} );
    objects.add(new Sphere {sf::Vector3f(0, -5001, 0), 5000, {1000}, sf::Color::Yellow} );   

    lights.add(new Light {Light::Type::AMBIENT, 0.2f});
    lights.add(new Light {Light::Type::POINT, 0.6f, sf::Vector3f(2, 1, 0)});
    lights.add(new Light {Light::Type::DIRECTIONAL, 0.2f, sf::Vector3f(1, 4, 4)});


    sf::Uint8* frame = renderer(objects, lights);
    texture.update(frame);
    sprite.setTexture(texture, true);



    #if 0
    fprintf(stderr, "openning file\n");
    FILE* fp = fopen("test.ppm", "wb");

    //fprintf(fp, "HELLO\n");

    if(!fp)
        fprintf(stderr, "failed to open file\n");

    fprintf(fp, "P6\n%d %d\n255\n", width, height);

    for (size_t line = 0; line < height; line++)
    {   
        for(size_t column = 0; column < width; column++)
            {
                //fprintf(stderr, "writting in file string --> (%d)(%d)(%d)\n", (unsigned char) (255 * frame[line * width + column].x),\
                //(unsigned char) (255 * frame[line * width + column].y), (unsigned char) (255 * frame[line * width + column].z));
                fprintf(fp, "%c%c%c", (unsigned char) (255 * frame[line * width + column].x),\
                (unsigned char) (255 * frame[line * width + column].y), (unsigned char) (255 * frame[line * width + column].z));
            }
            //fprintf(fp, "\n");
    }
    fclose(fp);
    #endif

    sf::Clock Clock;

    while (window.isOpen())
    {
        float Framerate = 1.f / Clock.getElapsedTime().asSeconds();
        Clock.restart();

        fprintf(stderr, "Framrate: %lf\r", Framerate);


        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        
    
        
        
        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}