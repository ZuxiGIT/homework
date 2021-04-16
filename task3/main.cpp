#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "objects.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

#define width   768
#define height  768
#define Vw      1
#define Vh      1
#define C2Vd    1

inline float dot(const sf::Vector3f& lhs, const sf::Vector3f& rhs);
inline float length(const sf::Vector3f& obj);

inline float ComputeLighting(sf::Vector3f& point, sf::Vector3f& normal, const LightManager& lights)
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

            float cos_alpha = dot(L, normal) / (length(L) * length(normal));
            if(cos_alpha > 0)
                intensity += lights[i].m_intensity * cos_alpha;  
        }
    return intensity;
}




inline void setPixel(sf::Uint8* framebuffer, unsigned int x, unsigned int y, sf::Vector3f pixel)
{
    framebuffer[(y * width + x) * 4]     = (unsigned char) (255 * pixel.x);
    framebuffer[(y * width + x) * 4 + 1] = (unsigned char) (255 * pixel.y);
    framebuffer[(y * width + x) * 4 + 2] = (unsigned char) (255 * pixel.z);
    framebuffer[(y * width + x) * 4 + 3] = (unsigned char) (255);
}

inline sf::Vector3f CanvasToViewPort(unsigned int x, unsigned int y)
{
    return sf::Vector3f (((float)x * (float)(Vw) / width - 0.5f), (-(float)y * (float)(Vh) / height + 0.5f), C2Vd);
}

inline sf::Vector3f ray_cast( const sf::Vector3f& origin, const sf::Vector3f& direction, const ObjectManager& objects)
{
    float max_dist = (float)99999999;
    Sphere closest_sphere;


    for(size_t i = 0; i < objects.size(); i++)
        if(objects[i].ray_intersect(origin, direction)) 
            return objects[i].getColor();
            //return sf::Vector3f(1.f, 1.f, 0.f);
    
    return sf::Vector3f(0.2f, 0.7f, 0.8f);
}



inline sf::Uint8* renderer()
{
    // const double fov = M_PI / 6;

    ObjectManager& objects = ObjectManager::createManager();
    
    //objects.add(new Sphere {sf::Vector3f(0, 0, 15), 5, sf::Color::Red} );
    objects.add(new Sphere {sf::Vector3f(0, -1, 3), 1, sf::Color::Red} );
    objects.add(new Sphere {sf::Vector3f(2,  0, 4), 1, sf::Color::Blue} );
    objects.add(new Sphere {sf::Vector3f(-2, 0, 4), 1, sf::Color::Green} );
    objects.add(new Sphere {sf::Vector3f(0, -5001, 0), 5000, sf::Color::Yellow} );   


    static sf::Uint8 framebuffer [width * height * 4];
    
    for (size_t line = 0; line < height; line++)
        for(size_t column = 0; column < width; column++)
        {
            sf::Vector3f dir  = CanvasToViewPort(column, line);
            //fprintf(stderr, "direction is %lf %lf %lf\n", dir.x, dir.y, dir.z);
            sf::Vector3f res = ray_cast(sf::Vector3f(0,0,0), dir, objects); 
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
    sf::Uint8* frame = renderer();
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