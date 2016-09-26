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

void PhysicMgr::paint()
{

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

bool PhysicMgr::RayCastAABBAndAABB(sf::FloatRect box1Last, sf::FloatRect box1, sf::FloatRect box2)
{
	Vector2 aLast, bLast, cLast, dLast, centerLast, a, b, c, d, center;
	aLast.x = box1Last.left;
	aLast.y = box1Last.top;

	a.x = box1.left;
	a.y = box1.top;

	bLast.x = box1Last.left + box1Last.width;
	bLast.y = box1Last.top;

	b.x = box1.left + box1.width;
	b.y = box1.top;

	cLast.x = box1Last.left + box1Last.width;
	cLast.y = box1Last.top + box1.height;

	c.x = box1.left + box1.width;
	c.y = box1.top + box1.height;

	dLast.x = box1Last.left;
	dLast.y = box1Last.top + box1Last.height;

	d.x = box1.left;
	d.y = box1.top + box1.height;

	centerLast.x = box1Last.left + (box1Last.width / 2.0f);
	centerLast.y = box1Last.top + (box1Last.height / 2.0f);

	center.x = box1.left + (box1.width / 2.0f);
	center.y = box1.top + (box1.height / 2.0f);

	return (CollissionAABBAndSeg(box2, centerLast, center) || CollissionAABBAndSeg(box2, aLast, a) || CollissionAABBAndSeg(box2, bLast, b) || CollissionAABBAndSeg(box2, cLast, c) || CollissionAABBAndSeg(box2, dLast, d));
}

bool PhysicMgr::CollissionAABBAndSeg(sf::FloatRect box1, Vector2 sStart, Vector2 sEnd)
{
	Vector2 a, b, c, d;
	a.x = box1.left;
	a.y = box1.top;
	
	b.x = box1.left + box1.width;
	b.y = box1.top;
	
	c.x = box1.left + box1.width;
	c.y = box1.top + box1.height;

	d.x = box1.left;
	d.y = box1.top + box1.height;
	
	return CollisionSegAndSeg(a, b, sStart, sEnd) || CollisionSegAndSeg(b, c, sStart, sEnd) || CollisionSegAndSeg(c, d, sStart, sEnd) || CollisionSegAndSeg(d, a, sStart, sEnd);
}

bool PhysicMgr::CollisionLineAndSeg(Vector2 dStart, Vector2 dEnd, Vector2 sStart, Vector2 sEnd)
{
	Vector2 AO, AP, AB;
	AB.x = dEnd.x - dStart.x;
	AB.y = dEnd.y - dStart.y;
	AP.x = sEnd.x - dStart.x;
	AP.y = sEnd.y - dStart.y;
	AO.x = sStart.x - dStart.x;
	AO.y = sStart.y - dStart.y;
	if ((AB.x*AP.y - AB.y*AP.x)*(AB.x*AO.y - AB.y*AO.x) < 0)
		return true;
	else
		return false;
}

bool PhysicMgr::CollisionSegAndSeg(Vector2 s1Start, Vector2 s1End, Vector2 s2Start, Vector2 s2End)
{
	if (CollisionLineAndSeg(s1Start, s1End, s2Start, s2End) == false)
		return false;
	if (CollisionLineAndSeg(s2Start, s2End, s1Start, s1End) == false)
		return false;
	return true;
}


void PhysicMgr::checkValidityOfPosition(Object* obj)
{
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