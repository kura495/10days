#include "ObjectManager.h"

void ObjectManager::Init() {

	enemy_Bee = std::make_unique<Enemy_Bee>();
	enemy_Bee->Init("project/resources/BlackTexture.png", { 64.0f,64.0f });

	//player_ = std::make_unique<Player>();
	//player_->Init("project/resources/BlackTexture.png", { 64.0f,64.0f });

}

void ObjectManager::Update() {
	enemy_Bee->Update();
	//player_->Update();
}

void ObjectManager::Draw() {
	enemy_Bee->Draw();
	//player_->Draw();
}