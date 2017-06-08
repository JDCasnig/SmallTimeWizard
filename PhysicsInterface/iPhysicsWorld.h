#ifndef _iPhysicsWorld_HG_
#define _iPhysicsWorld_HG_

#include "iRigidBody.h"
//#include "iSoftBody.h"
//#include <GLMIncludes.h>
namespace nPhysics
{
	class iPhysicsWorld
	{
	public:
		virtual ~iPhysicsWorld() {}

		virtual bool AddBody(iRigidBody* body) = 0;
		virtual void AddFireBall(iRigidBody* body) = 0;
		virtual void RemoveFireBall(iRigidBody* body) = 0;
		virtual void AddGravityBall(iRigidBody* body) = 0;
		virtual void RemoveGravityBall(iRigidBody* body) = 0;
		virtual void MoveBodyToPosition(iRigidBody* body, glm::vec3 position)=0;
		//virtual bool AddBody(iSoftBody* body) = 0;
	//	virtual bool RemoveBody(iRigidBody* body) = 0;
		virtual void SetGravity(glm::vec3 gravity)=0;
		virtual void GetGravity(glm::vec3& gravity)=0;
		virtual void TimeStep(float dt) = 0;
		virtual void ApplyImpulse(iRigidBody* body,glm::vec3 Impulse) = 0;
		virtual void ApplyAngularImpulse(nPhysics::iRigidBody * body, glm::vec3 Impulse) = 0;
		virtual void AddConstraint() = 0;
		virtual void AddHingeConstraint(nPhysics::iRigidBody * body1, glm::vec3 point, glm::vec3 plane) = 0;
		virtual void AddP2PConstraint(iRigidBody* body1, glm::vec3 point)=0;
		virtual void AddSliderConstraint(iRigidBody* body1, glm::vec3 point)=0;
		virtual void AddConeRestraint(iRigidBody* body1, glm::vec3 point) = 0;
	};
}

#endif // !_iPhysicsWorld_HG_