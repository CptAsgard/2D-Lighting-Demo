#include "GameState.h"
#include "StateManager.h"

/**
 * Init creates the window, and starts the game.
 */
void StateManager::Init( const char* title, int width, int height ) {

	// Create the RenderWindow
	window = new sf::RenderWindow( sf::VideoMode( width, height ), title );
	window->setVerticalSyncEnabled( true );

	// Sets the application to running.
	running = true;
}

/**
 * CleanUp should be called when the game has to completely halt execution.
 */
void StateManager::CleanUp() {
	for( int i = 0; i < states.size(); i++ ) {
		PopState();
	}

	window->close();
	delete window;
}


/**
 * Will clean up the current state, and push the given state to the top of the stack, and Init() it.
 * ChangeState should be used when the state of the previous state doesn't matter anymore.
 */
void StateManager::ChangeState( GameState* state ) {
	if( !states.empty() ) {
		states.back()->CleanUp();
		states.pop_back();
	}
	
	states.push_back( state );
	states.back()->Init( this );
}

/**
 * Will pause the current state, and push the given state to the top of the stack, and Init() it.
 * PushState should be used when you only want to temporarily halt a state, and come back to it later.
 */
void StateManager::PushState( GameState* state ) {
	if( !states.empty() ) {
		states.back()->Pause();
	}
	
	states.push_back( state );
	states.back()->Init( this );
}

/**
 * Will clean up the current state, and go back to the previous state on the stack.
 * Use this to revert to the old state.
 */
void StateManager::PopState() {
	if( !states.empty() ) {
		states.back()->CleanUp();
		states.pop_back();
	}
	
	if( !states.empty() ) {
		states.back()->Resume();
	}
}


/**
 * Will let the current state handle the events.
 */
void StateManager::HandleEvents() {
	if( window->isOpen() ) {
		sf::Event event;
		while( window->pollEvent(event)) {
			
			if( event.type == sf::Event::Closed ) {
				Quit();
			}
		}
	}

	states.back()->HandleEvents( this );
}

/**
 * Will let the current state update.
 */
void StateManager::Update() {
	states.back()->Update( this, this->clock.restart().asSeconds() );
}

/**
 * Will let the current state draw.
 */
void StateManager::Draw() {
	window->clear();

	states.back()->Draw( this );

	window->display();
}