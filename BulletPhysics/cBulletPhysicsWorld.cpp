#include "cBulletPhysicsWorld.h"
#include <cRigidBodyDef.h>
#include <iostream>
#include <functional>
cBulletPhysicsWorld::cBulletPhysicsWorld()
{
	createEmptyDynamicsWorld();
	gravityBall = NULL;
	fireBall = NULL;
}

cBulletPhysicsWorld::~cBulletPhysicsWorld()
{
	createEmptyDynamicsWorld();
}

bool cBulletPhysicsWorld::AddBody(nPhysics::iRigidBody* body)
{
	cBulletRigidBody* temp = (cBulletRigidBody*) body;
	btRigidBody* tempbody = temp->GetPhysicsBody();
	
	m_dynamicsWorld->addRigidBody(temp->GetPhysicsBody());
	mBodies.push_back(body);
	return true;
}

void cBulletPhysicsWorld::AddFireBall(nPhysics::iRigidBody * body)
{
	cBulletRigidBody* temp = (cBulletRigidBody*)body;
	btRigidBody* tempbody = temp->GetPhysicsBody();

	m_dynamicsWorld->addRigidBody(temp->GetPhysicsBody());
	mBodies.push_back(body);
	fireBall = body;
}

void cBulletPhysicsWorld::RemoveFireBall(nPhysics::iRigidBody * body)
{

}

void cBulletPhysicsWorld::AddGravityBall(nPhysics::iRigidBody * body)
{
	cBulletRigidBody* temp = (cBulletRigidBody*)body;
	btRigidBody* tempbody = temp->GetPhysicsBody();

	m_dynamicsWorld->addRigidBody(temp->GetPhysicsBody());
	mBodies.push_back(body);
	gravityBall = body;

}

void cBulletPhysicsWorld::RemoveGravityBall(nPhysics::iRigidBody * body)
{

}

//	virtual bool RemoveBody(iRigidBody* body) = 0;
void cBulletPhysicsWorld::SetGravity(glm::vec3 gravity)
{

}

void cBulletPhysicsWorld::GetGravity(glm::vec3& gravity)
{

}

void cBulletPhysicsWorld::TimeStep(float dt)
{
	if (fireBall != NULL)
	{
		fireBall->GetRigidBodyDef()->collisionFlag = false;
	}
	if (gravityBall != NULL)
	{
		gravityBall->GetRigidBodyDef()->collisionFlag = false;
	}
	m_dynamicsWorld->stepSimulation(dt);
	for (unsigned int i = 0; i < mBodies.size(); i++)
	{

		cBulletRigidBody* tempBody = (cBulletRigidBody*)mBodies[i];
		btRigidBody* tempBulletBody = tempBody->GetPhysicsBody();
		tempBulletBody->updateInertiaTensor();
		nPhysics::cRigidBodyDef* temp = mBodies[i]->GetRigidBodyDef();
		btVector3 tempPos = tempBulletBody->getCenterOfMassPosition();
		//std::cout << "Position is: " << tempPos.x() << " : " << tempPos.y() << " : " << tempPos.z() << std::endl;
		temp->position = glm::vec3(tempPos.x(), tempPos.y(), tempPos.z());
		//btMotionState* tempState = tempBulletBody->getMotionState();
		btQuaternion tempOrientation = tempBulletBody->getOrientation();
		//btQuaternion tempOrientation2 =tempBulletBody->getOrientation();
		glm::quat  tempQuat = glm::quat(tempOrientation.w(), tempOrientation.x(), tempOrientation.y(), tempOrientation.z());
		temp->qOrientation = tempQuat;
		//temp->qOrientation.x = tempOrientation.x();
		//temp->qOrientation.y = tempOrientation.y();
		//temp->qOrientation.z = tempOrientation.z();
		//temp->qOrientation.w = tempOrientation.w();
		//std::cout << "Position is: " << temp->position.x << " : " << temp->position.y << " : " << temp->position.z << std::endl;
	//	std::cout << "tempQuat is: " << tempQuat.x << " : " << tempQuat.y << " : " << tempQuat.z << " : " << tempQuat.z << std::endl;
	//	std::cout << "tempBodyDefQuat is: " << temp->qOrientation.x << " : " << temp->qOrientation.y << " : " << temp->qOrientation.z << " : " << temp->qOrientation.z << std::endl;
		//btMotionState* tempState = tempBody->getMotionState();
		//btTransform tranState;
		//tempState->getWorldTransform(tranState);

		//tempBulletBody->getCollisionFlags()

	}
	btRigidBody* fireBody;
	if (fireBall != NULL)
	{
		 fireBody = ((cBulletRigidBody*)fireBall)->GetPhysicsBody();
	}
	
	btRigidBody* gravityBody;

	if (gravityBall != NULL)
	{

		btRigidBody* gravityBody = ((cBulletRigidBody*)gravityBall)->GetPhysicsBody();
	}
	

	int contactPoints = this->m_dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < contactPoints; i++)
	{
		btPersistentManifold* contactManifold = this->m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		//const btCollisionObject* obA = contactManifold->getBody0();
		btRigidBody* obA = (btRigidBody*) contactManifold->getBody0();
		btRigidBody* obB = (btRigidBody*) contactManifold->getBody1();
		if (fireBall != NULL)
		{
			if (obB == fireBody)//obA == fireBody)// || obB == fireBody)
			{
				fireBall->GetRigidBodyDef()->collisionFlag = true;
			}
		}

		if (gravityBall != NULL)
		{
			if (obA == gravityBody || obB == gravityBody)
			{
			//	gravityBall->GetRigidBodyDef()->collisionFlag = true;
			}
		}


		//this->m_dynamicsWorld->contactTest(((cBulletRigidBody*)mBodies[0])->GetPhysicsBody(),  btCollisionWorld::ContactResultCallback setFireBallFlag)
		//obA->getCollisionFlags
		//((cBulletRigidBody*)mBodies[0])->GetPhysicsBody()
	}

	


}

void cBulletPhysicsWorld::ApplyImpulse(nPhysics::iRigidBody * body, glm::vec3 Impulse)
{
	for (unsigned int i = 0; i < mBodies.size(); i++)
	{
		if (mBodies[i] == body)
		{

			cBulletRigidBody* tempBody = (cBulletRigidBody*)mBodies[i];
			btRigidBody* tempBulletBody = tempBody->GetPhysicsBody();
			tempBulletBody->activate();
			tempBulletBody->applyCentralImpulse(btVector3(Impulse.x,Impulse.y,Impulse.z));
			
		}
	}
}


void cBulletPhysicsWorld::ApplyAngularImpulse(nPhysics::iRigidBody * body, glm::vec3 Impulse)
{
	for (unsigned int i = 0; i < mBodies.size(); i++)
	{
		if (mBodies[i] == body)
		{

			cBulletRigidBody* tempBody = (cBulletRigidBody*)mBodies[i];
			btRigidBody* tempBulletBody = tempBody->GetPhysicsBody();
			tempBulletBody->activate();
			tempBulletBody->applyTorqueImpulse(btVector3(Impulse.x, Impulse.y, Impulse.z));

		}
	}
}

 void cBulletPhysicsWorld::createEmptyDynamicsWorld() 
 {
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);
	m_broadphase = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
	m_solver = sol;
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
	m_dynamicsWorld->setGravity(btVector3(0, -10, 0));
	
}

void cBulletPhysicsWorld::AddConstraint()
{
	
	btRigidBody* pointer1 = ((cBulletRigidBody*)this->mBodies[0])->GetPhysicsBody();
	//btRigidBody* pointer2 = new btRigidBody(0, NULL, NULL);

	//btGeneric6DofConstraint* constraint = new btGeneric6DofConstraint( *pointer1,/* *pointer2, btTransform::getIdentity(),*/ btTransform::getIdentity(), false);
	//
	//constraint->setLinearLowerLimit(btVector3(1, 1, 1));
	//constraint->setLinearUpperLimit(btVector3(-1, -1, -1));

	//constraint->setAngularLowerLimit(btVector3(0, 1, 0));
	//constraint->setAngularUpperLimit(btVector3(0, -1, 0));

	//this->m_dynamicsWorld->addConstraint(constraint);


	pointer1->setLinearFactor(btVector3(1,1,1));
	pointer1->setAngularFactor(btVector3(0, 1, 0));

}

void cBulletPhysicsWorld::AddHingeConstraint(nPhysics::iRigidBody * body1, glm::vec3 point, glm::vec3 plane)
{
	btRigidBody* pointer1 = ((cBulletRigidBody*)body1)->GetPhysicsBody();
	btTypedConstraint* newConstraint = new btHingeConstraint(*pointer1,btVector3(point.x, point.y, point.z), btVector3(plane.x, plane.y, plane.z ),true);// &((cBulletRigidBody*)body1)->GetPhysicsBody(), &((cBulletRigidBody*)body2)->GetPhysicsBody())
	this->m_dynamicsWorld->addConstraint(newConstraint, false);
}

void cBulletPhysicsWorld::AddP2PConstraint(nPhysics::iRigidBody * body1, glm::vec3 point)
{
	btRigidBody* pointer1 = ((cBulletRigidBody*)body1)->GetPhysicsBody();
	//btRigidBody* pointer2 = ((cBulletRigidBody*)body2)->GetPhysicsBody();


	btTypedConstraint* newConstraint = new btPoint2PointConstraint(*pointer1, btVector3(point.x, point.y, point.z));
	this->m_dynamicsWorld->addConstraint(newConstraint, false);
}

void cBulletPhysicsWorld::AddSliderConstraint(nPhysics::iRigidBody * body1, glm::vec3 point)
{
	btRigidBody* pointer1 = ((cBulletRigidBody*)body1)->GetPhysicsBody();
	//btRigidBody* pointer2 = ((cBulletRigidBody*)body2)->GetPhysicsBody();
	btTransform fromA(btTransform::getIdentity());

	fromA.setOrigin(btVector3(point.x,point.y,point.z));

	btSliderConstraint* newConstraint = new btSliderConstraint(*pointer1, fromA, false);//(*pointer1, btVector3(3, 1, 3));
	
//	newConstraint->setLowerLinLimit(-10.0f);
//	newConstraint->setUpperLinLimit(10.0f);

//	newConstraint->setLowerAngLimit(0.0f);
//	newConstraint->setUpperAngLimit(0.0f);

	this->m_dynamicsWorld->addConstraint(newConstraint, false);
}

void cBulletPhysicsWorld::AddConeRestraint(nPhysics::iRigidBody * body1, glm::vec3 point)
{

	btRigidBody* pointer1 = ((cBulletRigidBody*)body1)->GetPhysicsBody();
	btTransform fromA(btTransform::getIdentity());

	fromA.setOrigin(btVector3(point.x,point.y,point.z));
	btConeTwistConstraint* newConstraint = new btConeTwistConstraint(*pointer1, fromA);
	this->m_dynamicsWorld->addConstraint(newConstraint, false);
}

void cBulletPhysicsWorld::MoveBodyToPosition(nPhysics::iRigidBody * body, glm::vec3 position)
{
	btRigidBody* rigidBody= ((cBulletRigidBody*)body)->GetPhysicsBody();


	btQuaternion quat;
//	body->GetRigidBodyDef()->
	quat.setW(body->GetRigidBodyDef()->qOrientation.w); //= bodyDef.qOrientation;
	quat.setX(body->GetRigidBodyDef()->qOrientation.x);
	quat.setY(body->GetRigidBodyDef()->qOrientation.y);
	quat.setZ(body->GetRigidBodyDef()->qOrientation.z);
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(quat, btVector3(position.x, position.y, position.z)));

	rigidBody->setLinearVelocity(btVector3(0.f, 0.f, 0.f));
	rigidBody->setMotionState(motionState);
}

int setFireBallFlag()
{
	return 0;
}
