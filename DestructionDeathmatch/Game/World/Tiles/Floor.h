#ifndef _FLOOR_H_
#define _FLOOR_H_

#include "TileBase.h"

class Floor : public TileBase {

public:
	Floor();
	virtual ~Floor() { ; }

	virtual void update( StateManager* game, float deltaTime ) { sprite.setPosition(*(getPosition())); }

private:
	virtual void draw( sf::RenderTarget& target, sf::RenderStates states ) const;

	static DerivedRegister<Floor> reg;

	sf::Texture tex;
	sf::Sprite sprite;
};

#endif // _FLOOR_H_