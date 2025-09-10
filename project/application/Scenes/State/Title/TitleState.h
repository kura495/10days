#pragma once
//ゲームクリアシーン//
#include"Scenes/State/IgameState.h"
#include "Input/Input.h"
#include "WorldTransform/WorldTransform.h"
#include "Editer/Editer.h"
#include "Object/Camera/FollowCamera.h"
#include "ParticleSystem/ParticleSystem.h"

class TitleState :public IGameState
{
public:	
	TitleState() { StateNo = GameStateNo::TITLE; };
	void Init()override;
	void Update()override;
	void Draw()override;
private:
	std::unique_ptr<FollowCamera>followCamera;

	//タイトル用画像
	std::unique_ptr<Sprite> titleSprite;
	WorldTransform title;

	std::unique_ptr<Sprite>texture;
	std::unique_ptr<Sprite>instructions;

	Model* skydome_ = nullptr;

	bool instructionsScene = false;

};
