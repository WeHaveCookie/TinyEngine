#include "stdafx.h"
#include "ObjectMgr.h"
#include "Object/Object.h"

ObjectMgr* ObjectMgr::s_singleton = NULL;

ObjectMgr::ObjectMgr()
	:Manager(ManagerType::Enum::Object)
{
	s_singleton = this;
}

ObjectMgr::~ObjectMgr()
{

}

void ObjectMgr::init()
{

}

void ObjectMgr::process(const float dt)
{

}

void ObjectMgr::end()
{

}

void ObjectMgr::paint()
{

}