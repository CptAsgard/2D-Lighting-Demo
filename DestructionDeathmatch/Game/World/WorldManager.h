#ifndef _WORLDMANAGER_H_
#define _WORLDMANAGER_H_

#include <fstream>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include "..\..\Util\Defines.h"

#include "..\State\StateManager.h"
#include "..\World\Light.h"

#include "Tiles\TileBase.h"
#include "Tiles\Floor.h"
#include "Tiles\Wall.h"

#include "Player.h"

#include "..\..\Util\JSON.h"

class Player;

enum TileType {

	EMPTY,

	FLOOR,

	WALL
};

class WorldManager
{

public:
	WorldManager();
	~WorldManager();

	void init( StateManager* game );

	void update( StateManager* game, float deltaTime );
	void draw( StateManager* game );

	void createTile( std::wstring const& type, sf::Vector2f position );
	void loadMap( const char* map );

	void createPlayer( sf::Vector2f position, bool isSelf );

	std::vector<TileBase*> tiles;
	std::vector< Light* > lights;
	std::vector< Player* > players;

	Player* self;

	// Contains a ptr to all the objects that should be rendered by lights.
	std::vector< Collidable* > objects;

private:
	sf::RenderTexture target;

};

#endif // _WORLDMANAGER_H_