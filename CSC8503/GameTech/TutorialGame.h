#pragma once
#include "GameTechRenderer.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "../CSC8503Common/StateGameObject.h"
#include "../CSC8503Common/NavigationGrid.h"
#include "../CSC8503Common/PathFindingStateGameObject.h"
#include "../CSC8503Common/PlatformStateGameObject.h"
#include "../CSC8503Common/GameWorld.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Common/TextureLoader.h"
#include "../CSC8503Common/PositionConstraint.h"
#include "../CSC8503Common/IceObject.h"
#include "../CSC8503Common/LavaObject.h"
#include "../CSC8503Common/FloorObject.h"
#include "../CSC8503Common/TrampolineObject.h"
#include "../CSC8503Common/CubeObject.h"
#include "../CSC8503Common/SphereObject.h"
#include "../CSC8503Common/RotatingCubeObject.h"
#include "../CSC8503Common/SpringObject.h"
#include "../CSC8503Common/PlayerObject.h"
#include "../CSC8503Common/EnemyObject.h"
#include "../CSC8503Common/BonusObject.h"
#include "../CSC8503Common/CapsuleObject.h"
#include "../CSC8503Common/FinishObject.h"

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
			void DrawDebugInfo();
			void CheckFinished(float dt);
			void UpdateLevel1(float dt);
			void FireObjects();
			void UpdateLevel2(float dt);
			void EnemyRaycast();
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
			void CreateMazeWalls();
			void InitGameExamples(int level);
			void InitGameObstacles(int level);
			void InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			void InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims);
			void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			
			GameObject* AddFloorToWorld(GameObject* floor, const Vector3& position, const Vector3& size, const Matrix4& orientation = Matrix4());
			GameObject* AddSphereToWorld(GameObject* sphere, const Vector3& position, float radius);
			GameObject* AddCubeToWorld(GameObject* cube, const Vector3& position, Vector3 dimensions);
			GameObject* AddCapsuleToWorld(GameObject* capsule, const Vector3& position, float halfHeight, float radius);
			void AddBridgeToWorld(Vector3 startPos);

			GameObject* AddPlayerToWorld(GameObject* p, const Vector3& position);
			GameObject* AddEnemyToWorld(GameObject* e, const Vector3& position);
			GameObject* AddBonusToWorld(const Vector3& position);

			bool SelectObject();
			void DebugObjectMovement();
			void LockedObjectMovement();

			GameTechRenderer* renderer;
			PhysicsSystem* physics;
			GameWorld* world;

			bool useGravity;
			bool useBroadphase;
			bool inSelectionMode;

			float forceMagnitude;
			float reloadTime;
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
			EnemyStateGameObject* enemy;
			vector<PlatformStateGameObject*> platforms;

			bool lockedOrientation;

			int currentLevel;
			NavigationGrid* grid;

			Vector4 white;
			float textSize = 15.0f;

		};
	}
}

