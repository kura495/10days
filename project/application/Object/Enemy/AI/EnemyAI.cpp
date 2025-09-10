#include "EnemyAI.h"
#include "Enemy/IEnemy.h"
#include "MapObject/FloorManager.h"

void EnemyAI::Init(IEnemy* enemy, FloorManager* floorManager) {

	// 敵キャラのポインタを保存
	enemy_ = enemy;
	
	// 初期状態を待機に設定
	currentState_ = std::make_unique<IdleState>();
	currentState_->Init(enemy_);

	// ビヘイビアツリーの駆動状態
	behaviorState_ = IBehavior::State::READY;


	// 経路探索クラスの初期化
	pathFinding_ = std::make_unique<PathFinding>();
	pathFinding_->Init(enemy_, floorManager);

}

void EnemyAI::Update() {

	// 経路探索クラスの更新
	pathFinding_->Update();

	// 現在の状態を更新
	currentState_->Update();


	// 状態遷移
	// Statte Patternを用いて状態遷移を管理
	if (currentState_->GetIsStateChengeRequest()) {
		
		switch (currentState_->GetNextState())
		{
			case IEnemyState::State::IDLE:
			currentState_ =  std::make_unique<IdleState>();
			currentState_->Init(enemy_);
			break;

			case IEnemyState::State::PATROL:
				currentState_ = std::make_unique<PatrolState>();
				currentState_->Init(enemy_);
				break;
			
			case IEnemyState::State::CHASE:
				currentState_ = std::make_unique<ChaseState>();
				currentState_->Init(enemy_);
				break;
			
			case IEnemyState::State::ATTACK:
				currentState_ = std::make_unique<AttackState>();
				currentState_->Init(enemy_);
				break;
			
			case IEnemyState::State::DEAD:
				currentState_ = std::make_unique<DeadState>();
				currentState_->Init(enemy_);
				break;

		default:
			break;
		}

	}
	
}


