#pragma once
#include "Enemy/AI/State/IEnemyState.h"

// -- 敵キャラ 状態：巡回 -- //
class PatrolState :
    public IEnemyState
{
    public:
    // 初期化
    void Init(IEnemy* enemy) override;

    // 更新
    IBehavior::State Update() override;
    // 状態取得
    State GetState() override {
        return state_;
	};


};

