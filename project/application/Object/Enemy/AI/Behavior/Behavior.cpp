#include "Behavior.h"
#include "Enemy/IEnemy.h"

Action::Action(IEnemy* enemy, Action::Name actionName)
{
	// 敵クラスのポインタ
	enemy_ = enemy;

	// 行動名の保持
	actionName_ = actionName;
}

// アクションノード実行処理
// Enemyクラスに干渉するため[.cpp]にて定義
IBehavior::State Action::Tick()
{
	switch (state_)
	{
		// 待機時
	case State::READY:

		// 行動を実行
		enemy_->SetAction(actionName_);
		
		// 状態を実行中にする 
		state_ = State::RUNNING;

		return state_;
		break;

		// 実行中
	case State::RUNNING:


		return state_;
		break;

		// 成功
	case State::SUCCESS:
		// 結果を返す
		return state_;
		break;

		// 失敗
	case State::FAILURE:
		// 結果を返す
		return state_;
		break;


	default:
		break;
	}

	// いずれかに引っかからない場合はエラー
	// 一旦そのままreturnする
	return state_;
}

Condition::Condition(IEnemy* enemy, std::function<bool()> func)
{
	enemy_ = enemy;
	func_ = func;
}

Decorator::Decorator(IEnemy* enemy, std::function<bool()> func)
{
	enemy_ = enemy;
	func_ = func;
}


