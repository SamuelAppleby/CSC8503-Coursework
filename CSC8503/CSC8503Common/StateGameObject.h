#pragma once
#include "GameObject.h"
#include "StateMachine.h"
namespace NCL {
	namespace CSC8503 {
		enum class state { IDLE, FOLLOWROUTE, FOLLOWPATH, FOLLOWPLAYER };
		class StateMachine;
		class StateGameObject : public GameObject {
		public:
			StateGameObject();
			~StateGameObject();
			virtual void Update(float dt);
			string StateToString() const;
			void Idle();
		protected:
			state currentState;
			StateMachine* stateMachine;
			State* idleState;
		};
	}
}