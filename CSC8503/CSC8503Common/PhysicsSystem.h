#pragma once
#include "../CSC8503Common/GameWorld.h"
#include <set>

namespace NCL {
	namespace CSC8503 {
		class PhysicsSystem	{
		public:
			PhysicsSystem(GameWorld& g);
			~PhysicsSystem();

			void Clear();
			void ClearDeletedCollisions();
			void Update(float dt);

			void UseGravity(bool state) {
				applyGravity = state;
			}

			void SetGlobalDamping(float d) {
				globalDamping = d;
			}

			void SetGravity(const Vector3& g) {
				gravity = g;
			}

			float GetDampingFactor() const {
				return dampingFactor;
			}

			void SetDampingFactor(float val) {
				dampingFactor = val;
			}

			bool GetBroadPhase() const {
				return useBroadPhase;
			}

			void SetBroadPhase(bool val) {
				useBroadPhase = val;
			}

			int GetConstraintIterationCount() const {
				return constraintIterationCount;
			}
		protected:
			void BasicCollisionDetection();
			void BroadPhase();
			void NarrowPhase();

			void ClearForces();

			void IntegrateAccel(float dt);
			void IntegrateVelocity(float dt);

			void UpdateConstraints(float dt);

			void UpdateCollisionList();
			void UpdateObjectAABBs();

			void ImpulseResolveCollision(GameObject& a , GameObject&b, CollisionDetection::ContactPoint& p) const;
			void SpringResolveCollision(GameObject& a, GameObject& b, CollisionDetection::ContactPoint& p) const;
			void SpringOnPoint(GameObject* a, Vector3 rest) const;

			GameWorld& gameWorld;

			bool	applyGravity;
			Vector3 gravity;
			float	dTOffset;
			float	globalDamping;
			float	dampingFactor;
			std::set<CollisionDetection::CollisionInfo> allCollisions;
			std::set<CollisionDetection::CollisionInfo> broadphaseCollisions;
			bool useBroadPhase;
			int numCollisionFrames	= 5;
			int constraintIterationCount = 10;
		};
	}
}

