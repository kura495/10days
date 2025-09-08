#pragma once
#include "Enemy/AI/State/IEnemyState.h"


// -- 敵キャラ 状態：待機 -- //
// この状態の場合、敵キャラはその場で待機します。
// 基本的に画面外 かつ プレイヤーが一定距離にいない場合に実行される
// この状態の際は一部更新処理と描画処理をスキップすること 
class IdleState : public IEnemyState
{
public:
    
    // 初期化
    void Init(IEnemy* enemy) override;

    // 更新
    void Update() override;

    // 状態取得
    State GetState() override {
        return state_;
	};


};

