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

	// 初期化
	void Init(std::vector<Model*> models);
	// 更新
	void Update();
	// 描画
	void Draw();

protected:

	// 入力
	XINPUT_STATE joyState;

	// モデル配列
	std::vector<Model*> models_;
	// アニメーション
	Animation* animation_;

	// ステート(各行動/状態に応じた処理を分割)


	// ワールド座標
	WorldTransform world_;
	



	// ストレスメーター

	// スペース範囲


};

