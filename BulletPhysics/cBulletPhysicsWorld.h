#ifndef  _cBulletPhysicsWorld_HG_
#define  _cBulletPhysicsWorld_HG_

#include <iPhysicsWorld.h>

#include <GLMIncludes.h>
#include "src\btBulletCollisionCommon.h"
#include "src\btBulletDynamicsCommon.h"
#include "cBulletRigidBody.h"
#include <vector>

class cBulletPhysicsWorld : public nPhysics::iPhysicsWorld
{
public:
	cBulletPhysicsWorld();
	~cBulletPhysicsWorld();

	bool AddBody(nPhysics::iRigidBody* body);
	void AddFireBall(nPhysics::iRigidBody* body);
	void RemoveFireBall(nPhysics::iRigidBody* body);
	void AddGravityBall(nPhysics::iRigidBody* body);
	void RemoveGravityBall(nPhysics::iRigidBody* body);
	//	virtual bool RemoveBody(iRigidBody* body) = 0;
	void SetGravity(glm::vec3 gravity);
	void GetGravity(glm::vec3& gravity);
	void TimeStep(float dt);
	void ApplyImpulse(nPhysics::iRigidBody* body, glm::vec3 Impulse);
	void ApplyAngularImpulse(nPhysics::iRigidBody * body, glm::vec3 Impulse);
	void AddConstraint();
	void AddHingeConstraint(nPhysics::iRigidBody * body1, glm::vec3 point, glm::vec3 plane);
	void AddP2PConstraint(nPhysics::iRigidBody* body1, glm::vec3 point);
	void AddSliderConstraint(nPhysics::iRigidBody* body1, glm::vec3 point);
	void AddConeRestraint(nPhysics::iRigidBody* body1, glm::vec3 point);
	void MoveBodyToPosition(nPhysics::iRigidBody* body, glm::vec3 position);
private:



	void cBulletPhysicsWorld::createEmptyDynamicsWorld();
	btDefaultCollisionConfiguration*	m_collisionConfiguration;
	btCollisionDispatcher*	m_dispatcher; 
	btDbvtBroadphase*	m_broadphase;
	btSequentialImpulseConstraintSolver* m_solver; 
	btDiscreteDynamicsWorld*	m_dynamicsWorld;
	nPhysics::iRigidBody* fireBall;
	nPhysics::iRigidBody* gravityBall;
	std::vector<nPhysics::iRigidBody*> mBodies;
};

#endif