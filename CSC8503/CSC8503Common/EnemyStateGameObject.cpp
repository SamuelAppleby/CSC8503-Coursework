#include "EnemyStateGameObject.h"
#include <algorithm>
#include "../../Common/Maths.h"
using namespace NCL;
using namespace CSC8503;
EnemyStateGameObject::EnemyStateGameObject(GameObject* o) {
	object = o;
	rayTime = 0.5f;
	finished = false;
	followTimeout = 0.0f;
	travelDir = object->GetTransform().GetPosition() - this->GetTransform().GetPosition();
	followPlayerState = new State([&](float dt)->void {
		this->FollowPlayer(dt);
	});
	stateMachine->AddState(followPlayerState);
	stateMachine->AddTransition(new StateTransition(idleState, followPlayerState, [&]()->bool {
		if (this->followTimeout > 0.0f) {
			currentState = state::FOLLOWPLAYER;
			return true;
		}
		return false;
	}));
	stateMachine->AddTransition(new StateTransition(followPlayerState, idleState, [&]()->bool {
		if (this->followTimeout < 0.0f) {
			currentState = state::IDLE;
			return true;
		}
		return false;
	}));
	name = "EnemyAI";
}

void EnemyStateGameObject::Update(float dt) {
	if (currentState != state::IDLE) {
		Matrix3 rotMatrix;
		rotMatrix.SetColumn(0, -Vector3::Cross(Vector3(0, 1, 0), travelDir.Normalised()));
		rotMatrix.SetColumn(1, Vector3(0, 1, 0));
		rotMatrix.SetColumn(2, -travelDir.Normalised());
		GetTransform().SetOrientation(Matrix4(rotMatrix));
	}
	rayTime -= dt;
	stateMachine->Update(dt);
}

void EnemyStateGameObject::FollowPlayer(float dt) {
	followTimeout -= dt;
	travelDir = object->GetTransform().GetPosition() - this->GetTransform().GetPosition();
	travelDir.y = 0;
	if (GetPhysicsObject()->GetLinearVelocity().Length() < 30)
		GetPhysicsObject()->ApplyLinearImpulse(travelDir * 0.01);
}
