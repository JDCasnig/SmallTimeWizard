#include "cGameObject.h"
#include "GlobalStuff.h"
int cGameObject::getID(void)
{
	return this->m_uniqueID;
}
	
//static 
int cGameObject::m_nextID = 1;

cGameObject::cGameObject()
{
	this->m_uniqueID = cGameObject::m_nextID;
	cGameObject::m_nextID++;
	this->bodyDef = new nPhysics::cRigidBodyDef();
	this->bHasRigidBody = false;
	// C'tor to init all values to 0.0f;
//	this->x = this->y = this->z = 0.0f;
//	this->Vx = this->Vy = this->Vz = 0.0f;
//	this->Ax = this->Ay = this->Az = 0.0f;

	//this->pre_Rot_X = this->pre_Rot_Y = this->pre_Rot_Z = 0.0f;
	//this->post_Rot_X = this->post_Rot_Y = this->post_Rot_Z = 0.0f;
	this->scale = 1.0f;

	this->bIsWireframe = false;	// Solid 
	this->debugRGBA = glm::vec4(1.0f);	// Solid white

	this->bIsVisible = true;
	this->bUseDebugColours = false;
	this->hasUpdateFunction = false;
	this->alpha = 1.0f;

	this->radius = 0.0f;

	this->meshID = 0;

	this->bIsImposter = false;
	this->usesLowDetailMesh = false;
	// Assume they are updated by the physics "sub-system"
//	this->bIsUpdatedByPhysics = false;

	this->amountReflective = 0.0f;	// 0 - 1.0   (no reflection to 100%)
	this->amountRefractive = 0.0f;	// 0 - 1.0   (no refraction to 100%)
	this->amountOfDiffuse = 1.0f;		// 0 - 1.0  (underlying "colour")
	this->indexOfRefraction = 1.0f;	// 1.0f to something



	return;
}

void cGameObject::Update()
{
	if (glm::distance(::gPlayer->bodyDef->position, this->bodyDef->position) < 2.f)
	{
		::playerHealth -= 10.f;
	}
	else if (glm::distance(::gPlayer->bodyDef->position, this->bodyDef->position) < 5.f)
	{
		this->debugRGBA = glm::vec4(1.f, 0.f, 0.f, 1.f);
	}
	else if (glm::distance(::gPlayer->bodyDef->position, this->bodyDef->position) < 10.f)
	{
		this->debugRGBA = glm::vec4(0.f, 1.f, 0.f, 1.f);
		glm::vec3 direction;
		direction = ::gPlayer->bodyDef->position - this->bodyDef->position;
		direction = glm::normalize(direction);
		if (this == gEnemy1)
		{
			physicsWorld->ApplyImpulse(enemyBody, direction*7.f);
		}
		else if (this == gEnemy2)
		{
			physicsWorld->ApplyImpulse(enemyBody2, direction*7.f);
		}
	}
	else if (glm::distance(::gPlayer->bodyDef->position, this->bodyDef->position) > 10.f)
	{
		this->debugRGBA = glm::vec4(1.f, 1.f, 1.f, 1.f);
	}
}
