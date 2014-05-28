#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <SFML\Graphics.hpp>

#include "..\State\StateManager.h"
#include "Collidable.h"
#include "..\..\Util\Defines.h"
#include "..\..\Util\Math.h"

class Player : public sf::Transformable
{

public:
	Player();

	void update( StateManager* game, float deltaTime );
	void draw( StateManager* game );

	virtual void onCollision( Collidable* other );

	sf::Sprite sprite;
	sf::Sprite* weaponSprite;

	Collidable col;

	sf::ConvexShape flashlight;
	sf::CircleShape radiusmask;

private:
	int speed;

	sf::ConvexShape collider;
	sf::Texture texSprite;
};

#endif // _PLAYER_H_