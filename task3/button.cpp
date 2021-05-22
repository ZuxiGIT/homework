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
	float X = mouse_pos.x - (m_position.x + m_size.x / 2);
	float Y = mouse_pos.y - (m_position.y + m_size.y / 2);


	double a = m_radius_a;
	double b = m_radius_b;
    double length = X * X / (a * a) + Y * Y / (b * b);
    
    std::cout << "Mouse position: " << X<<", "<< Y<<std::endl;
	//std::cout << "Formula is "<< X*X <<"/"<< a*a<<"+"<<Y*Y<<"/"<<b*b<<"="<< X * X / (a * a) + Y * Y / (b * b)<<std::endl;
	//fprintf(stderr, "clicked ellipse, length is %f\n", length);
    
    if (length <= 1)
	{
		action();
	}
}

void MenuEllipseButton::scaleText()
{
	double glyph_height = m_text.getCharacterSize();
	double glyph_width 	= m_text.getCharacterSize() / 2;

	const char* str 			= m_text.getString().toAnsiString().c_str();
	size_t num_of_glyphs 		= 0;
	
	while(*str++)	num_of_glyphs++; 

	double y = m_text.getCharacterSize() / 2;
	double x = sqrt(1 -  y * y / m_radius_b / m_radius_b ) * m_radius_a;

	double right_height = 2 * y;
	double right_width 	= 2 * x;

    fprintf(stderr, "ellipse: right size (%lf, %lf)\n", right_width, right_height);
    fprintf(stderr, "ellipse: glyph str size (%lf, %lf\n", num_of_glyphs * glyph_width, glyph_height);

	double Xscale =  right_width 	/ (num_of_glyphs * glyph_width);
	double Yscale =	 right_height 	/ (glyph_height);

	Xscale = Xscale > 1 ? 1 : Xscale; 
	Yscale = Yscale > 1 ? 1 : Yscale;
	
	double Xoffset = (m_size.x - Xscale * (num_of_glyphs * glyph_width)) / 2;
	double Yoffset = (m_size.y - Yscale * glyph_height) / 2;

	Xoffset = Xoffset;// > 0 ? Xoffset : 0;    ---> negative offset??? 
	Yoffset = Yoffset;// > 0 ? Yoffset : 0;    ---> negative offset???

    fprintf(stderr, "ellipse: scale parametres (%lf, %lf)\nellipse: offset parametres (%lf, %lf)\n", Xscale, Yscale, Xoffset, Yoffset);
    m_text.setPosition(static_cast<float>(m_position.x + Xoffset), static_cast<float>(m_position.y + Yoffset));
	m_text.setScale(static_cast<float>(Xscale), static_cast<float>(Yscale));

    
}


//----------------------------MenuRectangleButton----------------------------
void MenuRectangleButton::draw()
{
    sf::RectangleShape Body {m_size};
    Body.setPosition(m_position);
    Body.setFillColor(Color(1.f, 0.f, 0.f));
    
    m_target->draw(Body);

    m_target->draw(m_text);
}

void MenuRectangleButton::clicked(const Vector2f& mouse_pos)
{
    float X = mouse_pos.x - m_position.x;
    float Y = mouse_pos.y - m_position.y;

	if ( X > 0 && X < m_size.x && 
		 Y > 0 && Y < m_size.y)
	{
		action();
	}
}

void MenuRectangleButton::scaleText()
{
    double glyph_height 	= m_text.getCharacterSize();
	double glyph_width 	= m_text.getCharacterSize() / 2;

	const char* str 			= m_text.getString().toAnsiString().c_str();
	size_t num_of_glyphs 		= 0;
	
	while(*str++)	num_of_glyphs++; 
	
	double Xscale 				= (m_size.x - 4)	/ (num_of_glyphs * glyph_width);
	double Yscale 				= (m_size.y )		/ (glyph_height);
	
	Xscale = Xscale > 1 ? 1 : Xscale; 
	Yscale = Yscale > 1 ? 1 : Yscale;

	double Xoffset = (m_size.x - num_of_glyphs * glyph_width) / 2;
	double Yoffset = (m_size.y - glyph_height) / 2;

	Xoffset = Xoffset;// > 0 ? Xoffset : 0;    ---> negative offset???
	Yoffset = Yoffset;// > 0 ? Yoffset : 0;    ---> negative offset???

    fprintf(stderr, "--------rectangle: offset parametres (%lf, %lf)\n", Xoffset, Yoffset);
	fflush(NULL);
    
    m_text.setPosition(static_cast<float>(m_position.x + Xoffset), static_cast<float>(m_position.y + Yoffset));
	m_text.setScale(static_cast<float>(Xscale), static_cast<float>(Yscale));
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