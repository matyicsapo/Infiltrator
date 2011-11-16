#include "ColliderManager.hpp"

#include <cmath>

#include "GO/Colliders.hpp"
#include "Utilities.hpp"

ColliderManager* ColliderManager::Instance () {
	static ColliderManager instance;

	return &instance;
}

ColliderManager::~ColliderManager () {
	for (std::list<ColliderBase*>::iterator itC = mColliders.begin();
			itC != mColliders.end(); itC++) {
		delete *itC;
	}
}

void ColliderManager::Move (ColliderBase* c, sf::Vector2f& motion) {
	for (std::list<ColliderBase*>::iterator itC = mColliders.begin();
			itC != mColliders.end(); itC++) {

		if (*itC != c) {
			if (c->isCircle && (*itC)->isCircle) {
				// CIRCLE - CIRCLE collision
				CircleCollider* mCC = dynamic_cast<CircleCollider*>(c);
				CircleCollider* xCC = dynamic_cast<CircleCollider*>(*itC);

				if ( Utils->Dst(mCC->GetPosition() + motion, xCC->GetPosition()) < (mCC->radius + xCC->radius) ) {
					float maxDst = Utils->Dst(mCC->GetPosition() + motion, xCC->GetPosition()) - (mCC->radius + xCC->radius);

					if (maxDst < 0) {
						// needed to avoid jittering when not sliding and odd speeds when do
						maxDst = 0;

						//if (noslide)
						//return;
					}

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
					}
				}
			}
			else if (c->isCircle || (*itC)->isCircle) {
				if (c->isCircle) {
					// CIRCLE - RECT
					CircleCollider* mCC = dynamic_cast<CircleCollider*>(c);
					RectCollider* xRC = dynamic_cast<RectCollider*>(*itC);

					float d = 0;

					sf::Vector2f xRCMin(xRC->GetPosition() - (xRC->size / 2.0f));
					sf::Vector2f xRCMax(xRC->GetPosition() + (xRC->size / 2.0f));

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
						sf::Vector2f motionLeft = motion;

						Utils->ClampLength(motion, d);

						motionLeft -= motion;

						if (motionLeft.x != 0 || motionLeft.y != 0) {
							sf::Vector2f p(mCC->GetPosition() + (motion - motionLeft));

							sf::Vector2f n;

// slide dir stuff ////////////////////////////////////////////////////////////
							if (p.x + mCC->radius < xRCMin.x) {
								// left

								if (p.y + mCC->radius > xRCMin.y) {
									if (p.y - mCC->radius < xRCMax.y) {
										// left side
										n = sf::Vector2f(0, Utils->Sign(motionLeft.y, false));
									}
									else {
										// bottom-left corner
										if (std::abs(motionLeft.x) != std::abs(motionLeft.y)) {
											n = std::abs(motionLeft.x) < std::abs(motionLeft.y) ?
												sf::Vector2f(Utils->Sign(motionLeft.y, false), 0)
												:
												sf::Vector2f(0, Utils->Sign(motionLeft.y, false));
										}
									}
								}
								else {
									if (std::abs(motionLeft.x) != std::abs(motionLeft.y)) {
										n = std::abs(motionLeft.x) < std::abs(motionLeft.y) ?
											sf::Vector2f(Utils->Sign(motionLeft.y, false), 0)
											:
											sf::Vector2f(0, Utils->Sign(motionLeft.y, false));
									}
								}
							}
							else
							if (p.x - mCC->radius > xRCMax.x) {
								// right

								if (p.y + mCC->radius > xRCMin.y) {
									if (p.y - mCC->radius < xRCMax.y) {
										// right side
										n = sf::Vector2f(0, Utils->Sign(motionLeft.y, false));
									}
									else {
										// bottom-right corner
										if (std::abs(motionLeft.x) != std::abs(motionLeft.y)) {
											n = std::abs(motionLeft.x) < std::abs(motionLeft.y) ?
												sf::Vector2f(Utils->Sign(motionLeft.y, false), 0)
												:
												sf::Vector2f(0, Utils->Sign(motionLeft.y, false));
										}
									}
								}
								else {
									// top-right corner
									if (std::abs(motionLeft.x) != std::abs(motionLeft.y)) {
										n = std::abs(motionLeft.x) < std::abs(motionLeft.y) ?
											sf::Vector2f(Utils->Sign(motionLeft.y, false), 0)
											:
											sf::Vector2f(0, Utils->Sign(motionLeft.y, false));
									}
								}
							}
							else {
								if (p.y < xRCMin.y) {
									// top side
									n = sf::Vector2f(Utils->Sign(motionLeft.x, false), 0);
								}
								else {
									// bottom side
									n = sf::Vector2f(Utils->Sign(motionLeft.x, false), 0);
								}
							}
// slide dir stuff ////////////////////////////////////////////////////////////

							motion += n * Utils->Length(motionLeft);
						}
					}
				}
//				else {
//					// RECT - CIRCLE
//				}
			}
//			else {
//				// RECT - RECT collision
//				RectCollider* mRC = dynamic_cast<RectCollider*>(c);
//				RectCollider* xRC = dynamic_cast<RectCollider*>(*itC);
//
//				sf::Vector2f D((xRC->GetPosition() + motion) - mRC->GetPosition());
//
//				if ((int)mRC->GetRotation() % 90 == 0 && (int)xRC->GetRotation() % 90 == 0) {
//					// AABB - AABB collision
//
//					if (std::abs(D.x) <= mRC->size.x / 2 + xRC->size.x / 2
//							&& std::abs(D.y) <= mRC->size.y / 2 + xRC->size.y / 2) {
//						std::cout << "AABB - AABB collision" << std::endl;
//
//						motion = sf::Vector2f(0, 0);
//					}
//				}
//				else {
//					// OBB - OBB collision
//
//					// 4 tengelyt kell megvizsgálni - mind a 2 téglalap mind a 2 élpárjának megfelelõt
//					//{
//						//sf::Vector2f a1(mRC->size.x, 0);
//						//Utils->RotateByDegrees(a1, mRC->GetRotation());
//						//float projectionScalar_a1 = Utils->Length(a1); //mRC->size.x
//
////						sf::Vector2f a_size = mRC->size;
////						Utils->RotateByDegrees(a_size, mRC->GetRotation());
////						float projectionScalar_a_size = Utils->Length(a_size.x);
//
//						// projecting other OBB to a1
//						//sf::Vector2f q(xCC->size);
//						//Utils->RotateByDegrees(xRC->GetRotation() -
//					//}
//
//
//sf::Vector2f A(std::cos(mRC->GetRotation() * (PFWConstants::PI / 180)),
//				std::sin(mRC->GetRotation() * (PFWConstants::PI / 180)));
//sf::Vector2f B(std::cos(xRC->GetRotation() * (PFWConstants::PI / 180)),
//				std::sin(xRC->GetRotation() * (PFWConstants::PI / 180)));
//
//float AbsAdB [4];
//
//bool c1 = true;
//// axis C0+t*A0 (proj B onto Ax axis)  =>  w0 + proj(B->A0)  =>  w0 + A0*B0 + A0*B1
//AbsAdB[0] = std::abs(A.x *  B.x + A.y * B.y); //A0*B0
//AbsAdB[1] = std::abs(A.x * -B.y + A.y * B.x); //A0*B1
//if ( std::abs(-A.y * D.x + A.x * D.y) * 2
//		> mRC->size.x + xRC->size.x * AbsAdB[0] + xRC->size.y * AbsAdB[1] )
//	c1 = false;
//
//bool c2 = true;
//// axis C0+t*A1 (proj B onto Ay axis)  => h0 + proj(B->A1)  =>  h0 + A1*B0 + A1*B1
//AbsAdB[2] = std::abs(-A.y * B.x + A.x * B.y); //A1*B0
//AbsAdB[3] = AbsAdB[0]; //A1*B1 = -Ay*-By + Ax*Bx
//if ( std::abs(A.x * D.x + A.y * D.y) * 2
//		> mRC->size.y + xRC->size.x * AbsAdB[2] + xRC->size.y * AbsAdB[3] )
//	c2 = false;
//
//bool c3 = true;
//// axis C0+t*B0 (proj A onto Bx axis)   => w1 + proj(A->B0)  =>  w1 + B0*A0 + B0*A1
//if ( std::abs(-B.y * D.x + B.x * D.y) * 2
//		> xRC->size.x + mRC->size.x * AbsAdB[0] + mRC->size.y * AbsAdB[2] )
//	c3 = false;
//
//bool c4 = true;
//// axis C0+t*B1 (proj A onto By axis)   => h1 + proj(A->B1)  => h1 + B1*A0 + B1*A1
//if ( std::abs(B.x * D.x + B.y * D.y) * 2
//		> xRC->size.y + mRC->size.x * AbsAdB[1] + mRC->size.y * AbsAdB[3] )
//	c4 = false;
//
//if (c1 && c2 && c3 && c4) {
//	std::cout << "OBB - OBB collision" << D.x << std::endl;
//
//	motion = sf::Vector2f(0, 0);
//}
//
//					//std::cout << "OBB - OBB collision" << std::endl;
//
//					//std::cout << mRC->GetRotation() << std::endl;
//
////	sf::Vector2f mRC_localRight(1, 0);
////	sf::Vector2f mRC_localDown(0, 1);
////	Utils->Rotate(mRC_localRight, mRC->GetRotation());
////	Utils->Rotate(mRC_localDown, mRC->GetRotation());
////
////	//translation, in A's frame
////	sf::Vector2f T(	Utils->Dot(vToOther, mRC_localRight),
////					Utils->Dot(vToOther, mRC_localRight));
//
//					//std::cout << "OBB collision" << std::endl;
//				}
//
////				std::cout << std::endl;
//			}
		}
	}

	c->Move(motion);
}
