#include "Test.h"

void TestState::Init()
{
	StateNo = GameStateNo::TEST;

	Editer::GetInstance()->SetViewProjection(&Renderer::GetViewProjection());
	Editer::GetInstance()->IsEnable(true);

	followCamera = std::make_unique<FollowCamera>();
	followCamera->Initialize();
	followCamera->GetParameter().translation_ = { 0.0f,0.0f,-10.0f };

	enemy_Bee = std::make_unique<Enemy_Bee>();
	//enemy_Bee->Init("project/resources/BlackTexture.png", { 64.0f,64.0f });
}

void TestState::Update()
{
	Renderer::SetViewProj(followCamera->GetParameter());
	enemy_Bee->Update();
}

void TestState::Draw()
{
	enemy_Bee->Draw();
}