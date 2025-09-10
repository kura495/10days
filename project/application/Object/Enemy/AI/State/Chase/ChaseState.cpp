#include "ChaseState.h"
#include "Enemy/IEnemy.h"

void ChaseState::Init(IEnemy* enemy)
{
	// 敵キャラのポインタを取得
	enemy_ = enemy;
	// 状態を巡回に設定
	state_ = CHASE;

	// ルートノードの初期化
	rootNode_ = std::make_unique<Selector>();

	// ビヘイビアツリーの構築

	

	// プレイヤーが15.0f以内にいない場合、巡回状態へ移行
	std::unique_ptr<IBehavior> patrolDecrator = std::make_unique<Decorator>(
		enemy_,
		[this]() { return this->enemy_->IsPlayerOutOfRange(15.0f); }
	);

	// 待機状態への処理
	std::unique_ptr<IBehavior> patrolAction = std::make_unique<Action>(
		enemy_,
		Action::kPATROL
	);
	patrolDecrator->SetChild(std::move(patrolAction));


	// 追跡移動 処理
	std::unique_ptr<IBehavior> chaseAction = std::make_unique<Action>(
		enemy_,
		Action::MOVE
	);

	// セット
	rootNode_->SetChild(std::move(patrolDecrator));
	rootNode_->SetChild(std::move(chaseAction));


}

IBehavior::State ChaseState::Update()
{
	// 待機状態の処理
	// ビヘイビアツリーの実行
	IBehavior::State result = rootNode_->Tick();

	// ビヘイビアツリーの実行結果が成功か失敗だったら( ≒ 実行中ではない場合)
	if (result != IBehavior::State::RUNNING) {

		// ビヘイビアツリーをリセット
		rootNode_->Reset();
	}

	// 結果を返す
	return result;
}
