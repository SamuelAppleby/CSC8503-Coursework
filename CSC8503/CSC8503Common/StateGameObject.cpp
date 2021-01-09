#include "StateGameObject.h"
using namespace NCL;
using namespace CSC8503;
StateGameObject::StateGameObject() {
	stateMachine = new StateMachine();
	currentState = state::IDLE;
	idleState = new State([&](float dt)->void {
		this->Idle();
	});
	stateMachine->AddState(idleState);
}

StateGameObject::~StateGameObject() {
	delete stateMachine;
}

void StateGameObject::Update(float dt) {
	stateMachine->Update(dt);
}

string StateGameObject::StateToString() const {
	switch (currentState) {
		case state::IDLE:
			return "Idling";
		case state::FOLLOWROUTE:
			return "Following Route";
		case state::FOLLOWPATH:
			return "Following A* Path";
		case state::FOLLOWPLAYER:
			return "Following Player";
		default:
			return "State Not Known";
	}
}

void StateGameObject::Idle() {
}