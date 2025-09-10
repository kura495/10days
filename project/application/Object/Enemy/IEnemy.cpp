#include "IEnemy.h"
#include "Player/Player.h"
#include "MapObject/FloorManager.h"

void IEnemy::Init(std::vector<Model*> models, Player* player, FloorManager* floorManager )
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
	enemyAI_->Init(this, floorManager);

}

int32_t IEnemy::Move()
{
	// 経路探索を活用し、移動方向を取得
	Vector3 moveDir = enemyAI_->GetPathFinding()->GetMoveDirection();

	if (moveDir.Length() == 0.0f) {
		moveDir = this->GetPlayerPos() - this->GetPos();

		if(moveDir.Length() == 0.0f){
			moveDir = { 0.0f ,0.0f ,0.0f };
		}

	}

	world_.transform.translate += moveDir * 0.01f;

	return 0;
}

bool IEnemy::IsPlayerInRange(float range)
{	
	// プレイヤーが範囲内にいるか
	if (Vector3::Distance(world_.transform.translate, player_->GetWorld().transform.translate) <= range) {
		// いる場合はtrueを返す
		return true;
	}

	// いない場合はfalseを返す
	return false;
}

bool IEnemy::IsPlayerOutOfRange(float range)
{
	// プレイヤーが範囲外にいるか
	if (Vector3::Distance(world_.transform.translate, player_->GetWorld().transform.translate) >= range) {
		// いる場合はtrueを返す
		return true;
	}

	// いない場合はfalseを返す
	return false;
}

Vector3 IEnemy::GetPlayerPos()
{
	return player_->GetWorld().transform.translate;
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

		FixTranslate(ICollider.GetCenter(), ICollider.GetSize());

		world_.Update();
		
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

void IEnemy::SetAction(Action::Name actionName)
{
	switch (actionName)
	{
	case Action::MOVE:

		// 移動
		Move();

		break;
	case Action::JUMP:
		break;
	case Action::SHOT:
		break;
	case Action::kIDLE:

		// 待機状態への変更リクエストを送る
		enemyAI_->GetCurrentStatePtr()->SetStateChengeRequest(IEnemyState::State::IDLE);

		break;
	case Action::kPATROL:

		// 巡回状態への変更リクエストを送る
		enemyAI_->GetCurrentStatePtr()->SetStateChengeRequest(IEnemyState::State::PATROL);

		break;
	case Action::kCHASE:

		// 追跡状態への変更リクエストを送る
		enemyAI_->GetCurrentStatePtr()->SetStateChengeRequest(IEnemyState::State::CHASE);

		break;
	case Action::kATTACK:

		// 攻撃状態への変更リクエストを送る
		enemyAI_->GetCurrentStatePtr()->SetStateChengeRequest(IEnemyState::State::ATTACK);

		break;
	case Action::kDEAD:

		// 死亡状態への変更リクエストを送る
		enemyAI_->GetCurrentStatePtr()->SetStateChengeRequest(IEnemyState::State::DEAD);

		break;
	default:
		break;
	}
}


void IEnemy::FixTranslate(Vector3 colliderPos, Vector3 HitcolliderSize)
{
#pragma region
	if (tlanslatePre.x - colliderSize.x + colliderOffset.x < colliderPos.x + HitcolliderSize.x && tlanslatePre.x + colliderSize.x + colliderOffset.x > colliderPos.x - HitcolliderSize.x) {

		if (tlanslatePre.y >= colliderPos.y + HitcolliderSize.y) {
			//上から下
			if (world_.transform.translate.y - colliderSize.y < colliderPos.y + HitcolliderSize.y) {
				world_.transform.translate.y = colliderPos.y + HitcolliderSize.y;

			}
		}

		if (tlanslatePre.y <= colliderPos.y - HitcolliderSize.y) {
			//下から上
			if (world_.transform.translate.y + colliderSize.y > colliderPos.y - HitcolliderSize.y) {
				float hogehoge = (colliderPos.y - HitcolliderSize.y) - ((colliderSize.y) + (colliderOffset.y / 2))/*- (world_.transform.translate.y - colliderSize.y) + (colliderOffset.y)*/;

				world_.transform.translate.y = hogehoge;


				gravity_ = kMaxGravity;

				return;
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
