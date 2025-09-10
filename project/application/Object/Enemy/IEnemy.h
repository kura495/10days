#pragma once

#include "Object/IObject.h"
#include "Sprite/Sprite.h"
#include "Model/Model.h"
#include "Animation/Animation.h"

#include "Math/Matrix/MatrixCalc.h"
#include "Collider/Box/BoxCollider.h"
#include "Collider/OBB/OBBoxCollider.h"

#include "Enemy/AI/EnemyAI.h"

class Player;
class FloorManager;


// -- 敵キャラ 基底クラス -- //
class IEnemy {
public:

	enum ColliderType {
		eCollider, // 敵キャラの当たり判定
		Attack, // 攻撃判定
		ColliderTypeEND,
	};


	// 初期化
	virtual void Init(std::vector<Model*> models,Player* player,FloorManager* floorManager);
	

	// 更新
	virtual void Update() {
		// アニメーション 更新
		animation_->PlayAnimation();
		// 座標更新
		world_.Update();
	}

	// 描画
	virtual void Draw() {
		// モデルの描画
		models_[0]->RendererSkinDraw(world_, animation_->GetSkinCluster());
#ifdef _DEBUG
		// コライダーの描画
		colliders_[ColliderType::eCollider].CollisionDraw();
#endif
	}

	// -- 座標関係 -- // 

	WorldTransform& GetWorld() { return world_; };
	Vector3 GetPos() { return this->GetWorld().transform.translate; };
	Vector3 GetPlayerPos();

	// コライダー 初期化
	virtual void ColliderInit();

	
	// -- 衝突関係 -- //
	virtual void OnCollision(const ICollider& collider);
	virtual void SetColliderUse(int number, bool flag);
	virtual void SetColliderAttribute(int number, uint32_t collisionAttribute);
	virtual void AttackColliderInit();
	virtual void AttackOnCollision(const ICollider& collider);

	void FixTranslate(Vector3 colliderPos, Vector3 HitcolliderSize);

	// -- 行動制御(共通の制御関数) -- //

	// 外部(主Behavior)から行動を指定する
	void SetAction(Action::Name actionName);

	// Behaviorの実行状況を返す
	IBehavior::State GetBehaviorState() { return enemyAI_->GetBehaviorState(); }
	
	// 行動の実行結果を返す(実行中/終了)
	IBehavior::State GetActionState(Action::Name actionName);

	// 移動
	int32_t Move();
	// ジャンプ
	int32_t Jump();


	// プレイヤーから指定値の範囲内にいるか
	bool IsPlayerInRange(float range);
	// プレイヤーから指定値の範囲外にいるか
	bool IsPlayerOutOfRange(float range);


protected:

	// プレイヤーのポインタ
	Player* player_ = nullptr;

	// -- オブジェクト / モデル / アニメーション -- //

	// モデル配列
	std::vector<Model*> models_;
	// アニメーション
	Animation* animation_;

	// -- 座標・行動 -- //

	// ワールド座標
	WorldTransform world_;

	// 移動量
	Vector3 move_ = { 0,0,0 };


	// -- 重力 -- //

	// 最大落下速度
	const float kMaxGravity = 0.098f;
	// 基本落下速度
	const float kGravity = 0.003f;
	// 落下速度
	float gravity_ = 0.003f;
	// 床に接地しているか
	bool isOnFloorFlag_ = false;


	// -- 衝突 -- //

	//当たり判定
	std::array<OBBoxCollider, ColliderType::ColliderTypeEND> colliders_;
	WorldTransform attackColliderWorld_;

	Vector3 colliderSize = { 0.5f,0.7f,0.5f };
	Vector3 colliderOffset = { 0.0f,0.7f,0.0f };

	//攻撃の当たり判定
	Vector3 attackColliderOffset = { 0.0f,0.5f,1.0f };
	float hitStopValue = 0.2f;
	float vibValue = 0.2f;

	Vector3 tlanslatePre;

	// -- 行動制御 -- //
	std::unique_ptr<EnemyAI> enemyAI_;

};