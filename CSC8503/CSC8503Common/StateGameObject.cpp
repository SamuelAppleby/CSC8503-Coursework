#include "StateGameObject.h"
using namespace NCL;
using namespace CSC8503;
StateGameObject::StateGameObject() {
	stateMachine = new StateMachine();
}
StateGameObject::~StateGameObject() {
	delete stateMachine;
}
void StateGameObject::Update(float dt) {
	stateMachine->Update(dt);
}
