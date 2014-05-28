/*
 * This class is the abstract base class for all game states.
 * By inheriting this class when creating a new state. ( e.g. IntroState, MenuState, PlayState )
 * you will be able to easily push these onto the StateManager stack.
 */

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "StateManager.h"

class GameState
{
	
public:
	virtual void Init( StateManager *game ) = 0;
	virtual void CleanUp() = 0;
	
	virtual void Pause() = 0;
	virtual void Resume() = 0;
	
	virtual void HandleEvents( StateManager *game ) = 0;
	virtual void Update( StateManager *game, float deltaTime ) = 0;
	virtual void Draw( StateManager *game ) = 0;
	
	/**
	 * Change the state to the next given state
	 * @param	game	pointer to the state manager
	 * @param	state	pointer to the next state
	 */
	void ChangeState( StateManager *game, GameState *state ) {
		game->ChangeState(state);
	}
	
protected:
	GameState() { }	// Declare a protected constructor, for Singleton behaviour.
	
};

#endif // GAMESTATE_H








