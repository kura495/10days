#include "PlayState.h"

void PlayState::Init()
{
	StateNo = GameStateNo::PLAY;

	Editer::GetInstance()->SetViewProjection(&Renderer::GetViewProjection());
	Editer::GetInstance()->IsEnable(true);

	followCamera = std::make_unique<FollowCamera>();
	followCamera->Initialize();
	followCamera->GetParameter().translation_ = { 0.0f,0.0f,-10.0f };

	enemy_Bee = std::make_unique<Enemy_Bee>();
	enemy_Bee->Init("project/resources/BlackTexture.png", { 64.0f,64.0f });


	// プレイヤーの初期化
	player = std::make_unique<Player>();
	player->Init("project/resources/BlackTexture.png", { 64.0f,64.0f });

}

void PlayState::Update()
{
	Renderer::SetViewProj(followCamera->GetParameter());
	enemy_Bee->Update();
	player->Update();
}

void PlayState::Draw()
{
	enemy_Bee->Draw();
	player->Draw();
}