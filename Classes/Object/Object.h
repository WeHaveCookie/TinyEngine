#pragma once

class Object
{
public:
	Object();
	~Object();

	const char* getName() const;
	uint32_t getUID() const;
	void showInfo();
	void paint();
};

class Circle : public Object
{
public:
	Circle(float radius);
	~Circle();

	const sf::CircleShape getShape() const { return m_shape; }

private:
	sf::CircleShape m_shape;
};

class AABB : public Object
{
public:
	AABB(float left, float top, float width, float height);
	AABB(Vector2 pos, Vector2 size);
	~AABB();

	const sf::FloatRect getShape() const { return m_shape; }

private:
	sf::FloatRect m_shape;
};

class OBB : public Object
{
public:
	OBB(float left, float top, float width, float height, float angle);
	OBB(AABB box, float angle);
	~OBB();

	const sf::FloatRect getShape() const { return m_shape; }
	const float getAngle() const { return m_angle; }

private:
	sf::FloatRect	m_bound;
	float			m_angle;

};

class KDOP : public Object
{
public:
	KDOP(int nbrPoints);
	~KDOP();

	void addPoint(Vector2 pos);
	const sf::ConvexShape getShape() const { return m_shape; }

private:
	sf::ConvexShape m_shape;
	int m_points;
	int m_currentPoint;
};

class Point : public Object
{
public:
	Point(float x, float y);
	~Point();

	const Vector2 getCoord() const { return m_coord; }

private:
	Vector2 m_coord;
};