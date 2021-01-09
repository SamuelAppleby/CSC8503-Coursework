#include "PlatformStateGameObject.h"
using namespace NCL;
using namespace CSC8503;
PlatformStateGameObject::PlatformStateGameObject(Vector3 start, Vector3 end) {
	currentState = state::FOLLOWROUTE;
	speed = Vector3();
	invMass = 0.0f;
	elasticity = 0.0f;
	friction = 0.5f;
	this->start = start;
	this->end = end;
	name = "MovingFloor";
	followRouteState = new State([&](float dt)->void {
		this->MoveToLocation(dt);
	});
	stateMachine->AddState(followRouteState);
	stateMachine->AddTransition(new StateTransition(idleState, followRouteState, [&]()->bool {
		return currentState == state::FOLLOWROUTE;
	}));
	stateMachine->AddTransition(new StateTransition(followRouteState, idleState, [&]()->bool {
		return currentState == state::IDLE;
	}));
	name = "PlatformAI";
}

void PlatformStateGameObject::MoveToLocation(float dt) {
	if (abs(start.x - end.x) > 0.0) {
		if (abs(GetTransform().GetPosition().x - start.x) < 0.5)
			speed.x = (start.x - end.x > 0) ? -5 : 5;
		if (abs(GetTransform().GetPosition().x - end.x) < 0.5)
			speed.x = (start.x - end.x > 0) ? 5 : -5;
	}
	if (abs(start.y - end.y) > 0.0) {
		if (abs(GetTransform().GetPosition().y - start.y) < 0.5)
			speed.y = (start.y - end.y > 0) ? -5 : 5;
		if (abs(GetTransform().GetPosition().y - end.y) < 0.5)
			speed.y = (start.y - end.y) > 0 ? 5 : -5;
	}
	if (abs(start.z - end.z) > 0.0) {
		if (abs(GetTransform().GetPosition().z - start.z) < 0.5)
			speed.z = (start.z - end.z > 0) ? -5 : 5;
		if (abs(GetTransform().GetPosition().z - end.z) < 0.5)
			speed.z = (start.z - end.z > 0) ? 5 : -5;
	}
	GetPhysicsObject()->SetLinearVelocity(speed);
}