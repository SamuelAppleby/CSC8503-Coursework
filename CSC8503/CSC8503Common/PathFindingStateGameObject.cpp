#include "PathFindingStateGameObject.h"
#include <algorithm>
#include "../../Common/Maths.h"
using namespace NCL;
using namespace CSC8503;
PathFindingStateGameObject::PathFindingStateGameObject(Vector3 pos, GameObject* val, NavigationGrid* map) {
	grid = map;
	mazeStart = { 220, 0, 440 };
	mazeEnd = { 220, 0, 220 };
	object = val;
	rayTime = 0.0f;
	seePlayer = false;
	finished = false;
	playerDirection = object->GetTransform().GetPosition() - this->GetTransform().GetPosition();
	State* stateA = new State([&](float dt)->void {
		this->DisplayPath();
		this->FollowPath(dt);
	});
	State* stateB = new State([&](float dt)->void {
		this->FollowPlayer(dt);
	});
	stateMachine->AddState(stateA);
	stateMachine->AddState(stateB);
	stateMachine->AddTransition(new StateTransition(stateA, stateB, [&]()->bool {
		return seePlayer;
	}));
	stateMachine->AddTransition(new StateTransition(stateB, stateA, [&]()->bool {
		return !seePlayer;
	}));
	name = "PathfindingAI";
	FindPath();
}

void PathFindingStateGameObject::Update(float dt) {
	if (seePlayer)
		pathdir = playerDirection;
	else if (path.size() > 0)
		pathdir = path[0] - GetTransform().GetPosition();
	pathdir.y = 0;
	Matrix3 rotMatrix;
	rotMatrix.SetColumn(0, -Vector3::Cross(Vector3(0, 1, 0), pathdir.Normalised()));
	rotMatrix.SetColumn(1, Vector3(0, 1, 0));
	rotMatrix.SetColumn(2, -pathdir.Normalised());
	GetTransform().SetOrientation(Matrix4(rotMatrix));
	rayTime += dt;
	playerDirection = object->GetTransform().GetPosition() - this->GetTransform().GetPosition();
	stateMachine->Update(dt);
}

void PathFindingStateGameObject::FindPath() {
	NavigationPath outPath;
	bool found = grid->FindPath(mazeStart, mazeEnd, outPath);
	Vector3 pos;
	while (outPath.PopWaypoint(pos)) {
		path.push_back(pos - Vector3(220, 0, 460));		// Offset the path position by world pos
	}
}

void PathFindingStateGameObject::DisplayPath() {
	for (int i = 1; i < path.size(); ++i) {
		Vector3 a = path[i - 1];
		Vector3 b = path[i];
		Debug::DrawLine(a + Vector3(0, 10, 0), b + Vector3(0, 10, 0), Vector4(0, 1, 0, 1));
	}
}

void PathFindingStateGameObject::FollowPath(float dt) {
	if (path.size() > 0) {
		if (pathdir.Length() < 10.0f) {
			path.erase(path.begin());
			if (path.size() == 0)		// Path completed
				return;
		}
		GetPhysicsObject()->ApplyLinearImpulse((path[0] - GetTransform().GetPosition()) * 0.01);
	}
}

void PathFindingStateGameObject::FollowPlayer(float dt) {
	GetPhysicsObject()->AddForce({ std::clamp(playerDirection.x, -5.0f, 5.0f),
		std::clamp(playerDirection.y, -5.0f, 5.0f), std::clamp(playerDirection.z, -5.0f, 5.0f) });
}
