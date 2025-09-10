#pragma once
#include "Enemy/AI/State/IEnemyState.h"
#include "Enemy/AI/State/Idle/IdleState.h"
#include "Enemy/AI/State/Patrol/PatrolState.h"
#include "Enemy/AI/State/Chase/ChaseState.h"
#include "Enemy/AI/State/Attack/AttackState.h"
#include "Enemy/AI/State/Dead/DeadState.h"
#include "Enemy/AI/PathFinding/PathFinding.h"

// 前方宣言
class IEnemy;
class FloorManager;

// -- 敵キャラ AI制御クラス -- //
class EnemyAI
{
public:

	EnemyAI() = default;
	~EnemyAI() = default;


	// 初期化
	void Init(class IEnemy* enemy, FloorManager* floorManager);
	// 更新
	void Update();
	// 現在の状態を取得
	IEnemyState::State GetCurrentState() { return currentState_->GetState(); }
	
	// 現在の状態のポインタを取得
	IEnemyState* GetCurrentStatePtr() { return currentState_.get(); }

	IBehavior::State GetBehaviorState()const { return this->behaviorState_; }
	
protected:

	// 敵キャラのポインタ
	IEnemy* enemy_;
	// 現在の状態
	std::unique_ptr<IEnemyState> currentState_;
	// ビヘイビアツリーの駆動状態
	IBehavior::State behaviorState_;
	// 経路探索クラス
	std::unique_ptr<PathFinding> pathFinding_;

};

