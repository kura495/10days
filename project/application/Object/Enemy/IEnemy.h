#pragma once
#include "Sprite/Sprite.h"
#include "Input/Input.h"

class IEnemy {
public:
	IEnemy() = default;
	virtual ~IEnemy() = default;

	virtual void Init(std::string mainTexName,Vector2 texSize) {
		mainTex_TF.Init();
		mainTex_TF.transform.translate = { 64.0f,64.0f };
		mainTex_Sprite = std::make_unique<Sprite>();

		mainTex_Sprite->Init({ 0.5f,0.5f }, texSize);
		mainTex_Sprite->TextureHandle = TextureManager::GetInstance()->LoadTexture(mainTexName);


	};
	virtual void Update() { 
#pragma region
		ImGui::Begin("Enemy");
		ImGui::DragFloat3("TexPos",&mainTex_TF.transform.translate.x);
		ImGui::End();
#pragma endregion
		
		mainTex_TF.Update();

	};
	virtual void Draw() { mainTex_Sprite->RendererDraw(mainTex_TF); };
	void PlayerPos(Math::Vector3 position) { player_Pos = position; };
protected:
	std::unique_ptr<Sprite> mainTex_Sprite;
	WorldTransform mainTex_TF;
	Vector3 player_Pos = {0.0f};

};