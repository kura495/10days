#pragma once
#include "Enemy/Bee/Enemy_Bee.h"
#include "Player/Player.h"

class ObjectManager {
public:
	void Init();
	void Update();
	void Draw();

private:

	std::unique_ptr<Enemy_Bee>enemy_Bee;

	std::unique_ptr<Player>player_;
};