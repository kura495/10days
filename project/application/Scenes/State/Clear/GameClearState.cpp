#include"Scenes/State/Clear/GameClearState.h"

void GameClearState::Init()
{
	
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

	
}

