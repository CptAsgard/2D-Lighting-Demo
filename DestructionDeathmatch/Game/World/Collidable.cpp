#include "Collidable.h"

void Collidable::registerCallback( std::function<void( Collidable* )> callback ) {
	this->callback = callback;
}