#pragma once

class Object
{
public:
	Object();
	~Object();

	const char* getName() const;
	uint32_t getUID() const;
	void showInfo();


};

class Cercle : public Object
{
public:
	Cercle();
	~Cercle();

private:
	Vector2 m_center;
	float	m_radius;
};

class AABB : public Object
{
public:
	AABB();
	~AABB();

private:
	sf::FloatRect m_bound;
};

class OBB : public Object
{
public:
	OBB();
	~OBB();

private:
	sf::FloatRect	m_bound;
	float			m_angle;

};

class KDOP : public Object
{
public:
	KDOP();
	~KDOP();

private:
	std::vector<Vector2> m_points;
};

class Point : public Object
{
public:
	Point();
	~Point();

private:
	Vector2 m_point;
};