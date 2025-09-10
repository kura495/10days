#pragma once
#include "Enemy/IEnemy.h"
#include "AI/EnemyAI.h"

// -- 敵キャラ(人間) クラス -- //
class HumanEnemy : public IEnemy
{
public:

	// 初期化
	void Init(std::vector<Model*> models, Player* player,FloorManager* floorManager)override;
	// 更新
	void Update()override;
};


