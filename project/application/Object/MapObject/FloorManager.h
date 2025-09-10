#pragma once
#include "Floor/Floor.h"
#include <set>
#include <queue>

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

class FloorManager
{
public:

	// 初期化
	void Init();
	// 更新
	void Update();
	// 描画
	void Draw();

	// -- 経路探索 関係 -- //

	// ワールド座標からマップチップ上での座標を取得
	Vector2 ConvertWorldPosToMapChipPos(const Vector3& worldPos);

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


protected:

	// 床配列
	std::vector<std::unique_ptr<Floor>> floors_;

	// 床のモデル
	std::vector<Model*> floorModel_;

	// 床の行列(マップチップ)
	std::array<std::array<int32_t, 20>, 10> floorMatrix_;


};

