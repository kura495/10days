#pragma once
#include "Enemy/IEnemy.h"
#include "AI/EnemyAI.h"

// -- 敵キャラ(人間) クラス -- //
class HumanEnemy : public IEnemy
{
public:

	// 初期化
	void Init(std::vector<Model*> models)override;
	
	// 更新
	void Update()override;



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

	//攻撃の当たり判定
	Vector3 attackColliderOffset = { 0.0f,0.5f,1.0f };
	float hitStopValue = 0.2f;
	float vibValue = 0.2f;


	// -- システム -- //


	// ストレスメーター


	// スペース範囲
	Model* spaceRangeModel_ = nullptr;
	WorldTransform spaceRangeWorld_;
	float spaceRange_ = 5.0f;

};


