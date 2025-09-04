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

	worldTF_bg.Init();
	worldTF_bg.transform.translate = {640.0f,360.0f,0.0f,};
	worldTF_bg.Update();
	sprite_bg = std::make_unique<Sprite>();
	sprite_bg->Init({0.5f,0.5f},{ 640.0f,360.0f });
	sprite_bg->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/BlackTexture.png");

	mainModel_3D = std::make_unique<Model>();
	mainModel_3D->Init("project/resources/Cube/", "Cube.obj");
	worldTF_3Dmodel_.Init();

}

void TestState::Update()
{
	Renderer::SetViewProj(followCamera->GetParameter());
	objectManager_->Update();
#ifdef _DEBUG
	ImGui::Begin("Test");
	ImGui::DragFloat3("TF",&worldTF_bg.transform.translate.x);
	ImGui::End();
#endif
	worldTF_bg.Update();

}

void TestState::Draw()
{
	//objectManager_->Draw();

	sprite_bg->RendererDraw(worldTF_bg);

	mainModel_3D->RendererDraw(worldTF_3Dmodel_);
}