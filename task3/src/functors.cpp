// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "button.hpp"
#include <algorithm>
#include <assert.h>
#include <SFML/System/Mutex.hpp>
#include <ctime>
#include <string.h>

#include "objects.hpp"
#include "button.hpp"

inline Drawable* generateObject(Drawable::Type type)
{   
    switch(type)
    {
        default:
        case Drawable::Type::PLANE:
        {
            return Plane::generate();
        }        
        case Drawable::Type::SPHERE:
        {
            return Sphere::generate();
        }
    }
}

extern sf::Mutex rendering_mutex;

MenuSwitcherFunctor::MenuSwitcherFunctor(MenuHandler* menu_handler, int* pages, unsigned int num)
:
m_num_of_pages(num),
m_pages(new int [num]),
m_menu(menu_handler)
{
    std::copy(pages, pages + num, m_pages);
}

void MenuSwitcherFunctor::operator()(void* const arg)
{
    fprintf(stderr, "switching menu\n");
    for(size_t i = 0; i < m_menu->getSize(); i ++)
        m_menu->setVisible(i, false);
    fprintf(stderr, "all set invisible menu\n");
    for(size_t i = 0; i < m_num_of_pages; i ++)
    {
        fprintf(stderr, "setting %d group_menu visible\n", m_pages[i]);
        m_menu->setVisible(m_pages[i], true);
    }
}


void AddObjectFunctor::operator()(void* const arg)
{
    fprintf(stderr, "adding object %s!\n", (+m_type).c_str());
    
    if((m_objects == nullptr))
        return;

    rendering_mutex.lock();
    
    Drawable* object = generateObject(m_type);
    m_objects->add(object);
    
    rendering_mutex.unlock();

}

void DeleteObjectFunctor::operator()(void* const arg)
{
    if((m_objects == nullptr))
        return;
 
    for(int i = m_objects->size() - 1; i >= 0; i--)
    {
        fprintf(stderr, "Object %s number %d\n", (+m_type).c_str(), i);
 
        if((*m_objects)[i].getType() == m_type)
        {
            fprintf(stderr, "found! number %d\n", i);
            m_objects->remove(i);
            return;
        }
    }
    fprintf(stderr, "not found!\n");
    return;
}