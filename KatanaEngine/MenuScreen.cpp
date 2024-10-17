﻿
/*
     ██╗  ██╗  █████╗  ████████╗  █████╗  ███╗   ██╗  █████╗ 
	 ██║ ██╔╝ ██╔══██╗ ╚══██╔══╝ ██╔══██╗ ████╗  ██║ ██╔══██╗
	 █████╔╝  ███████║    ██║    ███████║ ██╔██╗ ██║ ███████║
	 ██╔═██╗  ██╔══██║    ██║    ██╔══██║ ██║╚██╗██║ ██╔══██║
	 ██║  ██╗ ██║  ██║    ██║    ██║  ██║ ██║ ╚████║ ██║  ██║
	 ╚═╝  ╚═╝ ╚═╝  ╚═╝/\  ╚═╝    ╚═╝  ╚═╝ ╚═╝  ╚═══╝ ╚═╝  ╚═╝
   /vvvvvvvvvvvvvvvvvvv \=========================================,
   `^^^^^^^^^^^^^^^^^^^ /---------------------------------------"
        Katana Engine \/ © 2012 - Shuriken Studios LLC
			    http://www.shurikenstudios.com
*/

#include "KatanaEngine.h"

namespace KatanaEngine
{
	MenuScreen::MenuScreen()
	{
		m_selectedItemIndex = 0;
		m_displayStartIndex = 0;
		m_displayCount = 0;
		m_itemListWraps = true;
	}


	MenuScreen::~MenuScreen()
	{
		std::vector<MenuItem *>::iterator it;
		for (it = m_menuItems.begin(); it != m_menuItems.end(); ++it)
		{
			delete *it;
		}
	}


	void MenuScreen::HandleInput(const InputState& input)
	{
		if (m_displayCount > 0) m_itemListWraps = false;

		if (m_menuItems.size() > 0)
		{
			int8_t playerIndexOut;

			const int FIRST_INDEX = 0;
			const int LAST_INDEX = (int)(m_menuItems.size() - 1);

			if (input.IsNewKeyPress(Key::ENTER) ||
				input.IsNewButtonPress(Button::A, playerIndexOut))
			{
				m_menuItems[m_selectedItemIndex]->Select(this);
			}

			if (input.IsNewKeyPress(Key::UP) ||
				input.IsNewButtonPress(Button::DPAD_UP, playerIndexOut)) // todo: Make "IsMenuUp" in InputState ?
			{
				m_selectedItemIndex--;

				if (m_selectedItemIndex >= m_menuItems.size())
				{
					m_selectedItemIndex = (m_itemListWraps) ? LAST_INDEX : FIRST_INDEX;
				}
			}
			else if (input.IsNewKeyPress(Key::DOWN) ||
				input.IsNewButtonPress(Button::DPAD_DOWN, playerIndexOut)) // TODO?: Make "IsMenuDown" in InputState ?
			{
				m_selectedItemIndex++;

				if (m_selectedItemIndex >= m_menuItems.size())
				{
					m_selectedItemIndex = (m_itemListWraps) ? FIRST_INDEX : LAST_INDEX;
				}
			}

			if (m_displayCount > 0)
			{
				if (m_selectedItemIndex < m_displayStartIndex)
				{
					m_displayStartIndex--;
				}
				else if (m_selectedItemIndex >= m_displayStartIndex + m_displayCount)
				{
					m_displayStartIndex++;
				}
			}
		}
	}

	void MenuScreen::Update(const GameTime& gameTime)
	{
		unsigned int index = 0;

		std::vector<MenuItem *>::iterator it;
		for (it = m_menuItems.begin(); it != m_menuItems.end(); ++it)
		{
			MenuItem *pMenuItem = *it;

			bool displayed = (m_displayCount == 0
				|| (index >= m_displayStartIndex
					&& index < m_displayStartIndex + m_displayCount));

			pMenuItem->SetDisplayed(displayed);
			pMenuItem->SetSelected(index == m_selectedItemIndex);
			pMenuItem->Update(gameTime);

			index++;
		}
	}

	void MenuScreen::Draw(SpriteBatch& spriteBatch)
	{
		spriteBatch.Begin();

		std::vector<MenuItem *>::iterator it;
		for (it = m_menuItems.begin(); it != m_menuItems.end(); ++it)
		{
			MenuItem *pMenuItem = *it;
			if (pMenuItem->IsDisplayed()) pMenuItem->Draw(spriteBatch);
		}

		spriteBatch.End();
	}


	void MenuScreen::AddMenuItem(MenuItem *pItem) 
	{
		pItem->SetIndex((int)m_menuItems.size());
		pItem->SetMenuScreen(this);
		m_menuItems.push_back(pItem);
	}
}