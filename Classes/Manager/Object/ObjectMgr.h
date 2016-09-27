#pragma once

#include "Manager/Manager.h"

class Object;

class  ObjectMgr : public Manager
{
public:

	static ObjectMgr* getSingleton() { return s_singleton; }

	ObjectMgr();
	~ObjectMgr();

	void init();
	void process(const float dt);
	void end();
	void paint();
	void paintGui();

	uint32_t addCircle(float x, float y, float radius);
	uint32_t addAABB(float left, float top, float width, float height);
	uint32_t addOBB(float left, float top, float width, float height, float angle);
	uint32_t createKDOP(float x, float y, int nbrPoints);
	void addPointKDOP(uint32_t id, Vector2 pos);
	uint32_t addPoint(float x, float y);
	Object* getObject(uint32_t id);
	void removeObject(uint32_t id);

	uint32_t addRandom();
	void clear();

	void registerObject(Object* obj);
	void unregisterObject(Object* obj);

	void showImGuiWindow(bool* display);

private:
	static ObjectMgr* s_singleton;
	std::vector<Object*> m_objects;
	float m_left;
	float m_top;
	float m_width;
	float m_height;
	float m_angle;
	float m_radius;
	int m_nbrPoints;
	int m_populate;
};