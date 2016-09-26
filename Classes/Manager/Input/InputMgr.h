#pragma once

#include "Manager/Manager.h"

class InputMgr : public Manager
{

	public:

		static InputMgr* getSingleton() { return s_singleton; }

		InputMgr();
		~InputMgr();

		void init();
		void process(const float dt);
		void end();

	private:
		static InputMgr*							s_singleton;
};