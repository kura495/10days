#include "Enemy_Bee.h"

void Enemy_Bee::Update() {
	if (Input::pushPad(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
		ImGui::Begin("Enemy");
		ImGui::Text("IsPush");
		ImGui::End();
	}
}