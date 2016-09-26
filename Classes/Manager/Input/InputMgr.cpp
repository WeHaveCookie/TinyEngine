#include "stdafx.h"
#include "InputMgr.h"
#include "TinyEngineManagers.h"
#include "Manager/Render/RenderMgr.h"

InputMgr* InputMgr::s_singleton = NULL;

InputMgr::InputMgr()
:Manager(ManagerType::Enum::Input)
{
	s_singleton = this;
}

InputMgr::~InputMgr()
{

}

void InputMgr::init()
{
	
	sf::Joystick::update();
}

void InputMgr::process(const float dt)
{
	if (RenderMgr::getSingleton()->getMainRenderWindow()->hasFocus())
	{
		
		sf::Event event;
		auto renderMgr = RENDER_MGR;
		auto rdrWin = renderMgr->getMainRenderWindow();
		while (rdrWin->pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			switch (event.type)
			{
			case sf::Event::Closed:
				rdrWin->close();
				break;
			case sf::Keyboard::Escape:
				rdrWin->close();
				break;
			case sf::Event::KeyPressed:
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					rdrWin->close();
					break;
				default:
					break;
				}
			}
			break;
			case sf::Event::MouseMoved:
				break;
			case sf::Event::JoystickConnected:
				break;
			case sf::Event::JoystickDisconnected:
				break;
			default:
				break;
			}
		}
	}
	ImGui::SFML::Update(dt);
}

void InputMgr::end()
{

}
