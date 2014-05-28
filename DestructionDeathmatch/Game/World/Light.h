#ifndef LIGHT_H
#define LIGHT_H

#define _USE_MATH_DEFINES

#include <memory>

#include "Collision.h"
#include "../../Util/Math.h"

class Light : public sf::Drawable {

public:
	Light( std::vector<Collidable*>* objects ) : objects(objects) { }

	void calculateLighting();

	void attach( sf::Transformable* parent );

	void setColor( sf::Color color );
	sf::Color getColor();

	void setPosition( sf::Vector2f pos );
	sf::Vector2f Light::getPosition();

	void setSize( int size );
	int getSize();

private:

	virtual void draw( sf::RenderTarget& target, sf::RenderStates states ) const;

	std::vector< Collidable* >* objects;

	sf::Transformable* parent;

	std::vector< sf::ConvexShape > shadows;

	sf::Vector2f position;
	int size;

	sf::Color color;
};
#endif // LIGHT_H