#include "HumanEnemy.h"

void HumanEnemy::Init(std::vector<Model*> models, Player* player)
{
	// モデル配列を取得
	models_ = models;
	// プレイヤーのポインタを保存
	player_ = player;

	// ワールド座標
	world_.Init();
	world_.transform.translate = Vector3(20.0f, 5.0f, 0.0f);
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

void HumanEnemy::Update()
{
	// 行動制御
	enemyAI_->Update();

	// アニメーション 更新
	animation_->PlayAnimation();
	// 座標更新
	world_.Update();

#ifdef _DEBUG

	// 敵の情報のデバッグ表示
	// 主にワールド座標やenemyAIの状態を表示

	ImGui::Begin("HumanEnemy");
	ImGui::DragFloat3("Pos", &world_.transform.translate.x);
	// ステートはenumなので文字列に変換してから表示
	std::string stateStr;
	switch (enemyAI_->GetCurrentState())
	{
	case IEnemyState::State::IDLE:
		stateStr = "Idle";
		break;
	case IEnemyState::State::PATROL:
		stateStr = "Patrol";
		break;
	case IEnemyState::State::CHASE:
		stateStr = "Chase";
		break;
	case IEnemyState::State::ATTACK:
		stateStr = "Attack";
		break;
	case IEnemyState::State::DEAD:
		stateStr = "Dead";
		break;

	default:
		break;
	}
	ImGui::Text("State: %s", stateStr.c_str());
	ImGui::End();

#endif // _DEBUG



}
