#pragma once

#include "objects.hpp"
#include "ignore.hpp"
// #include "button.hpp"

//???? why we need to write "AbstractFunctor() = default;" otherwise compile error
//???? why also need to delete again  EnvFunctor(const EnvFunctor&) and EnvFunctor& operator=(const EnvFunctor&)

class MenuHandler;
// class ObjectManager;
// class LightManager;


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

	virtual void operator() (void* const arg = NULL) override { ignore(arg); fprintf(stderr, "test TESTFUNCTOR\nsizeof objectManager is %llu\nsizeof lightsManager is %llu\nwindow addres is 0x%p\n", m_objects->size(), m_lights->size(), m_target); }
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
	SetValueFunctor& operator= (const SetValueFunctor&& functor) { m_value = functor.m_value; return *this; }
	
	SetValueFunctor(const SetValueFunctor& functor) = delete;
	SetValueFunctor& operator=(const SetValueFunctor& functor) = delete;


	virtual void operator() (void* const arg = NULL) override 
	{
		*m_value = *(reinterpret_cast<T*>(arg));
	}

	T getParameter() { return *m_value; }

};


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


class AddObjectFunctor : public AbstractFunctor
{

protected:
	
	ObjectManager* m_objects = nullptr;
	Drawable::Type m_type;

public:
	AddObjectFunctor(ObjectManager* obj, Drawable::Type type)
	:
	m_objects(obj),
	m_type(type)
	{}

	AddObjectFunctor(const AddObjectFunctor&) = delete;
	AddObjectFunctor& operator=(const AddObjectFunctor&) = delete;

	virtual void operator() (void* const arg = NULL) override;
};

class DeleteObjectFunctor : public AbstractFunctor
{

protected:
	
	ObjectManager* m_objects = nullptr;
	Drawable::Type m_type;

public:
	DeleteObjectFunctor(ObjectManager* obj, Drawable::Type type)
	:
	m_objects(obj),
	m_type(type)
	{}

	DeleteObjectFunctor(const DeleteObjectFunctor&) = delete;
	DeleteObjectFunctor& operator=(const DeleteObjectFunctor&) = delete;

	virtual void operator() (void* const arg = NULL) override;
};
