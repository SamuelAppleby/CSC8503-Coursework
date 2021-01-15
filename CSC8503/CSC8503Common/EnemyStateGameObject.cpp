#include "EnemyStateGameObject.h"
#include <algorithm>
#include "../../Common/Maths.h"
using namespace NCL;
using namespace CSC8503;
EnemyStateGameObject::EnemyStateGameObject() {
	rayTime = 0.5f;
	finished = false;
	followTimeout = 0.0f;
	speed = 0.1f;
	followObjectState = new State([&](float dt)->void {
		this->FollowObject(dt);
	});
	stateMachine->AddState(followObjectState);
	stateMachine->AddTransition(new StateTransition(idleState, followObjectState, [&]()->bool {
		if (this->followTimeout > 0.0f) {
			currentState = state::FOLLOWOBJECT;
			return true;
		}
		return false;
	}));
	stateMachine->AddTransition(new StateTransition(followObjectState, idleState, [&]()->bool {
		if (this->followTimeout < 0.0f) {
			interestObjects.erase(currentObject);
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
	int i = 0;
	/* Sort our objects of interest and follow the closest */
	//std::cout << interestObjects.size() << std::endl;
	std::set<GameObject*>::iterator it = interestObjects.begin();
	std::cout << interestObjects.size() << std::endl;
	for (it = interestObjects.begin(); it != interestObjects.end(); ++it) {
		GameObject* o = *it; 
		std::cout << o->GetTimeInSet() << std::endl;
		if (o->GetTimeInSet() > 10.0f || !o->IsActive()) {
			interestObjects.erase(o);
			it = interestObjects.begin();
		}
		Vector3 dir = o->GetTransform().GetPosition() - this->GetTransform().GetPosition();
		dir.y = 0;
		if (!currentObject) {
			travelDir = dir;
			currentObject = o;
		}
		if (dir.Length() < travelDir.Length()) {
			travelDir = dir;
			if (o != currentObject) {
				currentObject = o;
			}
			currentObject->SetTimeInSet(0.0f);
		}
		else {
			if (currentObject) {
				travelDir = currentObject->GetTransform().GetPosition() - this->GetTransform().GetPosition();
				travelDir.y = 0;
			}
			if(o != currentObject) {
				o->IncreaseTimeInSet(dt);
			}
		}
	}
	rayTime -= dt;
	stateMachine->Update(dt);
}

void EnemyStateGameObject::FollowObject(float dt) {
	followTimeout -= dt;
	GetPhysicsObject()->ApplyLinearImpulse(Vector3(std::clamp(travelDir.x, -speed, speed),
		0, std::clamp(travelDir.z, -speed, speed)));
}

