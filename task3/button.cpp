#include "button.hpp"
#include "color.hpp"
#include <iostream>
#include "sfmlExtend.hpp"
#include <SFML/Graphics/RectangleShape.hpp>


sf::Font MenuButton::font = {};

//----------------------------MenuEllipseButton----------------------------
void MenuEllipseButton::draw() 
{
    sf::EllipseShape ellipse (sf::Vector2f(m_radius_a, m_radius_b), m_quality);
    
    ellipse.setPosition(m_position.x, m_position.y);
    ellipse.setFillColor(Color(0.f, 1.f, 0.f));



    m_target->draw(ellipse);
    m_target->draw(m_text);
}

void MenuEllipseButton::clicked(const Vector2f& mouse_pos)
{
	int X = mouse_pos.x - m_position.x;
	int Y = mouse_pos.y - m_position.y;


	double a = m_radius_a;
	double b = m_radius_b;
    float length = X * X / (a * a) + Y * Y / (b * b);
    
    std::cout << "Mouse position: " << X<<", "<< Y<<std::endl;
	std::cout << "Formula is "<< X*X <<"/"<< a*a<<"+"<<Y*Y<<"/"<<b*b<<"="<< X * X / (a * a) + Y * Y / (b * b)<<std::endl;
	fprintf(stderr, "clicked ellipse, length is %f\n", length);
    
    if (length <= 1)
	{
		//std::cout << "Mouse position: " << X<<", "<< Y<<std::endl;
		//std::cout << "Formula is "<< X*X <<"/"<< a*a<<"+"<<Y*Y<<"/"<<b*b<<"="<< X * X / (a * a) + Y * Y / (b * b)<<std::endl;
		action();
	}
}

void MenuEllipseButton::scaleText()
{
    unsigned int glyph_height 	= m_text.getCharacterSize();
	unsigned int glyph_width 	= m_text.getCharacterSize() / 2;

	const char* str 			= m_text.getString().toAnsiString().c_str();
	size_t num_of_glyphs 		= 0;
	
	while(*str++)	num_of_glyphs++; 
	
	double Xscale 				= (m_size.x - 4)	/ (num_of_glyphs * glyph_width);
	double Yscale 				= (m_size.y )		/ (glyph_height);
	
	Xscale = Xscale > 1 ? 1 : Xscale; 
	Yscale = Yscale > 1 ? 1 : Yscale;

	int Xoffset = (m_size.x - num_of_glyphs * glyph_width) / 2;
	int Yoffset = (m_size.y - glyph_height) / 2;

	Xoffset = Xoffset > 0 ? Xoffset : 0;
	Yoffset = Yoffset > 0 ? Yoffset : 0;

	m_text.setPosition(m_position.x + Xoffset, m_position.y + Yoffset);
	m_text.setScale(Xscale, Yscale);
}

//----------------------------ButtonManager----------------------------
void ButtonManager::add(AbstractButton* obj)
{
    m_count++;
    m_buttons.push_back(obj);
}

void ButtonManager::clicked(const Vector2f& mouse_pos)
{
    for(size_t i = 0; i < m_count; i++)
        m_buttons[i]->clicked(mouse_pos);
}
ButtonManager::~ButtonManager()
{
    for(size_t i = 0; i < m_count; i++)
        delete m_buttons[i];
}