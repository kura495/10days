#pragma once
#include "AI/Behavior/Behavior.h"

// 前方宣言
class IEnemy;

// -- 敵キャラ 状態基底クラス -- //
class IEnemyState
{
public:

	// 大まかな遷移状態
	enum State {
		IDLE,	// 待機
		PATROL, // 巡回
		CHASE,	// 追跡
		ATTACK,	// 攻撃
		DEAD,	// 死亡
	};

	IEnemyState() = default;
	virtual ~IEnemyState() = default;
	virtual void Init(IEnemy* enemy) = 0;
	virtual IBehavior::State Update() = 0;
	
	// 現在の状態を返す
	virtual State GetState() = 0;

	// 状態遷移のリクエストがされているか
	bool GetIsStateChengeRequest() const { return  isStateChengeRequest_; }
	// 遷移先を取得
	State GetNextState() const { return nextState_; }
	// 外部からリクエストを送る
	void SetStateChengeRequest(State nextState) {
		isStateChengeRequest_ = true;
		nextState_ = nextState;
	}

protected: 
	
	// 敵キャラのポインタ
	IEnemy* enemy_;

	// 状態
	State state_;
	// 次の状態
	State nextState_;
	// 状態遷移のリクエスト
	bool isStateChengeRequest_;

	// -- 個別行動制御 -- //

	// ルートノード
	std::unique_ptr<Selector> rootNode_;


};