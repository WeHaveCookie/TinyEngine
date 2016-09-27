#include "stdafx.h"
#include "ObjectMgr.h"
#include "Object/Object.h"
#include "Manager/Physic/PhysicMgr.h"
#include "Manager/Render/RenderMgr.h"
#include "Utils/Random.h"

ObjectMgr* ObjectMgr::s_singleton = NULL;

ObjectMgr::ObjectMgr()
	:Manager(ManagerType::Enum::Object)
{
	s_singleton = this;
}

ObjectMgr::~ObjectMgr()
{

}

void ObjectMgr::init()
{
	m_left = 200.0f;
	m_top = 200.0f;
	m_width = 100.0f;
	m_height = 100.0f;
	m_angle = 0.0f;
	m_radius = 5.0;
	m_nbrPoints = 1;
	m_populate = 1;
}

uint32_t ObjectMgr::addRandom()
{
	auto winSize = RenderMgr::getSingleton()->getWindowSize();

	switch (randIntBorned(0, 5))
	{
	case 0:
		return addCircle(randFloatBorned(0.0f, winSize.x), randFloatBorned(0.0f, winSize.y), randFloatBorned(1.0f, 200.0f));
		break;
	case 1:
		return addAABB(randFloatBorned(0.0f, winSize.x), randFloatBorned(0.0f, winSize.y), randFloatBorned(1.0f, 200.0f), randFloatBorned(1.0f, 200.0f));
		break;
	case 2:
		return addOBB(randFloatBorned(0.0f, winSize.x), randFloatBorned(0.0f, winSize.y), randFloatBorned(1.0f, 200.0f), randFloatBorned(1.0f, 200.0f), randIntBorned(0, 360));
		break;
	case 3:
	{
		int nbrPoint = randIntBorned(0, 10);
		int id = createKDOP(randFloatBorned(0.0f, winSize.x), randFloatBorned(0.0f, winSize.y), nbrPoint);
		for (int i = 0; i < nbrPoint; i++)
		{
			addPointKDOP(id, Vector2(randFloatBorned(0.0f, 200.0f), randFloatBorned(0.0f, 200.0f)));
		}
		return id;
		break;
	}
	case 4:
		// Intentional fall through
	default:
		return addPoint(randFloatBorned(0.0f, winSize.x), randFloatBorned(0.0f, winSize.y));
		break;
	}
}

void ObjectMgr::process(const float dt)
{

}

void ObjectMgr::end()
{
	for (auto& object : m_objects)
	{
		delete object;
	}
}

void ObjectMgr::paint()
{
	for (auto& object : m_objects)
	{
		object->paint();
	}
}

void ObjectMgr::paintGui()
{
	for (auto& object : m_objects)
	{
		object->displayImGui();
	}
}

uint32_t ObjectMgr::addCircle(float x, float y, float radius)
{
	Circle* obj = new Circle(x, y, radius);
	m_objects.push_back(obj);
	registerObject(obj);
	return obj->getUID();
}

uint32_t ObjectMgr::addAABB(float left, float top, float width, float height)
{
	AABB* obj = new AABB(left, top, width, height);
	m_objects.push_back(obj);
	registerObject(obj);
	return obj->getUID();
}

uint32_t ObjectMgr::addOBB(float left, float top, float width, float height, float angle)
{
	OBB* obj = new OBB(left, top, width, height, angle);
	m_objects.push_back(obj);
	registerObject(obj);
	return obj->getUID();
}

uint32_t ObjectMgr::createKDOP(float x, float y, int nbrPoints)
{
	KDOP* obj = new KDOP(x, y, nbrPoints);
	m_objects.push_back(obj);
	registerObject(obj);
	return obj->getUID();
}

void ObjectMgr::addPointKDOP(uint32_t id, Vector2 pos)
{
	Object* obj = getObject(id);
	KDOP* kdop = static_cast<KDOP*>(obj);
	if (kdop != NULL)
	{
		kdop->addPoint(pos);
	}
}

uint32_t ObjectMgr::addPoint(float x, float y)
{
	Point* obj = new Point(x, y);
	m_objects.push_back(obj);
	registerObject(obj);
	return obj->getUID();
}

Object* ObjectMgr::getObject(uint32_t id)
{
	for (auto& object : m_objects)
	{
		if (object->getUID() == id)
		{
			return object;
		}
	}

	return NULL;
}

void ObjectMgr::showImGuiWindow(bool* display)
{
	if (ImGui::Begin("ObjectMgr", display))
	{
		ImGui::InputFloat("Left", &m_left);
		ImGui::InputFloat("Top", &m_top);
		ImGui::InputFloat("Width", &m_width);
		ImGui::InputFloat("Height", &m_height);
		ImGui::InputFloat("Angle", &m_angle);
		ImGui::InputFloat("Radius", &m_radius);
		ImGui::InputInt("Number Points", &m_nbrPoints);

		if (ImGui::Button("Circle"))
		{
			addCircle(m_left, m_top, m_radius);
		}
		ImGui::SameLine();
		if (ImGui::Button("AABB"))
		{
			addAABB(m_left, m_top, m_width, m_height);
		}
		ImGui::SameLine();
		if (ImGui::Button("OBB"))
		{
			addOBB(m_left, m_top, m_width, m_height, m_angle);
		}
		if (ImGui::Button("KDOP"))
		{
			createKDOP(m_left, m_top, m_nbrPoints);
		}
		ImGui::SameLine();
		if (ImGui::Button("Point"))
		{
			addPoint(m_left, m_top);
		}
		
		ImGui::Separator();
		if (ImGui::Button("Close All"))
		{
			for (auto& object : m_objects)
			{
				object->closeInfo();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear Scene"))
		{
			clear();
		}
		ImGui::InputInt("Number populate", &m_populate);
		ImGui::SameLine();
		if (ImGui::Button("Populate"))
		{
			for (int i = 0; i < m_populate; i++)
			{
				addRandom();
			}
		}
		std::vector<uint32_t> removeList;
		for (auto& object : m_objects)
		{
			ImGui::Text("%i - %s", object->getUID(), object->getName());
			if (ImGui::IsItemClicked())
			{
				object->showInfo();
			}
			std::string label = "Delete###" + std::to_string(object->getUID());
			ImGui::SameLine();
			if (ImGui::Button(label.c_str()))
			{
				removeList.push_back(object->getUID());
			}
		}

		for (auto& id : removeList)
		{
			removeObject(id);
		}
	}
	ImGui::End();
}

void ObjectMgr::removeObject(uint32_t id)
{
	int i = 0;
	bool found = false;
	for (auto& object : m_objects)
	{
		if (object->getUID() == id)
		{
			unregisterObject(object);
			found = true;
			break;
		}
		i++;
	}
	
	if (found)
	{
		m_objects.erase(m_objects.begin() + i);
	}
}

void ObjectMgr::registerObject(Object* obj)
{
	PhysicMgr::getSingleton()->registerObject(obj);
}

void ObjectMgr::unregisterObject(Object* obj)
{
	PhysicMgr::getSingleton()->unregisterObject(obj);
}

void ObjectMgr::clear()
{
	for (auto& object : m_objects)
	{
		unregisterObject(object);
	}
	m_objects.clear();
}