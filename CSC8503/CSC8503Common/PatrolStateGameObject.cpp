#include "PatrolStateGameObject.h"
#include <algorithm>
using namespace NCL;
using namespace CSC8503;
PatrolStateGameObject::PatrolStateGameObject(GameObject* val) {
	object = val;
	direction = object->GetTransform().GetPosition() - this->GetTransform().GetPosition();
	State* stateA = new State([&](float dt)->void {
		this->Patrol(dt);
	});
	State* stateB = new State([&](float dt)->void {
		this->Follow(dt);
	});
	stateMachine->AddState(stateA);
	stateMachine->AddState(stateB);
	stateMachine->AddTransition(new StateTransition(stateA, stateB, [&]()->bool {
		return direction.Length() < 30.0f;
	}));
	stateMachine->AddTransition(new StateTransition(stateB, stateA, [&]()->bool {
		return direction.Length() > 30.0f;
	}));
	name = "PatrolAI";
}
void PatrolStateGameObject::Update(float dt) {
	direction = object->GetTransform().GetPosition() - this->GetTransform().GetPosition();
	stateMachine->Update(dt);
}
void PatrolStateGameObject::Patrol(float dt) {
	GetPhysicsObject()->SetLinearVelocity({ -1, 0, 0 });
}
void PatrolStateGameObject::Follow(float dt) {
	GetPhysicsObject()->AddForce({ std::clamp(direction.x, -5.0f, 5.0f), 
		std::clamp(direction.y, -5.0f, 5.0f), std::clamp(direction.z, -5.0f, 5.0f) });
}
