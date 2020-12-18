#pragma once
#include "FloorObject.h"

namespace NCL {
	namespace CSC8503 {
		class MovingFloorObject : public FloorObject {
		public:
			MovingFloorObject(Vector3 start, Vector3 end) {
				up = false;
				right = false;
				this->start = start;
				this->end = end;
				name = "MovingFloor";
			}
			void setUp(bool val) {
				up = val;
			}
			bool getUp() const {
				return up;
			}
			void setRight(bool val) {
				right = val;
			}
			bool getRight() const {
				return right;
			}
			Vector3 getStart() const {
				return start;
			}
			Vector3 getEnd() const {
				return end;
			}
		protected:
			bool up;
			bool right;
			Vector3 start;
			Vector3 end;
		};
	}
}