#ifndef _TILEBASE_H_
#define _TILEBASE_H_

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <functional>
#include <memory>

#include "..\..\..\Util\Defines.h"
#include "..\..\State\StateManager.h"
#include "..\Collidable.h"

class TileBase : public sf::Drawable {

public:
	TileBase();
	~TileBase();

	void setPosition( sf::Vector2f position );
	const sf::Vector2f* TileBase::getPosition() const;

	void setHealth( unsigned int health );
	unsigned int getHealth() const;

	void setCollision( bool hasCollision );
	bool hasCollision() const;

	virtual const sf::ConvexShape* getShape() { return nullptr; }

	virtual void update( StateManager* game, float deltaTime ) { }

	Collidable col;

protected:
	virtual void draw( sf::RenderTarget& target, sf::RenderStates states ) const;

private:
	// The world position of the block, measured from top left.
	sf::Vector2f pos;

	// The health of the block.
	unsigned int health;

	// If the block has collision.
	// If false, health doesn't matter.
	bool collision;

	// Texture is saved on the graphics card.
	// It shouldn't be manipulated. Manipulate via sprite if need be.
	sf::Texture texture;

	// Sprite made from the texture.
	sf::Sprite sprite;

	// The size rect of the tile.
	// Always 0, 0, TILE_SIZE, TILE_SIZE
	sf::IntRect rect;
};

template<typename T> TileBase* createT() { return new T; }

class TileFactory
{

public:
	typedef std::map<std::wstring, std::function<TileBase*()>> map_type;

	TileBase* createInstance( std::wstring const& type )	{
		map_type::iterator it = Instance()->getTypes()->find(type);
		if(it == Instance()->getTypes()->end())
			return 0;
		return it->second();
	}

	static TileFactory* Instance() {
		if (tileFactory == nullptr)
			tileFactory = new TileFactory();

		return tileFactory;
	}

	map_type* getTypes() {
		if( !types ) { types = new map_type; }
		return types;
	}

protected:
	TileFactory() { }

private:
	static TileFactory* tileFactory;
	
	map_type* types;
};

template<typename T>
struct DerivedRegister : TileFactory { 
    DerivedRegister(std::wstring const& s) { 
        TileFactory::Instance()->getTypes()->insert( std::make_pair( s, &createT<T> ) );
    }
};

#endif // _TILEBASE_H_