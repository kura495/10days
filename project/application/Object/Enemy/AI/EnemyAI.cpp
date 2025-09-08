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
}

void EnemyAI::Update() {

	// 現在の状態を更新
	currentState_->Update();

	// 状態遷移 
	switch (currentState_->GetState()) {

	case IEnemyState::IDLE:
		
		// 例: プレイヤーが近づいたら巡回状態へ
		currentState_ = patrolState_;
		currentState_->Init(enemy_);

		break;
	case IEnemyState::PATROL:
		// 例: プレイヤーを見つけたら追跡状態へ
		currentState_ = chaseState_;
		currentState_->Init(enemy_);
		break;
	case IEnemyState::CHASE:
		// 例: プレイヤーに近づいたら攻撃状態へ
		currentState_ = attackState_;
		currentState_->Init(enemy_);
		break;
	case IEnemyState::ATTACK:
		// 例: プレイヤーが遠ざかったら追跡状態へ
		currentState_ = chaseState_;
		currentState_->Init(enemy_);
		break;
	case IEnemyState::DEAD:
		// 死亡状態では遷移しない
		break;
	default:
		break;
	}
}


