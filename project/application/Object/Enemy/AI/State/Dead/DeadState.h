#pragma once
#include "Enemy/AI/State/IEnemyState.h"

// -- 敵キャラ 状態：死亡 -- //
class DeadState :
    public IEnemyState
{
    public:
    // 初期化
    void Init(IEnemy* enemy) override {
        // 敵キャラのポインタを取得
        enemy_ = enemy;
        // 状態を死亡に設定
        state_ = DEAD;
    };
    // 更新
    IBehavior::State Update() override {
        // 死亡状態の処理
        return IBehavior::State::RUNNING;
    };
    // 状態取得
    State GetState() override {
        return state_;
	};
};

