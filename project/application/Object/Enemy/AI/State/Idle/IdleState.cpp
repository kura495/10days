#include "IdleState.h"
#include "Enemy/IEnemy.h"

void IdleState::Init(IEnemy* enemy)
{
	// 敵キャラのポインタを取得
	enemy_ = enemy;

	// 状態を待機に設定
	state_ = IDLE;

	// ルートノードの初期化
	rootNode_ = std::make_unique<Selector>();
	// ビヘイビアツリーの構築

	// 待機状態は、移動や攻撃を行わずに条件関数のみを実行し、状況に応じて他の状態に遷移する(主に巡回)
	// Conditionノード内に条件用のメンバ関数ポインタを渡す
	std::unique_ptr<IBehavior> moveAction = std::make_unique<Condition>(
    enemy_,
    [this](){ return this->enemy_->IsPlayerInRange(8.0f); }
	);
	rootNode_->SetChild(std::move(moveAction));

}

IBehavior::State IdleState::Update()
{
	// 待機状態の処理
	// ビヘイビアツリーの実行
	IBehavior::State result = rootNode_->Tick();

	// ビヘイビアツリーの実行結果が成功か失敗だったら( ≒ 実行中ではない場合)
	if (result != IBehavior::State::RUNNING) {

		// 成功していたら( ≒ 条件を満たしていたら)
		if (result == IBehavior::State::SUCCESS) {
			isStateChengeRequest_ = true;
			nextState_ = PATROL;
		}

		// ビヘイビアツリーをリセット
		rootNode_->Reset();
	}

	// 結果を返す
	return result;
}
