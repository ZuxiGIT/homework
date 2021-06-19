#pragma once
#include "objects.hpp"

//???? why we need to write "AbstractFunctor() = default;" otherwise compile error
//???? why also need to delete again  EnvFunctor(const EnvFunctor&) and EnvFunctor& operator=(const EnvFunctor&)

class MenuHandler;


class AbstractFunctor
{
public:

	AbstractFunctor() = default;

	AbstractFunctor(const AbstractFunctor&) = delete;
	AbstractFunctor& operator=(const AbstractFunctor&) = delete;
    
	virtual ~AbstractFunctor() {}
	virtual void operator() (void* const arg = NULL) = 0;
};

class EnvFunctor : public AbstractFunctor 
{
protected:
	sf::RenderTarget* m_target  = NULL;
	ObjectManager* m_objects    = NULL;
	LightManager* m_lights      = NULL;

public:
	EnvFunctor(sf::RenderTarget* target, ObjectManager* objects, LightManager* lights)
	: 
	m_target(target),
	m_objects(objects),
	m_lights(lights)
	{}

	EnvFunctor(const EnvFunctor&) = delete;
	EnvFunctor& operator=(const EnvFunctor&) = delete;

	virtual ~EnvFunctor() override {}
};

class TestFunctor : public EnvFunctor
{
public:
	TestFunctor(sf::RenderTarget* target, ObjectManager* objects, LightManager* lights)
	:
	EnvFunctor(target, objects, lights)
	{}

	virtual ~TestFunctor() override {}

	virtual void operator() (void* const arg = NULL) override { fprintf(stderr, "test TESTFUNCTOR\nsizeof objectManager is %u\nsizeof lightsManager is %u\nwindow addres is 0x%p\n", m_objects->size(), m_lights->size(), m_target); }
};

template<typename T>
class SetValueFunctor : public AbstractFunctor
{
	T* m_value = nullptr;
public:
	SetValueFunctor(T* const val)
	:
	m_value(val)
	{}

	SetValueFunctor(const SetValueFunctor&& functor) { m_value = functor.m_value; }
	SetValueFunctor& operator= (const SetValueFunctor&& functor) { m_value = functor.m_value; }
	
	SetValueFunctor(const SetValueFunctor& functor) = delete;
	SetValueFunctor& operator=(const SetValueFunctor& functor) = delete;


	virtual void operator() (void* const arg = NULL) override 
	{
		*m_value = *(reinterpret_cast<T*>(arg));
	}

	T getParameter() { return *m_value; }

};

class MenuHandler;

class MenuSwitcherFunctor final : public AbstractFunctor
{
	unsigned int m_num_of_pages {0};
	int* m_pages = nullptr;
	MenuHandler* m_menu = nullptr;
public:
	MenuSwitcherFunctor(MenuHandler* menu_handler, int* pages, unsigned int num);

	MenuSwitcherFunctor(const MenuSwitcherFunctor&) = delete;
	MenuSwitcherFunctor& operator=(const MenuSwitcherFunctor&) = delete;

	virtual void operator() (void* const arg = NULL) override;
};



class AddSphereFunctor : public AbstractFunctor
{
	ObjectManager* m_objects = nullptr;
public:
	AddSphereFunctor(ObjectManager* obj)
	:
	m_objects(obj)
	{}

	virtual void operator() (void* const arg = NULL) override;
};