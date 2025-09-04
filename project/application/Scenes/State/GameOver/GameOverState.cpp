#include"Scenes/State/GameOver/GameOverState.h"

void GameOverState::Init()
{
	
}

void GameOverState::Update()
{
	if (Input::GetPadPrecede(XINPUT_GAMEPAD_A, 10)) {
		//ifの条件は後で変える
		if (StateNo != GameStateNo::TITLE) {
			StateNo = GameStateNo::TITLE;
		}
	}
}

void GameOverState::Draw()
{
	
}

