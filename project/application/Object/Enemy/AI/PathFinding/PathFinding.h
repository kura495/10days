#pragma once
#include <set>
#include <queue>
#include <vector>
#include "Math/Vector/Vector3.h"

using namespace Math;

struct MapNode {
	int32_t x, y;	// ノードの座標
	int32_t g;		// 開始ノードからの実際のコスト
	int32_t h;		// 終了ノードまでのManhattan距離
	int32_t f;		// g + h　の合計評価値
	MapNode* parent;	// 親ノードへのポインタ

	bool operator<(const MapNode& other) const {
		return f < other.f;
	}

	MapNode operator=(const MapNode& other) {
		return MapNode{
			x = other.x,
			y = other.y,
			g = other.g,
			h = other.h,
			f = other.f,
			parent = other.parent
		};
	}

	bool operator==(const MapNode& other) const {
		return{
			x == other.x &&
			y == other.y &&
			g == other.g &&
			h == other.h &&
			f == other.f &&
			parent == other.parent
		};
	}

};

// 前方宣言
class IEnemy;
class FloorManager;

class PathFinding
{
public:

	PathFinding() = default;
	~PathFinding() = default;

	void Init(IEnemy* enemy, FloorManager* floorManager);
	void Update();


	// -- 経路探索 関係 -- //

	// ワールド座標からマップチップ上での座標を取得
	MapNode ConvertWorldPosToMapChipPos(const Vector3& worldPos);

	// A*アルゴリズムによる経路探索を行い、経路上のノードの配列を返す
	std::vector<MapNode*> GetAstar(const MapNode& start, const MapNode& end);
	// AI用に経路をロード
	void LoadAI(const MapNode& enemyPos, const MapNode& playerPos);
	// 経路のリセット
	void ResetRoute(const MapNode& enemyPos, const MapNode& playerPos);
	// 経路の削除
	void DeleteRoute();
	// 指定座標が有効かどうか
	bool GetIsValid(int32_t x, int32_t y);
	// マンハッタン距離を計算
	int32_t GetManhattanDistance(const MapNode& a, const MapNode& b);
	// 斜め移動を許可したマンハッタン距離を計算
	int32_t ManhattanDistanceWithDiagonal(const MapNode& a, const MapNode& b);

	// 移動方向の取得
	Vector3 GetMoveDirection();

protected:

	// 敵キャラ ポインタ
	IEnemy* enemy_ = nullptr;
	// マップチップマネージャー ポインタ
	FloorManager* floorManager_ = nullptr;

	// 経路探索の更新頻度
	const int32_t kPathFindingInterval = 300;
	// 経路更新までのカウント
	int32_t pathUpdateCount_;

	// 現在地
	MapNode now = { 0,0,0,0,0,nullptr };
	// 初期地点
	MapNode prev_ = now;
	// ターゲットの位置
	MapNode next_ = now;

	// moveTimer
	float moveT = 0.0f;
	// 経路を保存
	std::vector<MapNode*>path_;

};

