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

private:
	static ObjectMgr* s_singleton;
	std::vector<Object*> m_objects;

};