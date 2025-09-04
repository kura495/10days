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

	mainModel_3D = std::make_unique<Model>();
	mainModel_3D->Init("project/resources/Plane/", "Plane.obj");
	worldTF_3Dmodel_.Init();

}

void TestState::Update()
{
	Renderer::SetViewProj(followCamera->GetParameter());
	objectManager_->Update();
#ifdef _DEBUG
	ImGui::Begin("Test");
	ImGui::DragFloat3("TF",&worldTF_3Dmodel_.transform.translate.x);
	ImGui::End();
#endif
	worldTF_3Dmodel_.Update();

}

void TestState::Draw()
{

	objectManager_->Draw();

	mainModel_3D->RendererDraw(worldTF_3Dmodel_);

}