#pragma once
#include <assert.h>
#include <vector>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "mymath.hpp"
#include "color.hpp"
#include "sfmlExtend.hpp"
#include "noncopyable.hpp"
#include "functors.hpp"


class AbstractButton
{
protected:
	sf::RenderWindow* m_target = {};

public:
	AbstractButton(sf::RenderWindow* target) : m_target(target) {}
	virtual ~AbstractButton() {}

	AbstractButton(const AbstractButton&) = delete;
	AbstractButton& operator=(const AbstractButton&) = delete;


	virtual void render() = 0;
	virtual bool update(const sf::Event& event) = 0;
};


class ActionButton : public AbstractButton
{
protected:
	AbstractFunctor* m_action = nullptr;

public:
	ActionButton(sf::RenderWindow* target, AbstractFunctor* action)
	:
	AbstractButton(target), 
	m_action(action) 
	{}

	ActionButton(const ActionButton&) = delete;
	ActionButton& operator=(const ActionButton&) = delete;

	virtual void action(void* const arg = NULL ) { if(m_action) (*m_action)(arg); }

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

	MenuButton(	sf::RenderWindow* target, const char* text,
				AbstractFunctor* action, const RGB& background_color, const RGB& text_color)
	:
	ActionButton(target, action), 
	m_background_color(background_color),
	m_text_color(text_color),
	m_text(sf::Text(text, font))
	{
		m_text.setCharacterSize(30);
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

	MenuEllipseButton(	sf::RenderWindow* target, Vector2f pos,  Vector2f radiuses, const char* text,
						AbstractFunctor* action, const RGB& background_color, const RGB& text_color)
	:
	MenuButton(target, text, action, background_color, text_color),
	m_body(radiuses, m_quality)
	{
		m_body.setPosition(pos);
		m_body.setFillColor(background_color);
		m_body.setOutlineThickness(2);
		m_body.setOutlineColor(sf::Color::Transparent);

		scaleText();
	}

	virtual ~MenuEllipseButton() override {}

	// virtual void action() override { m_action(); }
	virtual void render() override;
	virtual bool update(const sf::Event& event) override;
	virtual void scaleText() override;
};

class MenuCircleButton : public MenuEllipseButton
{
public:
	MenuCircleButton(	sf::RenderWindow* target, Vector2f pos,  float radius, const char* text,
						AbstractFunctor* action, const RGB& background_color, const RGB& text_color)
	:
	MenuEllipseButton(target, pos, Vector2f(radius, radius), text, action, background_color, text_color)
	{}

	virtual ~MenuCircleButton() override {}
};

class MenuRectangleButton final : public MenuButton
{
	sf::RectangleShape m_body;
public:

	MenuRectangleButton(sf::RenderWindow* target, Vector2f pos,  Vector2f sz, const char* text,
						AbstractFunctor* action, const RGB& background_color, const RGB& text_color)
	:
	MenuButton(target, text, action, background_color, text_color),
	m_body(sz) 
	{
		m_body.setPosition(pos);
		m_body.setFillColor(m_background_color);
		m_body.setOutlineThickness(2);
		m_body.setOutlineColor(sf::Color::Transparent);

		scaleText();
	}

	virtual ~MenuRectangleButton() override {}

	virtual void render() override;
	virtual bool update(const sf::Event& event) override;
	virtual void scaleText() override;

};

class ButtonManager
{
	size_t m_count = 0;
	std::vector<AbstractButton*> m_buttons = {};
public:
	ButtonManager() = default;
	
	~ButtonManager();

	ButtonManager(ButtonManager&) = delete;
	ButtonManager& operator=(const ButtonManager&) = delete;

	size_t size() const { return m_count; }

	AbstractButton& operator[](size_t index) { return *m_buttons[index]; }
	const AbstractButton& operator[] (size_t index) const  { return *m_buttons[index]; }

	void add(AbstractButton* obj);
	bool update(const sf::Event& event);
	void render() { for (size_t i = 0; i < m_count; i ++) m_buttons[i]->render();}
};



class TextField //: public sf::Transformable
{
	Vector2f m_position = {};
	size_t m_size = {};
	std::string m_text = {};
	std::string m_output_string = {};
	bool m_has_output = false;
	sf::Text m_body_text = {};
	sf::RectangleShape m_body;
	bool m_hasfocus = false;

	void shiftBodyText()
	{
		m_body_text.move(Vector2f(	(m_body.getSize().x - static_cast<float>(m_size) * static_cast<float>(m_body_text.getCharacterSize()) / 2.f) / 2.f, 
									(m_body.getSize().y - static_cast<float>(m_body_text.getCharacterSize())) / 2.f));
	}

public:
	TextField(Vector2f pos, float width, int character_size)
	:
	m_position(pos),
	m_size(static_cast<size_t>(width / (static_cast<float>(character_size) / 2.f))),
	m_body_text(m_text, MenuButton::font),
	m_body(Vector2f(width, 2.f * static_cast<float>(character_size)))
	{
		m_body_text.setPosition(pos);
		m_body_text.setFillColor(RGB(127, 127, 127));
		m_body_text.setCharacterSize(character_size);

		m_body.setFillColor(RGB(255, 255,255));
		m_body.setOutlineThickness(2);
		m_body.setOutlineColor(sf::Color::Transparent);
		m_body.setPosition(pos);

		shiftBodyText();
	}

	std::string getOutput();
	void handleInput(const sf::Event& event);

	void setBodyText(const std::string& text)
	{
		m_body_text.setString(text.substr(0, m_size));
	}

	bool hasOutput()
	{
		return m_has_output;
	}

	const std::string getText() const
	{
		return m_text;
	}

	bool contains(const Vector2f& mouse_pos)
	{
		return m_body.getGlobalBounds().contains(mouse_pos);
	}

	bool hasFocus()
	{
		return m_hasfocus;
	}

	void setFocus(bool focus)
	{
		m_hasfocus = focus;
		if(focus)
		{
			m_text.clear();
			
			m_has_output = false;
			
			m_output_string.clear();
			
			m_body_text.setString(m_text);
			m_body_text.setFillColor(RGB(0, 0, 0));

			m_body.setOutlineColor(RGB(0, 0, 255));
		}
		else
		{
			m_body_text.setFillColor(RGB(127, 127, 127));
			m_body.setOutlineColor(sf::Color::Transparent);
		}
	}

	void setPosition(const Vector2f& pos) 
	{
		m_position = pos;
		m_body.setPosition(pos);
		m_body_text.setPosition(pos);

		shiftBodyText();
	}

	Vector2f getPostion() { return m_position; }

	void setSize(const Vector2f& size)
	{
		float xTextScale = size.x / (static_cast<float>(m_size) * static_cast<float>(m_body_text.getCharacterSize()) / 2.f);
		float yTextScale = size.y / static_cast<float>(m_body_text.getCharacterSize());
		
		if((xTextScale <= 1) && (yTextScale <= 1))
			m_body_text.scale(Vector2f(xTextScale, yTextScale));
		
		m_body.setSize(size);
	}
	Vector2f getSize() { return m_body.getSize(); }

	void draw(sf::RenderTarget& target) { target.draw(m_body); target.draw(m_body_text); }
};

class MenuTextInputButton final : public MenuButton
{
	sf::RectangleShape m_bounding_rec;
	TextField m_body;

	float getParameter()
	{
		return (*(reinterpret_cast<SetValueFunctor<float>*>(m_action))).getParameter();
	}
public:

	MenuTextInputButton(sf::RenderWindow* target, Vector2f pos,  Vector2f sz, const char* text,
						AbstractFunctor* action, const RGB& text_color)
	:
	MenuButton(target, text, action, RGB(255, 255, 255), text_color),
	m_bounding_rec(sz),
	m_body(pos, sz.x, m_text.getCharacterSize())
	{

		assert(dynamic_cast<SetValueFunctor<float>*>(action) && "Functor must be SetValueFunctor<float> type");

		m_bounding_rec.setPosition(pos);
		m_bounding_rec.setOutlineColor(sf::Color::Green);
		m_bounding_rec.setFillColor(sf::Color::Transparent);
		
		// fprintf(stderr, "before width camera.x %f\n", m_text.getGlobalBounds().width);	
		
		scaleText();
		
		// fprintf(stderr, "after width camera.x %f\n", m_text.getGlobalBounds().width);	
		
		m_bounding_rec.setOutlineThickness(2);

	}

	virtual void scaleText() override;
	virtual void render() override;
	virtual bool update(const sf::Event& event) override ;
};


struct GroupOfButtons
{
	ButtonManager* m_buttons = nullptr;
 	bool m_visible = false;

	GroupOfButtons(ButtonManager& buttons) { m_buttons = &buttons; }
};

class MenuHandler
{
	std::vector<GroupOfButtons*> m_groups_of_buttons = {};

public:
	MenuHandler() {}

	void add(GroupOfButtons* GoB) { m_groups_of_buttons.push_back(GoB); }
	
	GroupOfButtons& operator[] (size_t index) const { return *(m_groups_of_buttons[index]); }
	
	size_t getSize()
	{
		return m_groups_of_buttons.size();
	}

	// int getVisible()
	// {
	// 	for( unsigned int i = 0; i < m_groups_of_buttons.size(); i ++)
	// 		if(m_groups_of_buttons[i]->m_visible)
	// 			return i;
	// }

	void setVisible(size_t index, bool flag)
	{
		m_groups_of_buttons[index]->m_visible = flag;
	}

	bool update(const sf::Event& event) 
	{
		for( unsigned int i = 0; i < m_groups_of_buttons.size(); i ++)
			if(m_groups_of_buttons[i]->m_visible)
				return m_groups_of_buttons[i]->m_buttons->update(event);
		return false;
	}

	void render()
	{
		for( unsigned int i = 0; i < m_groups_of_buttons.size(); i ++)
			if(m_groups_of_buttons[i]->m_visible)
				m_groups_of_buttons[i]->m_buttons->render();
	}

	~MenuHandler()
	{
		for( unsigned int i = 0; i < m_groups_of_buttons.size(); i ++)
			delete m_groups_of_buttons[i];
	} 
};