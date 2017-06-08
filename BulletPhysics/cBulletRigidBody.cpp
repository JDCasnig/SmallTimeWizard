#include "cBulletRigidBody.h"
#include <iostream>

cBulletRigidBody::cBulletRigidBody(nPhysics::cRigidBodyDef bodyDef, cBulletShape bodyShape)
{
	mBodyDef = bodyDef;

	if (bodyShape.GetShapeType() == nPhysics::eShapeType::PLANE_SHAPE)
	{
		nPhysics::sPlane temp;
		bodyShape.GetPlane(temp);
		mBodyShape = temp;

		btVector3 inertia = btVector3(0, 0, 0);

		btCollisionShape* shape = new btStaticPlaneShape(btVector3(temp.Normal.x, temp.Normal.y, temp.Normal.z), 0);

		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(bodyDef.position.x, bodyDef.position.y, bodyDef.position.z)));
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			0,                  // mass
			motionState,        // initial position
			shape,              // collision shape of body
			inertia    // local inertia
			);

		mBulletRigidBody = new btRigidBody(rigidBodyCI);
		mBulletRigidBody->setFriction(btScalar(0.4));
		mBulletRigidBody->setRestitution(btScalar(0.5));
		
		mBulletRigidBody->setRollingFriction(btScalar(0.5));
		mBulletRigidBody->updateInertiaTensor();
		//mBulletRigidBody->setLinearVelocity(btVector3(bodyDef.velocity.x, bodyDef.velocity.y, bodyDef.velocity.z));


	}
	else if (bodyShape.GetShapeType() == nPhysics::eShapeType::SPHERE_SHAPE)
	{
		nPhysics::sSphere temp;
		bodyShape.GetSphere(temp);
		mBodyShape = temp;

		btScalar mass = btScalar(bodyDef.mass);
		btCollisionShape* shape = new btSphereShape(temp.Radius);
		btVector3 inertia;
		btQuaternion quat;
		shape->calculateLocalInertia(1.0f, inertia);
		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(quat, btVector3(bodyDef.position.x, bodyDef.position.y, bodyDef.position.z)));
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			mass,                  // mass
			motionState,        // initial position
			shape,              // collision shape of body
			inertia    // local inertia
			);

		mBulletRigidBody = new btRigidBody(rigidBodyCI);
		mBulletRigidBody->setLinearVelocity(btVector3(bodyDef.velocity.x, bodyDef.velocity.y, bodyDef.velocity.z));
		mBulletRigidBody->setFriction(btScalar(0.4));
		mBulletRigidBody->setRestitution(btScalar(.9));
		mBulletRigidBody->setRollingFriction(btScalar(0.5));
		mBulletRigidBody->updateInertiaTensor();
	//	mBulletRigidBody->updateInertiaTensor();

	}
	else if (bodyShape.GetShapeType() == nPhysics::eShapeType::BOX_SHAPE)
	{
		nPhysics::sBox temp;
		bodyShape.GetBox(temp);
		mBodyShape = temp;

		btScalar mass = btScalar(bodyDef.mass);
		btCollisionShape* shape = new btBoxShape(btVector3(temp.height / 2.f, temp.length / 2.f, temp.width / 2.f));//btSphereShape(temp.Radius);
		btVector3 inertia;
		btQuaternion quat;
		if (temp.length < 10.f)
		{
			quat.setW(bodyDef.qOrientation.w); //= bodyDef.qOrientation;
			quat.setX(bodyDef.qOrientation.x);
			quat.setY(bodyDef.qOrientation.y);
			quat.setZ(bodyDef.qOrientation.z);
		}
		//if (bodyDef.staticBody)
		//{
		//	shape->calculateLocalInertia(0.f, inertia);
		//}
		//else
		//{
			shape->calculateLocalInertia(1.0f, inertia);
	//	}
		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(quat, btVector3(bodyDef.position.x, bodyDef.position.y, bodyDef.position.z)));
		
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			mass,                  // mass
			motionState,        // initial position
			shape,              // collision shape of body
			inertia    // local inertia
			);

		mBulletRigidBody = new btRigidBody(rigidBodyCI);
		mBulletRigidBody->setLinearVelocity(btVector3(bodyDef.velocity.x, bodyDef.velocity.y, bodyDef.velocity.z));
		mBulletRigidBody->setFriction(btScalar(0.9));
		mBulletRigidBody->setRestitution(btScalar(.9));
		mBulletRigidBody->setRollingFriction(btScalar(0.5));
		mBulletRigidBody->updateInertiaTensor();
		
		if (bodyDef.staticBody)
		{
			mBulletRigidBody->setGravity(btVector3(0, 0, 0));
			mBulletRigidBody->setMassProps(0, btVector3(0, 0, 0));
		}
		//mBulletRigidBody->set
		//	mBulletRigidBody->updateInertiaTensor();
	}
	else if (bodyShape.GetShapeType() == nPhysics::eShapeType::CAPSULE_SHAPE)
	{
		nPhysics::sCapsule temp;
		bodyShape.GetCapsule(temp);
		mBodyShape = temp;

		btScalar mass = btScalar(bodyDef.mass);
		btCollisionShape* shape = new btCapsuleShape(temp.radius, temp.height);
		btVector3 inertia;
		btQuaternion quat;
		quat.setW(bodyDef.qOrientation.w); //= bodyDef.qOrientation;
		quat.setX(bodyDef.qOrientation.x);
		quat.setY(bodyDef.qOrientation.y);
		quat.setZ(bodyDef.qOrientation.z);
		shape->calculateLocalInertia(1.0f, inertia);
		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(quat, btVector3(bodyDef.position.x, bodyDef.position.y, bodyDef.position.z)));
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			mass,                  // mass
			motionState,        // initial position
			shape,              // collision shape of body
			inertia    // local inertia
			);

		mBulletRigidBody = new btRigidBody(rigidBodyCI);
		mBulletRigidBody->setAngularFactor(btVector3(0, 1, 0));
		mBulletRigidBody->setLinearVelocity(btVector3(bodyDef.velocity.x, bodyDef.velocity.y, bodyDef.velocity.z));
		mBulletRigidBody->setFriction(btScalar(0.99));
		mBulletRigidBody->setRestitution(btScalar(.0));
		mBulletRigidBody->setRollingFriction(btScalar(0.9));
		mBulletRigidBody->setSpinningFriction(btScalar(0.9f));
		mBulletRigidBody->updateInertiaTensor();
		
		//	mBulletRigidBody->updateInertiaTensor();
	}
	//mBodyShape = bodyShape;
	//std::cout << mBodyShape.GetShapeType() << std::endl;
}

cBulletRigidBody::~cBulletRigidBody()
{
}

nPhysics::cRigidBodyDef * cBulletRigidBody::GetRigidBodyDef()
{
	return &mBodyDef;
}

nPhysics::iShape * cBulletRigidBody::GetShape()
{
	return &mBodyShape;
}

btRigidBody * cBulletRigidBody::GetPhysicsBody()
{
	return mBulletRigidBody;
}
