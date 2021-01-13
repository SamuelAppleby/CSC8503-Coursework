#include "PatrolStateGameObject.h"
#include <algorithm>
using namespace NCL;
using namespace CSC8503;
PatrolStateGameObject::PatrolStateGameObject(GameObject* val, vector<Vector3> positions) : EnemyStateGameObject(val) {
	currentState = state::FOLLOWROUTE;
	route = positions;
	object = val;
	currentDest = 0;
	patrolState = new State([&](float dt)->void {
		this->Patrol(dt);
	});
	stateMachine->AddState(patrolState);
	stateMachine->AddTransition(new StateTransition(followPlayerState, patrolState, [&]()->bool {
		if (this->followTimeout < 0.0f) {
			currentState = state::FOLLOWROUTE;
			return true;
		}
		return false;
	}));
	stateMachine->AddTransition(new StateTransition(patrolState, followPlayerState, [&]()->bool {
		if (this->followTimeout > 0.0f) {
			currentState = state::FOLLOWPLAYER;
			return true;
		}
		return false;
	}));
	stateMachine->AddTransition(new StateTransition(idleState, patrolState, [&]()->bool {
		if (this->route.size() > 0) {
			currentState = state::FOLLOWROUTE;
			return true;
		}
		return false;
	}));
	stateMachine->AddTransition(new StateTransition(patrolState, idleState, [&]()->bool {
		if (this->route.size() == 0) {
			currentState = state::IDLE;
			return true;
		}
		return false;
	}));
	name = "PatrolAI";
}

void PatrolStateGameObject::Patrol(float dt) {
	Vector3 currentPos = { GetTransform().GetPosition().x, 0, GetTransform().GetPosition().z };		// Ignore y axis
	Vector3 dirStrength = route.at(currentDest) - currentPos;
	if (dirStrength.Length() < 10.0f) {
		++currentDest;
		if (currentDest == route.size() - 1) 		// Path completed, so go back to start
			currentDest = 0;
	}
	GetPhysicsObject()->ApplyLinearImpulse((route.at(currentDest) - currentPos) * 0.005);
}
