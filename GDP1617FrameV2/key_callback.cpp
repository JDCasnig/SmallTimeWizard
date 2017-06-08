#include "GlobalStuff.h"

#include <sstream>
#include "Utilities.h"

//void CreateFireBall();
//void DetonateFireBall();
//void CreateConeOfCold();
//void CreateGravityBall();
//void CreateForceWave();
//void DetonateGravityBall();
//bool bGravityBallExists=false;
//cGameObject* gravityBall;
//nPhysics::iRigidBody* gravityBallBody;


bool isShiftDownAlone( int mods )
{
	if ( (mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT )
	{
		return true;
	}
	return false;
}

bool isCrtlDownAlone( int mods )
{
	if ( (mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL )
	{
		return true;
	}
	return false;
}

bool isAltDownAlone( int mods )
{
	if ( (mods & GLFW_MOD_ALT) == GLFW_MOD_ALT )
	{
		return true;
	}
	return false;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	//std::string coolSoundEffect = "";

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

	switch (key)
	{
	case GLFW_KEY_U:
			
		break;
	case GLFW_KEY_J:

			break;
	case GLFW_KEY_H:
	{

			break;
	}
	case GLFW_KEY_K:
	{

			break;
	}
	case GLFW_KEY_Y:
	{

			break;
	}
	case GLFW_KEY_I:
	{
	
			break;
	}

	case GLFW_KEY_L:	// for "line!")
	{
	
		break;
	}
	case GLFW_KEY_UP:
		{
			

		}
		break;
	case GLFW_KEY_SPACE:
		{
			if (action == 1)
			{
				magicVision = !magicVision;
			}
		}
		break;

	case GLFW_KEY_G:
		if (action == 1)
		CreateGravityBall();

		break;

	// Light WSAD, QE, but for the light...
	case GLFW_KEY_A:
		//physicsWorld->ApplyImpulse(::gPlayerBody, glm::vec3(.5f, 0.f, 0.f));
		physicsWorld->ApplyAngularImpulse(::gPlayerBody, glm::vec3(0.f, -0.5f, 0.f));
		break;
	case GLFW_KEY_F:
		if (action==1)
		CreateFireBall();
		
		





		break;
	case GLFW_KEY_D:
		//physicsWorld->ApplyImpulse(::gPlayerBody, glm::vec3(-.5f, 0.f, 0.f));
		physicsWorld->ApplyAngularImpulse(::gPlayerBody, glm::vec3(0.f, 0.5f, 0.f));
		break;
	case GLFW_KEY_W:
		physicsWorld->ApplyImpulse(::gPlayerBody, (glm::vec3(0.f, 0.f, -.5f)*-gPlayer->bodyDef->qOrientation));
		
	
		
		break;
	case GLFW_KEY_S:

		physicsWorld->ApplyImpulse(::gPlayerBody, (glm::vec3(0.f, 0.f, .5f)*-gPlayer->bodyDef->qOrientation));
		
		break;
	case GLFW_KEY_Q:
		if (action == 1)
		CreateForceWave();
	
		break;
	case GLFW_KEY_E:

	
		break;
	// Adjust lights attenuation
	case GLFW_KEY_1:
	{
		cParticleEmitter::sParticleDesc emitDescript;
		// Change stuff in the description, perhaps?
		emitDescript.lifetimeMin = 2.0f;
		emitDescript.lifetimeMax = 4.0f;
		emitDescript.particlesCreatedPerUpdate = 0;
		// You would change this "force" to be in the direction
		// you wanted to travel (or whatever)
		emitDescript.ongoingForce = glm::vec3(0.0f, 0.5f, 0.0f);

		emitDescript.initVelMin = glm::vec3(-0.1f, 0.0f, -0.1f);
		emitDescript.initVelMax = glm::vec3(0.1f, 0.0f, 0.1f);


		::g_FireParticles->allocateParticles(0, emitDescript , true);

		cParticleEmitter::sParticleDesc emitDescript2;
		// Change stuff in the description, perhaps?
		emitDescript2.lifetimeMin = 2.0f;
		emitDescript2.lifetimeMax = 4.0f;
		emitDescript2.particlesCreatedPerUpdate = 0;
		// You would change this "force" to be in the direction
		// you wanted to travel (or whatever)
		emitDescript2.ongoingForce = glm::vec3(0.0f, 0.5f, 0.0f);

		emitDescript2.initVelMin = glm::vec3(-0.1f, 0.0f, -0.1f);
		emitDescript2.initVelMax = glm::vec3(0.1f, 0.0f, 0.1f);


		::g_GravityParticles->allocateParticles(0, emitDescript, true);
		//		::g_pParticles->allocateParticles()
	}
		break;
	case GLFW_KEY_2:

		break;
	case GLFW_KEY_3:

		break;
	case GLFW_KEY_4:

		break;
	case GLFW_KEY_5:

		break;
	case GLFW_KEY_6:

		break;
	case GLFW_KEY_7:

		break;
	case GLFW_KEY_8:

		break;

	}//switch (key)




	// Update the camera position in the title...
//	glfwSetWindowTitle( window, "Sexy" );

	std::stringstream ssTitle;
	//ssTitle << "Camera: " 
	//	<< ::g_CameraX << ", "
	//	<< ::g_CameraY << ", "
	//	<< ::g_CameraZ
	//	<< "  Light: " 
	//	<< ::lightPos.x << ", " 
	//	<< ::lightPos.y << ", " 
	//	<< ::lightPos.z
	//	<< " Lin: " << ::lightLinearAtten
	//	<< " Quad: " << ::lightQuadAtten
	//	<< " Number of triangles per frame: " << g_NumberOfIndicesRendered / 3;
	ssTitle << g_coolSoundEffect;
	glfwSetWindowTitle( window, ssTitle.str().c_str() );


	return;
}

void CreateFireBall()
{
	//F is for FIREBALL
	
	if (fireBallExists)
	{
		DetonateFireBall();
	}
	else
	{
		cGameObject* pFireBall = new cGameObject();
		pFireBall->bIsVisible = true;
		pFireBall->bodyDef->staticBody = false;	// 
		pFireBall->bIsWireframe = false;
		//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
		//pFireBall->bodyDef->position = glm::vec3(0.0f, 0.0f, 0.0f);

		pFireBall->scale = .40f;	// Unit sphere (0.5 units raduis)
		pFireBall->meshName = "Isoshphere_xyz_InvertedNormals.ply";
		pFireBall->bUseDebugColours = false;
		pFireBall->debugRGBA = glm::vec4(1.f, 1.f, 1.f, 1.f);
		pFireBall->texture00Name = "fireball.bmp";
		pFireBall->bIsVisible = true;
		pFireBall->bHasRigidBody = true;
		nPhysics::cRigidBodyDef bodyDef;

		bodyDef.position = ::gPlayer->bodyDef->position - glm::vec3(0.f, 0.f, 2.f)*gPlayer->bodyDef->qOrientation + glm::vec3(0.f, .5f, 0.f);
		bodyDef.staticBody = false;
		bodyDef.mass = 1.0f;
		bodyDef.velocity = -glm::vec3(0.f, 0.f, 20.f)*gPlayer->bodyDef->qOrientation;
		bodyDef.force = glm::vec3(0.f);
		//	bodyDef.qOrientation = glm::quat();


		nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere(.4f));
		nPhysics::iRigidBody* fireBallBody = physicsFactory->CreateRigidBody(bodyDef, sphereShape);

		nPhysics::cRigidBodyDef* bodyDefCheck;
		bodyDefCheck = fireBallBody->GetRigidBodyDef();

		pFireBall->bodyDef = bodyDefCheck;

		::g_vec_pGOs.push_back(pFireBall);
		physicsWorld->AddFireBall(fireBallBody);
		//physicsWorld->AddBody(fireBallBody);




		//cParticleEmitter::sParticleDesc emitDescript;
		//// Change stuff in the description, perhaps?
		//emitDescript.lifetimeMin = 2.0f;
		//emitDescript.lifetimeMax = 4.0f;
		//emitDescript.particlesCreatedPerUpdate = 3;
		//// You would change this "force" to be in the direction
		//// you wanted to travel (or whatever)
		//emitDescript.ongoingForce = glm::vec3(0.0f, 0.5f, 0.0f);

		//emitDescript.initVelMin = glm::vec3(-0.1f, 0.0f, -0.1f);
		//emitDescript.initVelMax = glm::vec3(0.1f, 0.0f, 0.1f);




		::fireBall = pFireBall;

		::fireBallExists = true;

		//::g_FireParticles->allocateParticles(15, emitDescript /*, true*/);
	}

	//pViper->bIsImposter = true;
}

void CreateConeOfCold()
{
	
}

void CreateGravityBall()
{
	if (bGravityBallExists)
	{
		DetonateGravityBall();
	}
	else
	{

		//G is for GIREBALL

		gravityBall = new cGameObject();
		gravityBall->bIsVisible = true;
		gravityBall->bodyDef->staticBody = false;	// 
		gravityBall->bIsWireframe = false;
		//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
		//pFireBall->bodyDef->position = glm::vec3(0.0f, 0.0f, 0.0f);

		gravityBall->scale = .40f;	// Unit sphere (0.5 units raduis)
		gravityBall->meshName = "Isoshphere_xyz_InvertedNormals.ply";
		gravityBall->bUseDebugColours = false;
		gravityBall->debugRGBA = glm::vec4(1.f, 1.f, 1.f, 1.f);
		gravityBall->texture00Name = "GravityBall.bmp";
		gravityBall->bIsVisible = true;
		gravityBall->bHasRigidBody = true;
		gravityBall->amountReflective = 0.5f;
		gravityBall->amountRefractive = 0.1f;
		nPhysics::cRigidBodyDef bodyDef;

		bodyDef.position = ::gPlayer->bodyDef->position - glm::vec3(0.f, 0.f, 2.f)*gPlayer->bodyDef->qOrientation + glm::vec3(0.f,.5f,0.f);
		bodyDef.staticBody = false;
		bodyDef.mass = 1.0f;

		bodyDef.velocity = -glm::vec3(0.f, 0.f, 20.f)*gPlayer->bodyDef->qOrientation;
		bodyDef.force = glm::vec3(0.f);
		//	bodyDef.qOrientation = glm::quat();


		nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere(.4f));
		gravityBallBody = physicsFactory->CreateRigidBody(bodyDef, sphereShape);

		nPhysics::cRigidBodyDef* bodyDefCheck;
		bodyDefCheck = gravityBallBody->GetRigidBodyDef();

		gravityBall->bodyDef = bodyDefCheck;

		::g_vec_pGOs.push_back(gravityBall);
		physicsWorld->AddBody(gravityBallBody);
		//pViper->bIsImposter = true;


		bGravityBallExists = true;


		//cParticleEmitter::sParticleDesc emitDescript;
		//// Change stuff in the description, perhaps?
		//emitDescript.lifetimeMin = 2.0f;
		//emitDescript.lifetimeMax = 4.0f;
		//emitDescript.particlesCreatedPerUpdate = 3;
		//// You would change this "force" to be in the direction
		//// you wanted to travel (or whatever)
		//emitDescript.ongoingForce = glm::vec3(0.0f, 0.5f, 0.0f);

		//emitDescript.initVelMin = glm::vec3(-0.1f, 0.0f, -0.1f);
		//emitDescript.initVelMax = glm::vec3(0.1f, 0.0f, 0.1f);

		//::g_GravityParticles->allocateParticles(15, emitDescript /*, true*/);




	}
}

void DetonateGravityBall()
{
	for (unsigned int i = 0; i < ::g_vec_pGOs.size(); i++)
	{
		if (g_vec_pGOs[i] != gravityBall && g_vec_pGOs[i]->bHasRigidBody)
		{
			if (g_vec_pGOs[i]->bodyDef->staticBody)
			{
				float distance;

				distance = glm::distance(gravityBall->bodyDef->position, ::g_vec_pGOs[i]->bodyDef->position);

				if (distance < 10.f)
				{
					glm::vec3 direction;
					direction = gravityBall->bodyDef->position - ::g_vec_pGOs[i]->bodyDef->position;
					physicsWorld->ApplyImpulse(::g_vec_pGOs[i]->rigidBody, direction);

				}
			}
		}
	}
	bGravityBallExists = false;
}



void CreateForceWave()
{
	for (unsigned int i = 0; i < ::g_vec_pGOs.size(); i++)
	{
		if (g_vec_pGOs[i] != ::gPlayer && g_vec_pGOs[i]->bHasRigidBody)
		{
			if (g_vec_pGOs[i]->bodyDef->staticBody)
			{
				float dotProduct;

				
				glm::mat4 wizardMat = glm::mat4(::gPlayer->bodyDef->qOrientation);
				glm::vec3 wizardOrientation = glm::vec3(wizardMat[3].x, wizardMat[3].y, wizardMat[3].z);
				dotProduct = glm::dot(glm::normalize(wizardOrientation), glm::normalize(glm::vec3(::g_vec_pGOs[i]->bodyDef->position-gPlayer->bodyDef->position)));

				float distance;

				distance = glm::distance(::gPlayer->bodyDef->position, ::g_vec_pGOs[i]->bodyDef->position);

				if (dotProduct > 0.5f && distance <20.f)
				{
					glm::vec3 direction;
					direction =   ::g_vec_pGOs[i]->bodyDef->position - ::gPlayer->bodyDef->position;
					physicsWorld->ApplyImpulse(::g_vec_pGOs[i]->rigidBody, direction*2.f);



				}
			}
		}
	}
}

void DetonateFireBall()
{
	if (fireBall != NULL)
	{

		for (unsigned int i = 0; i < ::g_vec_pGOs.size(); i++)
		{
			if (g_vec_pGOs[i] != fireBall && g_vec_pGOs[i]->bHasRigidBody)
			{
				if (g_vec_pGOs[i]->bodyDef->staticBody)
				{
					float distance;

					distance = glm::distance(fireBall->bodyDef->position, ::g_vec_pGOs[i]->bodyDef->position);

					if (distance < 10.f)
					{
						glm::vec3 direction;
						direction = -(fireBall->bodyDef->position - ::g_vec_pGOs[i]->bodyDef->position);
						physicsWorld->ApplyImpulse(::g_vec_pGOs[i]->rigidBody, direction);

					}
				}
			}
		}

		
		float distance;

		distance = glm::distance(fireBall->bodyDef->position, gEnemy1->bodyDef->position);
		if (distance < 10.f)
		{
			physicsWorld->MoveBodyToPosition(enemyBody, glm::vec3(-100.f, -100.f, -100.f));
		}
		//fireBall->texture00Name = "fireball.bmp";


		float distance2;

		distance2 = glm::distance(fireBall->bodyDef->position, gEnemy2->bodyDef->position);
		if (distance2 < 10.f)
		{
			physicsWorld->MoveBodyToPosition(enemyBody2, glm::vec3(-100.f, -100.f, -100.f));
		}
		fireBall->texture00Name = "fireball.bmp";


		cGameObject* explosion = new cGameObject();
		explosion->bIsVisible = true;
		explosion->bodyDef->staticBody = false;	// 
		explosion->bIsWireframe = false;
		//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
		//pFireBall->bodyDef->position = glm::vec3(0.0f, 0.0f, 0.0f);

		explosion->scale = 1.0f;	// Unit sphere (0.5 units raduis)
		explosion->meshName = "Isoshphere_xyz_InvertedNormals.ply";
		explosion->bUseDebugColours = false;
		explosion->debugRGBA = glm::vec4(1.f, 1.f, 1.f, 1.f);
		explosion->texture00Name = "fireball.bmp";
		explosion->bIsVisible = true;
		explosion->bodyDef->position = fireBall->bodyDef->position;


		//nPhysics::cRigidBodyDef bodyDef;

		//bodyDef.position = ::gPlayer->bodyDef->position - glm::vec3(0.f, 0.f, 2.f)*gPlayer->bodyDef->qOrientation;
		//bodyDef.staticBody = false;
		//bodyDef.mass = 1.0f;
		////bodyDef.velocity = -glm::vec3(0.f, 0.f, 20.f)*gPlayer->bodyDef->qOrientation;
		//bodyDef.force = glm::vec3(0.f);
		////	bodyDef.qOrientation = glm::quat();


		/*nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere(.4f));
		gravityBallBody = physicsFactory->CreateRigidBody(bodyDef, sphereShape);
	*/
	//nPhysics::cRigidBodyDef* bodyDefCheck;
	//bodyDefCheck = gravityBallBody->GetRigidBodyDef();

	//gravityBall->bodyDef = bodyDefCheck;

		::g_vec_pGOs.push_back(explosion);
		::g_pExplosion = explosion;
		fireBallExists = false;
		fireBall = NULL;
		//physicsWorld->AddBody(gravityBallBody);

	}
}


