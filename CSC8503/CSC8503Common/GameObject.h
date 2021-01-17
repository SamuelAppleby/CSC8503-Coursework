#pragma once
#include <vector>
#include "Transform.h"
#include "CollisionVolume.h"
#include "PhysicsObject.h"
#include "RenderObject.h"
using std::vector;
namespace NCL {
	namespace CSC8503 {
		class GameObject {
		public:
			GameObject(string name = "");
			~GameObject();

			void SetName(string val) {
				name = val;
			}

			void SetBoundingVolume(CollisionVolume* vol) {
				boundingVolume = vol;
			}

			const CollisionVolume* GetBoundingVolume() const {
				return boundingVolume;
			}

			bool IsActive() const {
				return isActive;
			}

			Transform& GetTransform() {
				return transform;
			}

			RenderObject* GetRenderObject() const {
				return renderObject;
			}

			PhysicsObject* GetPhysicsObject() const {
				return physicsObject;
			}

			virtual void SetRenderObject(RenderObject* newObject) {
				renderObject = newObject;
			}

			virtual void SetPhysicsObject(PhysicsObject* newObject) {
				physicsObject = newObject;
			}

			const string& GetName() const {
				return name;
			}

			virtual void OnCollisionBegin(GameObject* otherObject){}
			
			virtual void OnCollisionEnd(GameObject* otherObject) {}

			bool GetBroadphaseAABB(Vector3&outsize) const;

			void UpdateBroadphaseAABB();

			void SetWorldID(int newID) {
				worldID = newID;
			}
			int GetWorldID() const {
				return worldID;
			}
			void SetIsActive(bool val) {
				isActive = val;
			}
			int GetSelected() const {
				return selectedObject;
			}
			void SetSelected(bool val) {
				selectedObject = val;
			}
			bool CanCollide() const {
				return canCollide;
			}
			void IncreaseTimeInSet(float dt) {
				timeInSet += dt;
			}
			void SetTimeInSet(float val) {
				timeInSet = val;
			}
			float GetTimeInSet() const {
				return timeInSet;
			}
			void SetPowerUpTimer(float val) {
				powerUpTimer = val;
			}
			float GetPowerUpTimer() const {
				return powerUpTimer;
			}
		protected:
			Transform			transform;

			CollisionVolume*	boundingVolume;
			PhysicsObject*		physicsObject;
			RenderObject*		renderObject;

			bool isActive;
			int	worldID;
			string	name;
			bool selectedObject;
			Vector3 broadphaseAABB;
			bool canCollide;
			float timeInSet;
			float powerUpTimer;
		};
	}
}

