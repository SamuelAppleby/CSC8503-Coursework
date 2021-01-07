#include "EnemyStateGameObject.h"
#include <algorithm>
#include "../../Common/Maths.h"
using namespace NCL;
using namespace CSC8503;
EnemyStateGameObject::EnemyStateGameObject(GameObject* val) {
	object = val;
	rayTime = 0.0f;
	seePlayer = false;
	finished = false;
	travelDir = object->GetTransform().GetPosition() - this->GetTransform().GetPosition();
	followPlayerState = new State([&](float dt)->void {
		this->FollowPlayer(dt);
	});
	stateMachine->AddState(followPlayerState);
	name = "EnemyAi";
}

void EnemyStateGameObject::Update(float dt) {
	if(seePlayer)
		travelDir = object->GetTransform().GetPosition() - this->GetTransform().GetPosition();
	else
		rayTime += dt;
	travelDir.y = 0;
	Matrix3 rotMatrix;
	rotMatrix.SetColumn(0, -Vector3::Cross(Vector3(0, 1, 0), travelDir.Normalised()));
	rotMatrix.SetColumn(1, Vector3(0, 1, 0));
	rotMatrix.SetColumn(2, -travelDir.Normalised());
	GetTransform().SetOrientation(Matrix4(rotMatrix));
	stateMachine->Update(dt);
}

void EnemyStateGameObject::Idle() {
	std::cout << "idle";
}

void EnemyStateGameObject::FollowPlayer(float dt) {
	GetPhysicsObject()->AddForce({ std::clamp(travelDir.x, -5.0f, 5.0f),
		std::clamp(travelDir.y, -5.0f, 5.0f), std::clamp(travelDir.z, -5.0f, 5.0f) });
}
