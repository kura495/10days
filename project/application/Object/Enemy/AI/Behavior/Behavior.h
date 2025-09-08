#pragma once
#include <vector>
#include <memory>
#include <functional>

// 前方宣言
class IEnemy;


// -- Behaviore Tree 基底クラス -- //
class IBehavior
{
public:

	// 現在のノードの成否/状態
	enum State {
		READY,		// 実行前
		RUNNING,	// 実行中
		SUCCESS,	// 成功
		FAILURE,	// 失敗
	};

	// コンストラクタ
	IBehavior() = default;

	// 仮想デストラクタ
	virtual ~IBehavior() = default;

	// 実行
	virtual State Tick() = 0;

	// 再起動
	virtual void Reset() = 0;

	// 子ノードの番号をセットする
	virtual void SetChild(std::unique_ptr<IBehavior> child) = 0;
	// 子ノードの番号をまとめてセットする
	virtual void SetChildren(std::vector<std::unique_ptr<IBehavior>>&& children) = 0;

protected:

	// 子ノード配列
	std::vector<std::unique_ptr<IBehavior>> children_;
	// 現在のノードの成否/状態
	State state_ = State::READY;

};


// -- Selector クラス -- //
class Selector :
	public IBehavior
{
public:

	// デストラクタ
	virtual ~Selector()override {};

	// 実行処理
	virtual State Tick()override {

		// 実行終了状態であれば早期リターンする
		if (state_ == State::SUCCESS || state_ == State::FAILURE) {
			return state_;
		}

		for (auto& child : children_) {

			// 子ノードの処理を実行する
			State state = child->Tick();

			switch (state)
			{
				// いずれかの子ノードが成功したら成功を返す
				// また、セレクターの特徴としていずれか成功したら終了になる
			case State::SUCCESS:
				state_ = State::SUCCESS;
				return state_;

				break;

				// 実行中の場合、実行中という結果を返して終了する
			case State::RUNNING:

				// 実行中状態にする
				state_ = State::RUNNING;
				return state_;

				break;
			default:
				break;
			}

		}

		// ここまで来た場合は全ての子ノードが失敗した場合なので失敗を返して終了する
		state_ = State::FAILURE;
		return state_;
	}

	// 再起動
	virtual void Reset() override {
		// 待機状態にする
		state_ = State::READY;
		for (auto& child : children_) {
			child->Reset();
		}
	}

	// 子ノードの番号をセットする
	inline virtual void SetChild(std::unique_ptr<IBehavior> child) override {
		children_.push_back(std::move(child));
	}

	// 子ノードの番号をまとめてセットする
	inline virtual void SetChildren(std::vector<std::unique_ptr<IBehavior>>&& children)override {
		for (auto&& child : children) {
			children_.push_back(std::move(child));
		}
	}

};

// -- Sequence クラス -- //
class Sequence :
	public IBehavior
{
public:

	// デストラクタ
	virtual ~Sequence()override {};

	// 実行処理
	virtual State Tick()override {

		// 実行終了状態であれば早期リターンする
		if (state_ == State::SUCCESS || state_ == State::FAILURE) {
			return state_;
		}

		for (auto& child : children_) {

			// 子ノードの処理を実行する
			State state = child->Tick();

			switch (state)
			{
				// いずれかの子ノードが成功したら成功を返す
				// また、シークエンスの特徴としていずれか失敗したら終了になる
			case State::FAILURE:
				state_ = State::FAILURE;
				return state_;

				break;

				// 実行中の場合、実行中という結果を返して終了する
			case State::RUNNING:

				// 実行中状態にする
				state_ = State::RUNNING;
				return state_;

				break;
			default:
				break;
			}

		}

		// ここまで来た場合は全ての子ノードが成功した場合なので成功を返して終了する
		state_ = State::SUCCESS;
		return state_;
	}

	// 再起動
	virtual void Reset() override {
		// 待機状態にする
		state_ = State::READY;
		for (auto& child : children_) {
			child->Reset();
		}
	}

	// 子ノードの番号をセットする
	inline virtual void SetChild(std::unique_ptr<IBehavior> child) override {
		children_.push_back(std::move(child));
	}

	// 子ノードの番号をまとめてセットする
	inline virtual void SetChildren(std::vector<std::unique_ptr<IBehavior>>&& children)override {
		for (auto&& child : children) {
			children_.push_back(std::move(child));
		}
	}

};

// -- Action クラス -- //
class Action :
	public IBehavior
{
public:

	// 行動名
	enum Name {
		MOVE,
		JUMP,
		ATTACK,

	};

	// コンストラクタ
	Action(IEnemy* enemy ,Action::Name actionName);

	// 仮想デストラクタ
	virtual ~Action()override {};

	// 実行
	State Tick()override;
	// 再起動
	virtual void Reset() override {
		// 待機状態にする
		state_ = State::READY;
	}
	
	// 子ノードの番号をセットする
	virtual void SetChild(std::unique_ptr<IBehavior> child)override { child; }
	// 子ノードの番号をまとめてセットする
	virtual void SetChildren(std::vector<std::unique_ptr<IBehavior>>&& children) override { children; }

protected:

	// 敵クラスのポインタ
	IEnemy* enemy_;

	// 行動名の保持
	Name actionName_;

};

// -- Condition クラス -- //
class Condition :
	public IBehavior
{
public:

	// コンストラクタ
	// 敵キャラのポインタと条件関数を受け取る
	Condition(IEnemy* enemy, std::function<bool()> func);

	// 仮想デストラクタ
	virtual ~Condition()override = default;

	// 実行
	State Tick()override {
		// 実行終了状態であれば早期リターンする
		if (state_ == State::SUCCESS || state_ == State::FAILURE) return state_;
		// 条件関数を実行し、成功であれば成功、失敗であれば失敗を返す
		state_ = func_() ? State::SUCCESS : State::FAILURE;
		return state_;
	}
	// 再起動
	virtual void Reset() override {
		// 待機状態にする
		state_ = State::READY;
	}

	// 子ノードの番号をセットする
	virtual void SetChild(std::unique_ptr<IBehavior> child)override { child; }
	// 子ノードの番号をまとめてセットする
	virtual void SetChildren(std::vector<std::unique_ptr<IBehavior>>&& children) override { children; }

protected:

	// 敵クラスのポインタ
	IEnemy* enemy_;
	// 条件関数
	std::function<bool()> func_;

};

// -- Decorator クラス -- //
class Decorator :
	public IBehavior
{
public:

	// コンストラクタ
	// 敵キャラのポインタと条件関数を受け取る
	Decorator(IEnemy* enemy, std::function<bool()> func);
	// 仮想デストラクタ
	virtual ~Decorator()override = default;

	// 実行
	State Tick()override {

		// 実行終了状態であれば早期リターンする
		if (state_ == State::SUCCESS || state_ == State::FAILURE) { 
			return state_; 
		}
		
		// 子ノードが存在しない場合は失敗を返す
		if (children_.empty()) {
			return state_ = State::FAILURE;
		}

		// 条件関数を実行し、成功であれば子ノードの処理を実行し、失敗であれば失敗を返す
		// 一度条件関数を実行し、成功していたら子ノードの実行のみを行う
		if (!isConditionChecked_) {

			// 条件関数を実行し、結果を保持する
			isConditionMet_ = func_();

			// 条件関数を実行したことを記録する
			isConditionChecked_ = true;
		}
		// 条件を満たしていなければ失敗を返す
		else if(!isConditionMet_){
			state_ = State::FAILURE;
		}
		// 条件を満たしていれば子ノードの処理を実行する
		else {
			state_ = children_[0]->Tick();
		}

		return state_;
	}
	// 再起動
	virtual void Reset() override {
		// 待機状態にする
		state_ = State::READY;
		isConditionChecked_ = false;
		isConditionMet_ = false;
		for (auto& child : children_) {
			child->Reset();
		}
	}

	// 子ノードの番号をセットする(※複数体のセットをしない)
	virtual void SetChild(std::unique_ptr<IBehavior> child)override { 

		// クリアしてからセットする
		children_.clear();
		children_.push_back(std::move(child));
	}
	// 子ノードの番号をまとめてセットする
	virtual void SetChildren(std::vector<std::unique_ptr<IBehavior>>&& children) override { children; }

protected:

	// 敵クラスのポインタ
	IEnemy* enemy_;
	// 条件関数
	std::function<bool()> func_;
	// 条件関数を満たしたかどうか
	bool isConditionMet_ = false;
	// 条件関数を実行したか
	bool isConditionChecked_ = false;

};

