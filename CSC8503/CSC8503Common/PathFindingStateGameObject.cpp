#include "PathFindingStateGameObject.h"
#include <algorithm>
#include "../../Common/Maths.h"
using namespace NCL;
using namespace CSC8503;
PathFindingStateGameObject::PathFindingStateGameObject(GameObject* val, bool ignore) : EnemyStateGameObject(val) {
	currentState = state::FOLLOWPATH;
	ignoreCosts = ignore;
	mazeStart = { 220, 0, 440 };
	mazeEnd = { 220, 0, 220 };
	FindPath();
	followPathState = new State([&](float dt)->void {
		this->DisplayPath();
		this->FollowPath(dt);
	});
	stateMachine->AddState(followPathState);
	stateMachine->AddTransition(new StateTransition(followPlayerState, followPathState, [&]()->bool {
		return currentState == state::FOLLOWPATH;
	}));
	stateMachine->AddTransition(new StateTransition(followPathState, followPlayerState, [&]()->bool {
		return currentState == state::FOLLOWPLAYER;
	}));
	stateMachine->AddTransition(new StateTransition(idleState, followPathState, [&]()->bool {
		return currentState == state::FOLLOWPATH;
	}));
	stateMachine->AddTransition(new StateTransition(followPathState, idleState, [&]()->bool {
		return currentState == state::IDLE;
	}));
	name = "PathfindingAI";
}

void PathFindingStateGameObject::FindPath() {
	NavigationPath outPath;
	NavigationGrid grid("MazePath.txt");
	bool found = grid.FindPath(mazeStart, mazeEnd, outPath, ignoreCosts);
	Vector3 pos;
	while (outPath.PopWaypoint(pos)) {
		path.push_back(pos - (mazeStart + Vector3(0, 0, 20)));		// Offset the path position by world pos
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
		travelDir = path[0] - GetTransform().GetPosition();
		travelDir.y = 0;
		if (travelDir.Length() < 10.0f) {
			path.erase(path.begin());
			if (path.size() == 0)		// Path completed
				return;
		}
		if(GetPhysicsObject()->GetLinearVelocity().Length() < 30)
			GetPhysicsObject()->ApplyLinearImpulse((path[0] - GetTransform().GetPosition()) * 0.01);
	}
}
