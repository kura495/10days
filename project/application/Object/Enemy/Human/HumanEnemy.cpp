#include "HumanEnemy.h"

void HumanEnemy::Init(std::vector<Model*> models)
{

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

void HumanEnemy::Update()
{

}
