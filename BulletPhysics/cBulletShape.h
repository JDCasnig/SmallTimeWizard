#ifndef _cBulletShape_HG_
#define _cBulletShape_HG_

#include <iShape.h>


class cBulletShape : public nPhysics::iShape
{
public:
	cBulletShape();
	cBulletShape(nPhysics::sSphere sphereIn);
	cBulletShape(nPhysics::sPlane planeIn);
	cBulletShape(nPhysics::sBox boxIn);
	cBulletShape(nPhysics::sCapsule capsuleIn);
	~cBulletShape();
	// Use GetShapeType()
	// if this isn't the right shape, it will do nothing
	// and return false
	bool GetSphere(nPhysics::sSphere& sphereOut);
	bool GetPlane(nPhysics::sPlane& planeOut);
	bool GetBox(nPhysics::sBox& boxOut);
	bool GetCapsule(nPhysics::sCapsule& capsuleOut);
	// eShapeType
	// what shape am I?
	nPhysics::eShapeType GetShapeType();// { return mShapeType; }


private:
	nPhysics::eShapeType mShapeType;
	nPhysics::sSphere mSphere;
	nPhysics::sPlane mPlane;
	nPhysics::sBox mBox;
	nPhysics::sCapsule mCapsule;
};

#endif // !_cBulletShape_HG_