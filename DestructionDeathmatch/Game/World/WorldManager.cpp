#include "WorldManager.h"

WorldManager::WorldManager() {

}

void WorldManager::init( StateManager* game ) {
	std::ifstream file( "../Content/map1.json" );
	std::string contents((std::istreambuf_iterator<char>(file)), 
		std::istreambuf_iterator<char>());

	loadMap( contents.c_str() );

	createPlayer( sf::Vector2f( 800, 800 ), true );

	target.create( game->window->getSize().x, game->window->getSize().y );
	target.clear( sf::Color::Transparent );
	target.display();
}

WorldManager::~WorldManager() {
	for( Light* &it : lights ) 
		delete it;

	lights.clear();
}

void WorldManager::update( StateManager* game, float deltaTime ) {
	for( Light* const light : lights ) {
		light->calculateLighting();
	}

	for( TileBase* const tile : tiles ) {
		tile->update( game, deltaTime );
	}

	for( Player* const player : players ) {
		player->update( game, deltaTime );
	}

	Collision::CheckAllABB( &objects );
}

void WorldManager::draw( StateManager* game ) {

	target.clear();

	/* First we draw the world*/
	for( TileBase* const tile : tiles ) {
		game->window->draw( *tile );
	}

	for( Player* const player : players ) {
		player->draw( game );
	}

	/* Then we draw all the flashlights onto a black filled render texture as a mask */
	for( Player* const player : players ) {
		target.draw( player->flashlight, sf::BlendNone );
	}

	target.draw( self->radiusmask, sf::BlendNone );

	game->window->draw( sf::Sprite( target.getTexture() ) );

	/* Then we draw the shadows to block out other people's flashlights if they're outside LOS */
	for( Light* const light : lights ) {
		game->window->draw( *light );
	}
}

void WorldManager::createTile( std::wstring const& type, sf::Vector2f position ) {
	TileBase* tile;

	tile = TileFactory::Instance()->createInstance( type );

	tile->setPosition(position);
	tile->col.isPlayer = false;
	tile->col.isProjectile = false;

	tiles.push_back( tile );

	// For light rendering.
	// Only render shadows for objects that have collision.
	if( tile->hasCollision() ) {
		objects.push_back( &tile->col );
	}
}

void WorldManager::createPlayer( sf::Vector2f position, bool isSelf ) {
	Player* p = new Player();
	p->setPosition( position );

	players.push_back( p );
	objects.push_back( &p->col );

	if( isSelf ) {
		Light* newLight = new Light(&objects);
		newLight->setPosition( sf::Vector2f( 100, 100 ) );
		newLight->setSize( 2000 );
		newLight->setColor( sf::Color::Blue );

		lights.push_back( newLight );

		newLight->attach( p );

		p->col.isPlayer = true;
		p->col.isProjectile = false;

		self = p;
	}
}

void WorldManager::loadMap( const char* map ) {
	JSONValue* value = JSON::Parse( map );

	if( value == NULL ) {
		printf( "something went wrong loading map file" );
		return;
	}

	JSONObject root = value->AsObject();

	const JSONArray f	= root[L"tiles"]->AsArray();
	for( int i = 0; i < f.size(); i++ ) {
		const JSONArray a	= f[i]->AsArray();

		for( int j = 0; j < a.size(); j++ ) {
			createTile( a[j]->AsString(), sf::Vector2f( j * TILE_SIZE, i * TILE_SIZE ) );
		}
	}
}