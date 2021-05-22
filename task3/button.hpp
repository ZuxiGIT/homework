#pragma once
#include "mymath.hpp"
#include "color.hpp"
#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include "noncopyable.hpp"
#include "functors.hpp"

class AbstractButton
{
protected:
    sf::RenderTarget* m_target = {};
    Vector2f m_position = {};
    Vector2f m_size    = {};
public:
    AbstractButton(sf::RenderTarget* target, Vector2f pos, Vector2f sz) : m_target(target), m_position(pos), m_size(sz) {}
    virtual ~AbstractButton() {}

    AbstractButton(const AbstractButton&) = delete;
    AbstractButton& operator=(const AbstractButton&) = delete;


    virtual void draw() = 0;
    virtual void clicked(const Vector2f& mouse_pos) = 0;
};


class ActionButton : public AbstractButton
{
protected:
    AbstractFunctor& m_action;
public:
    void action() { m_action(); }
    ActionButton(sf::RenderTarget* target, Vector2f pos, Vector2f sz, AbstractFunctor* action) : AbstractButton(target, pos, sz), m_action(*action) {}
    void setAction();
    virtual ~ActionButton() override {}
};

class MenuButton : public ActionButton
{
    static sf::Font font;

protected:
    sf::Text m_text = {};
public:
    MenuButton(sf::RenderTarget* target, Vector2f pos, Vector2f sz, const char* text, AbstractFunctor* action)
    : 
    ActionButton(target, pos, sz, action)
    {	
        m_text = sf::Text(text, font);
	    m_text.setCharacterSize(35);
	    m_text.setFillColor(Color(1.f, 1.f, 1.f));
    }
    
    static void loadFont(const char* filepath) { font.loadFromFile(filepath); }

    virtual void scaleText() = 0;
    virtual ~MenuButton() override {}
};

class MenuEllipseButton : public MenuButton
{
    float m_radius_a = {};
    float m_radius_b = {};
    int m_quality = 70;
public:
    
    MenuEllipseButton(sf::RenderTarget* target, Vector2f pos,  Vector2f radiuses, const char* text, AbstractFunctor* action)
    :
    MenuButton(target, pos, 2.f * radiuses, text, action),
    m_radius_a(radiuses.x),
    m_radius_b(radiuses.y) 
    {
        scaleText();
    }
    
    virtual ~MenuEllipseButton() override {}

    // virtual void action() override { m_action(); }
    virtual void draw() override;
    virtual void clicked(const Vector2f& mouse_pos) override;
    virtual void scaleText() override;
};

class MenuCircleButton : public MenuEllipseButton
{
public:
    MenuCircleButton(sf::RenderTarget* target, Vector2f pos,  float radius, const char* text, AbstractFunctor* action)
    : MenuEllipseButton(target, pos, Vector2f(radius, radius), text, action) {}

    virtual ~MenuCircleButton() override {}
};

class MenuRectangleButton final : public MenuButton
{
    public:
    
    MenuRectangleButton(sf::RenderTarget* target, Vector2f pos,  Vector2f sz, const char* text, AbstractFunctor* action)
    : MenuButton(target, pos, sz, text, action) 
    {
        scaleText();
    }

    virtual ~MenuRectangleButton() override {}

    // virtual void action() override { m_action(); }
    virtual void draw() override;
    virtual void clicked(const Vector2f& mouse_pos) override;
    virtual void scaleText() override;
};

class ButtonManager
{
    size_t m_count = 0;
    std::vector<AbstractButton*> m_buttons = {};

    ButtonManager() = default;
    ~ButtonManager();

public:
    static ButtonManager& createManager()
    { static ButtonManager instance; return instance; }

    ButtonManager(ButtonManager&) = delete;
    ButtonManager& operator=(const ButtonManager&) = delete;

    size_t size() const { return m_count; }
    
    AbstractButton& operator[](size_t index) { return *m_buttons[index]; }
    const AbstractButton& operator[] (size_t index) const  { return *m_buttons[index]; }

    void add(AbstractButton* obj);
    void clicked(const Vector2f& mouse_pos);
    void draw() { for (size_t i = 0; i < m_count; i ++) m_buttons[i]->draw();}
};