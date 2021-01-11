#pragma once
#include "PushdownState.h"
#include "../CSC8503Common/GlobalVariables.h"
#include "../GameTech/TutorialGame.h"
using namespace NCL;
using namespace CSC8503;
bool quit = false;
class IntroScreen : public PushdownState {
	PushdownResult OnUpdate(float dt, PushdownState** newState) override {
		if (quit) {
			return PushdownResult::Pop;
		}
		else {
			*newState = new TutorialGame();
			return PushdownResult::Push;
		}
		return PushdownResult::NoChange;
	};
	void OnAwake() override {
	}
};

