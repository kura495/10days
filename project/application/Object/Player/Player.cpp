#include "Player.h"
#include "Engine/Input/Input.h"

void Player::Init(std::string mainTexName, Vector2 texSize) {
	mainTex_TF.Init();
	mainTex_TF.transform.translate = { 640.0f,32.0f };
	mainTex_Sprite = std::make_unique<Sprite>();
	mainTex_Sprite->Init({ 0.5f,0.5f }, texSize);
	mainTex_Sprite->TextureHandle = TextureManager::GetInstance()->LoadTexture(mainTexName);
}

void Player::Update() {

	// -- 入力 -- //

	// 移動量の算出
	Vector2 addVel = Input::GetInstance()->GetPadTrreger();

	mainTex_TF.transform.translate.x += addVel.x;

	// ジャンプ
	if (Input::GetInstance()->pushPad(XINPUT_GAMEPAD_B)) {
		mainTex_TF.transform.translate.y += 1.0f;
	}



#pragma region
	ImGui::Begin("Player");
	ImGui::DragFloat3("TexPos", &mainTex_TF.transform.translate.x);
	ImGui::End();
#pragma endregion

	mainTex_TF.Update();

}

void Player::Draw() {
	mainTex_Sprite->RendererDraw(mainTex_TF);
}