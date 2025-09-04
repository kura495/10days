#pragma once
#include "Model/Model.h"
#include "Collider/OBB/OBBoxCollider.h"
#include <vector>

class Floor
{
public:

	// 初期化
	void Init(std::vector<Model*> models);
	// 更新
	void Update();
	// 描画
	void Draw();

	// 衝突
	void OnCollision(const ICollider& Collider);

protected:

	std::vector<Model*> models_;
	WorldTransform world_;
	uint32_t objectNumber_ = 0;

	OBBoxCollider collider;

};

