#include "PlatformStateGameObject.h"
using namespace NCL;
using namespace CSC8503;
PlatformStateGameObject::PlatformStateGameObject(Vector3 start, Vector3 end) {
	moving = true;
	xSpeed = 0;
	ySpeed = 0;
	zSpeed = 0;
	invMass = 0.0f;
	elasticity = 0.0f;
	friction = 0.5f;
	this->start = start;
	this->end = end;
	name = "MovingFloor";
	State* stateA = new State([&](float dt)->void {
		this->Idle(dt);
	});
	State* stateB = new State([&](float dt)->void {
		this->MoveToLocation(dt);
	});
	stateMachine->AddState(stateA);
	stateMachine->AddState(stateB);
	stateMachine->AddTransition(new StateTransition(stateA, stateB, [&]()->bool {
		return moving;
	}));
	stateMachine->AddTransition(new StateTransition(stateB, stateA, [&]()->bool {
		return !moving;
	}));
	name = "PlatformAI";
}
void PlatformStateGameObject::Idle(float dt) {
}

void PlatformStateGameObject::MoveToLocation(float dt) {
	if (abs(start.x - end.x) > 0.0) {
		if (abs(GetTransform().GetPosition().x - start.x) < 0.5)
			start.x - end.x > 0 ? xSpeed = -5 : xSpeed = 5;
		if (abs(GetTransform().GetPosition().x - end.x) < 0.5)
			start.x - end.x > 0 ? xSpeed = 5 : xSpeed = -5;
	}
	if (abs(start.y - end.y) > 0.0) {
		if (abs(GetTransform().GetPosition().y - start.y) < 0.5)
			start.y - end.y > 0 ? ySpeed = -5 : ySpeed = 5;
		if (abs(GetTransform().GetPosition().y - end.y) < 0.5)
			start.y - end.y > 0 ? ySpeed = 5 : ySpeed = -5;
	}
	if (abs(start.z - end.z) > 0.0) {
		if (abs(GetTransform().GetPosition().z - start.z) < 0.5)
			start.z - end.z > 0 ? zSpeed = -5 : zSpeed = 5;
		if (abs(GetTransform().GetPosition().z - end.z) < 0.5)
			start.z - end.z > 0 ? ySpeed = 5 : ySpeed = -5;
	}
	GetPhysicsObject()->SetLinearVelocity({ xSpeed, ySpeed, zSpeed });
}