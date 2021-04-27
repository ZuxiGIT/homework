#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "ray.hpp"
#include "raytrace.hpp"
#include "canvas.hpp"
#include "camera.hpp"
#include "objects.hpp"
#include "functions.inl"
#include <GLM/glm.hpp>



#if 0
#define _GET_RETURN_INTERSECT(_min_dist, _obj_pointer, _function_call)  \
                                                                        \
sf::Vector2f _returned_vector = _function_call;                         \
_min_dist = _returned_vector.x;                                         \
union { float fl; const Drawable* ptr; } _temp;                         \
_temp.fl = _returned_vector.y;                                          \
_obj_pointer = _temp.ptr;


#define width   1080
#define height  1080
#define Vw      1
#define Vh      1
#define C2Vd    1

inline sf::Vector3f reflectRay( const sf::Vector3f& L, const sf::Vector3f& normal)
{
    return 2.f * normal * dot(normal, L) - L;
}
inline sf::Vector2f closestIntersection(const sf::Vector3f& origin, const sf::Vector3f& direction, float t_min, float t_max, const ObjectManager& objects)
{
    float min_dist = _INFINITY;
    
    union{ float fl; const Drawable* ptr; }  closest_obj;
    
    closest_obj.ptr = nullptr;

    for(size_t i = 0; i < objects.size(); i++)
    {
        sf::Vector3f solutions = objects[i].ray_intersect(origin, direction);
        
        if(solutions.x < _EPS)
            continue;

        float t1 = solutions.y;
        float t2 = solutions.z;

        if(t1 < min_dist && t1 > t_min && t1 < t_max)
        {
            min_dist = t1;
            closest_obj.ptr = &objects[i];
        }

        if(t2 < min_dist && t2 > t_min && t2 < t_max)
        {
            min_dist = t2;
            closest_obj.ptr = &objects[i];
        }
    }

    return sf::Vector2f(min_dist, closest_obj.fl);
}


inline float ComputeLighting(const sf::Vector3f& point, const sf::Vector3f& normal, const sf::Vector3f& direction, Material prop, const ObjectManager& objects,  const LightManager& lights)
{
    float intensity = 0;

    for(size_t i = 0; i < lights.size(); i ++)
        if (lights[i].m_type == Light::Type::AMBIENT)
            intensity += lights[i].m_intensity;
        else
        {   
            sf::Vector3f L = {};
            float t_max = {};
            if (lights[i].m_type == Light::Type::POINT)
            {
                L = lights[i].m_position - point;
                t_max = 1;
            }
            else
            {
                L = -lights[i].m_direction;
                t_max = _INFINITY;
            }


            //check for shadows
            const Drawable* shadow_obj = nullptr;
            float shadow_obj_dist = {};
            
            _GET_RETURN_INTERSECT(shadow_obj_dist, shadow_obj, closestIntersection(point, L, 0.001f, t_max, objects));

            if(shadow_obj != nullptr)
                continue;

            //diffuse
            float cos_alpha = dot(L, normal) / (length(L) * length(normal));
            //fprintf(stderr, "cos-alpha is %f\n", cos_alpha);
            
            if(cos_alpha > 0)
                intensity += lights[i].m_intensity * cos_alpha;  

            //specular
            if( prop.specular != -1)
            {
                sf::Vector3f R = reflectRay(L, normal);
                
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
    return sf::Vector3f (((float)x /** (float)(Vw)*/ / width - 0.5f), (-(float)y /** (float)(Vh)*/ / height + 0.5f), C2Vd);
}

inline Color ray_cast(const sf::Vector3f& origin, const sf::Vector3f& direction, float t_min, float t_max, const ObjectManager& objects, const LightManager& lights, int depth)
{
    float min_dist = {};
    const Drawable* closest_obj = nullptr;

    _GET_RETURN_INTERSECT(min_dist, closest_obj, closestIntersection(origin, direction, t_min, t_max, objects));

    if(closest_obj == nullptr)
        return sf::Vector3f(0.2f, 0.7f, 0.8f);

    sf::Vector3f point = origin + min_dist * direction; // point intersection
    sf::Vector3f normal = closest_obj->getNormal(point); // normal at point intersection

    normal = normalize(normal);
    //local color 
    
    float intens = ComputeLighting(point, normal, -direction, closest_obj->m_properties, objects, lights) ;
    Ray __temp(point, -direction);
    float intens2 = Raytrace::ComputeLighting(__temp, normal, closest_obj->m_properties, objects, lights);

#if 0
    if(intens != intens2)
    {
        fprintf(stderr, "ComputeLightning: Not equal (%.8f) != (%.8f)\n", intens,intens2);
        exit(1);
    }
#endif

    Color local_color =  closest_obj->getColor() * intens;
    
    //reflection
    float r = closest_obj->m_properties.reflective;
    
    if( depth <= 0 || r <= 0)
        return local_color;

    sf::Vector3f R = reflectRay(-direction, normal);
    Color reflected_color = ray_cast(point, R, 0.001f, _INFINITY, objects, lights, depth - 1);

    return local_color*(1-r) + reflected_color * r;
    
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
            sf::Vector3f a = ray_cast(sf::Vector3f(0,0,0), dir, 1, _INFINITY, objects, lights, 10);
            Ray ray (sf::Vector3f(0,0,0), dir);
            //sf::Vector3f b = Raytrace::ray_cast(ray, 1, _INFINITY, objects, lights);

#if 0
            if( a != b)
            {
                fprintf(stderr, "ray_cast: Not equal (%.8f, %.8f, %.8f) != (%.8f, %.8f, %.8f)\n", a.x, a.y, a.z, b.x, b.y, b.z);
                exit(1);
            }
#endif
            
            setPixel(framebuffer, column, line, a);
        }
    
    for (size_t line = 0; line < height; line++)
        setPixel(framebuffer, width / 2, line, sf::Vector3f(0, 0,0));
    for (size_t column = 0; column < width; column++)
        setPixel(framebuffer, column, height / 2, sf::Vector3f(0, 0,0));
    return framebuffer;
}

#endif



int main()
{
    sf::RenderWindow window(sf::VideoMode(1080, 1080), "SFML works!");
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);
    bool mouse_hidden = true;

    Canvas canvas {1080, 1080};
    Camera camera(sf::Vector3f(0.f, 0.f, 0.f), sf::Vector3f(0.f, 0.f,1.f));
    canvas.setCamera(camera);
    
#if 0
    sf::Vector3f a = camera.CanvasToViewPort(738, 738); 
    sf::Vector3f b = CanvasToViewPort(738, 738);
    
    if( a != b)
    {
        fprintf(stderr, "Not equal (%f, %f, %f) != (%f, %f, %f)\n", a.x, a.y, a.z, b.x, b.y, b.z);
        exit(1);
    }
#endif

    fprintf(stderr, "sizeof(float) = %u bytes\nsizeof(double) = %u bytes\n", sizeof(float), sizeof(double));

    sf::ContextSettings settings = window.getSettings();

    std::cout << "depth bits:" << settings.depthBits << std::endl;
    std::cout << "stencil bits:" << settings.stencilBits << std::endl;
    std::cout << "antialiasing level:" << settings.antialiasingLevel << std::endl;
    std::cout << "version:" << settings.majorVersion << "." << settings.minorVersion << std::endl;

    ObjectManager& objects = ObjectManager::createManager();
    LightManager lights = {};

    
    //objects.add(new Sphere {sf::Vector3f(0, 0, 15), 5, sf::Color::Red} );
    objects.add(new Sphere {sf::Vector3f(0, 0, 4), 1, {500, 0.2f}, sf::Color::Red} );
    objects.add(new Sphere {sf::Vector3f(2,  0, 4), 1, {500, 0.6f}, sf::Color::Blue} );
    objects.add(new Sphere {sf::Vector3f(-2, 0, 4), 1, {10, 0.1f}, sf::Color::Green} );
    objects.add(new Plane  {sf::Vector3f(0, -2, 0), sf::Vector3f(0, 1, 0), {1, 0}, sf::Color::Yellow} );
    // objects.add(new Sphere {sf::Vector3f(0, -5001, 0), 5000, {1000}, sf::Color::Yellow} );   

    lights.add(new Light {Light::Type::AMBIENT, 0.2f});
    lights.add(new Light {Light::Type::POINT, 0.6f, sf::Vector3f(2, 10, 0)});
    lights.add(new Light {Light::Type::POINT, 0.6f, sf::Vector3f(-2, 0, 0)});
    lights.add(new Light {Light::Type::DIRECTIONAL, 0.6f, sf::Vector3f(1, -4, -4)});


    canvas.setObjects(objects);
    canvas.setLights(lights);




    #if 0
    sf::Texture texture;
    sf::Sprite sprite;
    
    texture.create(1080, 1080);



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
            if (event.type == sf::Event::Closed || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape) \
            && !mouse_hidden))
                window.close();
            else if (event.type == sf::Event::MouseMoved)
            {
                if(mouse_hidden)
                {
                    float mx = event.mouseMove.x / 1080.f - 0.5f;
                    float my = event.mouseMove.y / 1080.f - 0.5f;
                    fprintf(stderr, "mouse moved (%f, %f)\n", mx, my);
                    camera.rotate(mx, my);
                    sf::Mouse::setPosition(sf::Vector2i(540, 540), window);
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                window.setMouseCursorVisible(false);
                mouse_hidden = true;
            }
            else if (event.type == sf::Event::KeyPressed)
                if(event.key.code == sf::Keyboard::Escape && mouse_hidden)
                {
                    window.setMouseCursorVisible(true);
                    mouse_hidden = false;
                }

        }

        // sf::Uint8* frame = renderer(objects, lights);
        // texture.update(frame);
        // sprite.setTexture(texture, true);
        
        window.clear();
        canvas.draw(window);
        //window.draw(sprite);
        window.display();
    }

    return 0;
}