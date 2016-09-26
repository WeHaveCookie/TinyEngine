#pragma once
#include <vector>
#include "Manager/Manager.h"

#define DECLARE_MANAGER(ManagerClass)\
	class ManagerClass; extern ManagerClass* g_##ManagerClass; extern bool g_activate##ManagerClass;

extern float g_DeltaTimeRaw;
extern float g_DeltaTime;
extern float g_DeltaTimeFactor;
extern float g_Framerate;

DECLARE_MANAGER(PhysicMgr)
DECLARE_MANAGER(RenderMgr)
DECLARE_MANAGER(GuiMgr)
DECLARE_MANAGER(InputMgr)

#define PHYSIC_MGR		g_PhysicMgr;
#define RENDER_MGR		g_RenderMgr;
#define GUI_MGR			g_GuiMgr;
#define INPUT_MGR		g_InputMgr;

class Manager;

class TinyEngineManagers
{
public:
	static TinyEngineManagers* getSingleton()
	{
		return m_instance;
	}

	TinyEngineManagers();

	void					CreateManagers();
	void					InitManagers();
	void					UpdateManagers(float _dt);
	void					DestroyManagers();
	std::vector<Manager*>&	getManagers() { return m_managers; }
	Manager*				getManager(ManagerType::Enum _type);
	const bool				isRunning() const;

private:
	std::vector<Manager*>			m_managers;
	static TinyEngineManagers*	m_instance;
};