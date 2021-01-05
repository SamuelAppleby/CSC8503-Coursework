#include "OscillateStateGameObject.h"
using namespace NCL;
using namespace CSC8503;
OscillateStateGameObject::OscillateStateGameObject() {
	counter = 0.0f;
	State* stateA = new State([&](float dt)->void {
		this->MoveLeft(dt);
	});
	State* stateB = new State([&](float dt)->void {
		this->MoveRight(dt);
	});
	stateMachine->AddState(stateA);
	stateMachine->AddState(stateB);
	stateMachine->AddTransition(new StateTransition(stateA, stateB, [&]()->bool {
		return this->counter > 3.0f;
	}));
	stateMachine->AddTransition(new StateTransition(stateB, stateA, [&]()->bool {
		return this->counter < 0.0f;
	}));
	name = "OscillateAI";
}
void OscillateStateGameObject::MoveLeft(float dt) {
	GetPhysicsObject()->AddForce({ -1, 0, 0 });
	counter += dt;
}
void OscillateStateGameObject::MoveRight(float dt) {
	GetPhysicsObject()->AddForce({ 1, 0, 0 });
	counter -= dt;
}