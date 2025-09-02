#pragma once
#include "Sprite/Sprite.h"

class IEnemy {
public:
	virtual void Init(std::string mainTexName,Vector2 texSize) {
		mainTex_TF.Init();
		mainTex_TF.transform.translate = { 64.0f,64.0f };
		mainTex_Sprite = std::make_unique<Sprite>();

		mainTex_Sprite->Init({ 0.5f,0.5f }, texSize);
		mainTex_Sprite->TextureHandle = TextureManager::GetInstance()->LoadTexture(mainTexName);


	};
	virtual void Update() { 

		ImGui::Begin();
		ImGui::DragFloat3("TexPos",&mainTex_TF.transform.translate.x);
		ImGui::End();
		mainTex_TF.Update();

	};
	virtual void Draw() { mainTex_Sprite->RendererDraw(mainTex_TF); };
protected:
	std::unique_ptr<Sprite> mainTex_Sprite;
	WorldTransform mainTex_TF;
};