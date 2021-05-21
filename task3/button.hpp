#pragma once
#include "mymath.hpp"
#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include "noncopyable.hpp"

class AbstarctButton
{
protected:
    sf::RenderTarget* m_target = {};
    Vector2i m_postion = {};
    Vector2i m_size    = {};
public:
    AbstarctButton(sf::RenderTarget* target, Vector2i pos, Vector2i sz) : m_target(target), m_postion(pos), m_size(sz) {}
    virtual ~AbstarctButton() {}

    AbstarctButton(const AbstarctButton&) = delete;
    AbstarctButton& operator=(const AbstarctButton&) = delete;


    virtual void draw() = 0;
    virtual void clicked() = 0;
};


class ActionButton : public AbstarctButton
{
protected:
    virtual void action() = 0;
public:
    ActionButton(sf::RenderTarget* target, Vector2i pos, Vector2i sz) : AbstarctButton(target, pos, sz) {}
    ~ActionButton() {}
};

class MenuButton : public ActionButton
{
    sf::Font m_font = {};
    sf::Text m_text = {};
public:
    MenuButton(sf::RenderTarget* target, Vector2i pos, Vector2i sz, const char* text);
};


class ButtonManager
{
    unsigned int m_count = 0;
    std::vector<AbstarctButton*> m_objects = {};

    ButtonManager() = default;
    ~ButtonManager();

public:
    static ButtonManager& createManager()
    { static ButtonManager instance; return instance; }

    ButtonManager(ButtonManager&) = delete;
    ButtonManager& operator=(const ButtonManager&) = delete;

    unsigned int size() const { return m_count; }
    
    AbstarctButton& operator[](size_t index) { return *m_objects[index]; }
    const AbstarctButton& operator[] (size_t index) const  { return *m_objects[index]; }

    void add(AbstarctButton* obj);
};