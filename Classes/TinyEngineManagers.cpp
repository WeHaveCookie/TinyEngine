#include "stdafx.h"
#include "TinyEngineManagers.h"

#include "Manager/Physic/PhysicMgr.h"
#include "Manager/Render/RenderMgr.h"
#include "Manager/Render/GuiMgr.h"
#include "Manager/Input/InputMgr.h"
#include "Manager/Object/ObjectMgr.h"

TinyEngineManagers* TinyEngineManagers::m_instance = NULL;

#define REGISTER_MANAGER(ManagerClass) ManagerClass* g_##ManagerClass = NULL;

#define CREATE_MGR(ManagerClass) g_##ManagerClass = new ManagerClass(); m_managers.push_back(g_##ManagerClass);
#define INIT_MGR(ManagerClass) g_##ManagerClass->init();
#define PROCESS_MGR(ManagerClass) g_##ManagerClass->process(g_DeltaTime);
#define END_MGR(ManagerClass) g_##ManagerClass->end();	


float g_DeltaTimeRaw = 0.33f;
float g_DeltaTime = 0.33f;
float g_DeltaTimeFactor = 1.f;
float g_Framerate = 1.0f/60.0f;
float g_realTime = 0.0f;

REGISTER_MANAGER(PhysicMgr)
REGISTER_MANAGER(RenderMgr)
REGISTER_MANAGER(GuiMgr)
REGISTER_MANAGER(InputMgr)
REGISTER_MANAGER(ObjectMgr)

TinyEngineManagers::TinyEngineManagers()
{
	m_instance = this;
}


void TinyEngineManagers::CreateManagers()
{

		CREATE_MGR(PhysicMgr)
			CREATE_MGR(RenderMgr)
			CREATE_MGR(ObjectMgr)
			CREATE_MGR(GuiMgr)
			CREATE_MGR(InputMgr)
}

void TinyEngineManagers::InitManagers()
{

		INIT_MGR(InputMgr)
			INIT_MGR(PhysicMgr)
			INIT_MGR(RenderMgr)
			INIT_MGR(GuiMgr)
			INIT_MGR(ObjectMgr)
}

void TinyEngineManagers::UpdateManagers(float _dt)
{
	g_DeltaTimeRaw = _dt;
	g_DeltaTime = g_DeltaTimeRaw * g_DeltaTimeFactor;
	_dt = g_DeltaTime;

	// must be first
	g_RenderMgr->startFrame();

	PROCESS_MGR(InputMgr)
		PROCESS_MGR(PhysicMgr)
		PROCESS_MGR(ObjectMgr)
		PROCESS_MGR(RenderMgr)
		PROCESS_MGR(GuiMgr)
}

void TinyEngineManagers::DestroyManagers()
{

		END_MGR(PhysicMgr)
			END_MGR(RenderMgr)
			END_MGR(ObjectMgr)
			END_MGR(GuiMgr)
			END_MGR(InputMgr)


		for (int ID = (int)m_managers.size() - 1; ID > 0; --ID)
		{
			Manager* manager = m_managers[ID];
			m_managers[ID] = NULL;
			delete manager;
		}

	m_managers.clear();
}

Manager* TinyEngineManagers::getManager(ManagerType::Enum type)
{
	for (uint32_t ID = 0; ID < m_managers.size(); ++ID)
	{
		if (m_managers[ID]->getType() == type)
			return m_managers[ID];
	}
	return NULL;
}

const bool TinyEngineManagers::isRunning() const 
{ 
	return g_RenderMgr->isRunning(); 
}