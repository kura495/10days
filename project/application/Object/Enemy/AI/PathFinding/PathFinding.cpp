#include "PathFinding.h"
#include "Enemy/IEnemy.h"
#include "MapObject/FloorManager.h"

void PathFinding::Init(IEnemy* enemy, FloorManager* floorManager) {

	// 敵キャラのポインタを保存
	enemy_ = enemy;
	// 床マネージャーのポインタを保存
	floorManager_ = floorManager;
	// 経路更新までのカウント
	pathUpdateCount_ = kPathFindingInterval;

	// 初期座標を取得
	now = ConvertWorldPosToMapChipPos(enemy_->GetPos());
	prev_ = now;

}

void PathFinding::Update() {
    
	// 経路更新カウントを減算
	if (pathUpdateCount_ > 0) { --pathUpdateCount_; }


	// 経路更新タイミング
	if (pathUpdateCount_ == 0) {
		
		//now = next_;
		//prev_ = next_;
		pathUpdateCount_ = kPathFindingInterval;
		LoadAI(ConvertWorldPosToMapChipPos(enemy_->GetPos()), ConvertWorldPosToMapChipPos(enemy_->GetPlayerPos()));
	}



#ifdef _DEBUG

	ImGui::Begin("Route");
	int32_t enemyPos[2] = {ConvertWorldPosToMapChipPos(enemy_->GetPos()).x,ConvertWorldPosToMapChipPos(enemy_->GetPos()).y};
	ImGui::DragInt2("E-Node:",enemyPos );
	int32_t playerPos[2] = {ConvertWorldPosToMapChipPos(enemy_->GetPlayerPos()).x,ConvertWorldPosToMapChipPos(enemy_->GetPlayerPos()).y};
	ImGui::DragInt2("P-Node:", playerPos);
	ImGui::DragInt("Interval", &pathUpdateCount_);

	//
	ImGui::Text("Prev: (%d, %d, %d, %d, %d)", prev_.x, prev_.y, prev_.g, prev_.h, prev_.f);
	ImGui::Text("Now: (%d, %d, %d, %d, %d)", now.x, now.y, now.g, now.h, now.f);
	ImGui::Text("Next: (%d, %d, %d, %d, %d)", next_.x, next_.y, next_.g, next_.h, next_.f);


	// 経路(MapNodeの情報)を表示
	for (size_t i = 0; i < path_.size(); ++i) {
		ImGui::Text("Path[%d]: (%d, %d, %d, %d, %d)", static_cast<int32_t>(i), path_[i]->x, path_[i]->y, path_[i]->g, path_[i]->h, path_[i]->f);
	}

	ImGui::End();

#endif // _DEBUG




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
	
	// マップチップの1セルの大きさ
	const float cellSize = 2.0f;
	// ワールド座標をマップチップのスケールに変換
	Vector3 scaledPos = worldPos / cellSize;

	// 小数点以下を切り捨て
	result.x = static_cast<int32_t>(scaledPos.x);
	result.y = static_cast<int32_t>(scaledPos.y);


	// 座標がマップチップの範囲外の場合、範囲内に収める
	if (result.x < 0) result.x = 0;
	if (result.x >= mapWidth) result.x = mapWidth - 1;
	if (result.y < 0) result.y = 0;
	if (result.y >= mapHeight) result.y = mapHeight - 1;

	// 評価値や距離は0に初期化
	result.g = 0;
	result.h = 0;
	result.f = 0;

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
                path.push_back(new MapNode(*currentNode)); // コピーを作成
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
			if (neighbor.x < 0 || neighbor.x >= (int)floorManager_->GetFloorMatrix()[0].size() ||
				neighbor.y < 0 || neighbor.y >= (int)floorManager_->GetFloorMatrix().size() ||
				floorManager_->GetFloorMatrix()[neighbor.y][neighbor.x] == 1) {
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
	return {};

	//std::set<MapNode*> openSet{};
	//std::vector<MapNode*> closedSet{};

	//openSet.insert(new MapNode(start));

	//// オープンリストがから出ない場合
	//while (!openSet.empty()) {

	//	// オープンリストから最小評価値のノードを取り出す
	//	MapNode* current = *openSet.begin();
	//	// オープンリスト内の取り出したノードを削除
	//	openSet.erase(openSet.begin());
	//	// クローズドリストに取り出したノードを入れる
	//	closedSet.push_back(current);

	//	if (current->x == end.x && current->y == end.y) {
	//		std::reverse(closedSet.begin(), closedSet.end());
	//		while (current->parent != nullptr) {
	//			path_.push_back(current);
	//			current = current->parent;

	//			if (current->parent == current) {
	//				break;
	//			}
	//		}
	//		std::reverse(path_.begin(), path_.end());
	//		return path_;
	//	}

	//	for (int32_t dy = -1; dy <= 1; ++dy) {
	//		for (int32_t dx = -1; dx <= 1; ++dx) {
	//			// 現在のマスは除外する
	//			if (dx == 0 && dy == 0) {
	//				continue;
	//			}
	//			// 斜め移動をしない
	//			if ((dx == -1 && dy == -1) || (dx == 1 && dy == -1) ||
	//				(dx == -1 && dy == 1) || (dx == 1 && dy == 1)) {
	//				continue;
	//			}

	//			// 隣接した点の計算
	//			int32_t newX = current->x + dx;
	//			int32_t newY = current->y + dy;

	//			// 進行可能な場合のみ
	//			if (GetIsValid(newX, newY)) {
	//				if (std::find_if(closedSet.begin(), closedSet.end(), [newX, newY](const MapNode* MapNode) {
	//					return MapNode->x == newX && MapNode->y == newY;
	//					}) == closedSet.end()) {

	//					MapNode* newMapNode = new MapNode{
	//						newX, newY,
	//						current->g + 1,
	//						GetManhattanDistance({ newX, newY, 0, 0, 0, nullptr }, { end.x, end.y, 0, 0, 0, nullptr }),
	//						0,
	//						nullptr
	//					};

	//					newMapNode->f = newMapNode->g + newMapNode->h;

	//					if (!closedSet.empty()) {
	//						MapNode* lastClosedMapNode = closedSet.back();
	//						newMapNode->parent = lastClosedMapNode;
	//					}

	//					if (std::find_if(openSet.begin(), openSet.end(), [newX, newY](const MapNode* MapNode) {
	//						return MapNode->x == newX && MapNode->y == newY;
	//						}) != openSet.end()) {
	//						auto it = std::find_if(openSet.begin(), openSet.end(), [newX, newY](const MapNode* MapNode) {
	//							return MapNode->x == newX && MapNode->y == newY;
	//							});

	//						if (newMapNode->f < (*it)->f) {
	//							openSet.erase(it);
	//							openSet.insert(newMapNode);
	//						}
	//					}
	//					else {
	//						openSet.insert(newMapNode);
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	//return std::vector<MapNode*>();

}

void PathFinding::LoadAI(const MapNode& enemyPos, const MapNode& playerPos)
{
    DeleteRoute(); // 既存経路を必ず消す
    path_ = GetAstar(enemyPos, playerPos);
    //if (!path_.empty()) {
    //    // 最初のノードは現在地なので、次のノードをnext_にセット
    //    if (path_.size() > 1) {
    //        next_ = *path_[1];
    //    } else {
    //        next_ = *path_[0];
    //    }
    //}
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
    path_.clear();
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

Vector3 PathFinding::GetMoveDirection()
{
	// 経路が存在しない場合、ゼロベクトルを返す
	if (path_.empty()) {
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	// 現在のノードと次のノードのワールド座標を計算
	Vector3 currentWorldPos = Vector3(static_cast<float>(now.x) * 2.0f, static_cast<float>(now.y) * 2.0f, 0.0f);
	Vector3 nextWorldPos = Vector3(static_cast<float>(next_.x) * 2.0f, static_cast<float>(next_.y) * 2.0f, 0.0f);
	// 移動方向を計算
	Vector3 moveDir = Vector3::Normalize(nextWorldPos - currentWorldPos);

	return moveDir;

}
