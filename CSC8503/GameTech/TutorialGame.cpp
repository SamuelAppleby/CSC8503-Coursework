#include "TutorialGame.h"
#include "../CSC8503Common/GameWorld.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Common/TextureLoader.h"
#include "../CSC8503Common/PositionConstraint.h"

using namespace NCL;
using namespace CSC8503;

TutorialGame::TutorialGame()	{
	world		= new GameWorld();
	renderer	= new GameTechRenderer(*world);
	physics		= new PhysicsSystem(*world);
	physics->SetDampingFactor(0.3);
	forceMagnitude	= 10.0f;
	useGravity		= true;
	physics->UseGravity(useGravity);
	inSelectionMode = false;
	sceneTime = 0.0f;
	Debug::SetRenderer(renderer);
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

	loadFunc("cube.msh"		 , &cubeMesh);
	loadFunc("sphere.msh"	 , &sphereMesh);
	loadFunc("Male1.msh"	 , &charMeshA);
	loadFunc("courier.msh"	 , &charMeshB);
	loadFunc("security.msh"	 , &enemyMesh);
	loadFunc("coin.msh"		 , &bonusMesh);
	loadFunc("capsule.msh"	 , &capsuleMesh);

	basicTex	= (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	floorTex = (OGLTexture*)TextureLoader::LoadAPITexture("platform.png");
	lavaTex = (OGLTexture*)TextureLoader::LoadAPITexture("lava.png");
	trampolineTex = (OGLTexture*)TextureLoader::LoadAPITexture("trampoline.png");
	iceTex = (OGLTexture*)TextureLoader::LoadAPITexture("ice.png");
	obstacleTex = (OGLTexture*)TextureLoader::LoadAPITexture("obstacle.png");
	basicShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");

	InitCamera();
	InitWorld();
}

TutorialGame::~TutorialGame()	{
	delete cubeMesh;
	delete sphereMesh;
	delete charMeshA;
	delete charMeshB;
	delete enemyMesh;
	delete bonusMesh;

	delete basicTex;
	delete basicShader;

	delete physics;
	delete renderer;
	delete world;
}

void TutorialGame::UpdateGame(float dt) {
	if (!inSelectionMode && !lockedObject) {
		world->GetMainCamera()->UpdateCamera(dt);
	}
	else {
		world->ShowFacing();
	}
	UpdateKeys();

	if (useGravity) {
		Debug::Print("(G)ravity on", Vector2(0, 95));
	}
	else {
		Debug::Print("(G)ravity off", Vector2(0, 95));
	}
	if (physics->GetBroadPhase()) {
		Debug::Print("QuadTree on (B)", Vector2(0, 10));
	}
	else {
		Debug::Print("QuadTree off (B)", Vector2(0, 10));
	}
	sceneTime += dt;
	if (sceneTime > 2.0f)
		FireObjects();
	SelectObject();
	MoveSelectedObject();
	physics->Update(dt);

	if (lockedObject != nullptr) {
		Vector3 objPos = lockedObject->GetTransform().GetPosition();
		Vector3 camPos = objPos + lockedOffset;

		Matrix4 temp = Matrix4::BuildViewMatrix(camPos, objPos, Vector3(0,1,0));

		Matrix4 modelMat = temp.Inverse();

		Quaternion q(modelMat);
		Vector3 angles = q.ToEuler(); //nearly there now!
		if (!camSet) {
			stationaryCameraPos = camPos;
			world->GetMainCamera()->SetPosition(camPos);
			world->GetMainCamera()->SetPitch(-20);
			world->GetMainCamera()->SetYaw(angles.y);
			//camSet = true;
		}
		else {
			world->GetMainCamera()->SetPitch(angles.x);
			world->GetMainCamera()->SetYaw(angles.y);
			float k = 1;
			float extension = (camPos - stationaryCameraPos).Length();
			float force = -k * extension;
			std::cout << camPos << std::endl;
			/*physA->AddForceAtLocalPosition(p.normal * -force, p.localA);
			physB->AddForceAtLocalPosition(p.normal * force, p.localB);*/
		}
		//Debug::DrawAxisLines(lockedObject->GetTransform().GetMatrix(), 2.0f);
	}
	world->UpdateWorld(dt);
	renderer->Update(dt);

	Debug::FlushRenderables(dt);
	renderer->Render();
}

void TutorialGame::UpdateKeys() {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F1)) {
		InitWorld(); //We can reset the simulation at any time with F1
		selectionObject = nullptr;
		lockedObject	= nullptr;
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F2)) {
		InitCamera(); //F2 will reset the camera to a specific default place
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::G)) {
		useGravity = !useGravity; //Toggle gravity!
		physics->UseGravity(useGravity);
	}
	//Running certain physics updates in a consistent order might cause some
	//bias in the calculations - the same objects might keep 'winning' the constraint
	//allowing the other one to stretch too much etc. Shuffling the order so that it
	//is random every frame can help reduce such bias.
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F9)) {
		world->ShuffleConstraints(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F10)) {
		world->ShuffleConstraints(false);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F7)) {
		world->ShuffleObjects(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F8)) {
		world->ShuffleObjects(false);
	}

	if (lockedObject) {
		LockedObjectMovement();
	}
	else {
		DebugObjectMovement();
	}
}

void TutorialGame::LockedObjectMovement() {
	Matrix4 view		= world->GetMainCamera()->BuildViewMatrix();
	Matrix4 camWorld	= view.Inverse();

	Vector3 rightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!

	//forward is more tricky -  camera forward is 'into' the screen...
	//so we can take a guess, and use the cross of straight up, and
	//the right axis, to hopefully get a vector that's good enough!

	Vector3 fwdAxis = Vector3::Cross(Vector3(0, 1, 0), rightAxis);
	fwdAxis.y = 0.0f;
	fwdAxis.Normalise();

	Vector3 charForward  = lockedObject->GetTransform().GetOrientation() * Vector3(0, 0, 1);
	Vector3 charForward2 = lockedObject->GetTransform().GetOrientation() * Vector3(0, 0, 1);

	float force = 20.0f;

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::W)) {
		lockedObject->GetPhysicsObject()->AddForce(fwdAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::A)) {
		lockedObject->GetPhysicsObject()->AddForce(-rightAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::S)) {
		lockedObject->GetPhysicsObject()->AddForce(-fwdAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::D)) {
		Vector3 worldPos = selectionObject->GetTransform().GetPosition();
		lockedObject->GetPhysicsObject()->AddForce(rightAxis * force);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE)) {
		lockedObject->Jump();
	}
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

	}
}

void TutorialGame::InitCamera() {
	world->GetMainCamera()->SetNearPlane(0.1f);
	world->GetMainCamera()->SetFarPlane(1000.0f);
	world->GetMainCamera()->SetPitch(-15.0f);
	world->GetMainCamera()->SetYaw(0);
	world->GetMainCamera()->SetPosition(Vector3(0, 50, 80));
	lockedObject = nullptr;
}

void TutorialGame::InitWorld() {
	world->ClearAndErase();
	physics->Clear();
	InitFloors();
	BridgeConstraintTest();
	InitGameExamples();
	InitGameObstacles();
}

void TutorialGame::BridgeConstraintTest() {
	Vector3 cubeSize(5, 1, 1);
	Vector3 baseSize(5, 1, 5);

	float invCubeMass = 5; // how heavy the middle pieces are
	int numLinks = 20;
	float maxDistance = 5; // constraint distance
	float cubeDistance = 5; // distance between links

	Vector3 startPos = Vector3(0, 50, 0);
	GameObject* start = AddCubeToWorld(obstacleTex, startPos + Vector3(0, 0, 0), baseSize, 0.0f);
	GameObject* end = AddCubeToWorld(obstacleTex, startPos + Vector3(0, 0, (numLinks + 2) * cubeDistance), baseSize, 0.0f);
	GameObject* previous = start;
	for (int i = 0; i < numLinks; ++i) {
		GameObject * block = AddCubeToWorld(obstacleTex, startPos + Vector3(0, 0, (i + 1) * cubeDistance), cubeSize, invCubeMass);
		PositionConstraint* constraint = new PositionConstraint(previous, block, maxDistance);
		world->AddConstraint(constraint);
		previous = block;
	}
	PositionConstraint* constraint = new PositionConstraint(previous, end, maxDistance);
	world->AddConstraint(constraint);
}

/*

A single function to add a large immoveable cube to the bottom of our world

*/
GameObject* TutorialGame::AddFloorToWorld(OGLTexture* texture, const Vector3& position, const Vector3& size, float elasticity, float friction) {
	GameObject* floor = new GameObject("Floor");
	if (elasticity == 0.0f)
		floor->objectType = ObjectType::Death;
	AABBVolume* volume	= new AABBVolume(size);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform().SetScale(size * 2).SetPosition(position);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, texture, basicShader));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));
	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->SetElasticity(elasticity);
	floor->GetPhysicsObject()->SetFriction(friction);
	floor->GetPhysicsObject()->InitCubeInertia();
	world->AddGameObject(floor);

	return floor;
}

/*

Builds a game object that uses a sphere mesh for its graphics, and a bounding sphere for its
rigid body representation. This and the cube function will let you build a lot of 'simple' 
physics worlds. You'll probably need another function for the creation of OBB cubes too.

*/
GameObject* TutorialGame::AddSphereToWorld(OGLTexture* texture, const Vector3& position, float radius, float inverseMass, float elasticity, float friction) {
	GameObject* sphere = new GameObject("Sphere");

	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius);
	sphere->SetBoundingVolume((CollisionVolume*)volume);

	sphere->GetTransform().SetScale(sphereSize).SetPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, texture, basicShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));
	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->SetElasticity(elasticity);
	sphere->GetPhysicsObject()->SetFriction(friction);
	sphere->GetPhysicsObject()->InitSphereInertia();
	world->AddGameObject(sphere);

	return sphere;
}

GameObject* TutorialGame::AddCubeToWorld(OGLTexture* texture, const Vector3& position, Vector3 dimensions, float inverseMass, float elasticity, 
	float friction, bool rotate, bool spring) {
	GameObject* cube = new GameObject("Cube");
	if (rotate) {
		cube->objectType = ObjectType::Rotating;
		OBBVolume* volume = new OBBVolume(dimensions);
		cube->SetBoundingVolume((CollisionVolume*)volume);
	}
	else {
		AABBVolume* volume = new AABBVolume(dimensions);
		cube->SetBoundingVolume((CollisionVolume*)volume);
		if (spring) {
			cube->objectType = ObjectType::Spring;
			cube->SetRestPosition(position);
		}
	}
	cube->GetTransform().SetPosition(position).SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, texture, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));
	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->SetElasticity(elasticity);
	cube->GetPhysicsObject()->SetFriction(friction);
	cube->GetPhysicsObject()->InitCubeInertia();
	
	world->AddGameObject(cube);

	return cube;
}

GameObject* TutorialGame::AddCapsuleToWorld(OGLTexture* texture, const Vector3& position, float halfHeight, float radius, float inverseMass,
	float elasticity, float friction) {
	GameObject* capsule = new GameObject("Capsule");

	CapsuleVolume* volume = new CapsuleVolume(halfHeight, radius);
	capsule->SetBoundingVolume((CollisionVolume*)volume);

	capsule->GetTransform().SetScale(Vector3(radius* 2, halfHeight, radius * 2)).SetPosition(position);

	capsule->SetRenderObject(new RenderObject(&capsule->GetTransform(), capsuleMesh, texture, basicShader));
	capsule->SetPhysicsObject(new PhysicsObject(&capsule->GetTransform(), capsule->GetBoundingVolume()));
	capsule->GetPhysicsObject()->SetInverseMass(inverseMass);
	capsule->GetPhysicsObject()->SetElasticity(elasticity);
	capsule->GetPhysicsObject()->SetFriction(friction);
	capsule->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(capsule);

	return capsule;

}

void TutorialGame::InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius) {
	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddSphereToWorld(basicTex, position, radius, 1.0f);
		}
	}
}

void TutorialGame::InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims) {
	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddCubeToWorld(basicTex, position, cubeDims, 1.0f);
		}
	}
}

void TutorialGame::InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing) {
	float sphereRadius = 1.0f;
	Vector3 cubeDims = Vector3(1, 1, 1);
	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			int type = rand() % 2 + 1;
			if (type == 1) {
				if(rand() % 2)
					AddCubeToWorld(basicTex, position, cubeDims * Vector3(1, 4, 1));
				else
					AddCubeToWorld(basicTex, position, cubeDims);
			}
			else if (type == 2) {
				if (rand() % 2)
					AddSphereToWorld(basicTex, position, sphereRadius / 2);
				else
					AddSphereToWorld(basicTex, position, sphereRadius);
			}
			else {
				if (rand() % 2)
					AddCapsuleToWorld(basicTex, position, 3.0f, sphereRadius * 1.5);
				else
					AddCapsuleToWorld(basicTex, position, 2.0f, sphereRadius);
			}
		}
	}
}

void TutorialGame::InitFloors() {
	/* Lava */
	AddFloorToWorld(lavaTex, Vector3(0, -50, -400), Vector3(500, 1, 1000), 0.0f);

	AddFloorToWorld(floorTex, Vector3(0, 0, 0), Vector3(25, 1, 25));
	AddFloorToWorld(floorTex, Vector3(0, 0, -50), Vector3(12.5, 1, 25));
	AddFloorToWorld(floorTex, Vector3(0, 0, -85), Vector3(25, 1, 10));
	AddFloorToWorld(floorTex, Vector3(-15, 0, -145), Vector3(10, 1, 50));
	AddFloorToWorld(floorTex, Vector3(15, 0, -145), Vector3(10, 1, 50));
	AddFloorToWorld(floorTex, Vector3(0, 0, -205), Vector3(100, 1, 10));
	AddFloorToWorld(floorTex, Vector3(-90, 0, -225), Vector3(10, 1, 10));
	AddFloorToWorld(floorTex, Vector3(90, 0, -225), Vector3(10, 1, 10));

	/* Ice Shortcut */
	AddFloorToWorld(iceTex, Vector3(0, 0, -225), Vector3(5, 1, 5), 0.2f, 0.1f);

	AddFloorToWorld(floorTex, Vector3(0, 0, -245), Vector3(100, 1, 10));
	AddFloorToWorld(floorTex, Vector3(0, 0, -265), Vector3(10, 1, 10));

	/* High elasticity trampoline */
	AddFloorToWorld(trampolineTex, Vector3(0, -30, -315), Vector3(10, 1, 10), 6.0f);		
	AddFloorToWorld(trampolineTex, Vector3(0, -30, -395), Vector3(10, 1, 10), 6.0f);

	AddFloorToWorld(floorTex, Vector3(0, 0, -445), Vector3(10, 1, 10));

	/* Low Friction Ice */
	AddFloorToWorld(iceTex, Vector3(30, 0, -465), Vector3(40, 1, 10), 0.2f, 0.1f);
	AddFloorToWorld(iceTex, Vector3(60, 0, -515), Vector3(10, 1, 40), 0.2f, 0.1f);
	AddFloorToWorld(iceTex, Vector3(30, 0, -565), Vector3(40, 1, 10), 0.2f, 0.1f);

	AddFloorToWorld(floorTex, Vector3(0, 0, -605), Vector3(10, 1, 30));
	AddFloorToWorld(floorTex, Vector3(0, 0, -645), Vector3(80, 1, 10));
	AddFloorToWorld(floorTex, Vector3(-90, 0, -735), Vector3(10, 1, 100));
	AddFloorToWorld(floorTex, Vector3(90, 0, -735), Vector3(10, 1, 100));
	AddFloorToWorld(floorTex, Vector3(0, 0, -825), Vector3(80, 1, 10));
	AddFloorToWorld(floorTex, Vector3(0, 0, -865), Vector3(5, 1, 30));
	AddFloorToWorld(floorTex, Vector3(-25, 0, -900), Vector3(30, 1, 5));
	AddFloorToWorld(floorTex, Vector3(-60, 0, -915), Vector3(5, 1, 20));
	AddFloorToWorld(floorTex, Vector3(-60, -20, -1050), Vector3(5, 1, 20));

	/* Walls */
	AddFloorToWorld(floorTex, Vector3(-5, 3, -865.5), Vector3(1, 2, 30.5));
	AddFloorToWorld(floorTex, Vector3(5, 3, -869.5), Vector3(1, 2, 34.5));
	AddFloorToWorld(floorTex, Vector3(-25.25, 3, -905), Vector3(31.25, 2, 1));
	AddFloorToWorld(floorTex, Vector3(-36.25, 3, -895), Vector3(30.25, 2, 1));
	AddFloorToWorld(floorTex, Vector3(-65.5, 3, -915.5), Vector3(1, 2, 19.5));
	AddFloorToWorld(floorTex, Vector3(-55.5, 3, -920.5), Vector3(1, 2, 14.5));
}

void TutorialGame::InitGameExamples() {
	AddPlayerToWorld(Vector3(0, 10, -30));
	AddEnemyToWorld(Vector3(5, 10, -5));
	AddBonusToWorld(Vector3(10, 10, -5));
}

void TutorialGame::InitGameObstacles() {
	/* Projectile Walls */
	AddCubeToWorld(obstacleTex, Vector3(-50, 5, -140), Vector3(1, 5, 40), 0.0f);
	AddCubeToWorld(obstacleTex, Vector3(120, 5, -225), Vector3(1, 5, 30), 0.0f);

	/* Spinning Bar */
	//AddCubeToWorld(obstacleTex, Vector3(0, 3, -735), Vector3(1, 1, 100), 0.0f);
	AddCubeToWorld(obstacleTex, Vector3(0, 3, -150), Vector3(1, 1, 100), 0.0f, 0.2, 0.8, true);

	/* Spring Platforms*/
	AddCubeToWorld(obstacleTex, Vector3(0, 5, -890), Vector3(4, 4, 1), 10, 0.2, 0.8, false, true);
	AddCubeToWorld(obstacleTex, Vector3(-50, 5, -900), Vector3(1, 4, 4), 10, 0.2, 0.8, false, true);
	AddCubeToWorld(obstacleTex, Vector3(-60, -15, -985), Vector3(10, 1, 10), 10, 0.2, 0.8, false, true);
}

GameObject* TutorialGame::AddPlayerToWorld(const Vector3& position) {
	float meshSize = 3.0f;
	float inverseMass = 0.5f;

	GameObject* character = new GameObject("Player");
	character->objectType = ObjectType::Player;

	SphereVolume* volume = new SphereVolume(3);
	character->SetBoundingVolume((CollisionVolume*)volume);
	character->GetTransform().SetScale(Vector3(meshSize, meshSize, meshSize)).SetPosition(position);
	if (rand() % 2) {
		character->SetRenderObject(new RenderObject(&character->GetTransform(), charMeshA, nullptr, basicShader));
	}
	else {
		character->SetRenderObject(new RenderObject(&character->GetTransform(), charMeshB, nullptr, basicShader));
	}
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));
	character->GetPhysicsObject()->SetElasticity(0.4f);
	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(character);

	//lockedObject = character;

	return character;
}

GameObject* TutorialGame::AddEnemyToWorld(const Vector3& position) {
	float meshSize		= 3.0f;
	float inverseMass	= 2.0f;

	GameObject* character = new GameObject("Enemy");
	character->objectType = ObjectType::Enemy;

	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.9f, 0.3f) * meshSize);
	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform().SetScale(Vector3(meshSize, meshSize, meshSize)).SetPosition(position);

	character->SetRenderObject(new RenderObject(&character->GetTransform(), enemyMesh, nullptr, basicShader));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));
	character->GetPhysicsObject()->SetElasticity(0.4f);
	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(character);

	return character;
}

GameObject* TutorialGame::AddBonusToWorld(const Vector3& position) {
	GameObject* bonus = new GameObject("Bonus");
	bonus->objectType == ObjectType::Bonus;

	SphereVolume* volume = new SphereVolume(1.25f);
	bonus->SetBoundingVolume((CollisionVolume*)volume);
	bonus->GetTransform().SetScale(Vector3(0.25, 0.25, 0.25)).SetPosition(position);

	bonus->SetRenderObject(new RenderObject(&bonus->GetTransform(), bonusMesh, nullptr, basicShader));
	bonus->SetPhysicsObject(new PhysicsObject(&bonus->GetTransform(), bonus->GetBoundingVolume()));
	bonus->GetPhysicsObject()->SetElasticity(0.0f);
	bonus->GetPhysicsObject()->SetInverseMass(1.0f);
	bonus->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(bonus);

	return bonus;
}

void TutorialGame::FireObjects() {
	int size = 3;
	GameObject* cube = new GameObject("Cube");
	AABBVolume* volume = new AABBVolume(Vector3(size, size, size));
	int loc = ((rand() % (100 - 180 + 1)) + 100) * -1;
	cube->SetBoundingVolume((CollisionVolume*)volume);
	cube->GetTransform().SetPosition(Vector3(-40, 5, loc)).SetScale(Vector3(size, size, size));
	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, obstacleTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));
	cube->GetPhysicsObject()->SetLinearVelocity(Vector3(200, 0, 0));
	cube->GetPhysicsObject()->AddTorque(Vector3(1000, 1000, 1000));
	cube->GetPhysicsObject()->InitCubeInertia();
	world->AddGameObject(cube);

	GameObject* sphere = new GameObject("Sphere");
	SphereVolume* volume1 = new SphereVolume(size);
	if(rand() % 2)
		loc = ((rand() % (200 - 210 + 1)) + 200) * -1;
	else
		loc = ((rand() % (250 - 240 + 1)) + 240) * -1;
	sphere->SetBoundingVolume((CollisionVolume*)volume1);
	sphere->GetTransform().SetPosition(Vector3(90, 5, loc)).SetScale(Vector3(size, size, size));
	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, obstacleTex, basicShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));
	sphere->GetPhysicsObject()->SetLinearVelocity(Vector3(-200, 0, 0));
	sphere->GetPhysicsObject()->AddTorque(Vector3(1000, 1000, 1000));
	sphere->GetPhysicsObject()->InitCubeInertia();
	world->AddGameObject(sphere);	
	
	sceneTime = 0.0f;
}

/*

Every frame, this code will let you perform a raycast, to see if there's an object
underneath the cursor, and if so 'select it' into a pointer, so that it can be 
manipulated later. Pressing Q will let you toggle between this behaviour and instead
letting you move the camera around. 

*/
bool TutorialGame::SelectObject() {
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
	if (inSelectionMode) {
		if(selectionObject)
			Debug::Print(selectionObject->GetName(), Vector2(0, 80));
		Debug::Print("Press Q to change to camera mode!", Vector2(0, 90));
		if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT)) {
			if (selectionObject) {	//set colour to deselected;
				selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
				selectionObject = nullptr;
				lockedObject	= nullptr;
			}
			Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());
			RayCollision closestCollision;
			if (world->Raycast(ray, closestCollision, nullptr, true)) {
				selectionObject = (GameObject*)closestCollision.node;
				selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
				return true;
			}
			else {
				return false;
			}
		}
	}
	else {
		Debug::Print("Press Q to change to select mode!", Vector2(0, 90));
	}

	if (lockedObject) {
		Debug::Print("Press L to unlock object!", Vector2(0, 80));
	}

	else if(selectionObject) {
		Debug::Print("Press L to lock selected object object!", Vector2(0, 85));
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

/*
If an object has been clicked, it can be pushed with the right mouse button, by an amount
determined by the scroll wheel. In the first tutorial this won't do anything, as we haven't
added linear motion into our physics system. After the second tutorial, objects will move in a straight
line - after the third, they'll be able to twist under torque aswell.
*/
void TutorialGame::MoveSelectedObject() {
	Debug::Print("Click Force: " + std::to_string(forceMagnitude), Vector2(0, 5)); // Draw debug text at 10 ,20
	forceMagnitude += Window::GetMouse()->GetWheelMovement() * 10.0f;
	if (!selectionObject) {
		return;// we haven ’t selected anything !
	}
	// Push the selected object !
	if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::RIGHT)) {
		Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());
		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, nullptr, true)) {
			if (closestCollision.node == selectionObject) {
				selectionObject->GetPhysicsObject()->AddForceAtPosition(ray.GetDirection() * forceMagnitude, closestCollision.collidedAt);
			}
		}
	}
}
