#pragma once
#include "GameTechRenderer.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "../CSC8503Common/MovingFloorObject.h"
#include "../CSC8503Common/StateGameObject.h"
#include "../CSC8503Common/NavigationGrid.h"
#include "../CSC8503Common/PathFindingStateGameObject.h"

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
			void InitFloors(int level);
			void InitGameExamples(int level);
			void InitGameObstacles(int level);
			void InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			void InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims);
			void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			
			GameObject* AddFloorToWorld(GameObject* floor, const Vector3& position, const Vector3& size, const Matrix4& orientation = Matrix4());
			GameObject* AddSphereToWorld(GameObject* sphere, const Vector3& position, float radius);
			GameObject* AddCubeToWorld(GameObject* cube, const Vector3& position, Vector3 dimensions);
			GameObject* AddCapsuleToWorld(GameObject* cube, const Vector3& position, float halfHeight, float radius);
			void BridgeConstraintTest(Vector3 startPos);

			PlayerObject* AddPlayerToWorld(const Vector3& position);
			GameObject* AddEnemyToWorld(const Vector3& position);
			GameObject* AddBonusToWorld(const Vector3& position);
			StateGameObject* AddStateObjectToWorld(StateGameObject* object, const Vector3& position);
			//StateGameObject* AddPathFindingStateObjectToWorld(const Vector3& position, GameObject* follow, NavigationGrid* grid);

			void FireObjects();
			void MovePlatforms();
			bool SelectObject();
			void MoveSelectedObject();
			void DebugObjectMovement();
			void LockedObjectMovement();

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
			PathFindingStateGameObject* enemy;

			bool lockedOrientation;

			vector<MovingFloorObject*> movingPlatforms;
			int currentLevel;
			vector<Vector3> path;

			Vector4 white;
			float textSize = 15.0f;

			NavigationGrid* grid;
		};
	}
}

