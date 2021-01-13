#include "TutorialGame.h"
using namespace NCL;
using namespace CSC8503;

TutorialGame::TutorialGame() {
	world = new GameWorld();
	renderer = new GameTechRenderer(*world);
	physics = new PhysicsSystem(*world);
	physics->SetDampingFactor(0.3);
	forceMagnitude = 10.0f;
	useGravity = false;
	physics->UseGravity(useGravity);
	useBroadphase = true;
	physics->UseBroadphase(useBroadphase);
	inSelectionMode = false;
	reloadTime = 0.0f;
	timeOut = 0.0f;
	Debug::SetRenderer(renderer);
	player = nullptr;
	lockedOrientation = true;
	currentLevel = 0;
	grid = new NavigationGrid("MazePath.txt");
	currentlySelected = 1;
	numEnemies = 0;
	InitialiseAssets();
}

/*
Each of the little demo scenarios used in the game uses the same 2 meshes,
and the same texture and shader. There's no need to ever load in anything else
for this module, even in the coursework, but you can add it if you like!
*/
void TutorialGame::InitialiseAssets() {
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};

	loadFunc("cube.msh", &cubeMesh);
	loadFunc("sphere.msh", &sphereMesh);
	loadFunc("Male1.msh", &charMeshA);
	loadFunc("courier.msh", &charMeshB);
	loadFunc("security.msh", &enemyMesh);
	loadFunc("coin.msh", &bonusMesh);
	loadFunc("capsule.msh", &capsuleMesh);

	basicTex = (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	obstacleTex = (OGLTexture*)TextureLoader::LoadAPITexture("obstacle.png");
	floorTex = (OGLTexture*)TextureLoader::LoadAPITexture("platform.png");
	lavaTex = (OGLTexture*)TextureLoader::LoadAPITexture("lava.png");
	trampolineTex = (OGLTexture*)TextureLoader::LoadAPITexture("trampoline.png");
	iceTex = (OGLTexture*)TextureLoader::LoadAPITexture("ice.png");
	woodenTex = (OGLTexture*)TextureLoader::LoadAPITexture("wood.png");
	bonusTex = (OGLTexture*)TextureLoader::LoadAPITexture("bonus.png");
	playerTex = (OGLTexture*)TextureLoader::LoadAPITexture("player.png");
	enemyTex = (OGLTexture*)TextureLoader::LoadAPITexture("enemy.png");
	finishTex = (OGLTexture*)TextureLoader::LoadAPITexture("finish.png");
	menuTex = (OGLTexture*)TextureLoader::LoadAPITexture("menu.png");
	basicShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");

	InitCamera();
	InitWorld();
}

TutorialGame::~TutorialGame() {
	delete cubeMesh;
	delete sphereMesh;
	delete charMeshA;
	delete charMeshB;
	delete enemyMesh;
	delete bonusMesh;

	delete basicTex;
	delete obstacleTex;
	delete floorTex;
	delete lavaTex;
	delete trampolineTex;
	delete iceTex;
	delete woodenTex;
	delete bonusTex;
	delete playerTex;
	delete enemyTex;
	delete finishTex;
	delete menuTex;
	delete basicShader;

	delete physics;
	//delete renderer;
	delete world;
}

void TutorialGame::UpdateGame(float dt) {
	
}

void TutorialGame::UpdateMenu(float dt) {
	renderer->DrawString("Choose Level", Vector2(35, 5), Debug::WHITE, 30.0f);
	currentlySelected == 1 ? renderer->DrawString("Level 1: Single Player Obstacle Course", Vector2(0, 15), { 0,1,0,1 }) :
		renderer->DrawString("Level 1: Single Player Obstacle Course", Vector2(0, 15), Debug::WHITE);
	currentlySelected > 1 ? renderer->DrawString("Level 2: Multiplayer Maze", Vector2(0, 45), { 0,1,0,1 }) :
		renderer->DrawString("Level 2: Multiplayer Maze", Vector2(0, 45), Debug::WHITE);
	renderer->DrawString("Enemy Opponents: ", Vector2(0, 50), Debug::WHITE);
	currentlySelected == 2 ? renderer->DrawString("[0]", Vector2(30, 50), { 0,1,0,1 }) :
		renderer->DrawString("[0]", Vector2(30, 50), Debug::WHITE);
	currentlySelected == 3 ? renderer->DrawString("[1]", Vector2(36, 50), { 0,1,0,1 }) :
		renderer->DrawString("[1]", Vector2(36, 50), Debug::WHITE);
	currentlySelected == 4 ? renderer->DrawString("[2]", Vector2(42, 50), { 0,1,0,1 }) :
		renderer->DrawString("[2]", Vector2(42, 50), Debug::WHITE);
	currentlySelected == 5 ? renderer->DrawString("[3]", Vector2(48, 50), { 0,1,0,1 }) :
		renderer->DrawString("[3]", Vector2(48, 50), Debug::WHITE);
	renderer->DrawString("Exit(ESC)", Vector2(80, 5));
	renderer->DrawString("Choose[UP][DOWN]", Vector2(70, 90));
	renderer->DrawString("Select[SPACE]", Vector2(75, 95));
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::UP) || Window::GetKeyboard()->KeyPressed(KeyboardKeys::DOWN)) {
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::UP))
			currentlySelected = currentlySelected == 1 ? 5 : currentlySelected - 1;
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::DOWN))
			currentlySelected = currentlySelected == 5 ? 1 : currentlySelected + 1;
		if (currentlySelected < 3) {
			menuEnemies.at(0)->GetRenderObject()->SetColour({ 0.1,0.1,0.1,1 });
			menuEnemies.at(1)->GetRenderObject()->SetColour({ 0.1,0.1,0.1,1 });
			menuEnemies.at(2)->GetRenderObject()->SetColour({ 0.1,0.1,0.1,1 });
		}
		else {
			menuEnemies.at(0)->GetRenderObject()->SetColour({ 1,1,1,1 });
			if (currentlySelected == 3) {
				menuEnemies.at(1)->GetRenderObject()->SetColour({ 0.1,0.1,0.1,1 });
			}
			if (currentlySelected >= 4) {
				menuEnemies.at(1)->GetRenderObject()->SetColour({ 1,1,1,1 });
			}
			if (currentlySelected == 4) {
				menuEnemies.at(2)->GetRenderObject()->SetColour({ 0.1,0.1,0.1,1 });
			}
			if (currentlySelected == 5) {
				menuEnemies.at(2)->GetRenderObject()->SetColour({ 1,1,1,1 });
			}
		}
		numEnemies = currentlySelected - 2;
	}
	if(Window::GetKeyboard()->KeyPressed(KeyboardKeys::RETURN)) {
		currentLevel = currentlySelected >= 2 ? 2 : 1;
		InitWorld();		// Clears all objects and resets to new level
	}
}

void TutorialGame::UpdateLevel(float dt) {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::Q)) {
		inSelectionMode = !inSelectionMode;
		if (inSelectionMode) {
			Window::GetWindow()->ShowOSPointer(true);
			Window::GetWindow()->LockMouseToWindow(false);
		}
		else {
			Window::GetWindow()->ShowOSPointer(false);
			Window::GetWindow()->LockMouseToWindow(true);
		}
	}
	inSelectionMode ? renderer->DrawString("Change to play mode(Q)", Vector2(69, 10), Debug::WHITE, textSize) :
		renderer->DrawString("Change to debug mode(Q)", Vector2(68, 10), Debug::WHITE, textSize);
	renderer->DrawString("Exit to Menu (ESC)", Vector2(75, 5), Debug::WHITE, textSize);
	renderer->DrawString("Pause(P)", Vector2(88, 15), Debug::WHITE, textSize);
	if (inSelectionMode) {
		UpdateKeys();
		DrawDebugInfo();
		world->ShowFacing();
	}
	else {
		renderer->DrawString("Score:" + std::to_string(player->GetScore()), Vector2(0, 5), Debug::WHITE, textSize);
		world->GetMainCamera()->UpdateCamera(dt);
	}
	SelectObject();
	lockedObject ? LockedObjectMovement() : DebugObjectMovement();
	CheckFinished(dt);
	physics->Update(dt);
	currentLevel == 1 ? UpdateLevel1(dt) : UpdateLevel2(dt);
	if (lockedObject) {
		world->GetMainCamera()->UpdateCameraWithObject(dt, world->GetMainCamera(), lockedObject->GetTransform().GetPosition());
		if (lockedOrientation)
			lockedObject->GetTransform().SetOrientation(Matrix4::Rotation(world->GetMainCamera()->GetYaw(), { 0, 1, 0 }));
	}
}

void TutorialGame::DrawDebugInfo() {
	renderer->DrawString("Reset Camera(F1)", Vector2(0, 5), Debug::WHITE, textSize);
	world->GetShuffleObjects() ? renderer->DrawString("Shuffle Objects(F2):On", Vector2(0, 10), Debug::WHITE, textSize) :
		renderer->DrawString("Shuffle Objects(F2):Off", Vector2(0, 10), Debug::WHITE, textSize);
	world->GetShuffleConstraints() ? renderer->DrawString("Shuffle Constraints(F3):On", Vector2(0, 15), Debug::WHITE, textSize) :
		renderer->DrawString("Shuffle Constraints(F3):Off", Vector2(0, 15), Debug::WHITE, textSize);

	useGravity ? renderer->DrawString("Gravity(G):On", Vector2(0, 20), Debug::WHITE, textSize) : 
		renderer->DrawString("Gravity(G):Off", Vector2(0, 20), Debug::WHITE, textSize);
	renderer->DrawString("Click Force(Scroll Wheel):" + std::to_string((int)forceMagnitude), Vector2(0, 25), Debug::WHITE, textSize);
	renderer->DrawString("Constraint Iteration Count(I/O):" + std::to_string(physics->GetConstraintIterationCount()), Vector2(0, 30), Debug::WHITE, textSize);

	if (physics->GetBroadPhase()) {
		renderer->DrawString("QuadTree(B):On", Vector2(0, 35), Debug::WHITE, textSize);
		renderer->DrawString("Collisions Tested:" + std::to_string(physics->GetBroadPhaseCollisionsTested()), Vector2(68, 90), Debug::WHITE, textSize);
	}
	else {
		renderer->DrawString("QuadTree(B):Off", Vector2(0, 35), Debug::WHITE, textSize);
		renderer->DrawString("Collisions Tested:" + std::to_string(physics->GetBasicCollisionsTested()), Vector2(68, 90), Debug::WHITE, textSize);
	}
	
	renderer->DrawString("Current Collisions:" + std::to_string(physics->GetCollisionsSize()), Vector2(70, 95), Debug::WHITE, textSize);
	renderer->DrawString("Total Objects:" + std::to_string(world->GetTotalWorldObjects()), Vector2(75, 85), Debug::WHITE, textSize);
	if (selectionObject) {
		if(StateGameObject* g = dynamic_cast<StateGameObject*>(selectionObject))
			renderer->DrawString("State:" + g->StateToString(), Vector2(0, 55), Debug::WHITE, textSize);
		renderer->DrawString("Position:" + selectionObject->GetTransform().GetPosition().ToString(), Vector2(0, 60), Debug::WHITE, textSize);
		renderer->DrawString("Orientation:" + selectionObject->GetTransform().GetOrientation().ToEuler().ToString(), Vector2(0, 65), Debug::WHITE, textSize);
		renderer->DrawString("Selected Object:" + selectionObject->GetName(), Vector2(0, 70), Debug::WHITE, textSize);
		renderer->DrawString("Inverse Mass:" + std::to_string(selectionObject->GetPhysicsObject()->GetInverseMass()), Vector2(0, 75), Debug::WHITE, textSize);
		renderer->DrawString("Friction:" + std::to_string(selectionObject->GetPhysicsObject()->GetFriction()), Vector2(0, 80), Debug::WHITE, textSize);
		renderer->DrawString("Elasticity:" + std::to_string(selectionObject->GetPhysicsObject()->GetElasticity()), Vector2(0, 85), Debug::WHITE, textSize);
	}
	if (lockedObject) {
		renderer->DrawString("Unlock object(L)", Vector2(0, 90), Debug::WHITE, textSize);
		lockedOrientation ? renderer->DrawString("Lock object orientation(P): On", Vector2(0, 95), Debug::WHITE, textSize) :
			renderer->DrawString("Lock object orientation(P): Off", Vector2(0, 95), Debug::WHITE, textSize);
	}
	else
		renderer->DrawString("Lock selected object(L)", Vector2(0, 90), Debug::WHITE, textSize);
}

void TutorialGame::CheckFinished(float dt) {
	if (player && !inSelectionMode) {
		if (player->GetScore() <= 0 || player->GetFinished()) {
			timeOut += dt;
			if (player->GetScore() <= 0)
				renderer->DrawString("Time Up - Game Over", Vector2(30, 40), Vector4(1, 0, 0, 1), 25.0f);
			else if (player->GetFinished()) {
				renderer->DrawString("Level Finished", Vector2(34, 40), Vector4(0, 1, 0, 1), 25.0f);
				renderer->DrawString("Score: " + std::to_string(player->GetScore()), Vector2(42, 45), Vector4(1, 1, 0, 1));
			}
			renderer->DrawString("Exiting to Menu in:" + std::to_string((int)(5.0f - timeOut)) + "s", Vector2(30, 50));
		}
	}
	for (auto& e : enemies) {
		if (e->GetFinished()) {
			timeOut += dt;
			renderer->DrawString("AI Won - Game Over", Vector2(32, 40), Vector4(1, 0, 0, 1), 25.0f);
			renderer->DrawString("Score:" + std::to_string(player->GetScore()), Vector2(42, 45), Vector4(1, 1, 0, 1));
			renderer->DrawString("Exiting to Menu in:" + std::to_string((int)(5.0f - timeOut)) + "s", Vector2(30, 50));
		}
	}
	
	if (timeOut == 0.0f)
		player->DecreaseScore(dt);
}

void TutorialGame::UpdateLevel1(float dt) {
	for (auto& p : platforms) 
		p->Update(dt);
	reloadTime += dt;
	if (reloadTime > 2.0f)
		FireObjects();
	if (player) {
		if (player->GetTransform().GetPosition().z < -265) 
			player->SetSpawnPos({ 0, 10, -265 });
		if (player->GetTransform().GetPosition().z < -600) 
			player->SetSpawnPos({ 0, 10, -600 });
		if (player->GetTransform().GetPosition().z < -850) 
			player->SetSpawnPos({ 0, 10, -850 });
		if (player->GetTransform().GetPosition().z < -1060) {
			player->SetSpawnPos({ -30, 50, -1060 });
			player->GetTransform().SetOrientation(Matrix4::Rotation(-90, { 0, 1, 0 }));
		}
	}
}

void TutorialGame::FireObjects() {
	CubeObject* cube = new CubeObject;
	AddCubeToWorld(cube, Vector3(-40, 8, ((rand() % (100 - 180 + 1)) + 100) * -1), Vector3(2, 2, 2));
	cube->GetPhysicsObject()->SetLinearVelocity(Vector3(150, 0, 0));
	cube->GetPhysicsObject()->SetAngularVelocity(Vector3(10, 10, 10));

	SphereObject* sphere = new SphereObject;
	rand() % 2 ? AddSphereToWorld(sphere, Vector3(90, 5, ((rand() % (200 - 210 + 1)) + 200) * -1), 3) :
		AddSphereToWorld(sphere, Vector3(90, 5, ((rand() % (250 - 240 + 1)) + 240) * -1), 3);
	sphere->GetPhysicsObject()->SetLinearVelocity(Vector3(-150, 0, 0));
	reloadTime = 0.0f;
}

void TutorialGame::UpdateLevel2(float dt) {
	for (auto& e : enemies) {
		e->Update(dt);
		EnemyRaycast(e);
	}
}

/* Enemies fire 11 rays every 0.5s across a range to detect the player */
void TutorialGame::EnemyRaycast(EnemyStateGameObject* enemy) {
	float fov = -1, numRay = 11;
	if (enemy->GetRayTime() < 0.0f) {
		for (int i = 0; i < numRay; ++i) {
			Ray ray(enemy->GetTransform().GetPosition(), enemy->GetTransform().GetOrientation() * Vector3(fov, 0, -1));
			RayCollision closestCollision;
			if (world->Raycast(ray, closestCollision, enemy, true)) {
				Debug::DrawLine(ray.GetPosition(), closestCollision.collidedAt, Debug::MAGENTA);
				if (dynamic_cast<PlayerObject*>((GameObject*)closestCollision.node)) {
					enemy->SetFollowTimeOut(5.0f);		// If sees player reset their timer
					enemy->GetRenderObject()->SetColour({ 1,0,0,1 });
				}
			}
			else
				Debug::DrawLine(ray.GetPosition(), ray.GetPosition() + (ray.GetDirection() * 500), Debug::YELLOW);
			fov += 2 / numRay;
		}
		enemy->SetRayTime(0.5f);
	}
}

void TutorialGame::UpdateKeys() {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::G)) {
		useGravity = !useGravity; //Toggle gravity!
		physics->UseGravity(useGravity);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::B)) {
		useBroadphase = !useBroadphase;
		physics->UseBroadphase(useBroadphase);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::I)) 
		physics->IncreaseIterationCount();
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::O)) 
		physics->DecreaseIterationCount();
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F1)) 
		InitCamera(); //F2 will reset the camera to a specific default place

	/*Running certain physics updates in a consistent order might cause some
	bias in the calculations - the same objects might keep 'winning' the constraint
	allowing the other one to stretch too much etc. Shuffling the order so that it
	is random every frame can help reduce such bias. */
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F2))
		world->ShuffleObjects(!world->GetShuffleObjects());
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F3)) 
		world->ShuffleConstraints(!world->GetShuffleConstraints());
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F5)) 
		world->ShuffleConstraints(false);
	/* If an object has been clicked, it can be pushed with the right mouse button, by an amount
	determined by the scroll wheel. */
	forceMagnitude += Window::GetMouse()->GetWheelMovement() * 10.0f;
}

void TutorialGame::InitCamera() {
	world->GetMainCamera()->SetNearPlane(0.1f);
	world->GetMainCamera()->SetFarPlane(1200.0f);
	world->GetMainCamera()->SetPosition(Vector3(0, 50, 80));
	lockedObject = nullptr;
}

void TutorialGame::InitWorld() {
	world->ClearAndErase();
	physics->Clear();
	InitFloors(currentLevel);
	InitGameExamples(currentLevel);
	InitGameObstacles(currentLevel);
}

void TutorialGame::InitFloors(int level) {
	switch (level) {
	case 0:
		AddFloorToWorld(new FloorObject, Vector3(0, 50, 0), Vector3(80, 40, 1))->GetRenderObject()->SetDefaultTexture(menuTex);
		break;
	case 1:
		/* Lava */
		AddFloorToWorld(new LavaObject, Vector3(0, -60, -400), Vector3(500, 1, 1000));

		/* Projectile Walls */
		AddFloorToWorld(new FloorObject, Vector3(-50, 5, -140), Vector3(1, 5, 40));
		AddFloorToWorld(new FloorObject, Vector3(120, 5, -225), Vector3(1, 5, 30));

		/* Regular Platform */
		AddFloorToWorld(new FloorObject, Vector3(100, 0, 0), Vector3(40, 1, 20), Matrix4::Rotation(-30, { 1, 0, 0 }));
		AddFloorToWorld(new FloorObject, Vector3(0, 0, 0), Vector3(25, 1, 25));
		AddFloorToWorld(new FloorObject, Vector3(0, 0, -85), Vector3(25, 1, 10));
		AddFloorToWorld(new FloorObject, Vector3(-15, 0, -145), Vector3(10, 1, 50));
		AddFloorToWorld(new FloorObject, Vector3(15, 0, -145), Vector3(10, 1, 50));
		AddFloorToWorld(new FloorObject, Vector3(0, 0, -205), Vector3(100, 1, 10));
		AddFloorToWorld(new FloorObject, Vector3(-90, 0, -225), Vector3(10, 1, 10));
		AddFloorToWorld(new FloorObject, Vector3(90, 0, -225), Vector3(10, 1, 10));

		/* Ice Shortcut */
		AddFloorToWorld(new IceObject, Vector3(0, 0, -225), Vector3(5, 1, 5));

		AddFloorToWorld(new FloorObject, Vector3(0, 0, -245), Vector3(100, 1, 10));
		AddFloorToWorld(new FloorObject, Vector3(0, 0, -265), Vector3(10, 1, 10));

		/* High elasticity trampoline */
		AddFloorToWorld(new TrampolineObject, Vector3(0, -30, -315), Vector3(10, 1, 10));
		AddFloorToWorld(new TrampolineObject, Vector3(0, -30, -395), Vector3(10, 1, 10));

		AddFloorToWorld(new FloorObject, Vector3(0, 0, -445), Vector3(10, 1, 10));

		/* Low Friction Ice */
		AddFloorToWorld(new IceObject, Vector3(30, 0, -465), Vector3(40, 1, 10));
		AddFloorToWorld(new IceObject, Vector3(60, 0, -515), Vector3(10, 1, 40));
		AddFloorToWorld(new IceObject, Vector3(30, 0, -565), Vector3(40, 1, 10));

		/* Regular Platform */
		AddFloorToWorld(new FloorObject, Vector3(0, 0, -605), Vector3(10, 1, 30));
		AddFloorToWorld(new FloorObject, Vector3(0, 0, -645), Vector3(80, 1, 10));
		AddFloorToWorld(new FloorObject, Vector3(-90, 0, -735), Vector3(10, 1, 100));
		AddFloorToWorld(new FloorObject, Vector3(90, 0, -735), Vector3(10, 1, 100));
		AddFloorToWorld(new FloorObject, Vector3(0, 0, -825), Vector3(80, 1, 10));
		AddFloorToWorld(new FloorObject, Vector3(0, 0, -865), Vector3(5, 1, 30));
		AddFloorToWorld(new FloorObject, Vector3(-25, 0, -900), Vector3(30, 1, 5));
		AddFloorToWorld(new FloorObject, Vector3(-60, 0, -915), Vector3(5, 1, 20));
		AddFloorToWorld(new FloorObject, Vector3(-60, -20, -1050), Vector3(5, 1, 20));

		/* Regular Platform */
		AddFloorToWorld(new FloorObject, Vector3(165, 20, -1060), Vector3(80, 1, 10));

		/* Rotated Platform */
		AddFloorToWorld(new FloorObject, Vector3(205, 10, -1035), Vector3(40, 1, 20), Matrix4::Rotation(30, { 1, 0, 0 }));
		AddFloorToWorld(new FloorObject, Vector3(205, -10, -995), Vector3(40, 1, 20), Matrix4::Rotation(-30, { 1, 0, 0 }));

		/* Finish Platform */
		AddFloorToWorld(new FinishObject, Vector3(204, -50, -1065), Vector3(39, 1, 38));

		AddFloorToWorld(new FloorObject, Vector3(204, -52, -1065), Vector3(41, 1, 40));

		/* Walls */
		AddFloorToWorld(new FloorObject, Vector3(-5, 3, -865.5), Vector3(1, 2, 30.5));
		AddFloorToWorld(new FloorObject, Vector3(5, 3, -869.5), Vector3(1, 2, 34.5));
		AddFloorToWorld(new FloorObject, Vector3(-36.25, 3, -895), Vector3(30.25, 2, 1));
		AddFloorToWorld(new FloorObject, Vector3(-55.5, 3, -920.5), Vector3(1, 2, 14.5));
		AddFloorToWorld(new FloorObject, Vector3(205, -45, -1026), Vector3(40, 6, 1));
		AddFloorToWorld(new FloorObject, Vector3(205, -45, -1104), Vector3(40, 6, 1));
		AddFloorToWorld(new FloorObject, Vector3(164, -45, -1065), Vector3(1, 6, 40));
		AddFloorToWorld(new FloorObject, Vector3(244, -45, -1065), Vector3(1, 6, 40));
		break;
	case 2:
		/* Regular Platform */
		AddFloorToWorld(new FloorObject, Vector3(0, 0, 0), Vector3(30, 1, 30));

		/* Maze Floor */
		AddFloorToWorld(new FloorObject, Vector3(0, 0, -240), Vector3(210, 1, 210));
		CreateMaze();
		/* Finish Platform */
		AddFloorToWorld(new FinishObject, Vector3(0, 1, -240), Vector3(10, 1, 10));
		break;
	}
}

/* Places all walls and obstacles that are found in the grid file */
void TutorialGame::CreateMaze() {
	Vector3 offset(220, -20, 460), nodePos, icePos;
	float xSize, zSize, iceXSize, iceZsize;
	/* Place all horizontally efficient walls and obstacles */
	zSize = 10;
	iceZsize = 10;
	for (int y = 0; y < 23; ++y) {
		nodePos = grid->GetNodes()[23 * y].position;
		xSize = 10;
		iceXSize = 10;
		for (int x = 0; x < 23; ++x) {
			if (grid->GetNodes()[(23 * y) + x].type == SPRING_NODE)
				AddCubeToWorld(new SpringObject(nodePos - offset - Vector3(0, 10, 0)), nodePos - offset - Vector3(0, 10, 0), Vector3(8, 8, 1));
			if (grid->GetNodes()[(23 * y) + x].type == WALL_NODE) {
				if ((grid->GetNodes()[(23 * y) + x + 1].type != WALL_NODE || x == 22) && xSize > zSize)
					AddFloorToWorld(new FloorObject, nodePos - offset, { xSize, 20, zSize });
				else {
					nodePos.x += 10;
					xSize += 10;
				}
			}
			else {
				xSize = 10;
				nodePos = grid->GetNodes()[(23 * y) + x + 1].position;
			}
			if (grid->GetNodes()[(23 * y) + x].type == ICE_NODE) {
				if ((grid->GetNodes()[(23 * y) + x + 1].type != ICE_NODE || x == 22) && iceXSize > iceZsize)
					AddFloorToWorld(new IceObject, icePos - offset - Vector3(0, 20, 0), Vector3(iceXSize, 1.5, iceZsize));
				else {
					icePos.x += 10;
					iceXSize += 10;
				}
			}
			else {
				iceXSize = 10;
				icePos = grid->GetNodes()[(23 * y) + x + 1].position;
			}	
		}
	}
	/* Place all vertically efficient walls and obstacles */
	xSize = 10;
	iceXSize = 10;
	for (int x = 0; x < 23; ++x) {
		nodePos = grid->GetNodes()[x].position;
		icePos = grid->GetNodes()[x].position;
		zSize = 10;
		for (int y = 0; y < 23; ++y) {
			if (grid->GetNodes()[(23 * y) + x].type == WALL_NODE) {
				if ((grid->GetNodes()[(23 * y) + x + 23].type != WALL_NODE || y == 22) && zSize > xSize)
					AddFloorToWorld(new FloorObject, nodePos - offset, { xSize, 20, zSize });
				else {
					nodePos.z += 10;
					zSize += 10;
				}
			}
			else {
				zSize = 10;
				nodePos = grid->GetNodes()[(23 * y) + x + 23].position;
			}
			if (grid->GetNodes()[(23 * y) + x].type == ICE_NODE) {
				if ((grid->GetNodes()[(23 * y) + x + 23].type != ICE_NODE || y == 22) && iceZsize > iceXSize)
					AddFloorToWorld(new IceObject, icePos - offset - Vector3(0, 20, 0), Vector3(iceXSize, 1.5, iceZsize));
				else {
					icePos.z += 10;
					iceZsize += 10;
				}
			}
			else {
				iceZsize = 10;
				icePos = grid->GetNodes()[(23 * y) + x + 23].position;
			}
		}
	}
}

void TutorialGame::InitGameExamples(int level) {
	switch (level) {
	case 0:
		/* Menu Objects */
		AddPlayerToWorld(new PlayerObject, Vector3(-20, 55, 50))->GetTransform().SetOrientation(Matrix4::Rotation(180, { 0, 1, 0 }));
		AddPlayerToWorld(new PlayerObject, Vector3(-20, 46, 50))->GetTransform().SetOrientation(Matrix4::Rotation(180, { 0, 1, 0 }));
		menuEnemies.push_back(AddEnemyToWorld(new EnemyObject, { -15, 46, 50 }));
		menuEnemies.push_back(AddEnemyToWorld(new EnemyObject, { -10, 46, 50 }));
		menuEnemies.push_back(AddEnemyToWorld(new EnemyObject, { -5, 46, 50 }));
		for (auto& e : menuEnemies) {
			e->GetRenderObject()->SetColour({ 0.1,0.1,0.1,1 });
			e->GetTransform().SetOrientation(Matrix4::Rotation(180, Vector3(0, 1, 0)));
		}
		break;
	case 1:
		AddCapsuleToWorld(new CapsuleObject, Vector3(-30, 0, 0), 5.0f, 3);
		player = (PlayerObject*)AddPlayerToWorld(new PlayerObject, Vector3(0, 10, 0));
		platforms.push_back((PlatformStateGameObject*)AddFloorToWorld(
			new PlatformStateGameObject(Vector3(-12.5, 0, -50), Vector3(12.5, 0, -50)), Vector3(-12.5, 0, -50), { 12.5,1,25 }));
		platforms.push_back((PlatformStateGameObject*)AddFloorToWorld(
			new PlatformStateGameObject(Vector3(-45, -20, -1060), Vector3(-45, 20, -1060)), Vector3(-45, -20, -1060), {10,1,10}));
		AddBonusToWorld(Vector3(-90, 5, -225));
		AddBonusToWorld(Vector3(90, 5, -225));
		AddBonusToWorld(Vector3(0, 5, -225));
		AddBonusToWorld(Vector3(0, 10, -355));
		AddBonusToWorld(Vector3(60, 5, -465));
		AddBonusToWorld(Vector3(60, 5, -565));
		AddBonusToWorld(Vector3(-90, 5, -645));
		AddBonusToWorld(Vector3(90, 5, -645));
		AddBonusToWorld(Vector3(-90, 5, -825));
		AddBonusToWorld(Vector3(90, 5, -825));
		AddBonusToWorld(Vector3(-30, 5, -900))->GetTransform().SetOrientation(Matrix4::Rotation(90, { 0, 1, 0 }));
		AddBonusToWorld(Vector3(-60, 5, -915));
		AddBonusToWorld(Vector3(-60, -15, -1060));
		AddBonusToWorld(Vector3(20, 20, -1060))->GetTransform().SetOrientation(Matrix4::Rotation(270, { 0, 1, 0 }));
		break;
	case 2:
		player = (PlayerObject*)AddPlayerToWorld(new PlayerObject, Vector3(0, 10, -10));
		/* One enemy will not take costs into account, the other will, both using A* */
		if(numEnemies >= 1)
			enemies.push_back((PathFindingStateGameObject*)AddEnemyToWorld(new PathFindingStateGameObject(player, false), Vector3(5, 10, 20)));
		if (numEnemies >= 2)
			enemies.push_back((PathFindingStateGameObject*)AddEnemyToWorld(new PathFindingStateGameObject(player, true), Vector3(0, 10, 20)));
		if (numEnemies == 3)
			enemies.push_back((PathFindingStateGameObject*)AddEnemyToWorld(new PathFindingStateGameObject(player, false), Vector3(5, 10, 20)));
		break;
	}
}

void TutorialGame::InitGameObstacles(int level) {
	switch (level) {
	case 1:
		/* Spinning Bar */
		AddCubeToWorld(new RotatingCubeObject, Vector3(0, 3, -735), Vector3(1, 1, 100));

		/* Spring Platforms*/
		AddCubeToWorld(new SpringObject(Vector3(0, 5, -890)), Vector3(0, 5, -890), Vector3(4, 4, 1));
		AddCubeToWorld(new SpringObject(Vector3(-50, 5, -900)), Vector3(-50, 5, -900), Vector3(1, 4, 4));
		AddCubeToWorld(new SpringObject(Vector3(-60, -15, -985)), Vector3(-60, -15, -985), Vector3(10, 1, 45));

		/* Bridge */
		AddBridgeToWorld(Vector3(-30, 20, -1060));
		break;
	case 2:
		break;
	}
}

void TutorialGame::InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius) {
	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddSphereToWorld(new SphereObject, position, radius);
		}
	}
}

void TutorialGame::InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims) {
	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddCubeToWorld(new CubeObject, position, cubeDims);
		}
	}
}

void TutorialGame::InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing) {
	float sphereRadius = 1.0f;
	Vector3 cubeDims = Vector3(1, 1, 1);
	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			int type = rand() % 3 + 1;
			if (type == 1)
				(rand() % 2) ? AddCubeToWorld(new CubeObject, position, cubeDims * Vector3(1, 4, 1)) : AddCubeToWorld(new CubeObject, position, cubeDims);
			else if (type == 2)
				(rand() % 2) ? AddSphereToWorld(new SphereObject, position, sphereRadius / 2) : AddSphereToWorld(new SphereObject, position, sphereRadius);
			else
				(rand() % 2) ? AddCapsuleToWorld(new CapsuleObject, position, 3.0f, sphereRadius * 1.5) : AddCapsuleToWorld(new CapsuleObject, position, 2.0f, sphereRadius);
		}
	}
}

/*
A single function to add a large immoveable cube to the bottom of our world
*/
GameObject* TutorialGame::AddFloorToWorld(GameObject* floor, const Vector3& position, const Vector3& size, const Matrix4& orientation) {
	/* Checking if oriented (need OBB) */
	if (Vector3(1, 1, 1) != orientation * Vector3(1, 1, 1)) {
		OBBVolume* volume = new OBBVolume(size);
		floor->SetBoundingVolume((CollisionVolume*)volume);
	}
	else {
		AABBVolume* volume = new AABBVolume(size);
		floor->SetBoundingVolume((CollisionVolume*)volume);
	}
	floor->GetTransform().SetScale(size * 2).SetPosition(position).SetOrientation(orientation);
	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, floorTex, basicShader));
	if (dynamic_cast<LavaObject*>(floor))
		floor->GetRenderObject()->SetDefaultTexture(lavaTex);
	else if (dynamic_cast<IceObject*>(floor))
		floor->GetRenderObject()->SetDefaultTexture(iceTex);
	else if (dynamic_cast<TrampolineObject*>(floor))
		floor->GetRenderObject()->SetDefaultTexture(trampolineTex);
	else if (dynamic_cast<FinishObject*>(floor))
		floor->GetRenderObject()->SetDefaultTexture(finishTex);
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));
	floor->GetPhysicsObject()->InitCubeInertia();
	world->AddGameObject(floor);
	return floor;
}

/*
Builds a game object that uses a sphere mesh for its graphics, and a bounding sphere for its
rigid body representation. This and the cube function will let you build a lot of 'simple'
physics worlds. You'll probably need another function for the creation of OBB cubes too.
*/
GameObject* TutorialGame::AddSphereToWorld(GameObject* sphere, const Vector3& position, float radius) {
	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius);
	sphere->SetBoundingVolume((CollisionVolume*)volume);
	sphere->GetTransform().SetScale(sphereSize).SetPosition(position);
	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, obstacleTex, basicShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));
	sphere->GetPhysicsObject()->InitSphereInertia();
	world->AddGameObject(sphere);
	return sphere;
}

GameObject* TutorialGame::AddCubeToWorld(GameObject* cube, const Vector3& position, Vector3 dimensions) {
	if (dynamic_cast<RotatingCubeObject*>(cube)) {
		OBBVolume* volume = new OBBVolume(dimensions);
		cube->SetBoundingVolume((CollisionVolume*)volume);
	}
	else {
		AABBVolume* volume = new AABBVolume(dimensions);
		cube->SetBoundingVolume((CollisionVolume*)volume);
	}
	cube->GetTransform().SetPosition(position).SetScale(dimensions * 2);
	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, obstacleTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));
	cube->GetPhysicsObject()->InitCubeInertia();
	world->AddGameObject(cube);
	return cube;
}

GameObject* TutorialGame::AddCapsuleToWorld(GameObject* capsule, const Vector3& position, float halfHeight, float radius) {
	CapsuleVolume* volume = new CapsuleVolume(halfHeight, radius);
	capsule->SetBoundingVolume((CollisionVolume*)volume);
	capsule->GetTransform().SetScale(Vector3(radius * 2, halfHeight, radius * 2)).SetPosition(position);
	capsule->SetRenderObject(new RenderObject(&capsule->GetTransform(), capsuleMesh, obstacleTex, basicShader));
	capsule->SetPhysicsObject(new PhysicsObject(&capsule->GetTransform(), capsule->GetBoundingVolume()));
	capsule->GetPhysicsObject()->InitCubeInertia();
	world->AddGameObject(capsule);
	return capsule;
}

void TutorialGame::AddBridgeToWorld(Vector3 startPos) {
	Vector3 cubeSize(1, 0.5, 5);
	Vector3 baseSize(5, 1, 5);
	float bridgeWidth = 110;
	float invCubeMass = 5; // how heavy the middle pieces are
	int numLinks = 22;
	float maxDistance = (bridgeWidth / (numLinks + 2)); // constraint distance
	float cubeDistance = (bridgeWidth / (numLinks + 2)); // distance between links
	GameObject* start = AddFloorToWorld(new FloorObject, startPos + Vector3(0, 0, 0), baseSize);
	GameObject* end = AddFloorToWorld(new FloorObject, startPos + Vector3((numLinks + 2) * cubeDistance, 0, 0), baseSize);
	GameObject* previous = start;
	for (int i = 0; i < numLinks; ++i) {
		GameObject* block = AddCubeToWorld(new CubeObject, startPos + Vector3((i + 1) * cubeDistance, 0, 0), cubeSize);
		block->GetRenderObject()->SetDefaultTexture(woodenTex);
		PositionConstraint* constraint = new PositionConstraint(previous, block, maxDistance);
		world->AddConstraint(constraint);
		previous = block;
	}
	PositionConstraint* constraint = new PositionConstraint(previous, end, maxDistance);
	world->AddConstraint(constraint);
}

GameObject* TutorialGame::AddPlayerToWorld(GameObject* p, const Vector3& position) {
	float meshSize = 3.0f;
	SphereVolume* volume = new SphereVolume(meshSize * 0.85);
	p->SetBoundingVolume((CollisionVolume*)volume);
	p->GetTransform().SetScale(Vector3(meshSize, meshSize, meshSize)).SetPosition(position);
	(rand() % 2) ? p->SetRenderObject(new RenderObject(&p->GetTransform(), charMeshA, playerTex, basicShader)) :
		p->SetRenderObject(new RenderObject(&p->GetTransform(), charMeshB, playerTex, basicShader));
	p->SetPhysicsObject(new PhysicsObject(&p->GetTransform(), p->GetBoundingVolume()));
	p->GetPhysicsObject()->InitSphereInertia();
	world->AddGameObject(p);
	//lockedObject = character;
	return p;
}

GameObject* TutorialGame::AddEnemyToWorld(GameObject* e, const Vector3& position) {
	float meshSize = 3.0f;
	SphereVolume* volume = new SphereVolume(meshSize * 0.85);
	e->SetBoundingVolume((CollisionVolume*)volume);
	e->GetTransform().SetScale(Vector3(meshSize, meshSize, meshSize)).SetPosition(position);
	e->SetRenderObject(new RenderObject(&e->GetTransform(), enemyMesh, enemyTex, basicShader));
	e->SetPhysicsObject(new PhysicsObject(&e->GetTransform(), e->GetBoundingVolume()));
	e->GetPhysicsObject()->InitSphereInertia();
	world->AddGameObject(e);
	return e;
}

GameObject* TutorialGame::AddBonusToWorld(const Vector3& position) {
	BonusObject* bonus = new BonusObject;
	SphereVolume* volume = new SphereVolume(1.25f);
	bonus->SetBoundingVolume((CollisionVolume*)volume);
	bonus->GetTransform().SetScale(Vector3(0.25, 0.25, 0.25)).SetPosition(position);
	bonus->SetRenderObject(new RenderObject(&bonus->GetTransform(), bonusMesh, bonusTex, basicShader));
	bonus->SetPhysicsObject(new PhysicsObject(&bonus->GetTransform(), bonus->GetBoundingVolume()));
	bonus->GetPhysicsObject()->InitSphereInertia();
	world->AddGameObject(bonus);
	return bonus;
}

/*
Every frame, this code will let you perform a raycast, to see if there's an object
underneath the cursor, and if so 'select it' into a pointer, so that it can be
manipulated later. Pressing Q will let you toggle between this behaviour and instead
letting you move the camera around.
*/
bool TutorialGame::SelectObject() {
	if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT)) {
		Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());
		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, nullptr, true)) {
			if (selectionObject) {
				selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
				selectionObject->SetSelected(false);
			}
			selectionObject = (GameObject*)closestCollision.node;
			selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
			selectionObject->SetSelected(true);
			return true;
		}
		else {
			return false;
		}
	}
	if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::L)) {
		if (selectionObject) {
			if (lockedObject == selectionObject) {
				lockedObject = nullptr;
			}
			else {
				lockedObject = selectionObject;
			}
		}
	}
	return false;
}

void TutorialGame::DebugObjectMovement() {
	//If we've selected an object, we can manipulate it with some key presses
	if (inSelectionMode && selectionObject) {
		//Twist the selected object!
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(-10, 0, 0));
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, 0, -10));
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, 0, 10));
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM2)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, -10, 0));
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM8)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, 10, 0));
		}
		if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::RIGHT)) {
			Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());
			RayCollision closestCollision;
			if (world->Raycast(ray, closestCollision, nullptr, true)) {
				if (closestCollision.node == selectionObject) 
					selectionObject->GetPhysicsObject()->AddForceAtPosition(ray.GetDirection() * forceMagnitude, closestCollision.collidedAt);
			}
		}
	}
}

void TutorialGame::LockedObjectMovement() {
	Matrix4 view = world->GetMainCamera()->BuildViewMatrix();
	Matrix4 camWorld = view.Inverse();
	Vector3 rightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!
	//forward is more tricky -  camera forward is 'into' the screen...
	//so we can take a guess, and use the cross of straight up, and
	//the right axis, to hopefully get a vector that's good enough!
	Vector3 fwdAxis = Vector3::Cross(Vector3(0, 1, 0), rightAxis);
	fwdAxis.y = 0.0f;
	fwdAxis.Normalise();
	Vector3 charForward = lockedObject->GetTransform().GetOrientation() * Vector3(0, 0, 1);
	float force = 0.02f;
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::W))
		lockedObject->GetPhysicsObject()->ApplyLinearImpulse(fwdAxis * force);
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::A))
		lockedObject->GetPhysicsObject()->ApplyLinearImpulse(-rightAxis * force);
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::S))
		lockedObject->GetPhysicsObject()->ApplyLinearImpulse(-fwdAxis * force);
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::D))
		lockedObject->GetPhysicsObject()->ApplyLinearImpulse(rightAxis * force);
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE)) {
		if (dynamic_cast<PlayerObject*>(lockedObject))
			((PlayerObject*)lockedObject)->Jump();
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::P))
		lockedOrientation = !lockedOrientation;
}
