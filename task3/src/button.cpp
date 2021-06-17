// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com


#include "button.hpp"
#include <string>
#include <SFML/Graphics/RectangleShape.hpp>


sf::Font MenuButton::font = {};

//----------------------------MenuEllipseButton----------------------------
void MenuEllipseButton::render() 
{
	m_target->draw(m_body);
	m_target->draw(m_text);
}

bool MenuEllipseButton::update(const sf::Event& event)
{
	#if 0 
	float X = mouse_pos.x - (m_position.x + m_size.x / 2);
	float Y = mouse_pos.y - (m_position.y + m_size.y / 2);


	double a = ;
	double b = m_radius_b;
	double length = X * X / (a * a) + Y * Y / (b * b);

	std::cout << "Mouse position: " << X<<", "<< Y<<std::endl;
	std::cout << "Formula is "<< X*X <<"/"<< a*a<<"+"<<Y*Y<<"/"<<b*b<<"="<< X * X / (a * a) + Y * Y / (b * b)<<std::endl;
	fprintf(stderr, "clicked ellipse, length is %f\n", length);
	#endif

	Vector2f mouse_pos = sf::Mouse::getPosition(*m_target);

	if (!m_body.getGlobalBounds().contains(mouse_pos))
	{
		m_body.setOutlineColor(sf::Color::Transparent);
		m_body.setFillColor(m_background_color);
		
		return false;
	}
	
	m_body.setOutlineColor(RGB(0, 0, 255));
	
	if(	(event.type == sf::Event::MouseButtonPressed) && 
	(event.mouseButton.button == sf::Mouse::Left))
	{
		m_body.setFillColor(!RGB(m_background_color));
		return true;
	}

	if(	(event.type == sf::Event::MouseButtonReleased) && 
		(event.mouseButton.button == sf::Mouse::Left))
	{
		m_body.setFillColor(m_background_color);
		action();
		return true;
	}

	return false;
}

void MenuEllipseButton::scaleText()
{
	Vector2f radisuses = m_body.getRadius();
	Vector2f size = 2.f * radisuses;
	Vector2f position = m_body.getPosition();


	double glyph_height = m_text.getCharacterSize();
	double glyph_width 	= m_text.getCharacterSize() / 2;

	const char* str 			= m_text.getString().toAnsiString().c_str();
	size_t num_of_glyphs 		= 0;
	
	while(*str++)	num_of_glyphs++; 

	double y = m_text.getCharacterSize() / 2;
	double x = sqrt(1 -  y * y / radisuses.y / radisuses.y ) * radisuses.x;

	double right_height = 2 * y;
	double right_width 	= 2 * x;

	// fprintf(stderr, "ellipse: right size (%lf, %lf)\n", right_width, right_height);
	// fprintf(stderr, "ellipse: glyph str size (%lf, %lf\n", num_of_glyphs * glyph_width, glyph_height);

	double Xscale =  right_width 	/ (num_of_glyphs * glyph_width);
	double Yscale =	 right_height 	/ (glyph_height);

	Xscale = Xscale > 1 ? 1 : Xscale; 
	Yscale = Yscale > 1 ? 1 : Yscale;
	
	double Xoffset = (size.x - Xscale * (num_of_glyphs * glyph_width)) / 2;
	double Yoffset = (size.y - Yscale * glyph_height) / 2;

	Xoffset = Xoffset;// > 0 ? Xoffset : 0;    ---> negative offset??? 
	Yoffset = Yoffset;// > 0 ? Yoffset : 0;    ---> negative offset???

	// fprintf(stderr, "ellipse: scale parametres (%lf, %lf)\nellipse: offset parametres (%lf, %lf)\n", Xscale, Yscale, Xoffset, Yoffset);
	m_text.setPosition(static_cast<float>(position.x + Xoffset), static_cast<float>(position.y + Yoffset));
	m_text.setScale(static_cast<float>(Xscale), static_cast<float>(Yscale));

}


//----------------------------MenuRectangleButton----------------------------
void MenuRectangleButton::render()
{
	m_target->draw(m_body);

	m_target->draw(m_text);
}

bool MenuRectangleButton::update(const sf::Event& event)
{
	Vector2f mouse_pos = sf::Mouse::getPosition(*m_target);

	if (!m_body.getGlobalBounds().contains(mouse_pos))
	{
		m_body.setOutlineColor(sf::Color::Transparent);
		m_body.setFillColor(m_background_color);
		
		return false;
	}
	
	m_body.setOutlineColor(RGB(0, 0, 255));
	
	if(	(event.type == sf::Event::MouseButtonPressed) && 
	(event.mouseButton.button == sf::Mouse::Left))
	{
		m_body.setFillColor(!RGB(m_background_color));
		return true;
	}
	
	if(	(event.type == sf::Event::MouseButtonReleased) && 
		(event.mouseButton.button == sf::Mouse::Left))
	{
		m_body.setFillColor(m_background_color);
		action();
		return true;
	}

	return false;
}

void MenuRectangleButton::scaleText()
{
	Vector2f size = m_body.getSize();
	Vector2f position = m_body.getPosition();

	double glyph_height 	= m_text.getCharacterSize();
	double glyph_width 		= m_text.getCharacterSize() / 2;

	const char* str 			= m_text.getString().toAnsiString().c_str();
	size_t num_of_glyphs 		= 0;
	
	while(*str++)	num_of_glyphs++; 
	
	double Xscale 				= (size.x - 4)	/ (num_of_glyphs * glyph_width);
	double Yscale 				= (size.y )		/ (glyph_height);
	
	Xscale = Xscale > 1 ? 1 : Xscale; 
	Yscale = Yscale > 1 ? 1 : Yscale;

	double Xoffset = (size.x - num_of_glyphs * glyph_width) / 2;
	double Yoffset = (size.y - glyph_height) / 2;

	// Xoffset = Xoffset;// > 0 ? Xoffset : 0;    ---> negative offset???
	// Yoffset = Yoffset;// > 0 ? Yoffset : 0;    ---> negative offset???

	// fprintf(stderr, "--------rectangle: offset parametres (%lf, %lf)\n", Xoffset, Yoffset);
	// fflush(NULL);

	m_text.setPosition(static_cast<float>(position.x + Xoffset), static_cast<float>(position.y + Yoffset));
	m_text.setScale(static_cast<float>(Xscale), static_cast<float>(Yscale));
}

//----------------------------ButtonManager----------------------------
void ButtonManager::add(AbstractButton* obj)
{
	m_count++;
	m_buttons.push_back(obj);
}

bool ButtonManager::update(const sf::Event& event)
{
	for(size_t i = 0; i < m_count; i++)
		if(m_buttons[i]->update(event))
			return true;
	return false;
}
ButtonManager::~ButtonManager()
{
	for(size_t i = 0; i < m_count; i++)
		delete m_buttons[i];
}

//----------------------------TextField----------------------------
void TextField::handleInput(const sf::Event& event)
{
	if(!m_hasfocus || event.type != sf::Event::TextEntered)
		return;

	// 13 ---> enter
	if(event.text.unicode == 13)
	{
		if(m_text.empty())
			return;
		
		m_output_string = m_text;
		
		m_has_output = true;
		
		m_body_text.setFillColor(RGB(127, 127, 127));
		m_body_text.setString(m_text);
		
		// setFocus(false);

		return;
	}
	
	// 8 ---> Delete key
	if(m_has_output)
		return;

	if(event.text.unicode == 8)
	{
		m_text = m_text.substr(0, m_text.size() - 1);
	}
	else if(m_text.size() < m_size)
	{
		if(	(event.text.unicode >= '0') && (event.text.unicode <= '9') )	
			m_text += static_cast<char>(event.text.unicode);
		else if(event.text.unicode == '-')
			if(m_text.find('-') == std::string::npos)
				m_text += static_cast<char>(event.text.unicode);
	}
	m_body_text.setString(m_text);
}

std::string TextField::getOutput()
{
	std::string temp = m_output_string;
	m_output_string.clear();
	m_has_output = false;
	return temp;
}

//----------------------------MenuTextInputButton----------------------------
void MenuTextInputButton::scaleText()
{
	Vector2f position = m_bounding_rec.getPosition();
	m_text.setPosition(position);

	float text_width = m_text.getGlobalBounds().width;

	// fprintf(stderr, "text widt/width = %f/%f = %f\n", m_bounding_rec.getGlobalBounds().width, text_width, m_bounding_rec.getGlobalBounds().width/text_width);
	
	float text_offset = (m_bounding_rec.getGlobalBounds().width -  text_width) / 2.f;
	
	float xScale = m_bounding_rec.getGlobalBounds().width / text_width;


	if(text_offset > 0)
		m_text.move(Vector2f(text_offset, 0));

	if(xScale <= 1)
		m_text.scale(xScale, 1);

	float text_height = static_cast<float>(m_text.getCharacterSize());

	Vector2f size = m_bounding_rec.getSize();


	size.y -= text_height + 5;
	
	Vector2f box_position = position;
	box_position.y  += text_height + 5; 

	m_body.setPosition(box_position);
	m_body.setSize(size);
}

void MenuTextInputButton::render()
{
	if(!m_body.hasFocus())
		m_body.setBodyText(std::to_string(getParameter()));

	m_target->draw(m_bounding_rec);
	m_body.draw(*m_target);
	m_target->draw(m_text);
}

bool MenuTextInputButton::update(const sf::Event& event)
{ 
	Vector2f mouse_pos = sf::Mouse::getPosition(*m_target);

	if(m_body.contains(mouse_pos))
	{
		if(	(event.type == sf::Event::MouseButtonPressed) &&
			(event.mouseButton.button == sf::Mouse::Left))
		{
			m_body.setFocus(true);
			return true;
		}
	}
	
	if(	m_body.hasFocus() &&
		(event.type == sf::Event::KeyPressed) &&
		(event.key.code == sf::Keyboard::Escape))
	{
		
		if(m_body.hasOutput())
		{
			std::string result = m_body.getOutput();
			fprintf(stderr, "Output is %s\n", result.c_str());
			
			float result_number = std::stof(result);
			action(&result_number);
			m_body.setBodyText(std::to_string(getParameter()));
		}

		m_body.setFocus(false);

		return true;
	}

	if(m_body.hasFocus())
	{
		m_body.handleInput(event);
		return true;
	}

	return false;
}