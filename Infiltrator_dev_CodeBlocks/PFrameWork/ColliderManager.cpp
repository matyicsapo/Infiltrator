#include "ColliderManager.hpp"

#include <cmath>

#include "GO/Colliders.hpp"
#include "Utilities.hpp"

//#include <iostream>

ColliderManager* ColliderManager::instance = 0;

ColliderManager* ColliderManager::Instance () {
	if (instance == 0)
		instance = new ColliderManager();

	return instance;
}

void ColliderManager::Release () {
	delete instance;
	instance = 0;
}

ColliderManager::~ColliderManager () {
	for (std::list<ColliderBase*>::iterator itC = mColliders.begin();
			itC != mColliders.end(); itC++) {
		delete *itC--;
	}
}

void ColliderManager::Add (ColliderBase* c) {
	mColliders.push_back(c);
}

void ColliderManager::Pop (ColliderBase* c) {
	mColliders.remove(c);
}

void ColliderManager::ClampMotion (ColliderBase* c, sf::Vector2f& motion, std::set<ColliderBase*>* exceptions, int collisionType) {
	for (std::list<ColliderBase*>::iterator itC = mColliders.begin();
			itC != mColliders.end(); itC++) {

		if ((((*itC)->collisionType & collisionType) == 0)
				||
			(exceptions != 0 && exceptions->count(*itC) != 0)
				||
			(*itC == c))
		{
			continue;
		}

		if (c->isCircle && (*itC)->isCircle) {
			// CIRCLE - CIRCLE collision
			CircleCollider* mCC = dynamic_cast<CircleCollider*>(c);
			CircleCollider* xCC = dynamic_cast<CircleCollider*>(*itC);

			if ( Utils->Dst(mCC->GetPosition() + motion, xCC->GetPosition()) < (mCC->radius + xCC->radius) ) {
//				Utils->SetLength(motion, Utils->Length(motion) -
//												std::abs(Utils->Dst(mCC->GetPosition() + motion, xCC->GetPosition())
//													- (mCC->radius + xCC->radius)));

				sf::Vector2f test1 (motion);
				Utils->SetLength(test1, Utils->Length(test1) -
												std::abs(Utils->Dst(mCC->GetPosition() + test1, xCC->GetPosition())
													- (mCC->radius + xCC->radius)));
				float testLength1 = Utils->Length(test1);
				bool testCollision1 = false;
				if ( Utils->Dst(mCC->GetPosition() + test1, xCC->GetPosition()) < (mCC->radius + xCC->radius) ) {
					testCollision1 = true;
				}


				sf::Vector2f test2 (motion);
				Utils->SetLength(test2, //Utils->Length(test2) -
												std::abs(Utils->Dst(mCC->GetPosition() + test2, xCC->GetPosition())
													- (mCC->radius + xCC->radius)));
				float testLength2 = Utils->Length(test2);
				bool testCollision2 = false;
				if ( Utils->Dst(mCC->GetPosition() + test2, xCC->GetPosition()) < (mCC->radius + xCC->radius) ) {
					testCollision2 = true;
				}

				if (testCollision1 || testCollision2) {
					if (testCollision1 && testCollision2) {
						Utils->SetLength(motion, testLength1 < testLength2 ? testLength1 / 2 : testLength2 / 2);
					}
					else {
						if (testCollision1) {
							Utils->SetLength(motion, testLength2);
						}
						else {
							Utils->SetLength(motion, testLength1);
						}
					}
				}
				else {
					Utils->SetLength(motion, testLength1 < testLength2 ? testLength1 : testLength2);
				}

			}
		}
		else if (c->isCircle || (*itC)->isCircle) {
			if (c->isCircle) {
				// CIRCLE - RECT
				CircleCollider* mCC = dynamic_cast<CircleCollider*>(c);
				RectCollider* xRC = dynamic_cast<RectCollider*>(*itC);

				sf::Vector2f xRCMin(xRC->GetPosition() - (xRC->size / 2.0f));
				sf::Vector2f xRCMax(xRC->GetPosition() + (xRC->size / 2.0f));

				float d = 0; // >= 0

				if ((mCC->GetPosition().x + mCC->radius + motion.x) < xRCMin.x) {
					d += std::pow((mCC->GetPosition().x + motion.x) - xRCMin.x, 2);
				}
				else
				if ((mCC->GetPosition().x - mCC->radius + motion.x) > xRCMax.x) {
					d += std::pow((mCC->GetPosition().x + motion.x) - xRCMax.x, 2);
				}

				if ((mCC->GetPosition().y + mCC->radius + motion.y) < xRCMin.y) {
					d += std::pow((mCC->GetPosition().y + motion.y) - xRCMin.y, 2);
				}
				else
				if ((mCC->GetPosition().y - mCC->radius + motion.y) > xRCMax.y) {
					d += std::pow((mCC->GetPosition().y + motion.y) - xRCMax.y, 2);
				}

				if (d <= std::pow(mCC->radius, 2)) {
					sf::Vector2f p(mCC->GetPosition());

					if (p.x + mCC->radius <= xRCMin.x) {
						// left

						if (p.y + mCC->radius >= xRCMin.y) {
							if (p.y - mCC->radius <= xRCMax.y) {
								// left side
								motion.x = std::abs((p.x + mCC->radius) - xRCMin.x) * Utils->Sign(motion.x);
							}
							else {
								// bottom-left corner
								motion.x = std::abs((p.x + mCC->radius) - xRCMin.x) * Utils->Sign(motion.x);
								motion.y = std::abs((p.y - mCC->radius) - xRCMax.y) * Utils->Sign(motion.y);
							}
						}
						else {
							// top-left corner
							motion.x = std::abs((p.x + mCC->radius) - xRCMin.x) * Utils->Sign(motion.x);
							motion.y = std::abs((p.y + mCC->radius) - xRCMin.y) * Utils->Sign(motion.y);
						}
					}
					else
					if (p.x - mCC->radius >= xRCMax.x) {
						// right

						if (p.y + mCC->radius >= xRCMin.y) {
							if (p.y - mCC->radius <= xRCMax.y) {
								// right side
								motion.x = std::abs((p.x - mCC->radius) - xRCMax.x) * Utils->Sign(motion.x);
							}
							else {
								// bottom-right corner
								motion.x = std::abs((p.x - mCC->radius) - xRCMax.x) * Utils->Sign(motion.x);
								motion.y = std::abs((p.y - mCC->radius) - xRCMax.y) * Utils->Sign(motion.y);
							}
						}
						else {
							// top-right corner
							motion.x = std::abs((p.x - mCC->radius) - xRCMax.x) * Utils->Sign(motion.x);
							motion.y = std::abs((p.y + mCC->radius) - xRCMin.y) * Utils->Sign(motion.y);
						}
					}
					else {
						if (p.y <= xRCMin.y) {
							// top edge
							motion.y = std::abs((p.y + mCC->radius) - xRCMin.y) * Utils->Sign(motion.y);
						}
						else {
							// bottom edge
							motion.y = std::abs((p.y - mCC->radius) - xRCMax.y) * Utils->Sign(motion.y);
						}
					}
				}
			}
		}
	}
}

void ColliderManager::Move (ColliderBase* c, sf::Vector2f& motion, std::set<ColliderBase*>* exceptions, int collisionType) {
	for (std::list<ColliderBase*>::iterator itC = mColliders.begin();
			itC != mColliders.end(); itC++) {

		if ((((*itC)->collisionType & collisionType) == 0)
				||
			(exceptions != 0 && exceptions->count(*itC) != 0)
				||
			(*itC == c))
		{
			continue;
		}

		if (c->isCircle && (*itC)->isCircle) {
			// CIRCLE - CIRCLE collision
			CircleCollider* mCC = dynamic_cast<CircleCollider*>(c);
			CircleCollider* xCC = dynamic_cast<CircleCollider*>(*itC);

			if ( Utils->Dst(mCC->GetPosition() + motion, xCC->GetPosition()) < (mCC->radius + xCC->radius) ) {
				mCC->OnIntersect(xCC);
				xCC->OnIntersect(mCC);

				if ((mCC->collisionType & Collision::TRIGGER) || xCC->collisionType & Collision::TRIGGER)
					break;

				float maxDst = Utils->Length(motion)
								+ (Utils->Dst(mCC->GetPosition() + motion, xCC->GetPosition())
									- (mCC->radius + xCC->radius));

				sf::Vector2f motionLeft = motion;

				Utils->ClampLength(motion, maxDst);

				motionLeft -= motion;

				if (motionLeft.x != 0 || motionLeft.y != 0) {

					sf::Vector2f nvToOther(Utils->Normalized(xCC->GetPosition() - (mCC->GetPosition() + motion)));

					// the normal to use is the one with the lower angle to the motionLeft vector
					sf::Vector2f n;
					n = sf::Vector2f(nvToOther.y, -nvToOther.x);
					float a1 = Utils->AngleBetweenInRadians(Utils->Normalized(motionLeft), n);
					n = sf::Vector2f(-nvToOther.y, nvToOther.x);
					float a2 = Utils->AngleBetweenInRadians(Utils->Normalized(motionLeft), n);
					if (a1 < a2) {
						n = sf::Vector2f(nvToOther.y, -nvToOther.x);
					}

					motion += n * Utils->Length(motionLeft);

					mCC->Move(motion);
					std::set<ColliderBase*>* xExceptions = new std::set<ColliderBase*>();
					xExceptions->insert(*itC);
					if (IsColliding(mCC, xExceptions)) {
						mCC->Move(-motion);
						motion -= n * Utils->Length(motionLeft);
					}
					else {
						mCC->Move(-motion);
					}
					delete xExceptions;
				}
			}
		}
		else if (c->isCircle || (*itC)->isCircle) {
			if (c->isCircle) {
				// CIRCLE - RECT
				CircleCollider* mCC = dynamic_cast<CircleCollider*>(c);
				RectCollider* xRC = dynamic_cast<RectCollider*>(*itC);

				sf::Vector2f xRCMin(xRC->GetPosition() - (xRC->size / 2.0f));
				sf::Vector2f xRCMax(xRC->GetPosition() + (xRC->size / 2.0f));

				float d = 0; // >= 0

				if ((mCC->GetPosition().x + mCC->radius + motion.x) < xRCMin.x) {
					d += std::pow((mCC->GetPosition().x + motion.x) - xRCMin.x, 2);
				}
				else
				if ((mCC->GetPosition().x - mCC->radius + motion.x) > xRCMax.x) {
					d += std::pow((mCC->GetPosition().x + motion.x) - xRCMax.x, 2);
				}

				if ((mCC->GetPosition().y + mCC->radius + motion.y) < xRCMin.y) {
					d += std::pow((mCC->GetPosition().y + motion.y) - xRCMin.y, 2);
				}
				else
				if ((mCC->GetPosition().y - mCC->radius + motion.y) > xRCMax.y) {
					d += std::pow((mCC->GetPosition().y + motion.y) - xRCMax.y, 2);
				}

				if (d <= std::pow(mCC->radius, 2)) {
					mCC->OnIntersect(xRC);
					xRC->OnIntersect(mCC);

					if ((mCC->collisionType & Collision::TRIGGER) || xRC->collisionType & Collision::TRIGGER)
						break;


					sf::Vector2f p(mCC->GetPosition());

					sf::Vector2f n;

// slide dir stuff ////////////////////////////////////////////////////////////
					if (p.x + mCC->radius <= xRCMin.x) {
						// left

						if (p.y + mCC->radius >= xRCMin.y) {
							if (p.y - mCC->radius <= xRCMax.y) {
								// left side
								n = sf::Vector2f(0, Utils->Sign(motion.y, false));
							}
							else {
								// bottom-left corner
								if (std::abs(motion.x) != std::abs(motion.y)) {
									n = std::abs(motion.x) < std::abs(motion.y) ?
										sf::Vector2f(Utils->Sign(motion.y, false), 0)
										:
										sf::Vector2f(0, Utils->Sign(motion.y, false));
								}
							}
						}
						else {
							// top-left corner
							if (std::abs(motion.x) != std::abs(motion.y)) {
								n = std::abs(motion.x) < std::abs(motion.y) ?
									sf::Vector2f(Utils->Sign(motion.y, false), 0)
									:
									sf::Vector2f(0, Utils->Sign(motion.y, false));
							}
						}
					}
					else
					if (p.x - mCC->radius >= xRCMax.x) {
						// right

						if (p.y + mCC->radius >= xRCMin.y) {
							if (p.y - mCC->radius <= xRCMax.y) {
								// right side
								n = sf::Vector2f(0, Utils->Sign(motion.y, false));
							}
							else {
								// bottom-right corner
								if (std::abs(motion.x) != std::abs(motion.y)) {
									n = std::abs(motion.x) < std::abs(motion.y) ?
										sf::Vector2f(Utils->Sign(motion.y, false), 0)
										:
										sf::Vector2f(0, Utils->Sign(motion.y, false));
								}
							}
						}
						else {
							// top-right corner
							if (std::abs(motion.x) != std::abs(motion.y)) {
								n = std::abs(motion.x) < std::abs(motion.y) ?
									sf::Vector2f(Utils->Sign(motion.y, false), 0)
									:
									sf::Vector2f(0, Utils->Sign(motion.y, false));
							}
						}
					}
					else {
						if (p.y < xRCMin.y) {
							// top edge
							n = sf::Vector2f(Utils->Sign(motion.x, false), 0);
						}
						else {
							// bottom edge
							n = sf::Vector2f(Utils->Sign(motion.x, false), 0);
						}
					}
// slide dir stuff ////////////////////////////////////////////////////////////

					motion = n * Utils->Length(motion);

					mCC->Move(motion);
					std::set<ColliderBase*>* xExceptions = new std::set<ColliderBase*>();
					xExceptions->insert(*itC);
					if (IsColliding(mCC, xExceptions)) {
						mCC->Move(-motion);
						motion = sf::Vector2f(0, 0);
					}
					else {
						mCC->Move(-motion);
					}
					delete xExceptions;
				}
			}
			else {
				// RECT - CIRCLE
//std::cout << "WARNING: ***unimplemented*** " << __FILE__ << " " << __LINE__ << std::endl;
			}
		}
		else {
			// RECT - RECT collision
//std::cout << "WARNING: ***unimplemented*** " << __FILE__ << " " << __LINE__ << std::endl;
		}
	}

	c->Move(motion);
}

bool ColliderManager::IsColliding (ColliderBase* c, std::set<ColliderBase*>* exceptions, int collisionType) {
	for (std::list<ColliderBase*>::iterator itC = mColliders.begin();
			itC != mColliders.end(); itC++) {

		if ((((*itC)->collisionType & collisionType) == 0)
				||
			(exceptions != 0 && exceptions->count(*itC) != 0)
				||
			(*itC == c))
		{
			continue;
		}

		if (IsColliding(c, *itC))
			return true;
	}

	return false;
}

bool ColliderManager::IsColliding (sf::Vector2f A[], unsigned int nbPointsA,
									std::set<ColliderBase*>* exceptions, int collisionType) {
	for (std::list<ColliderBase*>::iterator itC = mColliders.begin();
			itC != mColliders.end(); itC++) {
		if ((((*itC)->collisionType & collisionType) == 0)
				||
			(exceptions != 0 && exceptions->count(*itC) != 0))
		{
			continue;
		}

		if (IsColliding(A, nbPointsA, *itC))
			return true;
	}

	return false;
}

bool ColliderManager::IsColliding (ColliderBase* c1, ColliderBase* c2) {
	if (c1->isCircle && c2->isCircle) {
		// CIRCLE - CIRCLE collision
		CircleCollider* cc1 = dynamic_cast<CircleCollider*>(c1);
		CircleCollider* cc2 = dynamic_cast<CircleCollider*>(c2);

		if ( Utils->Dst(cc1->GetPosition(), cc2->GetPosition())
					<
				(cc1->radius + cc2->radius) ) {
			return true;
		}
	}
	else if (c1->isCircle || c2->isCircle) {
		CircleCollider* cc;
		RectCollider* rc;

		if (c1->isCircle) {
			// CIRCLE - RECT
			cc = dynamic_cast<CircleCollider*>(c1);
			rc = dynamic_cast<RectCollider*>(c2);
		}
		else {
			// RECT - CIRCLE
			cc = dynamic_cast<CircleCollider*>(c2);
			rc = dynamic_cast<RectCollider*>(c1);
		}

		sf::Vector2f rcMin(rc->GetPosition() - (rc->size / 2.0f));
		sf::Vector2f rcMax(rc->GetPosition() + (rc->size / 2.0f));

		float d = 0;

		if ((cc->GetPosition().x + cc->radius) < rcMin.x) {
			d += std::pow((cc->GetPosition().x) - rcMin.x, 2);
		}
		else
		if ((cc->GetPosition().x - cc->radius) > rcMax.x) {
			d += std::pow((cc->GetPosition().x) - rcMax.x, 2);
		}

		if ((cc->GetPosition().y + cc->radius) < rcMin.y) {
			d += std::pow((cc->GetPosition().y) - rcMin.y, 2);
		}
		else
		if ((cc->GetPosition().y - cc->radius) > rcMax.y) {
			d += std::pow((cc->GetPosition().y) - rcMax.y, 2);
		}

		if (d <= std::pow(cc->radius, 2)) {
			return true;
		}
	}
	else {
		// RECT - RECT
		RectCollider* rc1 = dynamic_cast<RectCollider*>(c1);
		RectCollider* rc2 = dynamic_cast<RectCollider*>(c2);

		sf::Vector2f t = rc2->GetPosition() - rc1->GetPosition();

		if ( (std::abs(t.x) <= (rc1->size.x / 2.0f + rc2->size.x / 2.0f))
				&& (std::abs(t.y) <= (rc1->size.y / 2.0f + rc2->size.y / 2.0f)) )
			return true;
	}

	return false;
}

bool ColliderManager::IsColliding (sf::Vector2f A[], unsigned int nbPointsA,
									sf::Vector2f B[], unsigned int nbPointsB) {

	// Possible separating axis
	sf::Vector2f sa;

	// test against A's Possible separating axes
	if (nbPointsA > 2) {
		for (unsigned int i = 0; i != nbPointsA - 1; ++i) {
			// a Possible separating axis is the vector orthogonal to an edge of an object
			sa = Utils->Normal( A[i + 1] - A[i] );

			if (!DoProjectionsOverlap(A, nbPointsA, B, nbPointsB, sa))
				return false;
		}
		// an additional check on the edge between the first and last vertices
		sa = Utils->Normal( A[nbPointsA - 1] - A[0] );
		if (!DoProjectionsOverlap(A, nbPointsA, B, nbPointsB, sa))
			return false;
	}
	else if (nbPointsA == 2) {
		// the separating axis is the line
		sa = A[nbPointsA - 1] - A[0];
		if (!DoProjectionsOverlap(A, nbPointsA, B, nbPointsB, sa))
			return false;
	}

	// test against B's Possible separating axes
	if (nbPointsB > 2) {
		for (unsigned int i = 0; i != nbPointsB - 1; ++i) {
			sa = Utils->Normal( B[i + 1] - B[i] );

			if (!DoProjectionsOverlap(A, nbPointsA, B, nbPointsB, sa))
				return false;
		}
		sa = Utils->Normal( B[nbPointsB - 1] - B[0] );
		if (!DoProjectionsOverlap(A, nbPointsA, B, nbPointsB, sa))
			return false;
	}
	else if (nbPointsB == 2) {
		// the separating axis is the line
		sa = B[nbPointsB - 1] - B[0];
		if (!DoProjectionsOverlap(A, nbPointsA, B, nbPointsB, sa))
			return false;
	}

	// if none of the separating axis tests reported that there was no collision then there was
	return true;
}

bool ColliderManager::IsColliding (sf::Vector2f A[], unsigned int nbPointsA, ColliderBase* B) {
//	if (B->colliderOffset.x != 0 && B->colliderOffset.y != 0)
//std::cout << "WARNING: colliderOffset is not taken into account " << __FILE__ << " " << __LINE__ << std::endl;

	if (B->isCircle) {
		// Possible separating axis
		sf::Vector2f sa;

		sf::Vector2f centerA(0, 0);

		// test against A's Possible separating axes
		if (nbPointsA > 2) {
			for (unsigned int i = 0; i != nbPointsA - 1; ++i) {
				centerA += A[i]; // "pre-"calculating for later

				// a Possible separating axis is the vector orthogonal to an edge of an object
				sa = Utils->Normal( A[i + 1] - A[i] );

				if (!DoProjectionsOverlap(A, nbPointsA, B, sa))
					return false;
			}
			// an additional check on the edge between the first and last vertices
			sa = Utils->Normal( A[nbPointsA - 1] - A[0] );
			if (!DoProjectionsOverlap(A, nbPointsA, B, sa))
				return false;
		}
		else if (nbPointsA == 2) {
			// the separating axis is the line
			sa = A[nbPointsA - 1] - A[0];
			if (!DoProjectionsOverlap(A, nbPointsA, B, sa))
				return false;
		}

		// additionaly for circles against convex objects the line parallel to that connecting the 2 objects'
		// centers' has to be checked as a Possible separating axis
		centerA /= (float)nbPointsA;

		sa = sf::Vector2f(B->GetPosition() - centerA);
		if (!DoProjectionsOverlap(A, nbPointsA, B, sa))
			return false;

		return true;
	}
	else {
		RectCollider* R = dynamic_cast<RectCollider*>(B);

		sf::Vector2f pointsB[4] { sf::Vector2f(R->GetPosition() - R->size / 2.0f),
							sf::Vector2f(R->GetPosition() + R->size / 2.0f),
							sf::Vector2f(R->GetPosition() + sf::Vector2f(R->size.x / 2.0f, -R->size.y / 2.0f)),
							sf::Vector2f(R->GetPosition() + sf::Vector2f(-R->size.x / 2.0f, R->size.y / 2.0f)) };
		return IsColliding(A, nbPointsA, pointsB, 4);
	}
}

void ColliderManager::ProjectionRect (sf::Vector2f A[], unsigned int nbPointsA,
										sf::Vector2f projectTo,
										sf::Vector2f& max, sf::Vector2f& min) {
	// projecting all vertices of A onto the Possible separating axis
	// doing the projection of the 0th vertex outside the loop to initialize the 'min' and 'max' values below
	sf::Vector2f projection(Utils->Project(A[0], projectTo));

	// the minimum and maximum coordinates of a virtual axis aligned rectangle
	min = sf::Vector2f(projection);
	max = sf::Vector2f(projection);

	unsigned int j = 1;
	for (; j != nbPointsA; ++j) {
		projection = Utils->Project(A[j], projectTo);

		// straighforward updating of the minimum and maximum values
		if (min.x > projection.x) {
			min.x = projection.x;
		}
		if (min.y > projection.y) {
			min.y = projection.y;
		}

		if (max.x < projection.x) {
			max.x = projection.x;
		}
		if (max.y < projection.y) {
			max.y = projection.y;
		}
	}
}

bool ColliderManager::DoProjectionsOverlap (sf::Vector2f A[], unsigned int nbPointsA,
											sf::Vector2f B[], unsigned int nbPointsB,
											sf::Vector2f projectTo) {

	sf::Vector2f minA;
	sf::Vector2f maxA;
	ProjectionRect(A, nbPointsA, projectTo, minA, maxA);

	sf::Vector2f minB;
	sf::Vector2f maxB;
	ProjectionRect(B, nbPointsB, projectTo, minB, maxB);

	// check to see if the projections intersect
	sf::Vector2f centerA( (minA.x + maxA.x) / 2, (minA.y + maxA.y) / 2 );
	sf::Vector2f centerB( (minB.x + maxB.x) / 2, (minB.y + maxB.y) / 2 );

	// the distance vector of the 2 virtual axis aligned rectangles
	sf::Vector2f t(centerB - centerA);

	sf::Vector2f halfSizeA(std::abs(maxA.x - minA.x) / 2, std::abs(maxA.y - minA.y) / 2);
	sf::Vector2f halfSizeB(std::abs(maxB.x - minB.x) / 2, std::abs(maxB.y - minB.y) / 2);

	// the projections overlap if the virtual axis aligned bounding boxes overlap
	if ( (std::abs(t.x) <= halfSizeA.x + halfSizeB.x) && (std::abs(t.y) <= halfSizeA.y + halfSizeB.y) )
		return true;

	return false;
}

bool ColliderManager::DoProjectionsOverlap (sf::Vector2f A[], unsigned int nbPointsA,
											ColliderBase* B,
											sf::Vector2f projectTo) {
	if (B->isCircle) {
		sf::Vector2f minA;
		sf::Vector2f maxA;
		ProjectionRect(A, nbPointsA, projectTo, minA, maxA);

		// the 2nd 'B' object..which is a circle
		CircleCollider* C = dynamic_cast<CircleCollider*>(B);

		// the edge points are on the projecting axis, both 'radius' length units away
		// so we get the vectors to there from the center by multiplying the normalized projecting axis
		sf::Vector2f circleEdge1(C->GetPosition() + Utils->Normalized(projectTo) * C->radius);
		sf::Vector2f circleEdge2(C->GetPosition() - Utils->Normalized(projectTo) * C->radius);

		sf::Vector2f projection(Utils->Project(circleEdge1, projectTo));
		sf::Vector2f minB(projection);
		sf::Vector2f maxB(projection);
		projection = sf::Vector2f(Utils->Project(circleEdge2, projectTo));
		if (minB.x > projection.x) {
			minB.x = projection.x;
		}
		if (minB.y > projection.y) {
			minB.y = projection.y;
		}

		if (maxB.x < projection.x) {
			maxB.x = projection.x;
		}
		if (maxB.y < projection.y) {
			maxB.y = projection.y;
		}

		// check to see if the projections intersect
		sf::Vector2f centerA( (minA.x + maxA.x) / 2, (minA.y + maxA.y) / 2 );
		sf::Vector2f centerB( (minB.x + maxB.x) / 2, (minB.y + maxB.y) / 2 );

		// the distance vector of the 2 virtual axis aligned rectangles
		sf::Vector2f t(centerB - centerA);

		sf::Vector2f halfSizeA(std::abs(maxA.x - minA.x) / 2, std::abs(maxA.y - minA.y) / 2);
		sf::Vector2f halfSizeB(std::abs(maxB.x - minB.x) / 2, std::abs(maxB.y - minB.y) / 2);

		// the projections overlap if the virtual axis aligned bounding boxes overlap
		if ( (std::abs(t.x) <= halfSizeA.x + halfSizeB.x) && (std::abs(t.y) <= halfSizeA.y + halfSizeB.y) )
			return true;

		return false;
	}
	else {
		RectCollider* R = dynamic_cast<RectCollider*>(B);

		sf::Vector2f pointsB[4] { sf::Vector2f(R->GetPosition() - R->size / 2.0f),
							sf::Vector2f(R->GetPosition() + R->size / 2.0f),
							sf::Vector2f(R->GetPosition() + sf::Vector2f(R->size.x / 2.0f, -R->size.y / 2.0f)),
							sf::Vector2f(R->GetPosition() + sf::Vector2f(-R->size.x / 2.0f, R->size.y / 2.0f)) };
		return DoProjectionsOverlap(A, nbPointsA, pointsB, 4, projectTo);
	}
}
