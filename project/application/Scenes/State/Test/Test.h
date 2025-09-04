#pragma once
#pragma once
//ゲームクリアシーン//
#include"Scenes/State/IgameState.h"
#include "Input/Input.h"
#include "WorldTransform/WorldTransform.h"
#include "Editer/Editer.h"
#include "Object/Camera/FollowCamera.h"
#include "ParticleSystem/ParticleSystem.h"
#include "Object/Manager/ObjectManager.h"


class TestState :public IGameState
{
public:
	TestState() { StateNo = GameStateNo::TEST; };
	void Init()override;
	void Update()override;
	void Draw()override;
private:
	//タイトル用画像
	std::unique_ptr<Sprite> sprite_bg;
	WorldTransform worldTF_bg;

	std::unique_ptr<FollowCamera>followCamera;

	std::unique_ptr<ObjectManager>objectManager_;

	std::unique_ptr<Model> mainModel_3D;
	WorldTransform worldTF_3Dmodel_;

};