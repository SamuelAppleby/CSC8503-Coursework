#pragma once
#include "FloorObject.h"
namespace NCL {
	namespace CSC8503 {
		class MovingFloorObject : public FloorObject {
		public:
			MovingFloorObject(Vector3 start, Vector3 end) {
				xSpeed = 0;
				ySpeed = 0;
				zSpeed = 0;
				this->start = start;
				this->end = end;
				name = "MovingFloor";
			}
			void ToggleMoveRight(bool val) {
				val ? xSpeed = 5 : xSpeed = -5;
			}
			void ToggleMoveUp(bool val) {
				val ? ySpeed = 5 : ySpeed = -5;
			}
			void ToggleMoveForwards(bool val) {
				val ? zSpeed = -5 : zSpeed = 5;
			}
			Vector3 GetStart() const {
				return start;
			}
			Vector3 GetEnd() const {
				return end;
			}
			void Move() {
				GetPhysicsObject()->SetLinearVelocity({ xSpeed, ySpeed, zSpeed });
			}
		protected:
			float xSpeed;
			float ySpeed;
			float zSpeed;
			Vector3 start;
			Vector3 end;
		};
	}
}