#include "EnemyStateGameObject.h"
#include <algorithm>
#include "../../Common/Maths.h"
using namespace NCL;
using namespace CSC8503;
EnemyStateGameObject::EnemyStateGameObject(GameObject* o) {
	object = o;
	rayTime = 0.5f;
	finished = false;
	travelDir = object->GetTransform().GetPosition() - this->GetTransform().GetPosition();
	followPlayerState = new State([&](float dt)->void {
		this->FollowPlayer(dt);
	});
	stateMachine->AddState(followPlayerState);
	stateMachine->AddTransition(new StateTransition(idleState, followPlayerState, [&]()->bool {
		return currentState == state::FOLLOWPLAYER;
	}));
	stateMachine->AddTransition(new StateTransition(followPlayerState, idleState, [&]()->bool {
		return currentState == state::IDLE;
	}));
	name = "EnemyAi";
}

void EnemyStateGameObject::Update(float dt) {
	if (currentState != state::FOLLOWPLAYER)
		rayTime -= dt;
	if (currentState != state::IDLE) {
		Matrix3 rotMatrix;
		rotMatrix.SetColumn(0, -Vector3::Cross(Vector3(0, 1, 0), travelDir.Normalised()));
		rotMatrix.SetColumn(1, Vector3(0, 1, 0));
		rotMatrix.SetColumn(2, -travelDir.Normalised());
		GetTransform().SetOrientation(Matrix4(rotMatrix));
	}
	stateMachine->Update(dt);
}

void EnemyStateGameObject::FollowPlayer(float dt) {
	travelDir = object->GetTransform().GetPosition() - this->GetTransform().GetPosition();
	travelDir.y = 0;
	GetPhysicsObject()->AddForce({ std::clamp(travelDir.x, -5.0f, 5.0f),
		std::clamp(travelDir.y, -5.0f, 5.0f), std::clamp(travelDir.z, -5.0f, 5.0f) });
}
