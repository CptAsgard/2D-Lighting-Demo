#include "Light.h"

bool canSee( sf::Vector2f cam, sf::Vector2f cpoint, sf::Shape const& shape ) {
	int ovcount = shape.getPointCount();

	for( unsigned int j = 0; j < ovcount; j++ ) {
		sf::Vector2f point1 = shape.getPosition() + shape.getPoint( j );
		sf::Vector2f point2 = shape.getPosition() + shape.getPoint( j + 1 == ovcount ? 0 : j + 1 );
		sf::Vector2f* ptr = Collision::LineLine( cam, cpoint, point1, point2 );

		if( ptr != nullptr && Math::Abs( *ptr - cpoint ) > 0.01 && Math::Distance( point1, *ptr ) > 0.01 && Math::Distance( point2, *ptr ) > 0.01 ) {
			delete ptr;
			return false;
		}

		delete ptr;
	}

	return true;
}

/**
 * Special credit to: Alex Brouwer
 *					  brom.4o3.nl
 *					  Thanks for fixing the special case crap :)
 */
void Light::calculateLighting() {
	if( this->parent != nullptr ) {
		setPosition( parent->getPosition() );
	}

	shadows.clear();

	for( Collidable* o : *objects ) {
		if( o->isPlayer ) // TODO: Temporary solution. Naive looping through every object isn't as efficient as a component-based system, but it'll do. 
			continue;

		double dist = Math::Distance( this->position, o->shape->getPosition() );
		if( dist > this->size )
			continue;

		// We'll use this later to create the shape with. First fill it with the seen vertices.
		std::vector<sf::Vector2f> shapebuffer;

		int ovcount = o->shape->getPointCount();

		// These will be used later to save the first and last seen point
		int firstseen = -1, lastseen = -1;

		// For each point in this shape
		for( int i = 0; i < ovcount; i++ ) {

			// Get the current point in world pos.
			// o.getPoint is in object space so we add the shape's world position to it.
			sf::Vector2f currentpoint = o->shape->getPoint(i) + o->shape->getPosition();

			// If we can see the current point from our position
			if( canSee( this->position, currentpoint, *o->shape ) ) {

				// This means something is hiding between 2 of our points. Special case warning!
				if( lastseen != -1 && lastseen != i - 1 ) {
					
					for( int j = lastseen + 1; j < i; j++ ) {
						// can we see a part of the line that's hiding? 0 -> 1
						bool tmptest = false;
						sf::Vector2f tmppoint1 = o->shape->getPoint(j) + o->shape->getPosition();
						sf::Vector2f tmppoint2 = o->shape->getPoint(j + 1) + o->shape->getPosition();
						sf::Vector2f targetpoint = o->shape->getPoint(j - 1) + o->shape->getPosition();

						double ang = Math::Angle( this->position, targetpoint );
						sf::Vector2f proj_pos = sf::Vector2f( cos(ang), sin(ang) ) * (float) this->size + this->position;

						sf::Vector2f* ptr = Collision::LineLine( this->position, proj_pos, tmppoint1, tmppoint2 );

						if( ptr != nullptr ) {
							// can we really see that location?
							if( canSee( this->position, *ptr, *o->shape ) ) {
								shapebuffer.push_back( *ptr );
								tmptest = true;
							}

							delete ptr;
						}

						// this is to do the same, but then the other direction. 0 <- 1
						if( !tmptest ) {
							sf::Vector2f tmppoint1 = o->shape->getPoint(j) + o->shape->getPosition();
							sf::Vector2f tmppoint2 = o->shape->getPoint(j - 1) + o->shape->getPosition();
							sf::Vector2f targetpoint = o->shape->getPoint(j + 1) + o->shape->getPosition();

							double ang = Math::Angle( this->position, targetpoint );
							sf::Vector2f proj_pos = sf::Vector2f( cos(ang), sin(ang) ) * (float) this->size + this->position;

							sf::Vector2f* ptr = Collision::LineLine( this->position, proj_pos, tmppoint1, tmppoint2 );

							if( ptr != nullptr ) {
								// can we really see that location?
								if( canSee( this->position, *ptr, *o->shape ) ) {
									shapebuffer.push_back( *ptr );
									tmptest = true;
								}

								delete ptr;
							}
						}
					}
				}

				if( firstseen == -1 )
					firstseen = i;

				lastseen = i;
				shapebuffer.push_back( currentpoint );

				// now we need to fix the "half visible lines" between the end and the beginning of the seen parts.
				if( firstseen != 0 && i == ovcount - 1 ) {
					for( int j = 0; j < firstseen; j++ ) {
						// can we see a part of the line that's hiding? 0 -> 1
						bool tmptest = false;
						sf::Vector2f tmppoint1 = o->shape->getPoint(j) + o->shape->getPosition();
						sf::Vector2f tmppoint2 = o->shape->getPoint(j + 1) + o->shape->getPosition();
						sf::Vector2f targetpoint = o->shape->getPoint(j == 0 ? i : j - 1) + o->shape->getPosition();

						double ang = Math::Angle( this->position, targetpoint );
						sf::Vector2f proj_pos = sf::Vector2f( cos(ang), sin(ang) ) * (float) this->size + this->position;

						sf::Vector2f* ptr = Collision::LineLine( this->position, proj_pos, tmppoint1, tmppoint2 );

						if( ptr != nullptr ) {
							// can we really see that location?
							if( canSee( this->position, *ptr, *o->shape ) ) {
								shapebuffer.push_back( *ptr );
								tmptest = true;
							}

							delete ptr;
						}

						// this is to do the same, but then the other direction. 0 <- 1
						if( !tmptest ) {
							sf::Vector2f tmppoint1 = o->shape->getPoint(j) + o->shape->getPosition();
							sf::Vector2f tmppoint2 = o->shape->getPoint(j == 0 ? i : j - 1) + o->shape->getPosition();
							sf::Vector2f targetpoint = o->shape->getPoint(j + 1) + o->shape->getPosition();

							double ang = Math::Angle( this->position, targetpoint );
							sf::Vector2f proj_pos = sf::Vector2f( cos(ang), sin(ang) ) * (float) this->size + this->position;

							sf::Vector2f* ptr = Collision::LineLine( this->position, proj_pos, tmppoint1, tmppoint2 );

							if( ptr != nullptr ){
								// can we really see that location?
								if( canSee( this->position, *ptr, *o->shape ) ) {
									shapebuffer.push_back( *ptr );
								}

								delete ptr;
							}
						}
					}
				}
			}
		}

		// now to case the "black" magic upon the shadow.
		sf::ConvexShape curshape;
		int vcount = shapebuffer.size();

		// we need to shape it properly by making the first element pp1, and the last element pp2.
		int shapestart = 0;
		int shapeend = 1;

		sf::Vector2f pp1 = shapebuffer[0];
		sf::Vector2f pp2 = shapebuffer[1];
		bool pp1_set = false;
		bool pp2_set = false; // exception: when 2 points are in an EXACT line from the light location, use the first one hit, that's hopefully the correct one...

		// if we can see more than 2 points it's going to be a hassle to find out what ones are the corners
		if( vcount > 2 ) {
			// this is gonna be a bit more difficult, however, in the end, there are always 2 projected lines, so, #yolo
			for( int i = 0; i < vcount; i++ ) {
				double ang = Math::Angle( this->position, shapebuffer[i] );
				sf::Vector2f proj_pos = sf::Vector2f( cos(ang), sin(ang) ) * (float) this->size + this->position;

				// if we can see this, it means that nothing is blocking, which means, this is ONE of the pp points.
				if( canSee( this->position, proj_pos, *o->shape ) ) {
					if( !pp1_set ) {
						pp1_set = true;
						pp1 = shapebuffer[i];
						shapestart = i;
					} else if( !pp2_set ) {
						// we need pp2_set for the exception.
						pp2 = shapebuffer[i];
						shapeend = i;
						pp2_set = true;
					}
				}
			}
		}

		// allocate the new buffer
		sf::Vector2f* newpoints = new sf::Vector2f[vcount + 2];
		int curindex= 1; // skip first and last, those will be projected from pp later on

		// 2 = start, 1 = end
		// pp1, 0, 1, 2, pp2
		// pp1, 2, 0, 1, pp2

		// This happes, so we should write a special case for it.
		// fill buffers
		if( shapeend < shapestart ) {
			for( int i = shapestart; i < vcount; i++ )
				newpoints[curindex++] = shapebuffer[i];

			for( int i = shapeend + 1; i < shapestart; i++ )
				newpoints[curindex++] = shapebuffer[i];

			for( int i = 0; i <= shapeend; i++ )
				newpoints[curindex++] = shapebuffer[i];
		} else {
			for( int i = shapestart; i < shapeend; i++ )
				newpoints[curindex++] = shapebuffer[i];

			for( int i = shapestart - 1; i >= 0; i-- )
				newpoints[curindex++] = shapebuffer[i];

			for( int i = vcount - 1; i >= shapeend; i-- )
				newpoints[curindex++] = shapebuffer[i];
		}

		// get angles to first and last seen point, 0 and real-last index are 0,0 reserved for next stage.
		double firstangle = Math::Angle( this->position, pp1 );
		double secondangle = Math::Angle( this->position, pp2 );

		// create projected locations so that we can create a propper shadow object
		newpoints[0] = sf::Vector2f( cos( firstangle ), sin( firstangle ) ) * (float) this->size + this->position;
		newpoints[vcount + 1] = sf::Vector2f( cos( secondangle ), sin( secondangle ) ) * (float) this->size + this->position;

		// fill the shape
		curshape.setPointCount( vcount + 3 );
		for( int i = 0; i < vcount + 2; i++ ) 
			curshape.setPoint(i, newpoints[i]);

		// to make sure the shadow has a projection away from the light source
		// calc the middle point, and push that away from us
		sf::Vector2f pp_between = newpoints[0] + newpoints[vcount + 1];
		pp_between.x /= 2;
		pp_between.y /= 2;	// double frowny face for SFML :(

		// push, and insert it into the shape
		double between_ang = Math::Angle( this->position, pp_between );
		curshape.setPoint( vcount + 2, sf::Vector2f( cos( between_ang ), sin( between_ang ) ) * (float) this->size + this->position );

		curshape.setFillColor( sf::Color::Black );
		shadows.push_back( curshape );

		delete[] newpoints;
	}
}

void Light::draw( sf::RenderTarget& target, sf::RenderStates states ) const {
	for( sf::ConvexShape const& shape : shadows ) {
		target.draw( shape, states );

#ifdef DD_DEBUG
		for( int i = 0; i < shape.getPointCount(); i++ ) {
			sf::Vertex line[] = {
				sf::Vertex( shape.getPoint(i) ),
				sf::Vertex( shape.getPoint(i == shape.getPointCount() - 1 ? 0 : i + 1) )
			};

			line[0].color = sf::Color::Red;
			line[1].color = sf::Color::Red;
			target.draw( line, 2, sf::Lines );
		}
#endif

	}
}

void Light::attach( sf::Transformable* parent ) {
	this->parent = parent;
}

void Light::setColor( sf::Color color ) {
	this->color = color;
}

sf::Color Light::getColor() {
	return this->color;
}

void Light::setPosition( sf::Vector2f pos ) {
	this->position = pos;
}

sf::Vector2f Light::getPosition() {
	return this->position;
}

void Light::setSize( int size ) {
	this->size = size;
}

int Light::getSize() {
	return this->size;
}