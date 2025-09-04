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

	// 移動量を初期化
	move_ = { 0.0f,0.0f,0.0f };

	// 左右入力時に移動
	if (joyState.Gamepad.sThumbLX != 0 && joyState.Gamepad.sThumbLY != 0) {
		
		//スティックから移動量を計算
		move_ = {
		(float)joyState.Gamepad.sThumbLX / SHRT_MAX,
		(float)joyState.Gamepad.sThumbLY / SHRT_MAX 
		};

		world_.transform.translate.x += move_.x;
	}

	// ジャンプ
	if (Input::GetInstance()->pushPad(XINPUT_GAMEPAD_B)) {
		world_.transform.translate.y += 1.0f;
	}

	//重力を加える
	if (1) {
		world_.transform.translate.y -= gravity_;
	}
	//地面にいないなら落ちるスピードが加速する
	if (isOnFloorFlag_ == false) {
		gravity_ = std::min(gravity_ + kGravity, kMaxGravity);
	}
	isOnFloorFlag_ = false;

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

void Player::ColliderInit()
{
	colliders_[ColliderType::pCollider].Init(&world_);
	colliders_[ColliderType::pCollider].SetSize(colliderSize);
	colliders_[ColliderType::pCollider].SetOffset(colliderOffset);
	colliders_[ColliderType::pCollider].OnCollision = [this](ICollider& collider) { OnCollision(collider); };
	colliders_[ColliderType::pCollider].SetcollitionAttribute(Collider::Tag::Player);
	colliders_[ColliderType::pCollider].SetcollisionMask(~Collider::Tag::Player & ~Collider::Tag::Weapon);
	colliders_[ColliderType::pCollider].colliderName = "Player";
}
void Player::OnCollision(const ICollider& ICollider)
{
	if (ICollider.GetcollitionAttribute() == Collider::Tag::Enemy) {

		world_.transform.translate -= move_;
		world_.Update();
	}
	if (ICollider.GetcollitionAttribute() == Collider::Tag::Floor) {
		world_.transform.translate.y = ICollider.GetCenter().y;
		world_.Update();
		gravity_ = kGravity;
		isOnFloorFlag_ = true;
	}

	//state_->OnCollision(this, ICollider);
	return;
}