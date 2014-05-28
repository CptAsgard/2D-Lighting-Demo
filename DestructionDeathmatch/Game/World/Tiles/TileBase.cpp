#include "TileBase.h"

TileFactory* TileFactory::tileFactory;

TileBase::~TileBase(){ }
TileBase::TileBase() {
	rect = sf::IntRect( 0, 0, TILE_SIZE, TILE_SIZE );
}

void TileBase::draw( sf::RenderTarget& target, sf::RenderStates states ) const {
	// nvm soz dacht dat deze misschien niet declared was ;/, da's een andere error :D
	printf("derp");
}

void TileBase::setPosition( sf::Vector2f position ) {
	this->pos = position;
}

const sf::Vector2f* TileBase::getPosition() const {
	return &(this->pos);
}

void TileBase::setHealth( unsigned int health ) {
	this->health = health;
}

unsigned int TileBase::getHealth() const {
	return this->health;
}

void TileBase::setCollision( bool hasCollision ) {
	this->collision = hasCollision;
}

bool TileBase::hasCollision() const {
	return this->collision;
}