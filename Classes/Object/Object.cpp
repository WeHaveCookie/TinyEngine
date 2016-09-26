#include "stdafx.h"
#include "Object.h"

Object::Object()
{

}

Object::~Object()
{

}

void Object::showInfo()
{

}

const char* Object::getName() const
{
	return "";
}

uint32_t Object::getUID() const
{
	return 0;
}

Circle::Circle(float radius)
	:m_shape(radius)
{
}

Circle::~Circle()
{

}

AABB::AABB(float left, float top, float width, float height)
	:m_shape(left, top, width, height)
{

}

AABB::AABB(Vector2 pos, Vector2 size)
	:m_shape(pos, size)
{

}

AABB::~AABB()
{

}

OBB::OBB(float left, float top, float width, float height, float angle)
	:m_shape(left, top, width, height),
	m_angle(angle)
{

}

OBB::OBB(AABB box, float angle)
	:m_shape(box.getShape()),
	m_angle(angle)
{

}

OBB::~OBB()
{

}

KDOP::KDOP(int nbrPoints)
{
	m_shape.setPointCount(nbrPoints);
	m_points = nbrPoints;
	m_currentPoint = 0;
}

KDOP::~KDOP()
{

}

void KDOP::addPoint(Vector2 pos)
{
	if (m_currentPoint < m_points)
	{
		m_shape.setPoint(m_currentPoint++, pos.sf());
	}
}

Point::Point(float x, float y)
	:m_coord(x,y)
{

}

Point::~Point()
{

}