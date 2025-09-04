#include "Player.h"

void Player::Init(std::vector<Model*> models) {

	// モデル配列を取得
	models_ = models;
	
	// ワールド座標
	world_.Init();
	world_.Update();// 一度更新しておく

	// アニメーション
	animation_ = Animation::LoadAnimationFile("project/resources/Player", "player_walk.gltf");
	animation_->Init();
	animation_->Reset();
	animation_->AnimeInit(*models_[0],true);
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

		world_.transform.translate.x += move.x;

	}

	// ジャンプ
	if (Input::GetInstance()->pushPad(XINPUT_GAMEPAD_B)) {
		world_.transform.translate.y += 1.0f;
	}

	// -- アニメーション -- //

	animation_->PlayAnimation();

#pragma region
	ImGui::Begin("Player");
	ImGui::DragFloat3("TexPos", &world_.transform.translate.x);
	ImGui::End();
#pragma endregion

	// 座標更新
	world_.Update();

}

void Player::Draw() {

	// モデルの描画
	models_[0]->RendererSkinDraw(world_, animation_->GetSkinCluster());



}