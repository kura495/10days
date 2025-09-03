#pragma once
#include "Sprite/Sprite.h"
#include "Input/Input.h"

class Player {
public:
	void Init(std::string mainTexName, Vector2 texSize);
	void Update();
	void Draw();
private:
	std::unique_ptr<Sprite> mainTex_Sprite;
	WorldTransform mainTex_TF;
};