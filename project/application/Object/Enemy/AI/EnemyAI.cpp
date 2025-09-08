#include "EnemyAI.h"
#include "Enemy/IEnemy.h"

void EnemyAI::Init(IEnemy* enemy) {

	// 敵キャラのポインタを保存
	enemy_ = enemy;
	
	// 各状態のインスタンスを生成
	idleState_ = new IdleState();
	patrolState_ = new PatrolState();
	chaseState_ = new ChaseState();
	attackState_ = new AttackState();
	deadState_ = new DeadState();
	
	// 初期状態を待機に設定
	currentState_ = idleState_;
	currentState_->Init(enemy_);

	// ビヘイビアツリーの駆動状態
	behaviorState_ = IBehavior::State::READY;


}

void EnemyAI::Update() {

	// 現在の状態を更新
	currentState_->Update();

	// 状態遷移
	// Statte Patternを用いて状態遷移を管理
	if (currentState_->GetIsStateChengeRequest()) {
		
		switch (currentState_->GetNextState())
		{
			case IEnemyState::State::IDLE:
			currentState_ = idleState_;
			currentState_->Init(enemy_);
			break;

			case IEnemyState::State::PATROL:
				currentState_ = patrolState_;
				currentState_->Init(enemy_);
				break;
			
			case IEnemyState::State::CHASE:
				currentState_ = chaseState_;
				currentState_->Init(enemy_);
				break;
			
			case IEnemyState::State::ATTACK:
				currentState_ = attackState_;
				currentState_->Init(enemy_);
				break;
			
			case IEnemyState::State::DEAD:
				currentState_ = deadState_;
				currentState_->Init(enemy_);
				break;

		default:
			break;
		}

	}
	
}


