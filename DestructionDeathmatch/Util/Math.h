#ifndef MATH_H
#define MATH_H

#define _USE_MATH_DEFINES
#include <math.h>

#include <SFML/Graphics.hpp>

class Math {

public:
	static double Distance( sf::Vector2f point0, sf::Vector2f point1 );
	static double Angle( sf::Vector2f point0, sf::Vector2f point1 );
	static float Abs(sf::Vector2f point);
	static sf::Vector2f Rotate( const sf::Vector2f& vector, float angle );

};
#endif // MATH_H