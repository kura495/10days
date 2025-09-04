#include "Player.h"

void Player::Init(std::string mainTexName, Vector2 texSize) {
	mainTex_TF.Init();
	mainTex_TF.transform.translate = { 640.0f,32.0f };
	mainTex_Sprite = std::make_unique<Sprite>();
	mainTex_Sprite->Init({ 0.5f,0.5f }, texSize);
	mainTex_Sprite->TextureHandle = TextureManager::GetInstance()->LoadTexture(mainTexName);
}

void Player::Update() {

	// -- 入力 -- //

	// 入力内容の取得
	Input::GetInstance()->GetJoystickState(joyState);

	//移動量
	if (joyState.Gamepad.sThumbLX != 0 && joyState.Gamepad.sThumbLY != 0) {
		
		//スティックから移動量を計算
		Math::Vector2 move = {
		(float)joyState.Gamepad.sThumbLX / SHRT_MAX,
		(float)joyState.Gamepad.sThumbLY / SHRT_MAX 
		};

		mainTex_TF.transform.translate.x += move.x;

	}

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