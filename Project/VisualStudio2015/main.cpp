#include "stdafx.h"

#include "TinyEngineManagers.h"


int main()
{
	TinyEngineManagers* gameMgrs = new TinyEngineManagers();
	gameMgrs->CreateManagers();
	gameMgrs->InitManagers();
	sf::Clock timeElapsed;
	sf::Clock timeReal;
	while (gameMgrs->isRunning())
	{
		timeElapsed.restart();
		gameMgrs->UpdateManagers(g_DeltaTime);
		g_DeltaTime = timeElapsed.restart().asSeconds();
		if (g_DeltaTime < g_Framerate)
		{
			sf::sleep(sf::milliseconds(g_Framerate*1000 - g_DeltaTime*1000));
		}
		g_realTime = timeReal.restart().asSeconds();
	}
	
	ImGui::SFML::Shutdown();
	gameMgrs->DestroyManagers();

    return 0;
}
