#pragma once
#include"Scenes/State/IgameState.h"
#include "Input/Input.h"
#include "WorldTransform/WorldTransform.h"
#include "Editer/Editer.h"
#include "Object/Camera/FollowCamera.h"
#include "ParticleSystem/ParticleSystem.h"
#include "Enemy/Bee/Enemy_Bee.h"
#include "Player/Player.h"

// -- ゲームシーン -- //
class PlayState :
    public IGameState
{
public:

	PlayState() { StateNo = GameStateNo::PLAY; };
	void Init()override;
	void Update()override;
	void Draw()override;
private:
	std::unique_ptr<FollowCamera>followCamera;
	
	// プレイヤー
	std::unique_ptr<Player> player;
	std::vector<Model*> playerModel_;

	// 床
	std::unique_ptr<Model> floor_;

	std::unique_ptr<Enemy_Bee>enemy_Bee;

};

