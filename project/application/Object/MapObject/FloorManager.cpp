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
		1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,
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
