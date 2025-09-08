#pragma once

#include "Object/IObject.h"
#include "Sprite/Sprite.h"
#include "Model/Model.h"
#include "Animation/Animation.h"

#include "Math/Matrix/MatrixCalc.h"
#include "Collider/Box/BoxCollider.h"
#include "Collider/OBB/OBBoxCollider.h"

#include "Enemy/AI/EnemyAI.h"

// -- 敵キャラ 基底クラス -- //
class IEnemy {
public:

	enum ColliderType {
		eCollider, // 敵キャラの当たり判定
		Attack, // 攻撃判定
		ColliderTypeEND,
	};


	// 初期化
	virtual void Init(std::vector<Model*> models) {

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

		// コライダー
		ColliderInit();
	}
	

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

	// コライダー 初期化
	void ColliderInit() {
		colliders_[ColliderType::eCollider].Init(&world_);
		colliders_[ColliderType::eCollider].SetSize(colliderSize);
		colliders_[ColliderType::eCollider].SetOffset(colliderOffset);
		colliders_[ColliderType::eCollider].OnCollision = [this](ICollider& collider) { OnCollision(collider); };
		colliders_[ColliderType::eCollider].SetcollitionAttribute(Collider::Tag::Player);
		colliders_[ColliderType::eCollider].SetcollisionMask(~Collider::Tag::Player & ~Collider::Tag::Weapon);
		colliders_[ColliderType::eCollider].colliderName = "Player";
	}

	// -- 座標関係 -- // 

	WorldTransform& GetWorld() { return world_; };


	void OnCollision(const ICollider& collider) { collider; return; }

protected:

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

	// -- 行動制御 -- //
	EnemyAI enemyAI_;

};