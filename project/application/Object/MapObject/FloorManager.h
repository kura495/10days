#pragma once
#include "Floor/Floor.h"

class FloorManager
{
public:

	// 初期化
	void Init();
	// 更新
	void Update();
	// 描画
	void Draw();

	// 床の行列(マップチップ)取得
	const std::array<std::array<int32_t, 20>, 10>& GetFloorMatrix() { return floorMatrix_; }
	

protected:

	// 床配列
	std::vector<std::unique_ptr<Floor>> floors_;

	// 床のモデル
	std::vector<Model*> floorModel_;

	// 床の行列(マップチップ)
	std::array<std::array<int32_t, 20>, 10> floorMatrix_;


};

