#include "Collision.h"

sf::Vector2f* Collision::LineLine( const sf::Vector2f& Line1Start, const sf::Vector2f& Line1End, const sf::Vector2f& Line2Start, const sf::Vector2f& Line2End ) {
	float ua = ( Line2End.x - Line2Start.x ) * ( Line1Start.y - Line2Start.y ) - ( Line2End.y - Line2Start.y ) * ( Line1Start.x - Line2Start.x );
	float ub = ( Line1End.x - Line1Start.x ) * ( Line1Start.y - Line2Start.y ) - ( Line1End.y - Line1Start.y ) * ( Line1Start.x - Line2Start.x );
	float denominator = ( Line2End.y - Line2Start.y ) * ( Line1End.x - Line1Start.x ) - ( Line2End.x - Line2Start.x ) * ( Line1End.y - Line1Start.y );
	
	if( abs( denominator ) > 0.00001f ) {
		ua /= denominator;
		ub /= denominator;

		if( ua >= 0.00001f && ua <= 1 && ub >= 0.00001f && ub <= 1 )
			return new sf::Vector2f( Line1Start.x + ua * ( Line1End.x - Line1Start.x ), Line1Start.y + ua * ( Line1End.y - Line1Start.y ) );
	}

	return nullptr;
}

void Collision::CheckAllABB( const std::vector< Collidable* >* objects ) {

	for( Collidable* o1 : *objects ) {
		for( Collidable* o2 : *objects ) {
			if( o1 == o2 )
				continue;

			// If true, collision!
			if( o1->shape->getGlobalBounds().intersects( o2->shape->getGlobalBounds() ) ) {
				o1->onCollision( o2 );
				o2->onCollision( o1 );
			}
		}
	}
}