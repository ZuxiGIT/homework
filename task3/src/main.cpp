// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

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
#include "button.hpp"
#include "functions.inl"
#include "functors.hpp"
//#include <GLM/glm.hpp>
#include <SFML/OpenGL.hpp>


// TODO 
// remove setPosition / getPosition and so on methods and add sf::Transformable inheritance
// improve text scaling
// improve TextField

int main()
{
    // std::cout << "sizeof(float) " << sizeof(float)<<" sizeof(double*) " << sizeof(double*) <<std::endl;  ---> sizeof(float) 4 sizeof(double*) 4 <-- WTF????
    
    
    // RGB test1(67, 113, 4);
    // HSL test2 = RGB2HSL(test1);
    // RGB test3 = HSL2RGB(HSL {});

    // fprintf(stderr, "rgb (%u, %u, %u)\nhsl(%u, %u, %u)\nrgb (%u, %u, %u)\n", test1.r, test1.g, test1.b, test2.h, test2.s, test2.l, test3.r, test3.g, test3.b);
    sf::ContextSettings settings;

    settings.depthBits = 0;
    settings.antialiasingLevel = 8;
    settings.stencilBits = 0;
    settings.majorVersion = 4;
    settings.minorVersion = 6;
    // std::cout << "depth bits:" << settings.depthBits << std::endl;
    // std::cout << "stencil bits:" << settings.stencilBits << std::endl;
    // std::cout << "antialiasing level:" << settings.antialiasingLevel << std::endl;
    // std::cout << "version:" << settings.majorVersion << "." << settings.minorVersion << std::endl;
    
    
    sf::RenderWindow window(sf::VideoMode(1080, 1080), "SFML works!", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(true);
    bool mouse_hidden = false;

    Canvas canvas {800, 800, 10, 10};
    Camera camera(sf::Vector3f(0.f, 0.f, 0.f), sf::Vector3f(0.f, 0.f,1.f));
    canvas.setCamera(camera);

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

    TestFunctor test {&window, &objects, &lights};
    
    float test_number = 10;
    SetValueFunctor<float> test2 {&test_number};
    SetValueFunctor<float> test3 {&test_number};
    SetValueFunctor<float> test4 {&test_number};

    MenuButton::loadFont("TrueTypeFonts/UbuntuMono-R.ttf");
    ButtonManager& buttons = ButtonManager::createManager();


    buttons.add(new MenuEllipseButton   {&window, Vector2f{0, 20},    Vector2f{200, 20},  "test",         &test,   HSL2RGB(HSL{164, 100, 50}),  RGB(0,0,0)} );
    buttons.add(new MenuRectangleButton {&window, Vector2f(400, 800), Vector2f(200, 300), "test2",        &test,   HSL2RGB(HSL{164 , 100, 50}), RGB(0,0,0)});
    buttons.add(new MenuTextInputButton {&window, Vector2f(700, 700), Vector2f(200,100),  "test)number", &test2, RGB(255, 0, 255) });
    buttons.add(new MenuTextInputButton {&window, Vector2f(900, 700), Vector2f(200,100),  "test)number", &test3, RGB(255, 0, 255) });
    buttons.add(new MenuTextInputButton {&window, Vector2f(500, 700), Vector2f(200,100),  "test)number", &test4, RGB(255, 0, 255) });

    canvas.setObjects(objects);
    canvas.setLights(lights);
    
    sf::Clock Clock;

    while (window.isOpen())
    {
        float Framerate = 1.f / Clock.getElapsedTime().asSeconds();
        Clock.restart();

        fprintf(stderr, "Framrate: %lf\r", Framerate);

        sf::Event event;

        while (window.pollEvent(event))
        {
            if (mouse_hidden && (event.type == sf::Event::MouseMoved))
            {
                if(!(static_cast<float>(event.mouseMove.x) < _EPS) && !(static_cast<float>(event.mouseMove.y) < _EPS))
                {
                    float mx = static_cast<float>(event.mouseMove.x) / 1080.f - 0.5f;
                    float my = static_cast<float>(event.mouseMove.y) / 1080.f - 0.5f;
                    
                    fprintf(stderr, "---mouse moved (%d, %d)\n", event.mouseMove.x, event.mouseMove.y);
                    fprintf(stderr, "mouse moved (%f, %f)\n", mx, my);
                    
                    camera.rotate(mx, my);
                    sf::Mouse::setPosition(sf::Vector2i(540, 540), window);
                }
                continue;
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape && mouse_hidden)
                {
                    window.setMouseCursorVisible(true);
                    mouse_hidden = false;
                    continue;
                }
            }

            Vector2f mouse_pos = sf::Mouse::getPosition(window);

            fprintf(stderr,"-----mouse position(%f, %f)\n", mouse_pos.x, mouse_pos.y);
            
            if(buttons.update(event))
                continue;
            if((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Button::Left))
                if(canvas.isInCanvas(mouse_pos))
                    {
                        window.setMouseCursorVisible(false);
                        mouse_hidden = true;
                        continue;
                    }

            if ((event.type == sf::Event::Closed) || 
                (   (event.type == sf::Event::KeyPressed) && 
                    (event.key.code == sf::Keyboard::Escape) && 
                        !mouse_hidden))
            {
                window.close();
                continue;
            }
        }

        
        window.clear(Color(0.2f, 0.3f, 0.3f));
        // canvas.draw(window);
        buttons.render();
        window.display();
    } 

    return 0;
}