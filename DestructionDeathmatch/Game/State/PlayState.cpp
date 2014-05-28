#include "PlayState.h"

PlayState PlayState::_playState;

PlayState::PlayState() {

}

void PlayState::Init( StateManager* game ) {	
	if( !worldManager )
		worldManager = new WorldManager();

	worldManager->init( game );
}

void PlayState::CleanUp() {
	delete worldManager;
}

void PlayState::Pause() {

}

void PlayState::Resume() {

}

void PlayState::HandleEvents( StateManager* game ) {

}

void PlayState::Update( StateManager* game, float deltaTime ) {

	worldManager->update( game, deltaTime );
}

void PlayState::Draw( StateManager* game )  {	
	this->worldManager->draw( game );
}