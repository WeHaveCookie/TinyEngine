#include "stdafx.h"

#include "TinyEngineManagers.h"


int main()
{
	TinyEngineManagers* gameMgrs = new TinyEngineManagers();
	gameMgrs->CreateManagers();
	gameMgrs->InitManagers();
	sf::Clock framerate;

	while (gameMgrs->isRunning())
	{
		gameMgrs->UpdateManagers(g_DeltaTime);
		g_DeltaTime = framerate.restart().asSeconds();
		g_Framerate = 1.0f / g_DeltaTime;
	}
	
	ImGui::SFML::Shutdown();
	gameMgrs->DestroyManagers();

    return 0;
}
