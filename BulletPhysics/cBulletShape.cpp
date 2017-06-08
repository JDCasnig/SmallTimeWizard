#include "cBulletShape.h"


cBulletShape::cBulletShape()
{
	mShapeType = nPhysics::GHOST_SHAPE;
}

cBulletShape::cBulletShape(nPhysics::sSphere sphereIn)
{
	mSphere = sphereIn;
	mShapeType = nPhysics::SPHERE_SHAPE;
}

cBulletShape::cBulletShape(nPhysics::sPlane planeIn)
{
	mPlane = planeIn;
	mShapeType = nPhysics::PLANE_SHAPE;
}

cBulletShape::cBulletShape(nPhysics::sBox boxIn)
{
	mBox = boxIn;
	mShapeType = nPhysics::BOX_SHAPE;
}

cBulletShape::cBulletShape(nPhysics::sCapsule capsuleIn)
{
	mCapsule = capsuleIn;
	mShapeType = nPhysics::CAPSULE_SHAPE;
}

cBulletShape::~cBulletShape()
{

}
// Use GetShapeType()
// if this isn't the right shape, it will do nothing
// and return false
bool cBulletShape::GetSphere(nPhysics::sSphere& sphereOut)
{

	if (this->GetShapeType() == nPhysics::eShapeType::SPHERE_SHAPE)
		//if (mShapeType == nPhysics::eShapeType::SPHERE_SHAPE)
	{
		sphereOut = mSphere;
		return true;
	}

	return false;
}
bool cBulletShape::GetPlane(nPhysics::sPlane& planeOut)
{
	//if (mShapeType == nPhysics::eShapeType::SPHERE_SHAPE)

	if (this->GetShapeType() == nPhysics::eShapeType::PLANE_SHAPE)
	{
		planeOut = mPlane;
		return true;
	}

	return false;
}

bool cBulletShape::GetBox(nPhysics::sBox & boxOut)
{
	if (this->GetShapeType() == nPhysics::eShapeType::BOX_SHAPE)
	{
		boxOut = mBox;
		return true;
	}
	return false;
}

bool cBulletShape::GetCapsule(nPhysics::sCapsule & capsuleOut)
{
	if (this->GetShapeType() == nPhysics::eShapeType::CAPSULE_SHAPE)
	{
		capsuleOut = mCapsule;
		return true;
	}
	return false;
}

nPhysics::eShapeType cBulletShape::GetShapeType()
{
	return mShapeType;
}