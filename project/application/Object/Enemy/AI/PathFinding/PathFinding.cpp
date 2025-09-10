#include "PathFinding.h"
#include "Enemy/IEnemy.h"
#include "MapObject/FloorManager.h"

void PathFinding::Init(IEnemy* enemy, FloorManager* floorManager) {

	// 敵キャラのポインタを保存
	enemy_ = enemy;
	// 床マネージャーのポインタを保存
	floorManager_ = floorManager;

}

void PathFinding::Update() {

	// 経路更新カウントを加算
	if(pathUpdateCount_ < kPathFindingInterval) {
		pathUpdateCount_++;
	}

	// カウントが更新頻度に達したら経路を更新
	if (pathUpdateCount_ >= kPathFindingInterval) {
		
		// 経路が存在する場合、リセットを行う
		if (!path_.empty()) {
			DeleteRoute();
		}

		// A*アルゴリズムで経路探索を行い、経路上のノードのポインタ配列を取得
		LoadAI(ConvertWorldPosToMapChipPos(enemy_->GetPos()),ConvertWorldPosToMapChipPos(enemy_->GetPlayerPos()));
		
		// カウントのリセット
		pathUpdateCount_ = 0;
	}

}

MapNode PathFinding::ConvertWorldPosToMapChipPos(const Vector3& worldPos)
{
	// 戻り値用のノード
	MapNode result{};

	// マップチップの行列を取得
	const auto& floorMatrix = floorManager_->GetFloorMatrix();
	
	// マップチップの行列のサイズを取得
	int32_t mapWidth = static_cast<int32_t>(floorMatrix[0].size());
	int32_t mapHeight = static_cast<int32_t>(floorMatrix.size());
	
	// ワールド座標をマップチップの座標に変換
	result.x = static_cast<int32_t>(worldPos.x);
	result.y = mapHeight - static_cast<int32_t>(worldPos.y) - 1;

	// 座標がマップチップの範囲外の場合、範囲内に収める
	if (result.x < 0) result.x = 0;
	if (result.x >= mapWidth) result.x = mapWidth - 1;
	if (result.y < 0) result.y = 0;
	if (result.y >= mapHeight) result.y = mapHeight - 1;

	// 戻り値を返す
	return result;
}

std::vector<MapNode*> PathFinding::GetAstar(const MapNode& start, const MapNode& end)
{
	// A*アルゴリズムで経路探索を行い、経路上のノードのポインタ配列を返す
	std::set<MapNode*> openList{}; // オープンリスト
	std::vector<MapNode*> closedList{}; // クローズドリスト

	// スタートノードをオープンリストに追加
	openList.insert(new MapNode(start));

	// 終了ノードに到達するまでループ
	while (!openList.empty()) {

		// オープンリストから最小のf値を持つノードを取得
		auto currentIt = std::min_element(openList.begin(), openList.end(),
			[](MapNode* a, MapNode* b) { return a->f < b->f; });

		MapNode* currentNode = *currentIt;
		// 終了ノードに到達した場合、経路を構築して返す
		if (currentNode->x == end.x && currentNode->y == end.y) {
			std::vector<MapNode*> path;
			while (currentNode) {
				path.push_back(currentNode);
				currentNode = currentNode->parent;
			}
			std::reverse(path.begin(), path.end());
			// メモリ解放
			for (auto node : openList) delete node;
			for (auto node : closedList) delete node;
			return path;
		}
		// 現在のノードをオープンリストから削除し、クローズドリストに追加
		openList.erase(currentIt);
		closedList.push_back(currentNode);
		// 隣接ノードを取得
		std::vector<MapNode> neighbors = {
			{currentNode->x + 1, currentNode->y, 0, 0, 0, currentNode}, // 右
			{currentNode->x - 1, currentNode->y, 0, 0, 0, currentNode}, // 左
			{currentNode->x, currentNode->y + 1, 0, 0, 0, currentNode}, // 下
			{currentNode->x, currentNode->y - 1, 0, 0, 0, currentNode}  // 上
		};
		for (auto& neighbor : neighbors) {
			// クローズドリストに存在する場合はスキップ
			if (std::find_if(closedList.begin(), closedList.end(),
				[&neighbor](MapNode* node) { return node->x == neighbor.x && node->y == neighbor.y; }) != closedList.end()) {
				continue;
			}
			// 障害物の場合はスキップ
			if (neighbor.x < 0 || neighbor.x >= (int) floorManager_->GetFloorMatrix()[0].size() ||
				neighbor.y < 0 || neighbor.y >= (int) floorManager_->GetFloorMatrix().size() ||
				 floorManager_->GetFloorMatrix()[neighbor.y][neighbor.x] == 0) {
				continue;
			}
			// g, h, f値を計算
			neighbor.g = currentNode->g + 1;
			neighbor.h = abs(neighbor.x - end.x) + abs(neighbor.y - end.y);
			neighbor.f = neighbor.g + neighbor.h;
			// オープンリストに存在する場合、g値が小さい場合のみ更新
			auto openIt = std::find_if(openList.begin(), openList.end(),
				[&neighbor](MapNode* node) { return node->x == neighbor.x && node->y == neighbor.y; });
			if (openIt != openList.end()) {
				if (neighbor.g < (*openIt)->g) {
					(*openIt)->g = neighbor.g;
					(*openIt)->f = neighbor.f;
					(*openIt)->parent = currentNode;
				}
			}
			else {
				// オープンリストに追加
				openList.insert(new MapNode(neighbor));
			}
		}
	}

	// 経路が見つからなかった場合、空の配列を返す
	// メモリ解放
	for (auto node : openList) delete node;
	for (auto node : closedList) delete node;
	return std::vector<MapNode*>();
}

void PathFinding::LoadAI(const MapNode& enemyPos, const MapNode& playerPos)
{

	// A*アルゴリズムで経路探索を行い、経路上のノードのポインタ配列を取得
	path_ = GetAstar(enemyPos, playerPos);

}

void PathFinding::ResetRoute(const MapNode& enemyPos, const MapNode& playerPos)
{
	// 計算済みの経路が存在する場合、消去する
	while (!path_.empty()) {
		// 経路から最初のノードを削除
		path_.erase(path_.begin());
	}

	// 経路削除後に再計算を行う
	LoadAI(enemyPos, playerPos);
}

void PathFinding::DeleteRoute()
{
	// 計算済みの経路が存在する場合、消去する
	while (!path_.empty()) {
		// 経路から最初のノードを削除
		path_.erase(path_.begin());
	}
}

// マップの境界・障害物チェック
bool PathFinding::GetIsValid(int32_t x, int32_t y) {

	// マップの境界チェック
	if (x < 0 || x >= floorManager_->GetFloorMatrix()[0].size() || y < 0 || y >= floorManager_->GetFloorMatrix().size()) {
		return false;
	}

	// マップの障害物チェック
	if (floorManager_->GetFloorMatrix()[y][x] == 1) {
		return false;// 障害物がある場合は移動できない
	}

	return true;// 移動可能な場合
}

// マンハッタン距離を計算する
int32_t PathFinding::GetManhattanDistance(const MapNode& a, const MapNode& b) {
	return abs(b.x - a.x) + abs(b.y - a.y);
}

// 斜め移動可能なマンハッタン距離の計算
int32_t PathFinding::ManhattanDistanceWithDiagonal(const MapNode& a, const MapNode& b) {
	if (std::abs(b.x - a.x) >= std::abs(b.y - a.y)) {
		return std::abs(b.x - a.x);
	}
	return std::abs(b.y - a.y);
}