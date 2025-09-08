#pragma once
#include <vector>
#include <memory>

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

	// 仮想デストラクタ
	virtual ~IBehavior() = default;

	// 実行
	virtual State Tick() = 0;

	// 再起動
	virtual void Reset() = 0;

	// 子ノードの番号をセットする
	virtual void SetChild(IBehavior* child) = 0;
	// 子ノードの番号をまとめてセットする
	virtual void Setchildren(const std::vector<IBehavior*>& children) = 0;


protected:

	// 子ノード配列
	std::vector<IBehavior*> mChildren;
	// 現在のノードの成否/状態
	State mState = State::READY;

};


// -- Selector クラス -- //
class Selector :
	public IBehavior
{

	// デストラクタ
	virtual ~Selector()override {};

	// 実行処理
	virtual State Tick()override {

		// 実行終了状態であれば早期リターンする
		if (mState == State::SUCCESS || mState == State::FAILURE) {
			return mState;
		}

		for (auto& child : mChildren) {

			// 子ノードの処理を実行する
			State state = child->Tick();

			switch (state)
			{
				// いずれかの子ノードが成功したら成功を返す
				// また、セレクターの特徴としていずれか成功したら終了になる
			case State::SUCCESS:
				mState = State::SUCCESS;
				return mState;

				break;

				// 実行中の場合、実行中という結果を返して終了する
			case State::RUNNING:

				// 実行中状態にする
				mState = State::RUNNING;
				return mState;

				break;
			default:
				break;
			}

		}

		// ここまで来た場合は全ての子ノードが失敗した場合なので失敗を返して終了する
		mState = State::FAILURE;
		return mState;
	}

	// 再起動
	virtual void Reset() override {
		// 待機状態にする
		mState = State::READY;
		for (auto& child : mChildren) {
			child->Reset();
		}
	}

	// 子ノードの番号をセットする
	inline virtual void SetChild(IBehavior* child) override {
		mChildren.push_back(child);
	}

	// 子ノードの番号をまとめてセットする
	inline virtual void Setchildren(const std::vector<IBehavior*>& children)override {
		for (const auto& child : children) {
			mChildren.push_back(child);
		}
	}

};

// -- Sequence クラス -- //
class Sequence :
	public IBehavior
{

	// デストラクタ
	virtual ~Sequence()override {};

	// 実行処理
	virtual State Tick()override {

		// 実行終了状態であれば早期リターンする
		if (mState == State::SUCCESS || mState == State::FAILURE) {
			return mState;
		}

		for (auto& child : mChildren) {

			// 子ノードの処理を実行する
			State state = child->Tick();

			switch (state)
			{
				// いずれかの子ノードが成功したら成功を返す
				// また、シークエンスの特徴としていずれか失敗したら終了になる
			case State::FAILURE:
				mState = State::FAILURE;
				return mState;

				break;

				// 実行中の場合、実行中という結果を返して終了する
			case State::RUNNING:

				// 実行中状態にする
				mState = State::RUNNING;
				return mState;

				break;
			default:
				break;
			}

		}

		// ここまで来た場合は全ての子ノードが成功した場合なので成功を返して終了する
		mState = State::SUCCESS;
		return mState;
	}

	// 再起動
	virtual void Reset() override {
		// 待機状態にする
		mState = State::READY;
		for (auto& child : mChildren) {
			child->Reset();
		}
	}

	// 子ノードの番号をセットする
	inline virtual void SetChild(IBehavior* child) override {
		mChildren.push_back(child);
	}

	// 子ノードの番号をまとめてセットする
	inline virtual void Setchildren(const std::vector<IBehavior*>& children)override {
		for (const auto& child : children) {
			mChildren.push_back(child);
		}
	}

};

// -- Action クラス -- //
class Action :
	public IBehavior
{
	
	// 子ノードの番号をセットする
	virtual void SetChild(IBehavior* child)override { child; }
	// 子ノードの番号をまとめてセットする
	virtual void Setchildren(const std::vector<IBehavior*>& children) override { children; }
};

// -- Condition クラス -- //
class Condition :
	public IBehavior
{


	// 子ノードの番号をセットする
	virtual void SetChild(IBehavior* child)override { child; }
	// 子ノードの番号をまとめてセットする
	virtual void Setchildren(const std::vector<IBehavior*>& children) override { children; }
};

// -- Decorator クラス -- //
class Decorator :
	public IBehavior
{




	// 子ノードの番号をセットする(※複数体のセットをしない)
	virtual void SetChild(IBehavior* child)override { 

		// クリアしてからセットする
		mChildren.clear();
		mChildren.push_back(child);
	}
	// 子ノードの番号をまとめてセットする
	virtual void Setchildren(const std::vector<IBehavior*>& children) override { children; }
};

