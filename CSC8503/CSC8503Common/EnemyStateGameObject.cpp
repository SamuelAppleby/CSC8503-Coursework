#include "EnemyStateGameObject.h"
#include <algorithm>
#include "../../Common/Maths.h"
using namespace NCL;
using namespace CSC8503;
EnemyStateGameObject::EnemyStateGameObject() {
	invMass = 5;
	elasticity = 0.2;
	friction = 0.2;
	rayTime = 0.5f;
	finished = false;
	followTimeout = 0.0f;
	speed = 0.025f;
	displayPath = false;
	followObjectState = new State([&](float dt)->void {
		this->FollowObject(dt);
		if(displayPath)
			this->DisplayDirection();
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
			currentState = state::IDLE;
			return true;
		}
		return false;
	}));
	name = "EnemyAI";
}

void EnemyStateGameObject::Update(float dt) {
	/* Sort our objects of interest and follow the closest */
	for (auto itr = interestObjects.cbegin(); itr != interestObjects.cend();) {
		GameObject* o = *itr;
		Vector3 dir = o->GetTransform().GetPosition() - this->GetTransform().GetPosition();
		dir.y = 0;
		if (!currentObject)
			currentObject = o;
		else if (dir.Length() < travelDir.Length()) {
			currentObject = o;
			currentObject->SetTimeInSet(0.0f);
		}
		o->IncreaseTimeInSet(dt);
		/* Erase any objects that have existed too long, no longer exist, or are too far away */
		if (o->GetTimeInSet() > 10.0f || !o->IsActive() || dir.Length() > 80.0f) {
			itr = interestObjects.erase(itr);
			if (interestObjects.size() == 0) {
				followTimeout = 0.0f;
				break;
			}
		}
		else
			++itr;
	}
	rayTime -= dt;
	if (powerUpTimer > 0.0f) {
		speed = 0.05f;
		powerUpTimer -= dt;
	}
	else {
		GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));
		speed = 0.025f;
	}
	Vector3 up = Vector3::Cross(Vector3(0, 1, 0), travelDir.Normalised());
	/* std::clamp misbehaves here, so apologies for what is below */
	if (up.x >= 1.0f)
		up.x = 0.999f;
	if (up.x <= -1.0f)
		up.x = -0.999f;
	if (up.z >= 1.0f)
		up.z = 0.999f;
	if (up.z <= -1.0f)
		up.z = -0.999f;
	if (currentState != state::IDLE) {
		Matrix3 rotMatrix;
		rotMatrix.SetColumn(0, -up);
		rotMatrix.SetColumn(1, Vector3(0, 1, 0));
		rotMatrix.SetColumn(2, -travelDir.Normalised());
		GetTransform().SetOrientation(rotMatrix);
		stateMachine->Update(dt);
	}
}

void EnemyStateGameObject::FollowObject(float dt) {
	followTimeout -= dt;
	travelDir = currentObject->GetTransform().GetPosition() - this->GetTransform().GetPosition();
	travelDir.y = 0;
	GetPhysicsObject()->ApplyLinearImpulse(Vector3(std::clamp(travelDir.x, -speed, speed), 0, std::clamp(travelDir.z, -speed, speed)));
}

void EnemyStateGameObject::DisplayDirection() {
	Debug::DrawLine(GetTransform().GetPosition(), currentObject->GetTransform().GetPosition(), Debug::CYAN);
}

