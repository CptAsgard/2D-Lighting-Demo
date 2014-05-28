#include "Player.h"

Player::Player() : speed(250)
{
	col = Collidable( &(this->collider), std::bind( &Player::onCollision, this, std::placeholders::_1 ) );

	sf::Image img = sf::Image();
	if( !img.loadFromFile( "../Content/Textures/PlayerBot.png" ) )
		printf( "IMG.LOADFROMFILE FAIL" );

	texSprite = sf::Texture();
	if( !texSprite.loadFromImage( img ) )
		printf( "TEX.LOADFROMIMAGE FAIL" );

	sprite = sf::Sprite();
	sprite.setTexture( texSprite );

	radiusmask = sf::CircleShape(50);
	radiusmask.setFillColor( sf::Color::Transparent );
	
	{ // set vertices for flashlight mask
		sf::Vector2f v0 = sf::Vector2f( 0, 0 );
		sf::Vector2f v1 = sf::Vector2f( -200, 500 );
		sf::Vector2f v2 = sf::Vector2f( 200, 500 );

		flashlight.setPointCount( 3 );
		flashlight.setPoint( 0, v0 );
		flashlight.setPoint( 1, v1 );
		flashlight.setPoint( 2, v2 );

		flashlight.setOrigin( v0 );

		flashlight.setFillColor( sf::Color::Transparent );
	}

	{ // set vertices for collider
		sf::Vector2f v0 = sf::Vector2f( 0, 0 );
		sf::Vector2f v1 = sf::Vector2f( sprite.getLocalBounds().width+5, 0 );
		sf::Vector2f v2 = sf::Vector2f( sprite.getLocalBounds().width+5, sprite.getLocalBounds().height+5 );
		sf::Vector2f v3 = sf::Vector2f( 0, sprite.getLocalBounds().height+5 );

		collider.setPointCount( 4 );
		collider.setPoint( 0, v0 );
		collider.setPoint( 1, v1 );
		collider.setPoint( 2, v2 );
		collider.setPoint( 3, v3 );

		collider.setOrigin( v0 );

		collider.setFillColor( sf::Color::Red );
	}
}

bool mousePressed;
void Player::update( StateManager* game, float deltaTime ) {
	if( sf::Keyboard::isKeyPressed( sf::Keyboard::Left ) || sf::Keyboard::isKeyPressed( sf::Keyboard::A ) ) {
		move( -speed * deltaTime, 0 );
	} else if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Right ) || sf::Keyboard::isKeyPressed( sf::Keyboard::D ) ) {
		move( speed * deltaTime, 0 );
	}

	if( sf::Keyboard::isKeyPressed( sf::Keyboard::Up ) ||  sf::Keyboard::isKeyPressed( sf::Keyboard::W ) ) {
		move( 0, -speed * deltaTime );
	} else if( sf::Keyboard::isKeyPressed( sf::Keyboard::Down ) ||  sf::Keyboard::isKeyPressed( sf::Keyboard::S ) ) {
		move( 0, speed * deltaTime );
	}

	setRotation( Math::Angle( getPosition(), game->window->mapPixelToCoords( sf::Mouse::getPosition( *game->window ) ) ) * (180/M_PI) + 270 );

	flashlight.setRotation( getRotation() );

	flashlight.setPosition( getPosition() + sf::Vector2f( sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2 ) );
	sprite.setPosition( getPosition() );
	collider.setPosition( getPosition() );
	radiusmask.setPosition( getPosition() );
}

void Player::draw( StateManager* game ) {
	game->window->draw( sprite );
}

void Player::onCollision( Collidable* other ) {
	float otherCenter = other->shape->getLocalBounds().width/2;
	const sf::Vector2f otherCenterPos = ( other->shape->getPosition() + sf::Vector2f( otherCenter, otherCenter ) );

	const sf::Vector2f thisCenterPos = ( getPosition() + sf::Vector2f( sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2 ) );

	float xAxis = abs( thisCenterPos.x - otherCenterPos.x );
	float yAxis = abs( thisCenterPos.y - otherCenterPos.y );

	int cw = sprite.getLocalBounds().width/2 + other->shape->getLocalBounds().width/2;
	int ch = sprite.getLocalBounds().height/2 + other->shape->getLocalBounds().height/2;

	float ox = abs( xAxis - cw );
	float oy = abs( yAxis - ch );

	sf::Vector2f dir = sf::Vector2f( thisCenterPos - otherCenterPos );
	
	float length = sqrt( ( dir.x * dir.x ) + ( dir.y * dir.y ) );
	sf::Vector2f normalizedDir = sf::Vector2f( dir.x / length, dir.y / length );

	sf::Vector2f moveTo;
	if( ox > oy )
		moveTo = sf::Vector2f(0, normalizedDir.y * oy );
	else if( ox < oy )
		moveTo = sf::Vector2f(normalizedDir.x * ox, 0 );
	else
		moveTo = sf::Vector2f(normalizedDir.x * ox, normalizedDir.y * oy );

	move( moveTo );
}