#include "button.hpp"
#include <algorithm>

MenuSwitcherFunctor::MenuSwitcherFunctor(MenuHandler* menu_handler, int* pages, unsigned int num)
:
m_num_of_pages(num),
m_pages(new int [num]),
m_menu(menu_handler)
{
    std::copy(pages, pages + num, m_pages);
}

void MenuSwitcherFunctor::operator()(void* const arg)
{
    fprintf(stderr, "switching menu\n");
    for(size_t i = 0; i < m_menu->getSize(); i ++)
        m_menu->setVisible(i, false);
    fprintf(stderr, "all set invisible menu\n");
    for(size_t i = 0; i < m_num_of_pages; i ++)
    {
        fprintf(stderr, "setting %d group_menu visible\n", m_pages[i]);
        m_menu->setVisible(m_pages[i], true);
    }
    
}

