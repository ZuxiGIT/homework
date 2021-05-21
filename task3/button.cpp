#include "button.hpp"
#include "color.hpp"

MenuButton::MenuButton(sf::RenderTarget* target, Vector2i pos, Vector2i sz, const char* text)
: 
ActionButton(target, pos, sz)
{
    m_font.loadFromFile("TrueTypeFonts/UbuntuMono-R.ttf");
	
    m_text = sf::Text(text, m_font);
	m_text.setCharacterSize(20);
	m_text.setFillColor(Color(1.f, 1.f, 1.f));
}

void ButtonManager::add(AbstarctButton* obj)
{
    m_count++;
    m_objects.push_back(obj);
}

ButtonManager::~ButtonManager()
{
    for(size_t i = 0; i < m_count; i++)
        delete m_objects[i];
}