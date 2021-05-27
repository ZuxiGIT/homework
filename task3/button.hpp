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
    virtual bool clicked(const Vector2f& mouse_pos) = 0;
};


class ActionButton : public AbstractButton
{
protected:
    AbstractFunctor* m_action;

public:
    ActionButton(sf::RenderTarget* target, Vector2f pos, Vector2f sz, AbstractFunctor* action)
    :
    AbstractButton(target, pos, sz), 
    m_action(action) 
    {}

    ActionButton(const ActionButton&) = delete;
    ActionButton& operator=(const ActionButton&) = delete;

    void action() { if(m_action) (*m_action)(); }
    
    void setAction(AbstractFunctor* action) { m_action = action; }
    
    virtual ~ActionButton() override {}
};

class MenuButton : public ActionButton
{
    static sf::Font font;

protected:
    RGB m_background_color = {};
    RGB m_text_color = {};
    sf::Text m_text = {};
public:
    MenuButton( sf::RenderTarget* target, Vector2f pos, Vector2f sz, const char* text,
                AbstractFunctor* action, const RGB& background_color, const RGB& text_color)
    : 
    ActionButton(target, pos, sz, action), 
    m_background_color(background_color),
    m_text_color(text_color)
    {	
        m_text = sf::Text(text, font);
	    m_text.setCharacterSize(35);
	    m_text.setFillColor(m_text_color);
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
    
    MenuEllipseButton(  sf::RenderTarget* target, Vector2f pos,  Vector2f radiuses, const char* text,
                        AbstractFunctor* action, const RGB& background_color, const RGB& text_color)
    :
    MenuButton(target, pos, 2.f * radiuses, text, action, background_color, text_color),
    m_radius_a(radiuses.x),
    m_radius_b(radiuses.y) 
    {
        scaleText();
    }
    
    virtual ~MenuEllipseButton() override {}

    // virtual void action() override { m_action(); }
    virtual void draw() override;
    virtual bool clicked(const Vector2f& mouse_pos) override;
    virtual void scaleText() override;
};

class MenuCircleButton : public MenuEllipseButton
{
public:
    MenuCircleButton(   sf::RenderTarget* target, Vector2f pos,  float radius, const char* text,
                        AbstractFunctor* action, const RGB& background_color, const RGB& text_color)
    :
    MenuEllipseButton(target, pos, Vector2f(radius, radius), text, action, background_color, text_color)
    {}

    virtual ~MenuCircleButton() override {}
};

class MenuRectangleButton : public MenuButton
{
    public:
    
    MenuRectangleButton(sf::RenderTarget* target, Vector2f pos,  Vector2f sz, const char* text,
                        AbstractFunctor* action, const RGB& background_color, const RGB& text_color)

    : MenuButton(target, pos, sz, text, action, background_color, text_color) 
    {
        scaleText();
    }

    virtual ~MenuRectangleButton() override {}

    // virtual void action() override { m_action(); }
    virtual void draw() override;
    virtual bool clicked(const Vector2f& mouse_pos) override;
    virtual void scaleText() override;
};


class MenuTextInputButton : public MenuRectangleButton
{
    void shiftText();
public:

    MenuTextInputButton(sf::RenderTarget* target, Vector2f pos,  Vector2f sz, const char* text,
                        AbstractFunctor* action, const RGB& text_color)
    :
    MenuRectangleButton(target, pos, sz, text, action, RGB(255, 255, 255), text_color)
    {
        shiftText();
    }

    virtual void draw() override;
    virtual bool clicked(const Vector2f& mouse_pos) override {return false;}

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
    bool clicked(const Vector2f& mouse_pos);
    void draw() { for (size_t i = 0; i < m_count; i ++) m_buttons[i]->draw();}
};