#ifndef _iPhysicsWorld_HG_
#define _iPhysicsWorld_HG_

#include "iRigidBody.h"


namespace nPhysics
{
	class iPhysicsWorld
	{
	public:
		virtual ~iPhysicsWorld() {}

		virtual bool AddBody(iRigidBody* body) = 0;
		virtual void MoveBodyToPosition(iRigidBody* body, glm::vec3 position) = 0;
	};
}

#endif // !_iPhysicsWorld_HG_