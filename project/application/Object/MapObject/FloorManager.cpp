#include "FloorManager.h"

void FloorManager::Init()
{
	// 床のモデルを読み込み
	floorModel_.push_back(Model::CreateModelFromObj("project/resources/2DPlane", "2DPlane.obj"));
	// 床の行列(マップチップ)を設定
	floorMatrix_ = {
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,
		1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,
		1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	};

	// 床を生成
	for (uint32_t y = 0; y < floorMatrix_.size(); y++) {
		for (uint32_t x = 0; x < floorMatrix_[y].size(); x++) {
			if (floorMatrix_[y][x] != 0) {
				std::unique_ptr<Floor> floor = std::make_unique<Floor>();
				floor->Init(floorModel_, Vector2((float)x*2, (float)y*2));
				floors_.push_back(std::move(floor));
			}
		}
	}

}


void FloorManager::Update()
{
	// 床を更新
	for (auto& floor : floors_) {
		floor->Update();
	}
}

void FloorManager::Draw()
{
	// 床を描画
	for (auto& floor : floors_) {
		floor->Draw();
	}
}

std::vector<MapNode*> FloorManager::GetAstar(const MapNode& start, const MapNode& end)
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
			if (neighbor.x < 0 || neighbor.x >= (int)floorMatrix_[0].size() ||
				neighbor.y < 0 || neighbor.y >= (int)floorMatrix_.size() ||
				floorMatrix_[neighbor.y][neighbor.x] == 0) {
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
