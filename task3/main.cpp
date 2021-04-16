#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "objects.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

#define width   1024
#define height  1024
#define Vw      1
#define Vh      1
#define C2Vd    1


inline void setPixel(sf::Uint8* framebuffer, unsigned int x, unsigned int y, sf::Vector3f pixel)
{
    framebuffer[(y * width + x) * 4]     = (unsigned char) (255 * pixel.x);
    framebuffer[(y * width + x) * 4 + 1] = (unsigned char) (255 * pixel.y);
    framebuffer[(y * width + x) * 4 + 2] = (unsigned char) (255 * pixel.z);
    framebuffer[(y * width + x) * 4 + 3] = (unsigned char) (255);
}

inline sf::Vector3f CanvasToViewPort(unsigned int x, unsigned int y)
{
    return sf::Vector3f (((float)x * (float)(Vw) / width - 0.5f), ((float)y * (float)(Vh) / height - 0.5f), C2Vd);
}

inline sf::Vector3f ray_cast( const sf::Vector3f& origin, const sf::Vector3f& direction, const Sphere& sph)
{
    //float sphere_dist = 9999999;
    if(!sph.ray_intersect(origin, direction))
        return sf::Vector3f(0.2f, 0.7f, 0.8f);
    return sf::Vector3f(0.4f, 0.4f, 0.3f);
}



inline sf::Uint8* renderer()
{
    // /const double fov = M_PI / 6;

    static sf::Uint8 framebuffer [width * height * 4];
    
    for (size_t line = 0; line < height; line++)
        for(size_t column = 0; column < width; column++)
        {
            sf::Vector3f dir  = CanvasToViewPort(column, line);
            //fprintf(stderr, "direction is %lf %lf %lf\n", dir.x, dir.y, dir.z);
            sf::Vector3f res = ray_cast(sf::Vector3f(0,0,0), dir, Sphere(sf::Vector3f(0, 0, 40), 5));
            setPixel(framebuffer, column, line, res);
        }
    
    return framebuffer;
}





int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 768), "SFML works!");
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

    // /ofs << "P6\n" << width << " " << height << "\n255\n";
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