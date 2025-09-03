#include "Player.h"

void Player::Init(std::string mainTexName, Vector2 texSize) {
	mainTex_TF.Init();
	mainTex_TF.transform.translate = { 640.0f,64.0f };
	mainTex_Sprite = std::make_unique<Sprite>();

	mainTex_Sprite->Init({ 0.5f,0.5f }, texSize);
	mainTex_Sprite->TextureHandle = TextureManager::GetInstance()->LoadTexture(mainTexName);
}

void Player::Update() {

	mainTex_TF.Update();
}

void Player::Draw() {
	mainTex_Sprite->RendererDraw(mainTex_TF);
}