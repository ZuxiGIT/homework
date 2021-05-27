#pragma once
#include "mymath.hpp"
#include "color.hpp"
#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
#include "sfmlExtend.hpp"
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

    virtual void action() { if(m_action) (*m_action)(); }
    
    void setAction(AbstractFunctor* action) { m_action = action; }
    
    virtual ~ActionButton() override {}
};

class MenuButton : public ActionButton
{

protected:
    RGB m_background_color = {};
    RGB m_text_color = {};
    sf::Text m_text = {};
public:
    static sf::Font font;
    
    MenuButton( sf::RenderTarget* target, Vector2f pos, Vector2f sz, const char* text,
                AbstractFunctor* action, const RGB& background_color, const RGB& text_color)
    : 
    ActionButton(target, pos, sz, action), 
    m_background_color(background_color),
    m_text_color(text_color)
    {	
        m_text = sf::Text(text, font);
	    m_text.setCharacterSize(20);
	    m_text.setFillColor(m_text_color);
    }
    
    static void loadFont(const char* filepath) { font.loadFromFile(filepath); }

    void setCharacterSize(unsigned int size) { m_text.setCharacterSize(size); }

    virtual void scaleText() = 0;
    virtual ~MenuButton() override {}
};


class MenuEllipseButton : public MenuButton
{
    int m_quality = 70;
    sf::EllipseShape m_body;
public:
    
    MenuEllipseButton(  sf::RenderTarget* target, Vector2f pos,  Vector2f radiuses, const char* text,
                        AbstractFunctor* action, const RGB& background_color, const RGB& text_color)
    :
    MenuButton(target, pos, 2.f * radiuses, text, action, background_color, text_color),
    m_body(radiuses, m_quality)
    {
        m_body.setPosition(m_position);
        m_body.setFillColor(background_color);
        m_text.setFillColor(text_color);

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

    virtual void draw() override;
    virtual bool clicked(const Vector2f& mouse_pos) override;
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
    bool clicked(const Vector2f& mouse_pos);
    void draw() { for (size_t i = 0; i < m_count; i ++) m_buttons[i]->draw();}
};

class TextField
{
    Vector2f m_position = {};
    size_t m_size = {};
    sf::Font m_font = MenuButton::font;
    std::string m_text = {};
    sf::RectangleShape m_rect;
    bool m_hasfocus = false;

public:
    TextField(Vector2f pos, size_t max_chars)
    :
    m_position(pos),
    m_size(max_chars),
    m_rect(Vector2f( 10.f * static_cast<float>(max_chars), 20))
    {
        m_rect.setOutlineThickness(2);
        m_rect.setFillColor(RGB(255, 255,255));
        m_rect.setOutlineColor(RGB(127, 127, 127));
        m_rect.setPosition(m_position);
    }

    void handleInput(sf::Event event);

    const std::string getText() const
    {
        return m_text;
    }

    bool isInside(Vector2f mouse_pos)
    {
        return m_rect.getGlobalBounds().contains(mouse_pos);
    }

    void setFocus(bool focus)
    {
        m_hasfocus = focus;
        if(focus)
            m_rect.setOutlineColor(RGB(0, 0, 255));
        else
            m_rect.setOutlineColor(RGB(127, 127, 127));
    }

    void setPosition(const Vector2f& pos) 
    { 
        m_position = pos; 
        m_rect.setPosition(m_position); 
    }
    Vector2f getPostion() { return m_position; }

    void setSize(const Vector2f& size) { m_rect.setSize(size); }
    Vector2f getSize() { return m_rect.getSize(); }

    void draw(sf::RenderTarget& target) { target.draw(m_rect); }
};

class MenuTextInputButton : public MenuRectangleButton
{
    void shiftText();
    TextField m_body;
public:

    MenuTextInputButton(sf::RenderTarget* target, Vector2f pos,  Vector2f sz, const char* text,
                        AbstractFunctor* action, const RGB& text_color)
    :
    MenuRectangleButton(target, pos, sz, text, action, RGB(255, 255, 255), text_color),
    m_body(pos, sz.x / 10.f)
    {
        shiftText();
    }

    virtual void draw() override;
    virtual bool clicked(const Vector2f& mouse_pos) override {return false;}

};