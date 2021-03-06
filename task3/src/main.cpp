// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <stdio.h>
#include <iostream>
#include <memory>
#include <utility>
#define _USE_MATH_DEFINES
#include <math.h>

#include <SFML/Graphics.hpp>
#include <SFML/System/Thread.hpp>

#include "ray.hpp"
#include "raytrace.hpp"
#include "canvas.hpp"
#include "camera.hpp"
#include "objects.hpp"
#include "button.hpp"
#include "functions.inl"
#include "functors.hpp"


// TODO 
// remove setPosition / getPosition and so on methods and add sf::Transformable inheritance

sf::Mutex rendering_mutex;
bool SHOULD_CLOSE = false;


class renderingThread
{
    sf::RenderWindow& window;
    Canvas& canvas;
    MenuHandler& menu;

public:
    renderingThread(sf::RenderWindow* win, Canvas* can, MenuHandler* mn)
    :
    window(*win),
    canvas(*can),
    menu(*mn)
    {}

    void operator() ()
    {

        fprintf(stderr, "RENDERING THREAD:: Started\n");

        window.setActive(true);

        // sf::Clock Clock;

        while (!SHOULD_CLOSE)
        {
            // float Framerate = 1.f / Clock.getElapsedTime().asSeconds();
            // Clock.restart();

            // fprintf(stderr, "Framrate: %lf\r", Framerate);
            // rendering_mutex.lock();
            // fprintf(stderr, "RENDERING THREAD::Visible is %d\n", menu.getVisible());
            // rendering_mutex.unlock();

            window.clear(Color(0.2f, 0.3f, 0.3f));
            canvas.draw(window);
            menu.render();
            window.display();
        }

        window.setActive(false);
    }

};

Drawable* generateObject(Drawable::Type);


int main()
{

    sf::ContextSettings settings;

    settings.depthBits = 0;
    settings.antialiasingLevel = 8;
    settings.stencilBits = 0;
    settings.majorVersion = 4;
    settings.minorVersion = 6;
    
    sf::RenderWindow window(sf::VideoMode(1080, 1080), "SFML works!", sf::Style::Default, settings);
    
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(true);

    bool mouse_hidden = false;

    Canvas canvas {800, 800, 0, 0};

    Camera camera(sf::Vector3f(0.f, 0.f, 0.f), sf::Vector3f(0.f, 0.f, 1.f));
    
    canvas.setCamera(camera);

    ObjectManager& objects = ObjectManager::createManager();
    LightManager lights = {};

    
    //objects.add(new Sphere {sf::Vector3f(0, 0, 15), 5, sf::Color::Red} );
    objects.add(new Sphere {sf::Vector3f(0, 0, 4), 1, {500, 0.2f}, sf::Color::Red} );
    objects.add(new Sphere {sf::Vector3f(2,  0, 4), 1, {500, 0.6f}, sf::Color::Blue} );
    objects.add(new Sphere {sf::Vector3f(-2, 0, 4), 1, {10, 0.1f}, sf::Color::Green} );
    objects.add(new Plane  {sf::Vector3f(0, -2, 0), sf::Vector3f(0, 1, 0), {1, 0}, sf::Color::Yellow} );
    objects.remove(2);
    objects.add(generateObject(Drawable::Type::PLANE));
    
    // objects.add(new Sphere {sf::Vector3f(0, -5001, 0), 5000, {1000}, sf::Color::Yellow} );   

    // lights.add(new Light {Light::Type::AMBIENT, 0.2f});
    lights.add(new Light {Light::Type::POINT, 0.6f, sf::Vector3f(0, 0, -10)});
    lights.add(new Light {Light::Type::POINT, 0.6f, sf::Vector3f(-2, 0, 0)});
    // lights.add(new Light {Light::Type::DIRECTIONAL, 0.6f, sf::Vector3f(1, -4, -4)});
    // lights.add(new Light {Light::Type::DIRECTIONAL, 1.f,  sf::Vector3f(1, 4, 4)});


    
    MenuHandler menu {};
    ButtonManager firstPage {};
    ButtonManager secondPage {};

    menu.add(new GroupOfButtons{firstPage});
    menu.add(new GroupOfButtons{secondPage});


    AddObjectFunctor add_sphere {&objects, Drawable::Type::SPHERE};
    AddObjectFunctor add_plane {&objects, Drawable::Type::PLANE};

    DeleteObjectFunctor delete_sphere {&objects, Drawable::Type::SPHERE};
    DeleteObjectFunctor delete_plane {&objects, Drawable::Type::PLANE};


    TestFunctor test {&window, &objects, &lights};

    SetValueFunctor<float> cam_dir_x {&camera.m_direction.x};
    SetValueFunctor<float> cam_dir_y {&camera.m_direction.y};
    SetValueFunctor<float> cam_dir_z {&camera.m_direction.z};
    SetValueFunctor<float> cam_left_x {&camera.m_camera_left.x};
    SetValueFunctor<float> cam_left_y {&camera.m_camera_left.y};
    SetValueFunctor<float> cam_left_z {&camera.m_camera_left.z};
    SetValueFunctor<float> cam_up_x {&camera.m_camera_up.x};
    SetValueFunctor<float> cam_up_y {&camera.m_camera_up.y};
    SetValueFunctor<float> cam_up_z {&camera.m_camera_up.z};

    int m1 = 0;
    int m2 = 1;
    MenuSwitcherFunctor menu1 (&menu, &m1, 1);
    MenuSwitcherFunctor menu2 (&menu, &m2, 1);

    MenuButton::loadFont("TrueTypeFonts/UbuntuMono-R.ttf");
    menu[1].m_visible = true;

    firstPage.add(new MenuRectangleButton {&window, Vector2f{200, 800},     Vector2f{200, 300},     "add plane",        &add_plane,     HSL2RGB(HSL{164, 100, 50}),  RGB(0,0,0)});
    firstPage.add(new MenuRectangleButton {&window, Vector2f{600, 800},     Vector2f{200, 300},     "delete plane",     &delete_plane,  HSL2RGB(HSL{164, 100, 50}),  RGB(0,0,0)});
    firstPage.add(new MenuEllipseButton   {&window, Vector2f{0, 20},        Vector2f{200, 20},      "delete sphere",    &delete_sphere, HSL2RGB(HSL{164, 100, 50}),  RGB(0,0,0)});
    firstPage.add(new MenuRectangleButton {&window, Vector2f(400, 800),     Vector2f(200, 300),     "add sphere",       &add_sphere,    HSL2RGB(HSL{164 , 100, 50}), RGB(0,0,0)});
    firstPage.add(new MenuRectangleButton {&window, Vector2f(900, 0),       Vector2f(100, 100),     "secondPage",       &menu2,         HSL2RGB(HSL{164 , 100, 50}), RGB(0,0,0)});


    secondPage.add(new MenuTextInputButton {&window, Vector2f(900, 700), Vector2f(200,100),  "cam.dir.x",   &cam_dir_x,     RGB(255, 0, 255) });
    secondPage.add(new MenuTextInputButton {&window, Vector2f(700, 700), Vector2f(200,100),  "cam.dir.y",   &cam_dir_y,     RGB(255, 0, 255) });
    secondPage.add(new MenuTextInputButton {&window, Vector2f(500, 700), Vector2f(200,100),  "cam.dir.z",   &cam_dir_z,     RGB(255, 0, 255) });
    secondPage.add(new MenuTextInputButton {&window, Vector2f(900, 800), Vector2f(200,100),  "cam.left.x",  &cam_left_x,    RGB(255, 0, 255) });
    secondPage.add(new MenuTextInputButton {&window, Vector2f(700, 800), Vector2f(200,100),  "cam.left.y",  &cam_left_y,    RGB(255, 0, 255) });
    secondPage.add(new MenuTextInputButton {&window, Vector2f(500, 800), Vector2f(200,100),  "cam.left.z",  &cam_left_z,    RGB(255, 0, 255) });
    secondPage.add(new MenuTextInputButton {&window, Vector2f(900, 900), Vector2f(200,100),  "cam.up.x",    &cam_up_x,      RGB(255, 0, 255) });
    secondPage.add(new MenuTextInputButton {&window, Vector2f(700, 900), Vector2f(200,100),  "cam.up.y",    &cam_up_y,      RGB(255, 0, 255) });
    secondPage.add(new MenuTextInputButton {&window, Vector2f(500, 900), Vector2f(200,100),  "cam.up.z",    &cam_up_z,      RGB(255, 0, 255) });

    secondPage.add(new MenuRectangleButton {&window, Vector2f(900, 0),   Vector2f(100, 100), "firstPage", &menu1,  HSL2RGB(HSL{164 , 100, 50}), RGB(0,0,0)});


    
    canvas.setObjects(objects);
    canvas.setLights(lights);
    canvas.m_thread.launch();

    sf::Event event;

    // renderingThread rendering_thread {&window, &canvas, &menu};
    // window.setActive(false); 
    
    // sf::Thread th {rendering_thread};
    // th.launch();

    fprintf(stderr, "MAIN THREAD:: main thread started\n");

    while(window.isOpen())
    {

        while (window.pollEvent(event))
        {
            // rendering_mutex.lock();
            // fprintf(stderr, "MAIN THREAD::Visible is %d\n", menu.getVisible());
            // rendering_mutex.unlock();
            
            if (mouse_hidden && (event.type == sf::Event::MouseMoved))
            {
                double mx = static_cast<double>(static_cast<float>(event.mouseMove.x) - 400.f) / 800.f * M_PI;
                double my = static_cast<double>(static_cast<float>(event.mouseMove.y) - 400.f) / 800.f * M_PI;

                // fprintf(stderr, "---mouse moved (%d, %d)\n", event.mouseMove.x, event.mouseMove.y);
                // fprintf(stderr, "mouse moved (%f, %f)\n", mx, my);
                fprintf(stderr, "x_angle is %lf deg\n", mx * 180 / M_PI);
                fprintf(stderr, "y_angle is %lf deg\n", my * 180 / M_PI);


                camera.rotate(mx, my);
                sf::Mouse::setPosition(sf::Vector2i(400, 400), window);
                
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

            if(menu.update(event))
                continue;

            if((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Button::Left))
                if(canvas.isInCanvas(mouse_pos) && !mouse_hidden)
                    {
                        //window.setMouseCursorVisible(false);
                        mouse_hidden = true;
                        continue;
                    }

            if ((event.type == sf::Event::Closed) || 
                (   (event.type == sf::Event::KeyPressed) && 
                    (event.key.code == sf::Keyboard::Escape) && 
                        !mouse_hidden))
            {
                // fprintf(stderr, "before closing\n");
                
                // rendering_mutex.lock();
                // SHOULD_CLOSE = true;
                // rendering_mutex.unlock();
                
                // th.wait();
                // canvas.m_thread.wait();

                // window.setActive(true);
                window.close();
                
                // fprintf(stderr, "after closing\n");

                continue;
            }
        }
        
        window.clear(Color(0.2f, 0.3f, 0.3f));
        canvas.draw(window);
        menu.render();
        window.display();
    }

    return 0;
}