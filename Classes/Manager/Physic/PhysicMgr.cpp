#include "stdafx.h"
#include "PhysicMgr.h"
#include "blockingconcurrentqueue.h"
#include "Object/Object.h"
#include "TinyEngineManagers.h"
#include "Utils/Random.h"

PhysicMgr* PhysicMgr::s_singleton = NULL;

class PhysicMgr::RegisteryQueue : public moodycamel::BlockingConcurrentQueue<Object*> {};

PhysicMgr::PhysicMgr()
	:Manager(ManagerType::Enum::Physic)
{
	s_singleton = this;
	m_enable = true;
	m_animate = false;
	m_registeryQueue = new RegisteryQueue();
}

PhysicMgr::~PhysicMgr()
{
	delete m_registeryQueue;
}

void PhysicMgr::init()
{
	m_processTime = sf::Time::Zero;
	m_framerate = 60;
}

void PhysicMgr::process(const float dt)
{
	sf::Clock clock;
	processRegisteryQueue();
	for (auto& object : m_objects)
	{
		object->setCollided(false);
		if (m_animate)
		{
			object->move();
		}
	}
	if (m_enable)
	{

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
		int framerate = m_framerate;
		ImGui::InputInt("Framerate", &framerate);
		setFramerate(framerate);

		ImGui::Text("Time per Frame : %f", g_realTime);
		ImGui::Checkbox("Enable Phys", &m_enable);
		ImGui::Checkbox("Animate", &m_animate);
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


bool PhysicMgr::CollisionCircleAndCircle(Object* circle1, Object* circle2)
{
	Circle* circle1Ptr = dynamic_cast<Circle*>(circle1);
	Vector2 circlePos1 = circle1Ptr->getCenter();
	float radius1 = circle1Ptr->getRadius();
	Circle* circle2Ptr = dynamic_cast<Circle*>(circle2);
	Vector2 circlePos2 = circle2Ptr->getCenter();
	float radius2 = circle2Ptr->getRadius();

	int d2 = (circlePos1.x - circlePos2.x)*(circlePos1.x - circlePos2.x) + (circlePos1.y - circlePos2.y)*(circlePos1.y - circlePos2.y);
	if (d2 > (radius1 + radius2)*(radius1 + radius2))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool PhysicMgr::CollisionAABBandAABB(sf::FloatRect box1, sf::FloatRect box2)
{
	return !((box2.left >= box1.left + box1.width) ||
		(box2.left + box2.width <= box1.left) ||
		(box2.top >= box1.top + box1.height) ||
		(box2.top + box2.height <= box1.top));
}

bool PhysicMgr::CollisionOBBAndOBB(Object* obb1, Object* obb2)
{
	return false;
}

bool PhysicMgr::CollisionKDOPAndKDOP(Object* kdop1, Object* kdop2)
{
	return false;
}

bool PhysicMgr::CollisionPointAndPoint(Object* point1, Object* point2)
{
	return false;
}

int ProjectionSurSegment(int Cx, int Cy, int Ax, int Ay, int Bx, int By)
{
	int ACx = Cx - Ax;
	int ACy = Cy - Ay;
	int ABx = Bx - Ax;
	int ABy = By - Ay;
	int BCx = Cx - Bx;
	int BCy = Cy - By;
	int s1 = (ACx*ABx) + (ACy*ABy);
	int s2 = (BCx*ABx) + (BCy*ABy);
	if (s1*s2 > 0)
	{
		return 0;
	}
	return 1;
}

bool PhysicMgr::CollisionCircleAndAABB(Object* circle, sf::FloatRect aabbBounds)
{
	Circle* circlePtr = dynamic_cast<Circle*>(circle);
	sf::CircleShape circleShape = circlePtr->getShape();
	sf::FloatRect circleBound = circleShape.getGlobalBounds();

	if (CollisionAABBandAABB(aabbBounds, circleBound) == false)
	{
		return false;
	}

	Vector2 topLeft(aabbBounds.left, aabbBounds.top);
	Vector2 topRigt(aabbBounds.left, aabbBounds.top + aabbBounds.height);
	Vector2 bottomLeft(aabbBounds.left + aabbBounds.width, aabbBounds.height);
	Vector2 bottomRight(aabbBounds.left + aabbBounds.width, aabbBounds.top + aabbBounds.height);
	Vector2 posCircle = circlePtr->getCenter();

	if (CollisionCircleAndPoint(circle, topLeft) == 1
		|| CollisionCircleAndPoint(circle, topRigt) == 1
		|| CollisionCircleAndPoint(circle, bottomLeft) == 1
		|| CollisionCircleAndPoint(circle, bottomRight) == 1)
	{
		return true;
	}
	if (CollisionAABBAndPoint(aabbBounds, posCircle) == 1)
	{
		return true;
	}
	int projvertical = ProjectionSurSegment(posCircle.x, posCircle.y, aabbBounds.left, aabbBounds.top, aabbBounds.left, aabbBounds.top + aabbBounds.height);
	int projhorizontal = ProjectionSurSegment(posCircle.x, posCircle.y, aabbBounds.left, aabbBounds.top, aabbBounds.left + aabbBounds.width, aabbBounds.top);
	if (projvertical == 1 || projhorizontal == 1)
	{
		return true;
	}
	return false;  
}

bool PhysicMgr::CollisionCircleAndOBB(Object* circle, Object* obb)
{
	return false;
}

bool PhysicMgr::CollisionCircleAndKDOP(Object* circle, Object* kdop)
{
	return false;
}

bool PhysicMgr::CollisionCircleAndPoint(Object* circle, Vector2 pointPos)
{
	auto circlePtr = dynamic_cast<Circle*>(circle);
	Vector2 circlePos = circlePtr->getCenter();
	float radius = circlePtr->getRadius();
	int d2 = (pointPos.x - circlePos.x)*(pointPos.x - circlePos.x) + (pointPos.y - circlePos.y)*(pointPos.y - circlePos.y);
	if (d2 >= radius*radius)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool PhysicMgr::CollisionAABBAndCircle(sf::FloatRect box1, sf::CircleShape circleShape)
{
	float d2 = (box1.left - circleShape.getPosition().x)*(box1.left - circleShape.getPosition().x) + (box1.top - circleShape.getPosition().y)*(box1.top - circleShape.getPosition().y);
	if (d2 > circleShape.getRadius() * circleShape.getRadius())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool PhysicMgr::CollisionAABBAndOBB(Object* aabb, Object* obb)
{
	return false;
}

bool PhysicMgr::CollisionAABBAndKDOP(Object* aabb, Object* obb)
{
	return false;
}

bool PhysicMgr::CollisionAABBAndPoint(sf::FloatRect box, Vector2 pointPos)
{
	if (pointPos.x >= box.left
		&& pointPos.x < box.left + box.width
		&& pointPos.y >= box.top
		&& pointPos.y < box.top + box.height)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PhysicMgr::CollisionOBBAndKDOP(Object* obb, Object* kdop)
{
	return false;
}

bool PhysicMgr::CollisionOBBAndPoint(Object* obb, Object* point)
{
	return false;
}

int intersectsegment(sf::Vector2f A, sf::Vector2f B, sf::Vector2f I, sf::Vector2f P)
{
	Vector2 D, E;
	D.x = B.x - A.x;
	D.y = B.y - A.y;
	E.x = P.x - I.x;
	E.y = P.y - I.y;
	double denom = D.x*E.y - D.y*E.x;
	if (denom == 0)
	{
		return -1;
	}
	double t = -(A.x*E.y - I.x*E.y - E.x*A.y + E.x*I.y) / denom;
	if (t < 0 || t >= 1)
	{
		return 0;
	}
	double u = -(-D.x*A.y + D.x*I.y + D.y*A.x - D.y*I.x) / denom;
	if (u < 0 || u >= 1)
	{
		return 0;
	}
	return 1;
}


bool PhysicMgr::CollisionKDOPAndPoint(Object* kdop, Vector2 pointPos, int count)
{
	sf::ConvexShape kdopShape = dynamic_cast<KDOP*>(kdop)->getShape();
	int i;
	sf::Vector2f I = sf::Vector2f(randIntBorned(2000,10100), randIntBorned(2000, 10100));
	int nbintersections = 0;
	int nbp = kdopShape.getPointCount();
	sf::Vector2f kdopPos = kdopShape.getPosition();

	for (i = 0; i < nbp; i++)
	{
		sf::Vector2f A = kdopShape.getPoint(i) + kdopPos;
		sf::Vector2f B;
		if (i == nbp - 1)
		{
			B = kdopShape.getPoint(0) + kdopPos;
		}
		else
		{
			B = kdopShape.getPoint(i + 1) + kdopPos;
		}
		int iseg = intersectsegment(A, B, I, pointPos.sf());
		if (iseg == -1)
		{
			if (count > 10)
			{
				return false;
			}
			else
			{
				return CollisionKDOPAndPoint(kdop, pointPos, ++count);
			}
		}
		nbintersections += iseg;
	}
	if (nbintersections % 2 == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void PhysicMgr::checkValidityOfPosition(Object* obj)
{
	// Check Collision
	if (!obj->isResolved())
	{
		for (auto& collider : m_objects)
		{
			if (obj->getUID() != collider->getUID())
			{
				bool colliding = false;
				auto objType = obj->getType();
				auto colliderType = collider->getType();

				if (objType == ObjectType::Circle && colliderType == ObjectType::Circle)
				{
					colliding = CollisionCircleAndCircle(obj, collider);
				}
				else if (objType == ObjectType::AABB && colliderType == ObjectType::AABB)
				{
					colliding = CollisionAABBandAABB(dynamic_cast<AABB*>(obj)->getShape().getGlobalBounds(), dynamic_cast<AABB*>(collider)->getShape().getGlobalBounds());
				}
				else if (objType == ObjectType::OBB && colliderType == ObjectType::OBB)
				{
					colliding = CollisionOBBAndOBB(obj, collider);
				}
				else if (objType == ObjectType::KDOP && colliderType == ObjectType::KDOP)
				{
					colliding = CollisionKDOPAndKDOP(obj, collider);
				}
				else if (objType == ObjectType::Point && colliderType == ObjectType::Point)
				{
					colliding = CollisionPointAndPoint(obj, collider);
				}
				else if (objType == ObjectType::Circle && colliderType == ObjectType::AABB)
				{
					colliding = CollisionCircleAndAABB(obj, dynamic_cast<AABB*>(collider)->getShape().getGlobalBounds());
				}
				else if (objType == ObjectType::AABB && colliderType == ObjectType::Circle)
				{
					colliding = CollisionCircleAndAABB(collider, dynamic_cast<AABB*>(obj)->getShape().getGlobalBounds());
				}
				else if (objType == ObjectType::Circle && colliderType == ObjectType::OBB)
				{
					colliding = CollisionCircleAndOBB(obj, collider);
				}
				else if (objType == ObjectType::OBB && colliderType == ObjectType::Circle)
				{
					colliding = CollisionCircleAndOBB(collider, obj);
				}
				else if (objType == ObjectType::Circle && colliderType == ObjectType::KDOP)
				{
					colliding = CollisionCircleAndKDOP(obj, collider);
				}
				else if (objType == ObjectType::KDOP && colliderType == ObjectType::Circle)
				{
					colliding = CollisionCircleAndKDOP(collider, obj);
				}
				else if (objType == ObjectType::Circle && colliderType == ObjectType::Point)
				{
					colliding = CollisionCircleAndPoint(obj, collider->getPos());
				}
				else if (objType == ObjectType::Point && colliderType == ObjectType::Circle)
				{
					colliding = CollisionCircleAndPoint(collider, obj->getPos());
				}
				else if (objType == ObjectType::AABB && colliderType == ObjectType::OBB)
				{
					colliding = CollisionAABBAndOBB(obj, collider);
				}
				else if (objType == ObjectType::OBB && colliderType == ObjectType::AABB)
				{
					colliding = CollisionAABBAndOBB(collider, obj);
				}
				else if (objType == ObjectType::AABB && colliderType == ObjectType::KDOP)
				{
					colliding = CollisionAABBAndKDOP(obj, collider);
				}
				else if (objType == ObjectType::KDOP && colliderType == ObjectType::AABB)
				{
					colliding = CollisionAABBAndKDOP(collider, obj);
				}
				else if (objType == ObjectType::AABB && colliderType == ObjectType::Point)
				{
					colliding = CollisionAABBAndPoint(dynamic_cast<AABB*>(obj)->getShape().getGlobalBounds(), collider->getPos());
				}
				else if (objType == ObjectType::Point && colliderType == ObjectType::AABB)
				{
					colliding = CollisionAABBAndPoint(dynamic_cast<AABB*>(collider)->getShape().getGlobalBounds(), obj->getPos());
				}
				else if (objType == ObjectType::OBB && colliderType == ObjectType::KDOP)
				{
					colliding = CollisionOBBAndKDOP(obj, collider);
				}
				else if (objType == ObjectType::KDOP && colliderType == ObjectType::OBB)
				{
					colliding = CollisionOBBAndKDOP(collider, obj);
				}
				else if (objType == ObjectType::OBB && colliderType == ObjectType::Point)
				{
					colliding = CollisionOBBAndPoint(obj, collider);
				}
				else if (objType == ObjectType::Point && colliderType == ObjectType::OBB)
				{
					colliding = CollisionOBBAndPoint(collider, obj);
				}
				else if (objType == ObjectType::KDOP && colliderType == ObjectType::Point)
				{
					colliding = CollisionKDOPAndPoint(obj, collider->getPos());
				}
				else if (objType == ObjectType::Point && colliderType == ObjectType::KDOP)
				{
					colliding = CollisionKDOPAndPoint(collider, obj->getPos());
				}

				if (colliding)
				{
					obj->setCollided(true);
					collider->setCollided(true);
				}
			}
		}
	}
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

void PhysicMgr::setFramerate(int framerate)
{
	if (framerate != m_framerate)
	{
		m_framerate = framerate;
		g_Framerate = 1.0 / m_framerate;
	}
}