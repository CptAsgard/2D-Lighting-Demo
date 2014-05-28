/*
 * Albeit not very clear, the StateManager also functions as the game engine.
 * This is because the StateManager calls upon the HandleEvents, Update, and Draw functions
 * of each state. I found it most convenient to simply put them together.
 */

#ifndef STATEMANAGER_H
#define STATEMANAGER_H 

#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>

class GameState;

class StateManager
{
	
public:
	void Init( const char* title, int width, int height );
	void CleanUp();
	
	void ChangeState( GameState *state );
	void PushState( GameState *state );
	void PopState();
	
	void HandleEvents();
	void Update();
	void Draw();
	
	bool Running() { return running; }
	void Quit() { running = false; }

	sf::RenderWindow* window;
	
private:
	std::vector<GameState*> states;
	
	sf::Clock clock;
	
	bool running;
};

#endif // STATEMANAGER_H