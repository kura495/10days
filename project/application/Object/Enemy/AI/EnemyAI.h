#pragma once
#include "Enemy/AI/State/IEnemyState.h"
#include "Enemy/AI/State/Idle/IdleState.h"
#include "Enemy/AI/State/Patrol/PatrolState.h"
#include "Enemy/AI/State/Chase/ChaseState.h"
#include "Enemy/AI/State/Attack/AttackState.h"
#include "Enemy/AI/State/Dead/DeadState.h"

// 前方宣言
class IEnemy;

// -- 敵キャラ AI制御クラス -- //
class EnemyAI
{
public:

	// 初期化
	void Init(class IEnemy* enemy);
	// 更新
	void Update();
	// 現在の状態を取得
	IEnemyState::State GetCurrentState() { return currentState_->GetState(); }
	
	IBehavior::State GetBehaviorState()const { return this->behaviorState_; }
	
protected:

	// 敵キャラのポインタ
	IEnemy* enemy_;
	// 現在の状態
	IEnemyState* currentState_;
	// ビヘイビアツリーの駆動状態
	IBehavior::State behaviorState_;

	// 各状態のインスタンス
	IEnemyState* idleState_;
	IEnemyState* patrolState_;
	IEnemyState* chaseState_;
	IEnemyState* attackState_;
	IEnemyState* deadState_;



};

