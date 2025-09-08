#include"Scenes/State/Clear/GameClearState.h"

void GameClearState::Init()
{
	clearSprite = std::make_unique<Sprite>();
	clearSprite->Init({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
	clearSprite->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/Clear.png");
	clear.Init();
}

void GameClearState::Update()
{
	if (Input::GetPadPrecede(XINPUT_GAMEPAD_A, 10)) {
		//ifの条件は後で変える
		 if(StateNo != GameStateNo::TITLE){
			StateNo = GameStateNo::TITLE;
		 }
	}
}

void GameClearState::Draw()
{
	clearSprite->RendererDraw(clear);
	
}

