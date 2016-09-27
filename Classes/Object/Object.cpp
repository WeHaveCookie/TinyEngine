#include "stdafx.h"
#include "Object.h"
#include "Manager/Render/RenderMgr.h"
#include "Utils/Random.h"

int Object::s_newUID = 0;

Object::Object(const char* name, Vector2 pos, ObjectType::Enum type)
	:m_uid(s_newUID++),
	m_name(name),
	m_pos(pos),
	m_color(sf::Color::White),
	m_type(type)
{
	m_showInfo = false;
	m_motion = Vector2(randIntBorned(-5, 5), randIntBorned(-5, 5));
}

Object::~Object()
{

}

void Object::displayImGui()
{
	if (m_showInfo)
	{
		std::string label = m_name + " - " +std::to_string(m_uid);
		if (ImGui::Begin(label.c_str(), &m_showInfo, ImGuiWindowFlags_AlwaysAutoResize))
		{
			float x = m_pos.x;
			float y = m_pos.y;
			ImVec4 color = ImVec4(100.0f / 255.0f, 250.0f / 255.0f, 50.0f / 255.0f, 1.0f);
			bool collided = m_color == sf::Color::Red;
			if (collided)
			{
				color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
			}	
			ImGui::TextColored(color, "Collided : %s", (collided)?"True":"False");
			ImGui::Text("Position");
			auto winSize = RenderMgr::getSingleton()->getWindowSize();
			ImGui::SliderFloat("X", &x, 0.0f, (float)winSize.x);
			ImGui::SliderFloat("y", &y, 0.0f, (float)winSize.y);
			setPos(x, y);
		}
	}
}

void Object::setCollided(bool b)
{ 
	if (!b)
	{
		m_color = sf::Color(221, 160, 221);
	}
	else
	{
		m_color = sf::Color::Red;
	}
	setResolve(b); 
}

const char* Object::getName() const
{
	return m_name.c_str();
}

uint32_t Object::getUID() const
{
	return m_uid;
}

void Object::move()
{
	m_pos += m_motion;
	auto winSize = RenderMgr::getSingleton()->getWindowSize();

	if (m_pos.x > (float)winSize.x)
	{
		m_pos.x = (float)winSize.x;
		m_motion.x = -m_motion.x;
	}
	else if (m_pos.x < 0.0f)
	{
		m_pos.x = 0.0f;
		m_motion.x = -m_motion.x;
	}
	
	if (m_pos.y >(float)winSize.y)
	{
		m_pos.y = (float)winSize.y;
		m_motion.y = -m_motion.y;
	}
	else if (m_pos.y < 0.0f)
	{
		m_pos.y = 0.0f;
		m_motion.y = -m_motion.y;
	}
}

Circle::Circle(float x, float y, float radius)
	:Object("Circle", Vector2(x, y), ObjectType::Circle),
	m_radius(radius)
{
	m_shape = sf::CircleShape(m_radius);
	m_shape.setOutlineThickness(2);
	m_shape.setOutlineColor(sf::Color(100, 250, 50));
}

Circle::~Circle()
{

}

void Circle::displayImGui()
{
	if (isShow())
	{
		Object::displayImGui();
		ImGui::InputFloat("Radius", &m_radius);
		m_shape.setRadius(m_radius);
		ImGui::End();
	}
}

void Circle::paint()
{
	m_shape.setPosition(getPos().sf());
	m_shape.setFillColor(getColor());
	RenderMgr::getSingleton()->getMainRenderWindow()->draw(m_shape);
}

Vector2 Circle::getCenter() const
{
	auto bounds = m_shape.getGlobalBounds();
	return Vector2(bounds.left + (bounds.width / 2.0f), bounds.top + (bounds.height / 2.0f));
}

AABB::AABB(float left, float top, float width, float height)
	:Object("AABB", Vector2(left, top), ObjectType::AABB),
	m_shape(sf::Vector2f(left, top))
{
	m_shape.setSize(sf::Vector2f(width, height));
	m_shape.setOutlineThickness(2);
	m_shape.setOutlineColor(sf::Color(100, 250, 50));
}

AABB::~AABB()
{

}

void AABB::displayImGui()
{
	if (isShow())
	{
		Object::displayImGui();
		float width = m_shape.getSize().x;
		float height = m_shape.getSize().y;
		ImGui::InputFloat("Width", &width);
		ImGui::InputFloat("Height", &height);
		m_shape.setSize(sf::Vector2f(width, height));
		ImGui::End();
	}
}

void AABB::paint()
{
	m_shape.setPosition(getPos().sf());
	m_shape.setFillColor(getColor());
	RenderMgr::getSingleton()->getMainRenderWindow()->draw(m_shape);
}

OBB::OBB(float left, float top, float width, float height, float angle)
	:Object("OBB", Vector2(left, top), ObjectType::OBB), 
	m_shape(sf::Vector2f(left, top)),
	m_angle(angle)
{
	m_shape.setSize(sf::Vector2f(width, height));
	m_shape.setRotation(m_angle);
	m_shape.setOutlineThickness(2);
	m_shape.setOutlineColor(sf::Color(100, 250, 50));
}

OBB::~OBB()
{

}

void OBB::displayImGui()
{
	if (isShow())
	{
		Object::displayImGui();
		float width = m_shape.getSize().x;
		float height = m_shape.getSize().y;
		float angle = m_shape.getRotation() * DEGTORAD;
		ImGui::InputFloat("Width", &width);
		ImGui::InputFloat("Height", &height);
		m_shape.setSize(sf::Vector2f(width, height));
		ImGui::SliderAngle("Angle", &angle);
		m_shape.setRotation(angle * RADTODEG);
		ImGui::End();
	}
}

void OBB::paint()
{
	m_shape.setPosition(getPos().sf());
	m_shape.setFillColor(getColor());
	RenderMgr::getSingleton()->getMainRenderWindow()->draw(m_shape);
}

KDOP::KDOP(float x, float y, int nbrPoints)
	:Object("KDOP", Vector2(x, y), ObjectType::KDOP),
	m_points(nbrPoints)
{
	m_shape.setPointCount(m_points);
	m_currentPoint = 0;
	m_point[0] = 0.0f;
	m_point[1] = 0.0f;
	m_shape.setOutlineThickness(2);
	m_shape.setOutlineColor(sf::Color(100, 250, 50));
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

void KDOP::displayImGui()
{
	if (isShow())
	{
		Object::displayImGui();
		int last = m_points;
		ImGui::InputInt("Points Number", &m_points);
		if (last > m_points)
		{
			m_currentPoint = m_points - 1;
		}
		m_shape.setPointCount(m_points);
		ImGui::InputFloat2("Position", m_point);
		ImGui::SameLine();
		if (ImGui::Button("Add Point"))
		{
			addPoint(Vector2(m_point[0], m_point[1]));
		}
		for (int i = 0; i < m_points; i++)
		{
			ImGui::Text("Point %i : x = %f | y = %f", i, m_shape.getPoint(i).x, m_shape.getPoint(i).y);
		}
		ImGui::End();
	}
}

void KDOP::paint()
{
	m_shape.setPosition(getPos().sf());
	m_shape.setFillColor(getColor());
	RenderMgr::getSingleton()->getMainRenderWindow()->draw(m_shape);
}

Point::Point(float x, float y)
	:Object("Point", Vector2(x, y), ObjectType::Point),
	m_shape(sf::Vector2f(1.0f, 1.0f))
{
	m_shape.setPosition(getPos().sf());
}

Point::~Point()
{

}

void Point::displayImGui()
{
	if (isShow())
	{
		Object::displayImGui();
		ImGui::End();
	}
}

void Point::paint()
{
	m_shape.setPosition(getPos().sf());
	m_shape.setFillColor(getColor());
	RenderMgr::getSingleton()->getMainRenderWindow()->draw(m_shape);
}