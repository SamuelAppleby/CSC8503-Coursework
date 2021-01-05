#pragma once
#include "GameTechRenderer.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "../CSC8503Common/MovingFloorObject.h"
#include "../CSC8503Common/StateGameObject.h"

namespace NCL {
	namespace CSC8503 {
		class PlayerObject;
		class TutorialGame {
		public:
			TutorialGame();
			~TutorialGame();

			virtual void UpdateGame(float dt);
			void UpdateMenu(float dt);
			void UpdateLevel(float dt);
			void UpdateLevel1(float dt);
			void UpdateLevel2(float dt);
			void FindPath();
			void DisplayPath();
			int GetCurrentLevel() const {
				return currentLevel;
			}
			float GetTimeout() const {
				return timeOut;
			}
		protected:
			void InitialiseAssets();

			void InitCamera();
			void UpdateKeys();

			void InitWorld();

			void InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			void InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims);
			void InitFloors(int level);
			void InitGameExamples(int level);
			void InitGameObstacles(int level);
			void BridgeConstraintTest(Vector3 startPos);
			
			void FireObjects();
			void MovePlatforms();
			bool SelectObject();
			void MoveSelectedObject();
			void DebugObjectMovement();
			void LockedObjectMovement();

			GameObject* AddFloorToWorld(GameObject* floor, const Vector3& position, const Vector3& size, const Matrix4& orientation = Matrix4());
			GameObject* AddSphereToWorld(GameObject* sphere, const Vector3& position, float radius);
			GameObject* AddCubeToWorld(GameObject* cube, const Vector3& position, Vector3 dimensions);
			GameObject* AddCapsuleToWorld(GameObject* cube, const Vector3& position, float halfHeight, float radius);

			PlayerObject* AddPlayerToWorld(const Vector3& position);
			GameObject* AddEnemyToWorld(const Vector3& position);
			GameObject* AddBonusToWorld(const Vector3& position);
			StateGameObject* AddOscillateStateObjectToWorld(const Vector3& position);
			StateGameObject* AddPatrolStateObjectToWorld(const Vector3& position, GameObject* follow);

			GameTechRenderer*	renderer;
			PhysicsSystem*		physics;
			GameWorld*			world;

			bool useGravity;
			bool inSelectionMode;

			float		forceMagnitude;
			float sceneTime;
			float timeOut;
			GameObject* selectionObject = nullptr;

			OGLMesh*	capsuleMesh = nullptr;
			OGLMesh*	cubeMesh	= nullptr;
			OGLMesh*	sphereMesh	= nullptr;
			OGLTexture* basicTex	= nullptr;
			OGLTexture* floorTex = nullptr;
			OGLTexture* lavaTex = nullptr;
			OGLTexture* iceTex = nullptr;
			OGLTexture* trampolineTex = nullptr;
			OGLTexture* obstacleTex = nullptr;
			OGLTexture* woodenTex = nullptr;
			OGLTexture* bonusTex = nullptr;
			OGLTexture* playerTex = nullptr;
			OGLTexture* enemyTex = nullptr;
			OGLTexture* finishTex = nullptr;
			OGLTexture* menuTex = nullptr;
			OGLShader*	basicShader = nullptr;

			//Coursework Meshes
			OGLMesh*	charMeshA	= nullptr;
			OGLMesh*	charMeshB	= nullptr;
			OGLMesh*	enemyMesh	= nullptr;
			OGLMesh*	bonusMesh	= nullptr;

			//Coursework Additional functionality	
			GameObject* lockedObject	= nullptr;
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}
			PlayerObject* player;
			bool lockedOrientation;
			StateGameObject* testStateObject;

			vector<MovingFloorObject*> movingPlatforms;
			int currentLevel;
			vector<Vector3> path;

			Vector4 white;
			float textSize = 15.0f;
		};
	}
}

