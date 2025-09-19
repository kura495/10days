#include"Scenes/State/Clear/GameClearState.h"

void GameClearState::Init()
{
	clearSprite = std::make_unique<Sprite>();
	clearSprite->Init({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
	clearSprite->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/Clear.png");
	clear.Init();

	texture = std::make_unique<Sprite>();
	texture->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/ATuto.png");
	texture->Init({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });

	skydome_ = Model::CreateModelFromObj("project/resources/TitleSkyDome", "TitleSkyDome.obj");

	stressBar = std::make_unique<Sprite>();
	stressBar->Init({ 0.0f,0.0f }, { 0.0f,108.0f }, { nowStress,0.0f }, { nowStress,108.0f });
	stressBar->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/StressBar.png");
	nowStress = 0;
	maxStressBar = std::make_unique<Sprite>();
	maxStressBar->Init({ 0.0f,0.0f }, { 0.0f,108.0f }, { MaxStress,0.0f }, { MaxStress,108.0f });
	maxStressBar->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/MaxStressBar.png");

	stressMeter = std::make_unique<Sprite>();
	stressMeter->Init({ 0.0f,0.0f }, { 0.0f,108.0f }, { 445.0f,0.0f }, { 445.0f,108.0f });
	stressMeter->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/StressMeter.png");



}

void GameClearState::Update()
{
	if (Input::GetPadPrecede(XINPUT_GAMEPAD_B, 10) || Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		//ifの条件は後で変える
		 if(StateNo != GameStateNo::TITLE){
			StateNo = GameStateNo::TITLE;
		 }
	}

	

#ifndef NDEBUG
	if (nowStress <= MaxStress) {
		nowStress += 0.2f;
		stressBar->Init({ 0.0f,0.0f }, { 0.0f,108.0f }, { nowStress,0.0f }, { nowStress,108.0f });
	}
	else if(nowStress > MaxStress)
	{
		nowStress = 500;
	}
#endif



}

void GameClearState::Draw()
{
	clearSprite->RendererDraw(clear);
	texture->RendererDraw(clear);
	skydome_->RendererDraw(clear);
#ifndef NDEBUG
	maxStressBar->RendererDraw(clear);
	stressBar->RendererDraw(clear);
	stressMeter->RendererDraw(clear);
#endif
}

