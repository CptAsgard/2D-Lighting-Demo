#include "Floor.h"

DerivedRegister<Floor> Floor::reg(L"Floor");

Floor::Floor() {
	TileBase::setCollision( false );

	sf::Image img = sf::Image();
	if( !img.loadFromFile( "../Content/Textures/DestructionDeathmatchTexture002.png" ) )
		printf( "IMG.LOADFROMFILE FAIL" );

	tex = sf::Texture();
	if( !tex.loadFromImage( img ) )
		printf( "TEX.LOADFROMIMAGE FAIL" );

	sprite = sf::Sprite();
	sprite.setTexture( tex );
}

void Floor::draw( sf::RenderTarget& target, sf::RenderStates states ) const {
	target.draw( sprite, states );
}