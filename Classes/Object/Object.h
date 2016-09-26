#pragma once

class Object
{
public:
	Object();
	~Object();

	const char* getName() const;
	uint32_t getUID() const;
	void showInfo();

private:

};