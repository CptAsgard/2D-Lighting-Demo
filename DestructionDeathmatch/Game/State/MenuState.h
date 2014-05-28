#ifndef MENUSTATE_H
#define MENUSTATE_H

#include <memory>

#include "GameState.h"

class MenuState : public GameState
{

public:
	void Init( StateManager *game );
	void CleanUp();

	void Pause();
	void Resume();

	void HandleEvents( StateManager *game );
	void Update( StateManager *game, float deltaTime );
	void Draw( StateManager *game );

	static MenuState* Instance() {
		return &_menuState;
	}

protected:
	MenuState() { }

private:
	static MenuState _menuState;

};
#endif // MENUSTATE_H