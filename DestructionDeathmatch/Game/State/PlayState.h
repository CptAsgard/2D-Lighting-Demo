#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include <vector>

#include <SFML/Network.hpp>
#include <algorithm>
#include <memory>

#include "GameState.h"
#include "../World/WorldManager.h"

class WorldManager;

class PlayState : public GameState
{
	
public:
	void Init( StateManager *game );
	void CleanUp();
	
	void Pause();
	void Resume();
	
	void HandleEvents( StateManager *game );
	void Update( StateManager *game, float deltaTime );
	void Draw( StateManager *game );
	
	static PlayState* Instance() {
		return &_playState;
	}

	WorldManager* worldManager;

protected:
	PlayState();
	
private:
	static PlayState _playState;

};
#endif // PLAYSTATE_H










