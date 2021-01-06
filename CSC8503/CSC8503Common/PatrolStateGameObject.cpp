#include "PatrolStateGameObject.h"
#include <algorithm>
using namespace NCL;
using namespace CSC8503;
PatrolStateGameObject::PatrolStateGameObject(vector<Vector3> positions, GameObject* val) {
	locations = positions;
	object = val;
	direction = object->GetTransform().GetPosition() - this->GetTransform().GetPosition();
	State* stateA = new State([&](float dt)->void {
		this->Patrol(dt);
	});
	State* stateB = new State([&](float dt)->void {
		this->FollowPlayer(dt);
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
	for (int i = 0; i < locations.size(); ++i) {
		Vector3 currentPos = { GetTransform().GetPosition().x, 0, GetTransform().GetPosition().z };		// Ignore y axis
		Vector3 dirStrength = locations.at(i) - currentPos;
		if (dirStrength.Length() < 10.0f) {
			GetPhysicsObject()->ClearForces();
			locations.erase(locations.begin());
			if (locations.size() == locations.size() - 1) {		// Path completed, so go back to start
				i = 0;
			}
		}
		GetPhysicsObject()->ApplyLinearImpulse((locations.at(i) - currentPos) * 0.005);
	}
}

void PatrolStateGameObject::FollowPlayer(float dt) {
	GetPhysicsObject()->AddForce({ std::clamp(direction.x, -5.0f, 5.0f), 
		std::clamp(direction.y, -5.0f, 5.0f), std::clamp(direction.z, -5.0f, 5.0f) });
}
