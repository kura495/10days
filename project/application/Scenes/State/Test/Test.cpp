#include "Test.h"

void TestState::Init()
{
	StateNo = GameStateNo::TEST;

	Editer::GetInstance()->SetViewProjection(&Renderer::GetViewProjection());
	Editer::GetInstance()->IsEnable(true);

	followCamera = std::make_unique<FollowCamera>();
	followCamera->Initialize();
	followCamera->GetParameter().translation_ = { 0.0f,0.0f,-10.0f };

	objectManager_ = std::make_unique<ObjectManager>();
	objectManager_->Init();
}

void TestState::Update()
{
	Renderer::SetViewProj(followCamera->GetParameter());
	objectManager_->Update();


}

void TestState::Draw()
{
	objectManager_->Draw();
}