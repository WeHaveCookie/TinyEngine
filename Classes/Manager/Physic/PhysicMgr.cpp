#include "stdafx.h"
#include "PhysicMgr.h"
#include "blockingconcurrentqueue.h"
#include "Object/Object.h"

PhysicMgr* PhysicMgr::s_singleton = NULL;

class PhysicMgr::RegisteryQueue : public moodycamel::BlockingConcurrentQueue<Object*> {};

PhysicMgr::PhysicMgr()
	:Manager(ManagerType::Enum::Physic)
{
	s_singleton = this;
	m_enable = true;
	m_registeryQueue = new RegisteryQueue();
}

PhysicMgr::~PhysicMgr()
{
	delete m_registeryQueue;
}

void PhysicMgr::init()
{
	m_processTime = sf::Time::Zero;
}

void PhysicMgr::process(const float dt)
{
	sf::Clock clock;
	processRegisteryQueue();
	if (m_enable)
	{
		bool editableDone = false;
		for (auto& object : m_objects)
		{
			checkValidityOfPosition(object);
		}
	}
	m_processTime = clock.getElapsedTime();
}

void PhysicMgr::end()
{

}

void PhysicMgr::showImGuiWindow(bool* window)
{
	if (ImGui::Begin("PhysicMgr", window))
	{
		ImGui::Checkbox("Enable Phys", &m_enable);
		for (auto& object : m_objects)
		{
			ImGui::Text("%i : %s", object->getUID(), object->getName());
			if (ImGui::IsItemClicked())
			{
				object->showInfo();
			}
		}
	}
	ImGui::End();
}

void PhysicMgr::registerObject(Object* obj)
{
	m_registeryQueue->enqueue(obj);
}

void PhysicMgr::unregisterObject(Object* obj)
{
	auto pos = std::find(m_objects.begin(), m_objects.end(), obj);
	if (pos != m_objects.end())
	{
		m_objects.erase(pos);
	}
}


bool PhysicMgr::CollisionAABBAndCircle(sf::FloatRect box1, sf::CircleShape circle)
{
	float d2 = (box1.left - circle.getPosition().x)*(box1.left - circle.getPosition().x) + (box1.top - circle.getPosition().y)*(box1.top - circle.getPosition().y);
	if (d2 > circle.getRadius() * circle.getRadius())
		return false;
	else
		return true;
}

bool PhysicMgr::CollisionAABBandAABB(sf::FloatRect box1, sf::FloatRect box2)
{
	return !((box2.left >= box1.left + box1.width) ||
		(box2.left + box2.width <= box1.left) ||
		(box2.top >= box1.top + box1.height) ||
		(box2.top + box2.height <= box1.top));
}

void PhysicMgr::checkValidityOfPosition(Object* obj)
{
	// Check Collision
}


void PhysicMgr::processRegisteryQueue()
{
	Object* obj;
	bool dequeue = m_registeryQueue->try_dequeue(obj);
	while (dequeue)
	{
		m_objects.push_back(obj);
		dequeue = m_registeryQueue->try_dequeue(obj);
	}
}