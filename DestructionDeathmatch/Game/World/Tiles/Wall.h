#ifndef _WALL_H_
#define _WALL_H_

#include "TileBase.h"
#include "..\Collidable.h"

class Wall : public TileBase {

public:
	Wall();
	virtual ~Wall() {}

	virtual const sf::ConvexShape* getShape();

	void onCollision( Collidable* other );

	virtual void update( StateManager* game, float deltaTime );

private:
	virtual void draw( sf::RenderTarget& target, sf::RenderStates states ) const;

	static DerivedRegister<Wall> reg;

	sf::ConvexShape shape;

};

#endif // _WALL_H_