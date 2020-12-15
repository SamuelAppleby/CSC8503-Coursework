#pragma once
#include "Transform.h"
#include "CollisionVolume.h"

#include "PhysicsObject.h"
#include "RenderObject.h"

#include <vector>

using std::vector;

namespace NCL {
	namespace CSC8503 {
		enum class ObjectType {
			Regular = 1,
			Player = 2,
			Enemy = 4,
			Death = 8,
			Rotating = 16,
			Spring = 32,
			Bonus = 64
		};
		class GameObject	{
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

			void SetRenderObject(RenderObject* newObject) {
				renderObject = newObject;
			}

			void SetPhysicsObject(PhysicsObject* newObject) {
				physicsObject = newObject;
			}

			const string& GetName() const {
				return name;
			}

			virtual void OnCollisionBegin(GameObject* otherObject) {
				//std::cout << "OnCollisionBegin event occured!\n";
				if (objectType == ObjectType::Player) 
					canJump = true;
				if (otherObject->objectType == ObjectType::Player)
					canJump = true;
			}

			virtual void OnCollisionEnd(GameObject* otherObject) {
				//std::cout << "OnCollisionEnd event occured!\n";
				/*if (objectType == ObjectType::Player)
					canJump = false;
				if (otherObject->objectType == ObjectType::Player)
					canJump = false;*/
			}

			bool GetBroadphaseAABB(Vector3&outsize) const;

			void UpdateBroadphaseAABB();

			void SetWorldID(int newID) {
				worldID = newID;
			}

			int		GetWorldID() const {
				return worldID;
			}

			void Jump() {
				if(canJump)
					physicsObject->ApplyLinearImpulse(Vector3(0, 30, 0));
			}

			bool GetCanJump() {
				return canJump;
			}

			Vector3 GetRestPosition() const {
				return restPosition;
			}

			void SetRestPosition(Vector3 val) {
				restPosition = val;
			}

			ObjectType objectType;

		protected:
			Transform			transform;

			CollisionVolume*	boundingVolume;
			PhysicsObject*		physicsObject;
			RenderObject*		renderObject;

			bool	isActive;
			int		worldID;
			string	name;

			Vector3 broadphaseAABB;

			bool canJump;
			Vector3 restPosition;
		};
	}
}

