#ifndef COLLISION_H
#define COLLISION_H

#include <SFML\Graphics.hpp>
#include <vector>

#include "Collidable.h"

class Collision {

public:

	/**
		* Determine if two lines, made from four points, collide with eachother.
		* If the two points collide, it returns the point where the collision occured.
		*
		* @param Line1Start	First point of the first line
		* @param Line1End		Second point of the first line
		* @param Line2Start	First point of the second line
		* @param Line2End		Second point of the second line
		*
		* @returns Vector2f point where the two lines collide.
		*/
	static sf::Vector2f* Collision::LineLine( const sf::Vector2f& Line1Start, const sf::Vector2f& Line1End, const sf::Vector2f& Line2Start, const sf::Vector2f& Line2End );

	// Didn't feel like making a manager. 
	static void Collision::CheckAllABB( const std::vector< Collidable* >* objects );
};
#endif // COLLISION_H