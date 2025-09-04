#include "ObjectManager.h"

void ObjectManager::Init() {

	player_Test = std::make_unique<Player_Test>();
	player_Test->Init();

}

void ObjectManager::Update() {

	player_Test->Update();

}

void ObjectManager::Draw() {

	player_Test->Draw();

}