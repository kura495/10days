#pragma once
#include "Enemy/Bee/Enemy_Bee.h"
#include "testObject/Player_Test/Player_Test.h"

class ObjectManager {
public:

	void Init();
	void Update();
	void Draw();

private:

	std::unique_ptr<Player_Test>player_Test;

};