#include "PatrolState.h"
#include "Enemy/IEnemy.h"

void PatrolState::Init(IEnemy* enemy)
{
    // 敵キャラのポインタを取得
    enemy_ = enemy;
    // 状態を巡回に設定
    state_ = PATROL;

	// ルートノードの初期化
	rootNode_ = std::make_unique<Selector>();
	
	// ビヘイビアツリーの構築

	// プレイヤーが10.0f以内にいない場合、待機状態へ移行
	std::unique_ptr<IBehavior> idleDecrator = std::make_unique<Decorator>(
		enemy_,
		[this]() { return this->enemy_->IsPlayerOutOfRange(10.0f); }
	);

	// 待機状態への処理
	std::unique_ptr<IBehavior> idleAction = std::make_unique<Action>(
		enemy_,
		Action::kIDLE
	);
	idleDecrator->SetChild(std::move(idleAction));

	// プレイヤーが5.0f以内にいる場合、追跡状態に移行
	std::unique_ptr<IBehavior> chaseDecrator = std::make_unique<Decorator>(
		enemy_,
		[this]() { return this->enemy_->IsPlayerInRange(5.0f); }
	);
	// 追跡状態への処理
	std::unique_ptr<IBehavior> chaseAction = std::make_unique<Action>(
		enemy_,
		Action::kCHASE
	);
	chaseDecrator->SetChild(std::move(chaseAction));

	// セット
	rootNode_->SetChild(std::move(idleDecrator));
	rootNode_->SetChild(std::move(chaseDecrator));
}

IBehavior::State PatrolState::Update()
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

