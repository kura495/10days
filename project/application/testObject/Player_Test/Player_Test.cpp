#include "Player_Test.h"

void Player_Test::Init() {

	mainModel_3D = std::make_unique<Model>();
	mainModel_3D->Init("project/resources/Box/", "Box.obj");
	worldTF_3Dmodel_.Init();

}

void Player_Test::Update() {

#ifdef _DEBUG
	ImGui::Begin("Testa");
	ImGui::DragFloat3("TF", &worldTF_3Dmodel_.transform.translate.x);
	ImGui::End();
#endif
	worldTF_3Dmodel_.Update();

}

void Player_Test::Draw() {

	mainModel_3D->RendererDraw(worldTF_3Dmodel_);

}