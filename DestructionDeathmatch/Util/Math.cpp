#include "Math.h"

double Math::Distance( sf::Vector2f point0, sf::Vector2f point1 ) {
	return sqrt( ( ( point0.x - point1.x ) * ( point0.x - point1.x ) ) + ( ( point0.y - point1.y ) * ( point0.y - point1.y ) ) );
}

double Math::Angle( sf::Vector2f point0, sf::Vector2f point1 ) {
	return -atan2( point0.x - point1.x, point0.y - point1.y ) - M_PI / 2;
}

float Math::Abs( sf::Vector2f point ) {
	if (point.x < 0) point.x *= -1;
	if (point.y < 0) point.y *= -1;

	return point.x > point.y ? point.x : point.y;
}

sf::Vector2f Math::Rotate( const sf::Vector2f& vector, float angle ) {
	//float theta = angle * (M_PI/180); // to rad

	float cs = cos( angle );
	float sn = sin( angle );

	float px = vector.x * cs - vector.y * sn;
	float py = vector.x * sn + vector.y * cs;

	return sf::Vector2f( px, py );
}