#pragma once
#include "GameObject.h"
#include "StateMachine.h"
namespace NCL {
	namespace CSC8503 {
		class StateMachine;
		class StateGameObject : public GameObject {
		public:
			StateGameObject();
			~StateGameObject();
			virtual void Update(float dt);
		protected:
			StateMachine* stateMachine;
		};
	}
}