#pragma once
#include "objects.hpp"


//???? why we need to write "AbstractFunctor() = default;" otherwise compile error
//???? why also need to delete again  EnvFunctor(const EnvFunctor&) and EnvFunctor& operator=(const EnvFunctor&)


class AbstractFunctor
{
public:

    AbstractFunctor() = default;

    AbstractFunctor(const AbstractFunctor&) = delete;
    AbstractFunctor& operator=(const AbstractFunctor&) = delete;
    
    virtual ~AbstractFunctor() {}
    virtual void operator() () = 0;
};

class EnvFunctor : public AbstractFunctor 
{
protected:
    sf::RenderTarget* m_target  = NULL;
    ObjectManager* m_objects    = NULL;
    LightManager* m_lights      = NULL;

public:
    EnvFunctor(sf::RenderTarget* target, ObjectManager* objects, LightManager* lights)
    : m_target(target), m_objects(objects), m_lights(lights) {}

    EnvFunctor(const EnvFunctor&) = delete;
    EnvFunctor& operator=(const EnvFunctor&) = delete;

    virtual ~EnvFunctor() override {}
};

class TestFunctor : public EnvFunctor
{
public:
    TestFunctor(sf::RenderTarget* target, ObjectManager* objects, LightManager* lights)
    : EnvFunctor(target, objects, lights) {}
    
    virtual ~TestFunctor() override {}
    
    virtual void operator() () override { fprintf(stderr, "test TESTFUNCTOR\nsizeof objectManager is %u\nsizeof lightsManager is %u\nwindow addres is 0x%p\n", m_objects->size(), m_lights->size(), m_target); }
};
