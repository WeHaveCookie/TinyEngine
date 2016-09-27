#pragma once

#include "Manager/Manager.h"

class Object;

class PhysicMgr : public Manager
{
    public:

		static PhysicMgr* getSingleton() { return s_singleton; }

        PhysicMgr();
        virtual ~PhysicMgr();

		void init();
		void process(const float dt);
		void end();
		void showImGuiWindow(bool* window);

		void registerObject(Object* obj);
		void unregisterObject(Object* obj);

        // Function
		static bool CollisionCircleAndCircle(Object* circle1, Object* circle2);
        static bool CollisionAABBandAABB(sf::FloatRect box1, sf::FloatRect box2);
		static bool CollisionOBBAndOBB(Object* obb1, Object* obb2);
		static bool CollisionKDOPAndKDOP(Object* kdop1, Object* kdop2);
		static bool CollisionPointAndPoint(Object* point1, Object* point2);
		
		static bool CollisionCircleAndAABB(Object* circle, sf::FloatRect aabbBounds);
		static bool CollisionCircleAndOBB(Object* circle, Object* obb);
		static bool CollisionCircleAndKDOP(Object* circle, Object* kdop);
		static bool CollisionCircleAndPoint(Object* circle, Vector2 pointPos);

		static bool CollisionAABBAndCircle(sf::FloatRect box1, sf::CircleShape circleShape);
		static bool CollisionAABBAndOBB(Object* aabb, Object* obb);
		static bool CollisionAABBAndKDOP(Object* aabb, Object* obb);
		static bool CollisionAABBAndPoint(sf::FloatRect box, Vector2 pointPos);

		static bool CollisionOBBAndKDOP(Object* obb, Object* kdop);
		static bool CollisionOBBAndPoint(Object* obb, Object* point);
		static bool CollisionKDOPAndPoint(Object* kdop, Vector2 pointPos, int count = 0);
		
        
		sf::Time getProcessTime() { return m_processTime; }

		void setFramerate(int framerate);

        // Inline
    protected:
	private:
		static PhysicMgr* s_singleton;
		void checkValidityOfPosition(Object* ent);
		void processRegisteryQueue();

		std::vector<Object*>							m_objects;
		class RegisteryQueue;
		RegisteryQueue*									m_registeryQueue;
		sf::Time										m_processTime;
		bool											m_enable;
		int												m_framerate;
		bool											m_animate;
};
