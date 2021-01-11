#pragma once
using namespace NCL;
using namespace CSC8503;
class PauseScreen : public PushdownState {
	PushdownResult OnUpdate(float dt, PushdownState** newState) override {
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::P)) {
			return PushdownResult::Pop;
		}
		return PushdownResult::NoChange;
	}
	void OnAwake() override {
	}
};

