#include "Player.h"

void Player::Init(std::vector<Model*> models) {

	// モデル配列を取得
	models_ = models;

	// ワールド座標
	world_.Init();
	world_.transform.translate = Vector3(5.0f, 3.0f, 0.0f);
	world_.Update();// 一度更新しておく

	// アニメーション
	animation_ = Animation::LoadAnimationFile("project/resources/Player", "player_walk.gltf");
	animation_->Init();
	animation_->Reset();
	animation_->AnimeInit(*models_[0], true);

	// スペース範囲
	//spaceRangeModel_ = Model::CreateModelFromObj("project/resurces/Space","Space.obj");

	spaceRange_ = 5.0f;

	spaceRangeWorld_.Init();
	spaceRangeWorld_.SetParent(&world_);
	spaceRangeWorld_.transform.scale = Vector3(spaceRange_, spaceRange_, 0.0f);

	// コライダー
	ColliderInit();
	AttackColliderInit();
}

void Player::Update() {

	tlanslatePre = world_.transform.translate;
	// -- 入力 -- //

	// 入力内容の取得
	Input::GetInstance()->GetJoystickState(joyState);

	// 移動量を初期化
	move_ = { 0.0f,0.0f,0.0f };

	// 左右入力時に移動
	if (joyState.Gamepad.sThumbLX != 0 && joyState.Gamepad.sThumbLY != 0) {

		//スティックから移動量を計算(X軸のみ)
		move_ = {
		(float)joyState.Gamepad.sThumbLX / SHRT_MAX,
		0,
		};
		
		// 移動量を正規化
		move_ = Vector3::Normalize(move_);
		// 移動量に移動速度を掛ける
		move_.x *= moveSpeed_;
	}
	// キーボード入力
	else if (Input::GetInstance()->TriggerKey(DIK_D) || Input::GetInstance()->TriggerKey(DIK_RIGHT)) {
		move_.x += moveSpeed_;
	}
	else if (Input::GetInstance()->TriggerKey(DIK_A) || Input::GetInstance()->TriggerKey(DIK_LEFT)) {
		move_.x -= moveSpeed_;
	}

	//ジャンプする処理
	if ((Input::GetInstance()->pushPad(XINPUT_GAMEPAD_B) || Input::GetInstance()->TriggerKey(DIK_W)) && jumpLimit < 3) {
		gravity_ += 0.7f - (0.05f * jumpLimit);
		jumpLimit += 1;
	}
#pragma region 
	if ((Input::GetInstance()->pushPad(XINPUT_GAMEPAD_A) || Input::GetInstance()->TriggerKey(DIK_SPACE)) && coolTime_ > kMaxCoolTime_) {
		//攻撃処理を挟む
		colliders_[ColliderType::Attack].IsUsing = true;
		isAttackFlag_ = true;
		//コライダーの大きさ設定
		colliders_[ColliderType::Attack].SetSize({ 2.0f + (3.0f * stressMater),2.0f + (3.0f * stressMater),1.0f});
		coolTime_ = 0;
	}
	//攻撃後かつクールタイムが半分以上で攻撃をリセット
	if (isAttackFlag_ && coolTime_ > 0.5f) {
		colliders_[ColliderType::Attack].IsUsing = false;
		isAttackFlag_ = false;
	}

	//クールタイムの回復
	coolTime_ += kDeltaTime;
#pragma endregion 攻撃

	//落下している処理
	gravity_ = (std::max)(gravity_ - kGravity, kMaxGravity);
	move_.y += gravity_;

	// �ړ��ʂ���Z
	world_.transform.translate += move_;

	// -- �A�j���[�V���� -- //

	animation_->PlayAnimation();

#pragma region
	/*ImGui::Begin("Player");
	ImGui::DragFloat3("TexPos", &world_.transform.translate.x);
	ImGui::End();*/
#pragma endregion

	// ���W�X�V
	world_.Update();

}

void Player::Draw() {

	// モデルの描画
	models_[0]->RendererSkinDraw(world_, animation_->GetSkinCluster());

	// コライダーの描画
	colliders_[ColliderType::pCollider].CollisionDraw();
	colliders_[ColliderType::Attack].CollisionDraw();

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

void Player::SetColliderUse(int number, bool flag)
{
	colliders_[number].IsUsing = flag;
}

void Player::SetColliderAttribute(int number, uint32_t collisionAttribute)
{
	colliders_[number].SetcollitionAttribute(collisionAttribute);
}

void Player::OnCollision(const ICollider& ICollider)
{
    if (ICollider.GetcollitionAttribute() == Collider::Tag::Floor) {

		FixTranslate(ICollider.GetCenter(), ICollider.GetSize());

        world_.Update();

    }
    return;
}

void Player::AttackColliderInit()
{
	attackColliderWorld_.SetParent(&world_);
	colliders_[ColliderType::Attack].Init(&attackColliderWorld_);
	colliders_[ColliderType::Attack].SetSize({ 2.0f,2.0f,2.0f });
	colliders_[ColliderType::Attack].SetOffset(attackColliderOffset);
	colliders_[ColliderType::Attack].OnCollision = [this](ICollider& collider) { AttackOnCollision(collider); };
	colliders_[ColliderType::Attack].SetcollitionAttribute(Collider::Tag::Weapon);
	colliders_[ColliderType::Attack].SetcollisionMask(~Collider::Tag::Player & ~Collider::Tag::Weapon & ~Collider::Tag::Floor);
	colliders_[ColliderType::Attack].IsUsing = false;
	colliders_[ColliderType::Attack].colliderName = "PlayerAttack";
}
void Player::AttackOnCollision(const ICollider& collider)
{
	collider;
}

void Player::FixTranslate(Vector3 colliderPos, Vector3 HitcolliderSize)
{
#pragma region
	if (tlanslatePre.x - colliderSize.x + colliderOffset.x < colliderPos.x + HitcolliderSize.x && tlanslatePre.x + colliderSize.x + colliderOffset.x > colliderPos.x - HitcolliderSize.x) {

		if (tlanslatePre.y >= colliderPos.y + HitcolliderSize.y) {
			//上から下
			if (world_.transform.translate.y - colliderSize.y < colliderPos.y + HitcolliderSize.y) {
				world_.transform.translate.y = colliderPos.y + HitcolliderSize.y;

				jumpLimit = 0;
			}
		}

		if (tlanslatePre.y <= colliderPos.y - HitcolliderSize.y) {
			//下から上
			if (world_.transform.translate.y + colliderSize.y + colliderOffset.y > colliderPos.y - HitcolliderSize.y) {

				world_.transform.translate.y = colliderPos.y - HitcolliderSize.y - (colliderOffset.y * 2);
			}
		}
	}

	if (tlanslatePre.y - colliderSize.y + colliderOffset.y < colliderPos.y + HitcolliderSize.y && tlanslatePre.y + colliderSize.y + colliderOffset.y > colliderPos.y - HitcolliderSize.y) {
		if (tlanslatePre.x > colliderPos.x + colliderSize.x) {
			//左から右
			if (world_.transform.translate.x - colliderSize.x < colliderPos.x + HitcolliderSize.x) {
				world_.transform.translate.x = colliderPos.x + HitcolliderSize.x + colliderSize.x;
			}
		}
		if (tlanslatePre.x < colliderPos.x - HitcolliderSize.x) {
			//右から左
			if (world_.transform.translate.x + colliderSize.x > colliderPos.x - HitcolliderSize.x) {
				world_.transform.translate.x = colliderPos.x - HitcolliderSize.x - colliderSize.x;
			}
		}
	}
#pragma endregion 移動制御
}
