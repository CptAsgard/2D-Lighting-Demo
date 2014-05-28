#include "Game/State/StateManager.h"
#include "Game/State/PlayState.h"
#include <Windows.h>

int main(int argc, char **argv) {
	StateManager game;
	
	game.Init( "2D Lighting", 1024, 768 );
	
	game.ChangeState( PlayState::Instance() );

	while( game.Running() ) {
		game.HandleEvents();
		game.Update();
		game.Draw();
	}
	
	game.CleanUp();
	
	return 0;
}