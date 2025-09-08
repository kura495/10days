#pragma once

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

	virtual ~IEnemyState() = default;
	virtual void Init(IEnemy* enemy) = 0;
	virtual void Update() = 0;
	virtual State GetState() = 0;

protected: 
	
	// 敵キャラのポインタ
	IEnemy* enemy_;

	// 状態
	State state_;

};