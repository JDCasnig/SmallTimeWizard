#ifndef _cBulletFactory_HG_
#define _cBulletFactory_HG_
#include <iPhysicsFactory.h>
#include <iRigidBody.h>
#include "cBulletPhysicsWorld.h"
class cBulletFactory : public nPhysics::iPhysicsFactory
{
public:
	cBulletFactory();
	~cBulletFactory();
	nPhysics::iPhysicsWorld* CreatePhysicsWorld();
	nPhysics::iRigidBody* CreateRigidBody(nPhysics::cRigidBodyDef& rbDef, nPhysics::iShape* shape);
	nPhysics::iShape* CreateShape(const nPhysics::sSphere& sphere);
	nPhysics::iShape* CreateShape(const nPhysics::sPlane& plane);
	nPhysics::iShape* CreateShape(const nPhysics::sBox& box);
	nPhysics::iShape* CreateShape(const nPhysics::sCapsule& capsule);

};



#endif // !_cJPhysicsFactory_HG_

