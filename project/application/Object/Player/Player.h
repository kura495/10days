#pragma once
#include "Object/IObject.h"
#include "Sprite/Sprite.h"
#include "Model/Model.h"

#include "Math/Matrix/MatrixCalc.h"
#include "Collider/Box/BoxCollider.h"
#include "Collider/OBB/OBBoxCollider.h"
#include "Input/Input.h"
#include "Animation/Animation.h"

#include "Object/Camera/FollowCamera.h"
#include "ParticleSystem/ParticleSystem.h"


// -- プレイヤークラス -- //
class Player
{
public:

	enum ColliderType {
		pCollider,//プレイヤーの当たり判定
		Attack,//攻撃判定
		ColliderTypeEND,
	};

	// 初期化
	void Init(std::vector<Model*> models);
	// 更新
	void Update();
	// �`��
	void Draw();

	// -- 座標関係 -- // 

	WorldTransform& GetWorld() { return world_; };


	// -- 衝突関係処理 -- //

	//プレイヤーキャラ事態の当たり判定
	void ColliderInit();
	void SetColliderUse(int number, bool flag);
	void SetColliderAttribute(int number, uint32_t collisionAttribute);
	void OnCollision(const ICollider& collider);
	void AttackColliderInit();
	void AttackOnCollision(const ICollider& collider);

protected:

	// 入力
	XINPUT_STATE joyState;

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

	//移動制御
	void FixTranslate(Vector3 colliderPos, Vector3 HitcolliderSize);
	Vector3 tlanslatePre;

	// -- システム -- //


	// ストレスメーター


	// スペース範囲
	Model* spaceRangeModel_ = nullptr;
	WorldTransform spaceRangeWorld_;
	float spaceRange_ = 5.0f;

};

