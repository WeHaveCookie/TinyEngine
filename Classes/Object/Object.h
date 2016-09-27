#pragma once

namespace ObjectType
{
	enum Enum
	{
		Circle = 0,
		AABB,
		OBB,
		KDOP,
		Point
	};
}

class Object
{
public:
	Object(const char* name, Vector2 pos, ObjectType::Enum type);
	~Object();

	const char* getName() const;
	uint32_t getUID() const;
	void showInfo() { m_showInfo = !m_showInfo; };
	void closeInfo() { m_showInfo = false; }
	virtual void displayImGui();
	virtual void paint() = 0;
	const ObjectType::Enum getType() const { return m_type; }
	void setCollided(bool b);
	const sf::Color getColor() { return m_color; }
	void setPos(float x, float y) { m_pos = Vector2(x, y); }
	const bool isShow() const { return m_showInfo; }
	Vector2 getPos() const { return m_pos; }
	void move();
	void setResolve(bool b) { m_collideResolve = b; }
	const bool isResolved() const { return m_collideResolve; }

private:
	static int			s_newUID;
	int					m_uid;
	std::string			m_name;
	bool				m_showInfo;
	Vector2				m_pos;
	sf::Color			m_color;
	ObjectType::Enum	m_type;
	Vector2				m_motion;
	bool				m_collideResolve;
};

class Circle : public Object
{
public:
	Circle(float x, float y, float radius);
	~Circle();

	virtual void displayImGui();
	virtual void paint();
	Vector2 getCenter() const;
	const sf::CircleShape getShape() const { return m_shape; }
	const float getRadius() const { return m_radius; }

private:
	sf::CircleShape m_shape;
	float m_radius;
};

class AABB : public Object
{
public:
	AABB(float left, float top, float width, float height);
	~AABB();

	virtual void displayImGui();
	virtual void paint();
	const sf::RectangleShape getShape() const { return m_shape; }

private:
	sf::RectangleShape m_shape;
};

class OBB : public Object
{
public:
	OBB(float left, float top, float width, float height, float angle);
	~OBB();

	virtual void displayImGui();
	virtual void paint();
	const sf::RectangleShape getShape() const { return m_shape; }
	const float getAngle() const { return m_angle; }

private:
	sf::RectangleShape	m_shape;
	float				m_angle;

};

class KDOP : public Object
{
public:
	KDOP(float x, float y, int nbrPoints);
	~KDOP();

	virtual void displayImGui();
	virtual void paint();
	void addPoint(Vector2 pos);
	const sf::ConvexShape getShape() const { return m_shape; }

private:
	sf::ConvexShape m_shape;
	int m_points;
	int m_currentPoint;
	float m_point[2];
};

class Point : public Object
{
public:
	Point(float x, float y);
	~Point();

	virtual void displayImGui();
	virtual void paint();

private:
	sf::RectangleShape m_shape;
};