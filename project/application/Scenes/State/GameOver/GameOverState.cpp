#include"Scenes/State/GameOver/GameOverState.h"

void GameOverState::Init()
{
	

	gameOverSprite = std::make_unique<Sprite>();
	gameOverSprite->Init({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
	gameOverSprite->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/GameOver.png");
	gameOver.Init();
}

void GameOverState::Update()
{
	if (Input::GetPadPrecede(XINPUT_GAMEPAD_A, 10)) {
		//ifの条件は後で変える
		if (StateNo != GameStateNo::TITLE) {
			StateNo = GameStateNo::TITLE;
		}
	}


	//if (Input::pushKey(DIK_SPACE)) {
	//	//ifの条件は後で変える
	//	if (StateNo != GameStateNo::TITLE) {
	//		StateNo = GameStateNo::TITLE;
	//	}
	//}

}

void GameOverState::Draw()
{
	gameOverSprite->RendererDraw(gameOver);
}

