#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

class GameStateMachine;

class GameState {
public:
    virtual ~GameState () {}

	virtual void Enter (GameStateMachine* stateMachine) = 0;

	virtual void Execute (GameStateMachine* stateMachine) = 0;

	virtual void Exit (GameStateMachine* stateMachine) = 0;
};

#endif
