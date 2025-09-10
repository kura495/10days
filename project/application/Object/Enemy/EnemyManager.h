#pragma once
#include "Enemy/IEnemy.h"
#include "Enemy/Human/HumanEnemy.h"
#include <list>
#include "Model/Model.h"

class Player;
class FloorManager;

class EnemyManager
{
public:

	EnemyManager() = default;
	~EnemyManager() = default;

	void Init(Player* player,FloorManager* floorManager);
	void Update();
	void Draw();

	int32_t GetDefeatCount() { return defeatCount_; }

private:

	// 敵キャラ配列
	std::list<IEnemy*> enemies_;

	// 敵モデル
	std::vector<Model*> enemyModel_;

	// エネミー生成インターバル
	const float kEnemyCreateInterval_ = 1.0f;
	float enemyCreateTimer_ = 0.0f;

	Player* player_;
	FloorManager* floorManager_;

	// 撃破カウント
	int32_t defeatCount_ = 0;

};

