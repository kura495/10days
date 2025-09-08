#pragma once
#include "Enemy/AI/State/IEnemyState.h"

// -- 敵キャラ 状態：巡回 -- //
class PatrolState :
    public IEnemyState
{
    public:
    // 初期化
    void Init(IEnemy* enemy) override {
        // 敵キャラのポインタを取得
        enemy_ = enemy;
        // 状態を巡回に設定
        state_ = PATROL;
    };
    // 更新
    void Update() override {
        // 巡回状態の処理
    };
    // 状態取得
    State GetState() override {
        return state_;
	};


};

