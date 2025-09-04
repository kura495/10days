#pragma once
#include "Sprite/Sprite.h"
#include "Input/Input.h"
#include "Collider/OBB/OBBoxCollider.h"

class Player_Test {
public:
	void Init();
	void Update();
	void Draw();
private:

	std::unique_ptr<Model> mainModel_3D;
	WorldTransform worldTF_3Dmodel_;
};