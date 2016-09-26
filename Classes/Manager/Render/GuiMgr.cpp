#include "stdafx.h"
#include "GuiMgr.h"

#include "Manager/Physic/PhysicMgr.h"
#include "Manager/Render/RenderMgr.h"

GuiMgr* GuiMgr::s_singleton = NULL;

struct GuiMgr::Flags
{
	bool show_PhysicMgr;
	bool show_RenderMgr;
	bool show_Demo;

	Flags()
		:show_PhysicMgr(false),
		show_RenderMgr(false),
		show_Demo(false)
	{

	}
};

GuiMgr::GuiMgr()
	:Manager(ManagerType::Gui)
{
	s_singleton = this;
}

GuiMgr::~GuiMgr()
{
	delete m_flags;
}

void GuiMgr::init()
{
	m_flags = new Flags();
	m_displayGui = true;
}

void GuiMgr::process(const float dt)
{
}

void GuiMgr::paint()
{
	if (m_displayGui)
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Manager"))
			{
				ImGui::MenuItem("PhysicMgr", NULL, &m_flags->show_PhysicMgr);
				ImGui::MenuItem("RenderMgr", NULL, &m_flags->show_RenderMgr);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				ImGui::MenuItem("ImGuiDemo", NULL, &m_flags->show_Demo);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		

		if (m_flags->show_PhysicMgr)
		{
			PhysicMgr::getSingleton()->showImGuiWindow(&m_flags->show_PhysicMgr);
		}

		if (m_flags->show_RenderMgr)
		{
			RenderMgr::getSingleton()->showImGuiWindow(&m_flags->show_RenderMgr);
		}

		if (m_flags->show_Demo)
		{
			ImGui::ShowTestWindow(&m_flags->show_Demo);
		}
	}
}

void GuiMgr::end()
{

}
