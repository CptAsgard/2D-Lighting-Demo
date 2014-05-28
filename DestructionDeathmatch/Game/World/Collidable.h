#ifndef _COLLIDABLE_H_
#define _COLLIDABLE_H_

#include <SFML/Graphics.hpp>
#include <functional>

class Collidable
{

public:
	Collidable() { }

	Collidable( sf::Shape* shape, std::function<void( Collidable* )> callback ) : shape(shape), callback(callback) { ; }

	void onCollision( Collidable* other ) { callback( other ); }

	void registerCallback( std::function<void( Collidable* )> callback );

	// imagine getter/setter here
	const sf::Shape* shape;

	bool isPlayer;
	bool isProjectile;

private:
	std::function<void( Collidable* )> callback;

};

#endif // _COLLIDABLE_H_