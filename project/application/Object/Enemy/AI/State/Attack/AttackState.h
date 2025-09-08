#pragma once
#include "Enemy/AI/State/IEnemyState.h"

// -- 敵キャラ 状態：攻撃 -- //
class AttackState :
    public IEnemyState
{
    public:
    // 初期化
    void Init(IEnemy* enemy) override {
        // 敵キャラのポインタを取得
        enemy_ = enemy;
        // 状態を攻撃に設定
        state_ = ATTACK;
    };
    // 更新
    void Update() override {
        // 攻撃状態の処理
    };
    // 状態取得
    State GetState() override {
        return state_;
	};
};

