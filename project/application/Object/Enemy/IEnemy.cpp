#include "IEnemy.h"
#include "Player/Player.h"

void IEnemy::Init(std::vector<Model*> models, Player* player)
{
	// モデル配列を取得
	models_ = models;
	// プレイヤーのポインタを保存
	player_ = player;

	// ワールド座標
	world_.Init();
	world_.transform.translate = Vector3(5.0f, 3.0f, 0.0f);
	world_.Update();// 一度更新しておく

	// アニメーション
	animation_ = Animation::LoadAnimationFile("project/resources/Player", "player_walk.gltf");
	animation_->Init();
	animation_->Reset();
	animation_->AnimeInit(*models_[0], true);

	// コライダー
	ColliderInit();
	AttackColliderInit();

	// 行動制御
	enemyAI_ = std::make_unique<EnemyAI>();
	enemyAI_->Init(this);

}

bool IEnemy::IsPlayerInRange(float range)
{	
	// プレイヤーが一定範囲内(50.0f)にいるか
	if (Vector3::Distance(world_.transform.translate, player_->GetWorld().transform.translate) <= range) {
		// いる場合はtrueを返す
		return true;
	}

	// いない場合はfalseを返す
	return false;
}

bool IEnemy::IsPlayerOutOfRange(float range)
{
	// プレイヤーが一定範囲内(50.0f)にいるか
	if (Vector3::Distance(world_.transform.translate, player_->GetWorld().transform.translate) >= range) {
		// いる場合はtrueを返す
		return true;
	}

	// いない場合はfalseを返す
	return false;
}

void IEnemy::ColliderInit()
{
	colliders_[ColliderType::eCollider].Init(&world_);
	colliders_[ColliderType::eCollider].SetSize(colliderSize);
	colliders_[ColliderType::eCollider].SetOffset(colliderOffset);
	colliders_[ColliderType::eCollider].OnCollision = [this](ICollider& collider) { OnCollision(collider); };
	colliders_[ColliderType::eCollider].SetcollitionAttribute(Collider::Tag::EnemyCore);
	colliders_[ColliderType::eCollider].SetcollisionMask(~Collider::Tag::EnemyCore & ~Collider::Tag::EnemyBall & ~Collider::Tag::Weapon);
	colliders_[ColliderType::eCollider].colliderName = "Enemy";
}

void IEnemy::SetColliderUse(int number, bool flag)
{
	colliders_[number].IsUsing = flag;
}

void IEnemy::SetColliderAttribute(int number, uint32_t collisionAttribute)
{
	colliders_[number].SetcollitionAttribute(collisionAttribute);
}

void IEnemy::OnCollision(const ICollider& ICollider)
{
	if (ICollider.GetcollitionAttribute() == Collider::Tag::Floor) {

		// プレイヤーとブロックの差分を取得
		Vector3 diff = world_.transform.translate - ICollider.GetCenter();
		// プレイヤーとブロックの衝突方向を取得
		Vector3 direction = Vector3::Normalize(diff);
		// プレイヤーがブロックの上にいるかどうかを判定
		bool isAbove = direction.y > 0.5f;
		// プレイヤーがブロックの下にいるかどうかを判定
		bool isBelow = direction.y < -0.5f;
		// プレイヤーがブロックの左にいるかどうかを判定
		bool isLeft = direction.x < -0.5f;
		// プレイヤーがブロックの右にいるかどうかを判定
		bool isRight = direction.x > 0.5f;
		// 下方向への押し出し
		if (isBelow) {
			world_.transform.translate.y = -(colliders_[ColliderType::eCollider].GetSize().y / 2.0f + ICollider.GetSize().y / 2.0f);
		}
		// 上方向への押し出し
		if (isAbove) {
			world_.transform.translate.y = colliders_[ColliderType::eCollider].GetSize().y / 2.0f + ICollider.GetSize().y / 2.0f;
		}
		// 左方向への押し出し
		if (isLeft) {
			world_.transform.translate.x -= colliders_[ColliderType::eCollider].GetSize().y / 2.0f + ICollider.GetSize().y / 2.0f;
		}
		// 右方向への押し出し
		if (isRight) {
			world_.transform.translate.x += colliders_[ColliderType::eCollider].GetSize().y / 2.0f + ICollider.GetSize().y / 2.0f;
		}


		world_.Update();
		gravity_ = kGravity;
		isOnFloorFlag_ = true;
	}
	return;
}

void IEnemy::AttackColliderInit()
{
	attackColliderWorld_.SetParent(&world_);
	colliders_[ColliderType::Attack].Init(&attackColliderWorld_);
	colliders_[ColliderType::Attack].SetSize({ 1.0f,1.0f,1.0f });
	colliders_[ColliderType::Attack].SetOffset(attackColliderOffset);
	colliders_[ColliderType::Attack].OnCollision = [this](ICollider& collider) { AttackOnCollision(collider); };
	colliders_[ColliderType::Attack].SetcollitionAttribute(Collider::Tag::Weapon);
	colliders_[ColliderType::Attack].SetcollisionMask(~Collider::Tag::Player & ~Collider::Tag::Weapon & ~Collider::Tag::Floor);
	colliders_[ColliderType::Attack].IsUsing = false;
	colliders_[ColliderType::Attack].colliderName = "PlayerAttack";
}
void IEnemy::AttackOnCollision(const ICollider& collider)
{
	if (collider.GetcollitionAttribute() == Collider::Tag::EnemyCore) {
		colliders_[ColliderType::Attack].IsUsing = false;
		////パーティクル用のベクトル
		//attackVector = TransformNormal({ 0.0f,0.0f,1.0f }, Matrix4x4(MakeRotateMatrix(world_.transform.quaternion)));
		//attackVector.Normalize();
		//attackVector *= -1;
		////パーティクル生成
		//AttackHitParticleEmitter.world_.transform.translate = attackColliderWorld_.transform.translate + world_.transform.translate;
		//AttackHitParticleEmitter.world_.transform.translate.y += 1.0f;
		//attackHitParticle_->SpawnParticle(AttackHitParticleEmitter);
		////音関連
		//Audio::Stop(SEattack, true, false);
		//Audio::Play(SEHitattack, 1.0f);
		////ヒットストップ
		//PlayPhase::HitStop(hitStopValue);
		//コントローラー振動
		Input::VibrateController(VIBRATION_MAX, VIBRATION_MIN, vibValue);
	}
	if (collider.GetcollitionAttribute() == Collider::Tag::EnemyBall) {
		colliders_[ColliderType::Attack].IsUsing = false;
		//パーティクル用のベクトル
		//attackVector = TransformNormal({ 0.0f,0.0f,1.0f }, Matrix4x4(MakeRotateMatrix(world_.transform.quaternion)));
		//attackVector.Normalize();
		//attackVector *= -1;
		////パーティクル生成
		//AttackHitBombParticleEmitter.color = { 0.5f,0.5f,1.0f };
		//AttackHitBombParticleEmitter.world_.transform.translate = attackColliderWorld_.transform.translate + world_.transform.translate;
		//AttackHitBombParticleEmitter.world_.transform.translate.y += 1.0f;
		//attackHitBombParticle_->SpawnParticle(AttackHitBombParticleEmitter);
		////ヒットストップ
		//PlayPhase::HitStop(hitStopValue);
		//コントローラー振動
		Input::VibrateController(VIBRATION_MAX, VIBRATION_MIN, vibValue);
	}
}