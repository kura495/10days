#pragma once
#pragma once
//ゲームクリアシーン//
#include"Scenes/State/IgameState.h"
#include "Input/Input.h"
#include "WorldTransform/WorldTransform.h"
#include "Editer/Editer.h"
#include "Object/Camera/FollowCamera.h"
#include "ParticleSystem/ParticleSystem.h"
#include "Enemy/Bee/Enemy_Bee.h"

class TestState :public IGameState
{
public:
	TestState() { StateNo = GameStateNo::TEST; };
	void Init()override;
	void Update()override;
	void Draw()override;
private:
	std::unique_ptr<FollowCamera>followCamera;

	std::unique_ptr<Enemy_Bee>enemy_Bee;
};