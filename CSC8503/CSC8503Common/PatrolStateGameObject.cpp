#include "PatrolStateGameObject.h"
#include <algorithm>
using namespace NCL;
using namespace CSC8503;
PatrolStateGameObject::PatrolStateGameObject(GameObject* val, vector<Vector3> positions) : EnemyStateGameObject(val) {
	currentState = state::FOLLOWROUTE;
	locations = positions;
	object = val;
	patrolState = new State([&](float dt)->void {
		this->Patrol(dt);
	});
	stateMachine->AddState(patrolState);
	stateMachine->AddTransition(new StateTransition(followPlayerState, patrolState, [&]()->bool {
		return currentState == state::FOLLOWROUTE;
	}));
	stateMachine->AddTransition(new StateTransition(patrolState, followPlayerState, [&]()->bool {
		return currentState == state::FOLLOWPLAYER;
	}));
	stateMachine->AddTransition(new StateTransition(idleState, patrolState, [&]()->bool {
		return currentState == state::FOLLOWROUTE;
		}));
	stateMachine->AddTransition(new StateTransition(patrolState, idleState, [&]()->bool {
		return currentState == state::IDLE;
		}));
	name = "PatrolAI";
}

void PatrolStateGameObject::Patrol(float dt) {
	for (int i = 0; i < locations.size(); ++i) {
		Vector3 currentPos = { GetTransform().GetPosition().x, 0, GetTransform().GetPosition().z };		// Ignore y axis
		Vector3 dirStrength = locations.at(i) - currentPos;
		if (dirStrength.Length() < 10.0f) {
			GetPhysicsObject()->ClearForces();
			locations.erase(locations.begin());
			if (locations.size() == locations.size() - 1) 		// Path completed, so go back to start
				i = 0;
		}
		GetPhysicsObject()->ApplyLinearImpulse((locations.at(i) - currentPos) * 0.005);
	}
}
