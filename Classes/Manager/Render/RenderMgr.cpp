#include "stdafx.h"
#include "RenderMgr.h"

#include "TinyEngineManagers.h"
#include "Manager/Physic/PhysicMgr.h"
#include "Manager/Render/GuiMgr.h"
#include "Manager/Object/ObjectMgr.h"

RenderMgr* RenderMgr::s_singleton = NULL;

RenderMgr::RenderMgr()
:Manager(ManagerType::Enum::Render)
{
	s_singleton = this;
}

RenderMgr::~RenderMgr()
{

}

void RenderMgr::init()
{
	sf::VideoMode vm = sf::VideoMode::getDesktopMode();
	if (vm.height > 1080 || vm.width > 1920)
	{
		vm = sf::VideoMode(1920, 1080);
	}
	m_mainRenderWindow = new sf::RenderWindow(vm, "Ethereal Dream", sf::Style::Default /*| sf::Style::Fullscreen*/);
	srand(time(NULL));
	ImGui::SFML::Init(*m_mainRenderWindow);
}

void RenderMgr::process(const float dt)
{
	ObjectMgr::getSingleton()->paint();
	GuiMgr::getSingleton()->paint();

	endFrame();
}

void RenderMgr::end()
{

}

void RenderMgr::startFrame()
{
	m_mainRenderWindow->clear();
	m_timeFrame.restart();
}

void RenderMgr::endFrame()
{
	ImGui::Render();
	auto timer = m_timeFrame.getElapsedTime().asMilliseconds();
	
	m_mainRenderWindow->display();
}