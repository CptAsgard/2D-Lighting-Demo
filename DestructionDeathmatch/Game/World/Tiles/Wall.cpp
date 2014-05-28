#include "Wall.h"

DerivedRegister<Wall> Wall::reg(L"Wall");

Wall::Wall() {
	TileBase::col = Collidable( &(this->shape), std::bind( &Wall::onCollision, this, std::placeholders::_1 ) );

	TileBase::setHealth( 4 );
	TileBase::setCollision( true );

	shape = sf::ConvexShape(4);

	shape.setPoint( 0, sf::Vector2f( 0, 0 ) );
	shape.setPoint( 1, sf::Vector2f( 0, TILE_SIZE ) );
	shape.setPoint( 2, sf::Vector2f( TILE_SIZE, TILE_SIZE ) );
	shape.setPoint( 3, sf::Vector2f( TILE_SIZE, 0 ) );
	shape.setPosition( *(this->getPosition()) );
	shape.setFillColor( sf::Color::Black );

#ifdef DD_DEBUG

	shape.setOutlineColor( sf::Color::Red );
	shape.setOutlineThickness( 1 );	

#endif
}

const sf::ConvexShape* Wall::getShape() {
	return &this->shape;
}

void Wall::update( StateManager* game, float deltaTime ) {
	shape.setPosition(*(this->getPosition()));
}

void Wall::draw( sf::RenderTarget& target, sf::RenderStates states ) const {
	if (shape.getPointCount() == 0)
		return;

	target.draw( shape, states );
}

void Wall::onCollision( Collidable* other ) {
	printf( "a\n" );
}