#include "cGameObject.h"
#include "Utilities.h"	// For getRandFloat
#include <vector>
#include "GlobalStuff.h"
#include <pugixml\pugixml.hpp>
#include <iostream>
// variable is NOT in this file, but it's somewhere
// (i.e. the linker will "link" to it)
extern unsigned int g_DebugSphereID;
extern unsigned int g_SkyboxID;
extern unsigned int g_1SidedQuadID;
extern unsigned int g_2SidedQuadID;


#include <glm/glm.hpp>

cGameObject* PlaceObject( std::string objectName, glm::vec3 location );
cGameObject* PlaceObject( std::string objectName, glm::vec3 location, glm::vec3 rotation );
cGameObject* PlaceObject( std::string objectName, glm::vec3 location, glm::vec3 rotation, float scale );
bool LoadBMP1(std::string fileName);
bool LoadBMP2(std::string fileName);
bool LoadEnemies();
bool LoadObjectives();
char ReadNextChar(char* data, unsigned long &index);
unsigned short ReadNextUnsignedShort(char* data, unsigned long &index);
unsigned long ReadNextUnsignedLong(char* data, unsigned long &index);
unsigned long ShiftAndAdd_Byte_to_ULong(unsigned long theULong, char theByte, int bytesToShift);
unsigned short ShiftAndAdd_Byte_to_UShort(unsigned short theUShort, char theByte, int bytesToShift);

void AddGridLines(std::vector< cGameObject* > &vec_pGOs)
{
	//{	// Axes
	//	float gridLimit = 50.0f; 
	//	float gridStep = 10.0f;

	//	// Make x axis... lines along z and y axes
	//	for ( float y = -gridLimit; y <= gridLimit; y += gridStep )
	//	{
	//		for ( float z = -gridLimit; z <= gridLimit; z += gridStep )
	//		{
	//			cGameObject* pBar = new cGameObject();
	//			pBar->bIsUpdatedByPhysics = false;	// 
	//			pBar->bIsWireframe = false;
	//			pBar->position.y = y;
	//			pBar->position.z = z;
	//			pBar->bUseDebugColours = true;
	//			pBar->debugRGBA = glm::vec4( 1.0f, 0.0f, 0.0f, 0.5f );		// x = red
	//			pBar->meshName = "Long_bar_x_aligned_x10.ply";
	//			pBar->scale = 200.0f;			// Ground is 50.0x50.0 
	//			vec_pGOs.push_back( pBar );
	//		}
	//	}// for ( float y = -gridLimit;

	//	// Make y axis... lines along x and z axes
	//	for ( float x = -gridLimit; x <= gridLimit; x += gridStep )
	//	{
	//		for ( float z = -gridLimit; z <= gridLimit; z += gridStep )
	//		{
	//			cGameObject* pBar = new cGameObject();
	//			pBar->bIsUpdatedByPhysics = false;	// 
	//			pBar->bIsWireframe = false;
	//			pBar->position.x = x;
	//			pBar->position.z = z;
	//			pBar->bUseDebugColours = true;
	//			pBar->debugRGBA = glm::vec4( 0.0f, 1.0f, 0.0f, 0.5f );		// y = green
	//			pBar->meshName = "Long_bar_y_aligned_x10.ply";
	//			pBar->scale = 200.0f;			// Ground is 50.0x50.0 
	//			vec_pGOs.push_back( pBar );
	//		}
	//	}// for ( float y = -gridLimit;

	//	// Make z axis... lines along x and y axes
	//	for ( float x = -gridLimit; x <= gridLimit; x += gridStep )
	//	{
	//		for ( float y = -gridLimit; y <= gridLimit; y += gridStep )
	//		{
	//			cGameObject* pBar = new cGameObject();
	//			pBar->bIsUpdatedByPhysics = false;	// 
	//			pBar->bIsWireframe = false;
	//			pBar->position.x = x;
	//			pBar->position.y = y;
	//			pBar->bUseDebugColours = true;
	//			pBar->debugRGBA = glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f );		// z = blue
	//			pBar->meshName = "Long_bar_z_aligned_x10.ply";
	//			pBar->scale = 200.0f;			// Ground is 50.0x50.0 
	//			vec_pGOs.push_back( pBar );
	//		}
	//	}// for ( float y = -gridLimit;

	//}	// Axes

	//return;
}

bool LoadObjectsIntoScene( std::vector< cGameObject* > &vec_pGOs, std::string sceneFile )
{

	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file(sceneFile.c_str());

//	std::cout << "Load result: " << result.description() << ", Game Object: " << doc.child("GameObject").child("Mesh").attribute("name").value() << std::endl;

	pugi::xml_node_iterator it = doc.child("GameObjects").children().begin();

	for (it; it != doc.child("GameObjects").children().end(); it++)
	{
		std::string tempString = it->name();
		if (tempString == "Player")
		{


			cGameObject* pWizard = new cGameObject();
			pWizard->bIsVisible = true;
			pWizard->bodyDef->staticBody = true;	// 
			pWizard->bIsWireframe = false;
			float temp = it->child("position").attribute("y").as_double();
			//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
			pWizard->bodyDef->position = glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);

			pWizard->scale = 1.0f;	// Unit sphere (0.5 units raduis) 
			std::string tempMeshName = it->child("Mesh").attribute("name").value();
			pWizard->meshName = it->child("Mesh").attribute("name").value();
			//pWizard->meshName = "Cube2.ply"; 
			pWizard->bUseDebugColours = false;
			pWizard->debugRGBA = glm::vec4(0.f, 0.f, 1.f, 1.f);
			pWizard->texture00Name = "GridSmall.bmp";
			pWizard->bIsVisible = true;

			nPhysics::cRigidBodyDef bodyDef;
			bodyDef.position = glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);
			bodyDef.staticBody = true;
			bodyDef.mass = 1.0f;
			bodyDef.velocity = glm::vec3(0.f);
			bodyDef.force = glm::vec3(0.f);
			bodyDef.qOrientation = glm::quat(glm::vec3(0, 4, 0));
			//		bodyDef.qOrientation = glm::quat();

			/*nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());*/
			nPhysics::iShape* capsuleShape = physicsFactory->CreateShape(nPhysics::sCapsule());
			//nPhysics::iShape* cubeShape = physicsFactory->CreateShape()
			::gPlayerBody = physicsFactory->CreateRigidBody(bodyDef, capsuleShape);

			nPhysics::cRigidBodyDef* bodyDefCheck;
			bodyDefCheck = gPlayerBody->GetRigidBodyDef();
			//bodyDefCheck->qOrientation = glm::quat(glm::vec3(0, -1, -45));
			pWizard->bodyDef = bodyDefCheck;
			pWizard->rigidBody = ::gPlayerBody;
			pWizard->bHasRigidBody = true;
			//pViper->bIsImposter = true;
			camera1->AttachToGameObject(pWizard);
			vec_pGOs.push_back(pWizard);
			::gPlayer = pWizard;
			physicsWorld->AddBody(::gPlayerBody);
		}
		else if (tempString == "FloorPanel")
		{

			
			cGameObject* pFloorPanel = new cGameObject();
			pFloorPanel->bIsVisible = true;
			pFloorPanel->bodyDef->staticBody = true;	// 
			pFloorPanel->bIsWireframe = false;
			float temp = it->child("position").attribute("y").as_double();
			//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
		//	pFloorPanel->bodyDef->position = glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);

			pFloorPanel->scale = 1.0f;	// Unit sphere (0.5 units raduis) 
			//std::string tempMeshName = it->child("Mesh").attribute("name").value();
			pFloorPanel->meshName = "Cube2.ply";// it->child("Mesh").attribute("name").value();
			pFloorPanel->lowDetailMesh = "Cube_1x1x1_N.ply";
			pFloorPanel->usesLowDetailMesh = true;
			//pWizard->meshName = "Cube2.ply"; 
			pFloorPanel->bUseDebugColours = false;
			pFloorPanel->debugRGBA = glm::vec4(0.f, 0.f, 1.f, 1.f);
			pFloorPanel->texture00Name = "GridSmall.bmp";
			pFloorPanel->bIsVisible = true;

			nPhysics::cRigidBodyDef bodyDef;
			bodyDef.position = glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);
			bodyDef.staticBody = true;
			bodyDef.mass = 1.0f;
			bodyDef.velocity = glm::vec3(0.f);
			bodyDef.force = glm::vec3(0.f);
			bodyDef.qOrientation = glm::quat(glm::vec3(0, 4, 0));
			//		bodyDef.qOrientation = glm::quat();

			/*nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());*/
			nPhysics::iShape* capsuleShape = physicsFactory->CreateShape(nPhysics::sBox());
			//nPhysics::iShape* cubeShape = physicsFactory->CreateShape()
			pFloorPanel->rigidBody = physicsFactory->CreateRigidBody(bodyDef, capsuleShape);
			pFloorPanel->bHasRigidBody = true;
			nPhysics::cRigidBodyDef* bodyDefCheck;
			bodyDefCheck = pFloorPanel->rigidBody->GetRigidBodyDef();
			//bodyDefCheck->qOrientation = glm::quat(glm::vec3(0, -1, -45));
			pFloorPanel->bodyDef = bodyDefCheck;
			//pViper->bIsImposter = true;
		//	camera1->AttachToGameObject(pFloorPanel);
			vec_pGOs.push_back(pFloorPanel);
			physicsWorld->AddBody(pFloorPanel->rigidBody);
		}
		else if (tempString == "TestBox")
		{


			cGameObject* pFloorPanel = new cGameObject();
			pFloorPanel->bIsVisible = true;
			pFloorPanel->bodyDef->staticBody = true;	// 
			pFloorPanel->bIsWireframe = false;
			float temp = it->child("position").attribute("y").as_double();
			//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
			//	pFloorPanel->bodyDef->position = glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);

			pFloorPanel->scale = 1.0f;	// Unit sphere (0.5 units raduis) 
										//std::string tempMeshName = it->child("Mesh").attribute("name").value();
			pFloorPanel->meshName = "Cube2.ply";// it->child("Mesh").attribute("name").value();
												//pWizard->meshName = "Cube2.ply"; 
			pFloorPanel->lowDetailMesh = "Cube_1x1x1_N.ply";
			pFloorPanel->usesLowDetailMesh = true;
			pFloorPanel->bUseDebugColours = false;
			pFloorPanel->debugRGBA = glm::vec4(0.f, 0.f, 1.f, 1.f);
			pFloorPanel->texture00Name = "GridSmall.bmp";
			pFloorPanel->bIsVisible = true;

			nPhysics::cRigidBodyDef bodyDef;
			bodyDef.position = glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);
			bodyDef.staticBody = false;
			bodyDef.mass = 1.0f;
			bodyDef.velocity = glm::vec3(0.f);
			bodyDef.force = glm::vec3(0.f);
			bodyDef.qOrientation = glm::quat(glm::vec3(0, 4, 0));
			//		bodyDef.qOrientation = glm::quat();

			/*nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());*/
			nPhysics::iShape* capsuleShape = physicsFactory->CreateShape(nPhysics::sBox());
			//nPhysics::iShape* cubeShape = physicsFactory->CreateShape()
			pFloorPanel->rigidBody = physicsFactory->CreateRigidBody(bodyDef, capsuleShape);
			pFloorPanel->bHasRigidBody = true;
			nPhysics::cRigidBodyDef* bodyDefCheck;
			bodyDefCheck = pFloorPanel->rigidBody->GetRigidBodyDef();
			//bodyDefCheck->qOrientation = glm::quat(glm::vec3(0, -1, -45));
			pFloorPanel->bodyDef = bodyDefCheck;
			//pViper->bIsImposter = true;
			//	camera1->AttachToGameObject(pFloorPanel);
			vec_pGOs.push_back(pFloorPanel);
			physicsWorld->AddBody(pFloorPanel->rigidBody);
		}
	}

	//pugi::xml_node playerNode = doc.child("Player");

//	{	// Add a 1 sided quad
//
//
//		//nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere(it->child("Scale").attribute("Size").as_double()));
//		//nPhysics::cRigidBodyDef bodyDef;
//		//bodyDef.position = glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);
//		//bodyDef.staticBody = true;
//		//bodyDef.mass = 1.0f;
//		//bodyDef.velocity = glm::vec3(0.f);//glm::vec3(0.f, 0.f, -10.f) + (camera1->cameraTarget-camera1->position);
//		//nPhysics::iRigidBody* rigidBody = physicsFactory->CreateRigidBody(bodyDef, sphereShape);
//		//nPhysics::cRigidBodyDef* bodyDefCheck;
//		//bodyDefCheck = rigidBody->GetRigidBodyDef();
//		//cGameObject* pSphere = new cGameObject();
//		//pSphere->bIsUpdatedByPhysics = it->child("PhysicsEnabled").attribute("staticbody").as_bool();//false;
//		//pSphere->bodyDef = bodyDefCheck;
//		//pSphere->meshName = it->child("Mesh").attribute("name").value();
//		//pSphere->scale = it->child("Scale").attribute("Size").as_double();				// Ground is 50.0x50.0 
//
//		//pSphere->bIsWireframe = true;
//		//pSphere->debugRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//		//pSphere->bUseDebugColours = true;
//		//physicsWorld->AddBody(rigidBody);
//		//vec_pGOs.push_back(pSphere);
//
//
//
//
//		////////////////////////////////////////////////////////////////////
//
//		cGameObject* pWizard = new cGameObject();
//		pWizard->bIsVisible = true;
//		pWizard->bodyDef->staticBody = true;	// 
//		pWizard->bIsWireframe = false;
//		float temp = playerNode.child("position").attribute("y").as_double();
//		//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
//		pWizard->bodyDef->position = glm::vec3(playerNode.child("position").attribute("x").as_double(), playerNode.child("position").attribute("y").as_double(), playerNode.child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);
//		
//		pWizard->scale = 1.0f;	// Unit sphere (0.5 units raduis) 
//		std::string tempMeshName = playerNode.child("Mesh").attribute("name").value();
//		pWizard->meshName = playerNode.child("Mesh").attribute("name").value();
//		//pWizard->meshName = "Cube2.ply"; 
//		pWizard->bUseDebugColours = false;
//		pWizard->debugRGBA = glm::vec4(0.f, 0.f, 1.f, 1.f);
//		pWizard->texture00Name = "GridSmall.bmp";
//		pWizard->bIsVisible = true;
//		
//		nPhysics::cRigidBodyDef bodyDef;
//		bodyDef.position = glm::vec3(playerNode.child("position").attribute("x").as_double(), playerNode.child("position").attribute("y").as_double(), playerNode.child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);
//		bodyDef.staticBody = true;
//		bodyDef.mass = 1.0f;
//		bodyDef.velocity = glm::vec3(0.f);
//		bodyDef.force = glm::vec3(0.f);
//		bodyDef.qOrientation = glm::quat(glm::vec3(0,4,0));
////		bodyDef.qOrientation = glm::quat();
//
//		/*nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());*/
//		nPhysics::iShape* capsuleShape = physicsFactory->CreateShape(nPhysics::sCapsule());
//		//nPhysics::iShape* cubeShape = physicsFactory->CreateShape()
//		::gPlayerBody = physicsFactory->CreateRigidBody(bodyDef, capsuleShape);
//
//		nPhysics::cRigidBodyDef* bodyDefCheck;
//		bodyDefCheck = gPlayerBody->GetRigidBodyDef();
//		//bodyDefCheck->qOrientation = glm::quat(glm::vec3(0, -1, -45));
//		pWizard->bodyDef = bodyDefCheck;
//		//pViper->bIsImposter = true;
//		camera1->AttachToGameObject(pWizard);
//		vec_pGOs.push_back(pWizard);
//		::gPlayer = pWizard;
//		physicsWorld->AddBody(::gPlayerBody);
//	}

	//{	// Add a 1 sided quad
	//	cGameObject* pCube = new cGameObject();
	//	pCube->bIsVisible = true;
	//	pCube->bodyDef->staticBody = true;	// 
	//	pCube->bIsWireframe = false;
	//	//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
	//	pCube->bodyDef->position = glm::vec3(0.0f, 0.0f, 0.0f);

	//	pCube->scale = 1.0f;	// Unit sphere (0.5 units raduis)
	//							//pWizard->meshName = "wizard.ply";
	//	pCube->meshName = "Cube2.ply";
	//	pCube->bUseDebugColours = false;
	//	pCube->debugRGBA = glm::vec4(0.f, 0.f, 1.f, 1.f);
	//	pCube->texture00Name = "fireball.bmp";
	//	pCube->bIsVisible = true;

	//	nPhysics::cRigidBodyDef bodyDef;
	//	bodyDef.position = glm::vec3(0.f, 2.f, -4.f);
	//	bodyDef.staticBody = true;
	//	bodyDef.mass = 1.0f;
	//	bodyDef.velocity = glm::vec3(0.f);
	//	bodyDef.force = glm::vec3(0.f);
	//	//		bodyDef.qOrientation = glm::quat(glm::vec3(0,0,-45));
	//	bodyDef.qOrientation = glm::quat();

	//	/*nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());*/
	//	nPhysics::iShape* cubeShape = physicsFactory->CreateShape(nPhysics::sBox());
	//	//nPhysics::iShape* cubeShape = physicsFactory->CreateShape()
	//	pCube->rigidBody = physicsFactory->CreateRigidBody(bodyDef, cubeShape);
	//	pCube->bHasRigidBody = true;
	//	nPhysics::cRigidBodyDef* bodyDefCheck;
	//	bodyDefCheck = pCube->rigidBody->GetRigidBodyDef();

	//	pCube->bodyDef = bodyDefCheck;
	//	//pViper->bIsImposter = true;
	//	
	//	vec_pGOs.push_back(pCube);

	//	physicsWorld->AddBody(pCube->rigidBody);

	//	physicsWorld->AddHingeConstraint(pCube->rigidBody, glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));
	//}
	//

	//{	// Add a 1 sided quad
	//	cGameObject* p2Cube = new cGameObject();
	//	p2Cube->bIsVisible = true;
	//	p2Cube->bodyDef->staticBody = true;	// 
	//	p2Cube->bIsWireframe = false;
	//	//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
	//	p2Cube->bodyDef->position = glm::vec3(0.0f, 0.0f, 0.0f);

	//	p2Cube->scale = 1.0f;	// Unit sphere (0.5 units raduis)
	//							//pWizard->meshName = "wizard.ply";
	//	p2Cube->meshName = "Cube2.ply";
	//	p2Cube->bUseDebugColours = false;
	//	p2Cube->debugRGBA = glm::vec4(0.f, 0.f, 1.f, 1.f);
	//	p2Cube->texture00Name = "fireball.bmp";
	//	p2Cube->bIsVisible = true;

	//	nPhysics::cRigidBodyDef bodyDef;
	//	bodyDef.position = glm::vec3(0.f, 2.f, 4.f);
	//	bodyDef.staticBody = true;
	//	bodyDef.mass = 1.0f;
	//	bodyDef.velocity = glm::vec3(0.f);
	//	bodyDef.force = glm::vec3(0.f);
	//	//		bodyDef.qOrientation = glm::quat(glm::vec3(0,0,-45));
	//	bodyDef.qOrientation = glm::quat();

	//	/*nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());*/
	//	nPhysics::iShape* Cube2Shape = physicsFactory->CreateShape(nPhysics::sBox());
	//	//nPhysics::iShape* cubeShape = physicsFactory->CreateShape()
	//	p2Cube->rigidBody = physicsFactory->CreateRigidBody(bodyDef, Cube2Shape);
	//	p2Cube->bHasRigidBody = true;
	//	nPhysics::cRigidBodyDef* bodyDefCheck;
	//	bodyDefCheck = p2Cube->rigidBody->GetRigidBodyDef();

	//	p2Cube->bodyDef = bodyDefCheck;
	//	//pViper->bIsImposter = true;

	//	vec_pGOs.push_back(p2Cube);

	//	physicsWorld->AddBody(p2Cube->rigidBody);

	//	physicsWorld->AddP2PConstraint(p2Cube->rigidBody, glm::vec3(3.f,1.f,3.f));
	//}

	//{	// Add a 1 sided quad
	//	cGameObject* p3Cube = new cGameObject();
	//	p3Cube->bIsVisible = true;
	//	p3Cube->bodyDef->staticBody = true;	// 
	//	p3Cube->bIsWireframe = false;
	//	//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
	//	p3Cube->bodyDef->position = glm::vec3(0.0f, 0.0f, 0.0f);

	//	p3Cube->scale = 1.0f;	// Unit sphere (0.5 units raduis)
	//							//pWizard->meshName = "wizard.ply";
	//	p3Cube->meshName = "Cube2.ply";
	//	p3Cube->bUseDebugColours = false;
	//	p3Cube->debugRGBA = glm::vec4(0.f, 0.f, 1.f, 1.f);
	//	p3Cube->texture00Name = "GridSmall.bmp";
	//	p3Cube->bIsVisible = true;

	//	nPhysics::cRigidBodyDef bodyDef;
	//	bodyDef.position = glm::vec3(3.f, 2.f, 0.f);
	//	bodyDef.staticBody = true;
	//	bodyDef.mass = 1.0f;
	//	bodyDef.velocity = glm::vec3(0.f);
	//	bodyDef.force = glm::vec3(0.f);
	//	//		bodyDef.qOrientation = glm::quat(glm::vec3(0,0,-45));
	//	bodyDef.qOrientation = glm::quat();

	//	/*nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());*/
	//	nPhysics::iShape* cube3Shape = physicsFactory->CreateShape(nPhysics::sBox());
	//	//nPhysics::iShape* cubeShape = physicsFactory->CreateShape()
	//	p3Cube->rigidBody = physicsFactory->CreateRigidBody(bodyDef, cube3Shape);
	//	p3Cube->bHasRigidBody = true;
	//	nPhysics::cRigidBodyDef* bodyDefCheck;
	//	bodyDefCheck = p3Cube->rigidBody->GetRigidBodyDef();

	//	p3Cube->bodyDef = bodyDefCheck;
	//	//pViper->bIsImposter = true;

	//	vec_pGOs.push_back(p3Cube);

	//	physicsWorld->AddBody(p3Cube->rigidBody);

	//	physicsWorld->AddSliderConstraint(p3Cube->rigidBody, glm::vec3(1.f,0.f,0.f));
	//}

	//{	// Add a 1 sided quad
	//	cGameObject* p4Cube = new cGameObject();
	//	p4Cube->bIsVisible = true;
	//	p4Cube->bodyDef->staticBody = true;	// 
	//	p4Cube->bIsWireframe = false;
	//	//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
	//	p4Cube->bodyDef->position = glm::vec3(0.0f, 0.0f, 0.0f);

	//	p4Cube->scale = 1.0f;	// Unit sphere (0.5 units raduis)
	//							//pWizard->meshName = "wizard.ply";
	//	p4Cube->meshName = "Cube2.ply";
	//	p4Cube->bUseDebugColours = false;
	//	p4Cube->debugRGBA = glm::vec4(0.f, 0.f, 1.f, 1.f);
	//	p4Cube->texture00Name = "GridSmall.bmp";
	//	p4Cube->bIsVisible = true;

	//	nPhysics::cRigidBodyDef bodyDef;
	//	bodyDef.position = glm::vec3(3.f, 2.f, 0.f);
	//	bodyDef.staticBody = true;
	//	bodyDef.mass = 1.0f;
	//	bodyDef.velocity = glm::vec3(0.f);
	//	bodyDef.force = glm::vec3(0.f);
	//	//		bodyDef.qOrientation = glm::quat(glm::vec3(0,0,-45));
	//	bodyDef.qOrientation = glm::quat();

	//	/*nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());*/
	//	nPhysics::iShape* cube4Shape = physicsFactory->CreateShape(nPhysics::sBox());
	//	//nPhysics::iShape* cubeShape = physicsFactory->CreateShape()
	//	p4Cube->rigidBody = physicsFactory->CreateRigidBody(bodyDef, cube4Shape);
	//	p4Cube->bHasRigidBody = true;
	//	nPhysics::cRigidBodyDef* bodyDefCheck;
	//	bodyDefCheck = p4Cube->rigidBody->GetRigidBodyDef();

	//	p4Cube->bodyDef = bodyDefCheck;
	//	//pViper->bIsImposter = true;

	//	vec_pGOs.push_back(p4Cube);

	//	physicsWorld->AddBody(p4Cube->rigidBody);

	//	physicsWorld->AddConeRestraint(p4Cube->rigidBody, glm::vec3(1.f,0.f,0.f));
	//}

//	physicsWorld->AddConstraint();


	//::gPlayer = g_vGameObjects[g_vGameObjects.size()];

	//::gPlayer->physicsComponent->bodyDef = &bodyDefCheck;
	//nPhysics::cRigidBodyDef* bodyDefCheck;
	//bodyDefCheck = ::gPlayerBody->GetRigidBodyDef();
	//::gPlayer->physicsComponent->bodyDef = bodyDefCheck;
	




	camera1->cameraStyle = eCameraStyle::Attached;
	//camera1->AttachToGameObject(::gPlayer);
	camera1->cameraOffset = glm::vec3(0.f, 2.f, 5.f);





	nPhysics::iShape* planeShape = physicsFactory->CreateShape(nPhysics::sPlane());
	nPhysics::cRigidBodyDef bodyDef2;
	bodyDef2.position = glm::vec3(0.f, 0.f, 0.f);
	bodyDef2.staticBody = true;
	nPhysics::iRigidBody* rigidBody2 = physicsFactory->CreateRigidBody(bodyDef2, planeShape);
	physicsWorld->AddBody(rigidBody2);

	//lightPos.x = 0.f;
	//lightPos.y = 5.f;
	//lightPos.z = 0.f;


	camera1->cameraTarget = glm::vec3(0.f);

	camera1->position = glm::vec3(0.f, 15.f, 25.f);



	{	// Skybox
		cGameObject* pSkyBox = new cGameObject();
		pSkyBox->bodyDef->staticBody = true;	// 
		pSkyBox->bodyDef->position = glm::vec3(0.0f, 0.0f, 0.0f);
		pSkyBox->scale = 1000.0f;	// Unit sphere (0.5 units raduis)
		pSkyBox->meshName = "Isoshphere_xyz_InvertedNormals.ply";
		pSkyBox->texture00Name = "spacebox";
		pSkyBox->bIsVisible = false;
		::g_SkyboxID = pSkyBox->getID();
		vec_pGOs.push_back( pSkyBox );
	}

	{	// Add a 1 sided quad
		cGameObject* p1SidedQuad = new cGameObject();
		p1SidedQuad->bodyDef->staticBody = true;// = false;	// 
		p1SidedQuad->bIsWireframe = true;
		//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
		p1SidedQuad->bodyDef->position = glm::vec3(0.0f,0.0f,0.0f);
		p1SidedQuad->scale = 1.0f;	// Unit sphere (0.5 units raduis)
		p1SidedQuad->meshName = "1x1_2Tri_Quad_1_Sided_XY_Plane.ply";
		p1SidedQuad->texture00Name = "smoketex.bmp";
		p1SidedQuad->bIsVisible = false;
		p1SidedQuad->bIsImposter = true;
		::g_1SidedQuadID = p1SidedQuad->getID();
		vec_pGOs.push_back( p1SidedQuad );
	}
	{	// Add a 2 sided quad
		cGameObject* p2SidedQuad = new cGameObject();
		p2SidedQuad->bodyDef->staticBody = true;// bIsUpdatedByPhysics = false;	// 
		p2SidedQuad->bIsWireframe = true;
		//p2SidedQuad->x = 0.0f;	p2SidedQuad->y = 0.0f;	p2SidedQuad->z = 0.0f;
		p2SidedQuad->bodyDef->position = glm::vec3(0.0f,0.0f,0.0f);
		p2SidedQuad->scale = 1.0f;	// Unit sphere (0.5 units raduis)
		p2SidedQuad->meshName = "1x1_2Tri_Quad_2_Sided_XY_Plane.ply";
		p2SidedQuad->bIsVisible = false;
		p2SidedQuad->bIsImposter = true;
		p2SidedQuad->texture00Name = "smoketex.bmp"; 
		::g_2SidedQuadID = p2SidedQuad->getID();
		vec_pGOs.push_back( p2SidedQuad );
	}

	//for (int xIndex = -100; xIndex < 100; xIndex += 20)
	//
	//	for (int zIndex = -100; zIndex < 100; zIndex += 20)
	//	{

	//		cGameObject* newObj = new cGameObject();
	//		//newObj->meshName = "Isoshphere_xyz_InvertedNormals.ply";
	//		newObj->meshName = "1x1_2Tri_Quad_2_Sided_XZ_Plane.ply";

	//		newObj->bUseDebugColours = true;
	//		newObj->debugRGBA = glm::vec4(1.f);
	//		newObj->bIsWireframe = false;
	//		newObj->bIsVisible = true;
	//		newObj->bodyDef->position = glm::vec4(xIndex, 0.0f, zIndex, 1.0f);
	//		newObj->scale = 20.0f;
	//		newObj->bUseDebugColours = true;
	//		if (xIndex % 40 == 0)
	//		{
	//			newObj->debugRGBA = glm::vec4(.7f);
	//		}
	//		else if (zIndex % 40 == 0)
	//		{
	//			newObj->debugRGBA = glm::vec4(.7f);
	//		}
	//		else
	//		{
	//			newObj->debugRGBA = glm::vec4(.1f);
	//		}
	//		
	//		newObj->texture00Name = "fireball.bmp";
	//		::g_vec_pGOs.push_back(newObj);

	//	}
	//}

	for ( std::vector<cGameObject*>::iterator itpGO = vec_pGOs.begin();
		  itpGO != vec_pGOs.end(); itpGO++ )
	{
//		cGameObject* pTempGO;
//		pTempGO = *itpGO;		
//		pTempGO->alpha;
//		(*pTempGO).alpha;

//		(*itpGO)->alpha = getRandFloat( 0.1f, 0.75f );
		(*itpGO)->alpha = 1.0f;
	}

		  LoadBMP1("Level1.bmp");
		  LoadBMP2("Level2.bmp");
		  LoadEnemies();
		  LoadObjectives();
	return true;
}

bool LoadBMP1(std::string fileName)
{
	std::ifstream theFile;
	//	if (this->m_bHave_cout_output)
	//	{
	std::cout << "Reading texture file: " << fileName;
	//	}
	theFile.open(fileName.c_str(), std::ios_base::binary);
	if (!theFile)
	{
		//	this->m_lastErrorNum = CTextureFromBMP::ERORR_FILE_WONT_OPEN;
		std::cout << "Something bad happen" << std::endl;
		return false;
	}
	// Scan to the end to get the size
	theFile.seekg(0, std::ios::end);
	std::ios::pos_type fileSize = theFile.tellg();
	// Return to start
	theFile.seekg(0, std::ios::beg);

	char* pRawData = new char[static_cast<unsigned int>(fileSize)];

	theFile.read(pRawData, fileSize);
	theFile.close();

	// Now go through and decode the BMP file.
	unsigned long curIndex = 0;
	char letter1 = ReadNextChar(pRawData, curIndex);
	char letter2 = ReadNextChar(pRawData, curIndex);
	if ((letter1 != 'B') && (letter2 != 'M'))
	{
		//this->m_lastErrorNum = CTextureFromBMP::ERROR_NOT_A_BMP_FILE;
		std::cout << "Something else bad happen" << std::endl;
		return false;
	}

	unsigned long height;
	unsigned long width;
	unsigned long originalHeight;
	unsigned long originalWidth;
	unsigned short reserved1;
	unsigned short reserved2;
	unsigned long fileSize2;
	unsigned long offsetInBits;
	unsigned long headerSize;
	unsigned long numberOfRows;
	unsigned long numberOfColumns;
	unsigned short numberOfPlanes;
	unsigned short bitPerPixel;	// 1, 4, 8, 15, 24, 32, or 64
	unsigned long compressionMode;	// 0 if NOT compressed (i.e. 24 bit bitmap)
	unsigned long imageSizeInBytes;	// Total bytes in image - may be zero
	unsigned long PixelsPerMeterX;
	unsigned long PixelsPerMeterY;
	unsigned long numberOfLookUpTableEntries;	// Could be 0 for 24 bit format
	unsigned long numberOfImportantColours;	// 0 if all colours are important




											// File is BMP
	fileSize2 = ReadNextUnsignedLong(pRawData, curIndex);
	reserved1 = ReadNextUnsignedShort(pRawData, curIndex);
	reserved2 = ReadNextUnsignedShort(pRawData, curIndex);
	offsetInBits = ReadNextUnsignedLong(pRawData, curIndex);
	headerSize = ReadNextUnsignedLong(pRawData, curIndex);
	numberOfRows = ReadNextUnsignedLong(pRawData, curIndex);
	height = originalHeight = numberOfRows;
	numberOfColumns = ReadNextUnsignedLong(pRawData, curIndex);
	width = originalWidth = numberOfColumns;
	numberOfPlanes = ReadNextUnsignedShort(pRawData, curIndex);
	bitPerPixel = ReadNextUnsignedShort(pRawData, curIndex);
	std::cout << "bitPerPixel: " << bitPerPixel << std::endl;
	// Is is a 24 bit bitmap?
	if (bitPerPixel != 24)
	{
		//this->m_lastErrorNum = CTextureFromBMP::ERROR_NOT_A_24_BIT_BITMAP;
		std::cout << "Not 24 bit bitmap" << std::endl;
		return false;
	}
	C24BitBMPpixel* p_theImages;
	compressionMode = ReadNextUnsignedLong(pRawData, curIndex);
	imageSizeInBytes = ReadNextUnsignedLong(pRawData, curIndex);
	PixelsPerMeterX = ReadNextUnsignedLong(pRawData, curIndex);
	PixelsPerMeterY = ReadNextUnsignedLong(pRawData, curIndex);
	numberOfLookUpTableEntries = ReadNextUnsignedLong(pRawData, curIndex);
	numberOfImportantColours = ReadNextUnsignedLong(pRawData, curIndex);

	// 
	long bytesPerRow = ((3 * numberOfRows + 3) / 4) * 4;
	long numberOfPaddingBytes = bytesPerRow - 3 * numberOfColumns;

	// Allocate enough space...
	p_theImages = new C24BitBMPpixel[numberOfRows * numberOfColumns];

	// Did we run out of memory?
	if (!p_theImages)
	{
		//this->m_lastErrorNum = CTextureFromBMP::ERROR_NOT_ENOUGHT_MEMORY_FOR_BITMAP;
		std::cout << "No memory badness" << std::endl;
		return false;
	}


	// Read the bitmap into memory...
	unsigned long bytesRead = 0;
	unsigned long totalBytesInFile = numberOfRows * numberOfColumns;
	unsigned long pixelCount = 0;

	cGameObject* pFloorPanel = new cGameObject();
	pFloorPanel->bIsVisible = true;
	pFloorPanel->bodyDef->staticBody = true;	// 
	pFloorPanel->bIsWireframe = false;

	//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
	//	pFloorPanel->bodyDef->position = glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);

	pFloorPanel->scale = 4.f;	// Unit sphere (0.5 units raduis) 
								//std::string tempMeshName = it->child("Mesh").attribute("name").value();
	pFloorPanel->meshName = "Cube2.ply";// it->child("Mesh").attribute("name").value();
										//									pWizard->meshName = "Cube2.ply";
	pFloorPanel->lowDetailMesh = "Cube_1x1x1_N.ply";
	pFloorPanel->usesLowDetailMesh = true;

	pFloorPanel->bUseDebugColours = true;
	pFloorPanel->debugRGBA = glm::vec4(1.f, 1.f, 1.f, 1.f);
	pFloorPanel->texture00Name = "GridSmall.bmp";
	pFloorPanel->bIsVisible = true;
	//pFloorPanel->bodyDef->position= glm::vec3((float)numberOfRows*.75f, 18.f, float(numberOfColumns)*.75f);
	nPhysics::cRigidBodyDef bodyDef;

	bodyDef.position = glm::vec3((float)numberOfRows*.75f + (float)numberOfRows*1.5f+4.f, 18.f, float(numberOfColumns)*.75f);// glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);
	bodyDef.staticBody = true;
	bodyDef.mass = 1.0f;
	bodyDef.velocity = glm::vec3(0.f);
	bodyDef.force = glm::vec3(0.f);
	bodyDef.qOrientation = glm::quat(glm::vec3(0, 0, 0));
	//		bodyDef.qOrientation = glm::quat();
	pFloorPanel->bodyDef = &bodyDef;
	/*nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());*/
	nPhysics::iShape* boxShape = physicsFactory->CreateShape(nPhysics::sBox((float)numberOfRows*1.5f, float(numberOfColumns)*1.5f, 2.f));
	//nPhysics::iShape* cubeShape = physicsFactory->CreateShape()
	nPhysics::iRigidBody* rigidBody = physicsFactory->CreateRigidBody(bodyDef, boxShape);
	//pFloorPanel->bHasRigidBody = true;
	//nPhysics::cRigidBodyDef* bodyDefCheck;
	//bodyDefCheck = pFloorPanel->rigidBody->GetRigidBodyDef();
	//bodyDefCheck->qOrientation = glm::quat(glm::vec3(0, -1, -45));
	//pFloorPanel->bodyDef = bodyDefCheck;
	//pViper->bIsImposter = true;
	//	camera1->AttachToGameObject(pFloorPanel);



	//::g_vec_pGOs.push_back(pFloorPanel);
	physicsWorld->AddBody(rigidBody);




	int floorCount = 0;
	int wallCount = 0;
	int startCount = 0;
	int goalCount = 0;
	for (unsigned long row = 0; row < numberOfRows; row++)
	{
		for (unsigned long col = 0; col < numberOfColumns; col++)
		{
			char theblue = ReadNextChar(pRawData, curIndex);
			char thegreen = ReadNextChar(pRawData, curIndex);
			char thered = ReadNextChar(pRawData, curIndex);

			p_theImages[pixelCount].redPixel = static_cast<unsigned char>(thered);
			p_theImages[pixelCount].greenPixel = static_cast<unsigned char>(thegreen);
			p_theImages[pixelCount].bluePixel = static_cast<unsigned char>(theblue);

			/*	std::cout << pixelCount<<": Red: " << (int) p_theImages[pixelCount].redPixel << std::endl;
			std::cout << pixelCount << ": Green: " << (int)p_theImages[pixelCount].greenPixel << std::endl;
			std::cout << pixelCount << ": Blue: " << (int)p_theImages[pixelCount].bluePixel << std::endl;*/
			//m_p_theImages[pixelCount].alphaPixel = 255;
			if ((int)p_theImages[pixelCount].redPixel>220 && (int)p_theImages[pixelCount].greenPixel>220 && (int)p_theImages[pixelCount].bluePixel > 220)
			{


				//cGameObject* pFloorPanel = new cGameObject();
				//pFloorPanel->bIsVisible = true;
				//pFloorPanel->bodyDef->staticBody = true;	// 
				//pFloorPanel->bIsWireframe = false;
				//
				////p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
				////	pFloorPanel->bodyDef->position = glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);

				//pFloorPanel->scale = 1.2f;	// Unit sphere (0.5 units raduis) 
				//							//std::string tempMeshName = it->child("Mesh").attribute("name").value();
				//pFloorPanel->meshName = "Cube2.ply";// it->child("Mesh").attribute("name").value();
				//									//pWizard->meshName = "Cube2.ply"; 
				//pFloorPanel->bUseDebugColours = false;
				//pFloorPanel->debugRGBA = glm::vec4(0.f, 0.f, 1.f, 1.f);
				//pFloorPanel->texture00Name = "GridSmall.bmp";
				//pFloorPanel->bIsVisible = true;

				//nPhysics::cRigidBodyDef bodyDef;
				//bodyDef.position = glm::vec3((float)row + row*.5f, 18.f, float(col) + col*.5f);// glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);
				//bodyDef.staticBody = true;
				//bodyDef.mass = 1.0f;
				//bodyDef.velocity = glm::vec3(0.f);
				//bodyDef.force = glm::vec3(0.f);
				//bodyDef.qOrientation = glm::quat(glm::vec3(0, 4, 0));
				////		bodyDef.qOrientation = glm::quat();

				///*nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());*/
				//nPhysics::iShape* capsuleShape = physicsFactory->CreateShape(nPhysics::sBox());
				////nPhysics::iShape* cubeShape = physicsFactory->CreateShape()
				//pFloorPanel->rigidBody = physicsFactory->CreateRigidBody(bodyDef, capsuleShape);
				//pFloorPanel->bHasRigidBody = true;
				//nPhysics::cRigidBodyDef* bodyDefCheck;
				//bodyDefCheck = pFloorPanel->rigidBody->GetRigidBodyDef();
				////bodyDefCheck->qOrientation = glm::quat(glm::vec3(0, -1, -45));
				//pFloorPanel->bodyDef = bodyDefCheck;
				////pViper->bIsImposter = true;
				////	camera1->AttachToGameObject(pFloorPanel);



				//::g_vec_pGOs.push_back(pFloorPanel);
				//physicsWorld->AddBody(pFloorPanel->rigidBody);




				floorCount++;
			}
			else if ((int)p_theImages[pixelCount].redPixel == 0 && (int)p_theImages[pixelCount].greenPixel == 0 && (int)p_theImages[pixelCount].bluePixel == 0)
			{
				cGameObject* pFloorPanel = new cGameObject();
				pFloorPanel->bIsVisible = true;
				pFloorPanel->bodyDef->staticBody = true;	// 
				pFloorPanel->bIsWireframe = false;

				//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
				//	pFloorPanel->bodyDef->position = glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);

				pFloorPanel->scale = 1.2f;	// Unit sphere (0.5 units raduis) 
											//std::string tempMeshName = it->child("Mesh").attribute("name").value();
				pFloorPanel->meshName = "Cube2.ply";// it->child("Mesh").attribute("name").value();
													//pWizard->meshName = "Cube2.ply"; 
				pFloorPanel->lowDetailMesh = "Cube_1x1x1_N.ply";
				pFloorPanel->usesLowDetailMesh = true;

				pFloorPanel->bUseDebugColours = false;
				pFloorPanel->debugRGBA = glm::vec4(0.f, 0.f, 1.f, 1.f);
				pFloorPanel->texture00Name = "GridSmall.bmp";
				pFloorPanel->bIsVisible = true;

				nPhysics::cRigidBodyDef bodyDef;
				bodyDef.position = glm::vec3((float)numberOfRows*1.5f+ (float)row + row*.5f, 19.f, float(col) + col*.5f);// glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);
				bodyDef.staticBody = true;
				bodyDef.mass = 1.0f;
				bodyDef.velocity = glm::vec3(0.f);
				bodyDef.force = glm::vec3(0.f);
				bodyDef.qOrientation = glm::quat(glm::vec3(0, 0, 0));
				//		bodyDef.qOrientation = glm::quat();

				/*nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());*/
				nPhysics::iShape* capsuleShape = physicsFactory->CreateShape(nPhysics::sBox());
				//nPhysics::iShape* cubeShape = physicsFactory->CreateShape()
				pFloorPanel->rigidBody = physicsFactory->CreateRigidBody(bodyDef, capsuleShape);
				pFloorPanel->bHasRigidBody = true;
				nPhysics::cRigidBodyDef* bodyDefCheck;
				bodyDefCheck = pFloorPanel->rigidBody->GetRigidBodyDef();
				//bodyDefCheck->qOrientation = glm::quat(glm::vec3(0, -1, -45));
				pFloorPanel->bodyDef = bodyDefCheck;
				//pViper->bIsImposter = true;
				//	camera1->AttachToGameObject(pFloorPanel);



				::g_vec_pGOs.push_back(pFloorPanel);
				physicsWorld->AddBody(pFloorPanel->rigidBody);

				wallCount++;
			}
			else if ((int)p_theImages[pixelCount].redPixel == 0 && (int)p_theImages[pixelCount].greenPixel == 128 && (int)p_theImages[pixelCount].bluePixel == 128)
			{
				cGameObject* pFloorPanel = new cGameObject();
				pFloorPanel->bIsVisible = true;
				pFloorPanel->bodyDef->staticBody = true;	// 
				pFloorPanel->bIsWireframe = false;

				//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
				//	pFloorPanel->bodyDef->position = glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);

				pFloorPanel->scale = 1.2f;	// Unit sphere (0.5 units raduis) 
											//std::string tempMeshName = it->child("Mesh").attribute("name").value();
				pFloorPanel->meshName = "Cube2.ply";// it->child("Mesh").attribute("name").value();
													//pWizard->meshName = "Cube2.ply"; 
				pFloorPanel->lowDetailMesh = "Cube_1x1x1_N.ply";
				pFloorPanel->usesLowDetailMesh = true;

				pFloorPanel->bUseDebugColours = false;
				pFloorPanel->debugRGBA = glm::vec4(0.f, 0.f, 1.f, 1.f);
				pFloorPanel->texture00Name = "GridSmall.bmp";
				pFloorPanel->bIsVisible = true;

				nPhysics::cRigidBodyDef bodyDef;
				bodyDef.position = glm::vec3((float)row + row*.5f, 18.f, float(col) + col*.5f);// glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);
				bodyDef.staticBody = true;
				bodyDef.mass = 1.0f;
				bodyDef.velocity = glm::vec3(0.f);
				bodyDef.force = glm::vec3(0.f);
				bodyDef.qOrientation = glm::quat(glm::vec3(0, 0, 0));
				//		bodyDef.qOrientation = glm::quat();

				/*nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());*/
				nPhysics::iShape* capsuleShape = physicsFactory->CreateShape(nPhysics::sBox());
				//nPhysics::iShape* cubeShape = physicsFactory->CreateShape()
				pFloorPanel->rigidBody = physicsFactory->CreateRigidBody(bodyDef, capsuleShape);
				pFloorPanel->bHasRigidBody = true;
				nPhysics::cRigidBodyDef* bodyDefCheck;
				bodyDefCheck = pFloorPanel->rigidBody->GetRigidBodyDef();
				//bodyDefCheck->qOrientation = glm::quat(glm::vec3(0, -1, -45));
				pFloorPanel->bodyDef = bodyDefCheck;
				//pViper->bIsImposter = true;
				//	camera1->AttachToGameObject(pFloorPanel);



				::g_vec_pGOs.push_back(pFloorPanel);
				physicsWorld->AddBody(pFloorPanel->rigidBody);

				startCount++;
			}
			else if ((int)p_theImages[pixelCount].redPixel == 0 && (int)p_theImages[pixelCount].greenPixel == 0 && (int)p_theImages[pixelCount].bluePixel == 255)
			{
				cGameObject* pFloorPanel = new cGameObject();
				pFloorPanel->bIsVisible = true;
				pFloorPanel->bodyDef->staticBody = true;	// 
				pFloorPanel->bIsWireframe = false;

				//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
				//	pFloorPanel->bodyDef->position = glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);

				pFloorPanel->scale = 1.2f;	// Unit sphere (0.5 units raduis) 
											//std::string tempMeshName = it->child("Mesh").attribute("name").value();
				pFloorPanel->meshName = "Cube2.ply";// it->child("Mesh").attribute("name").value();
													//pWizard->meshName = "Cube2.ply"; 
				pFloorPanel->lowDetailMesh = "Cube_1x1x1_N.ply";
				pFloorPanel->usesLowDetailMesh = true;

				pFloorPanel->bUseDebugColours = false;
				pFloorPanel->debugRGBA = glm::vec4(0.f, 0.f, 1.f, 1.f);
				pFloorPanel->texture00Name = "GridSmall.bmp";
				pFloorPanel->bIsVisible = true;

				nPhysics::cRigidBodyDef bodyDef;
				bodyDef.position = glm::vec3((float)row + row*.5f, 18.f, float(col) + col*.5f);// glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);
				bodyDef.staticBody = true;
				bodyDef.mass = 1.0f;
				bodyDef.velocity = glm::vec3(0.f);
				bodyDef.force = glm::vec3(0.f);
				bodyDef.qOrientation = glm::quat(glm::vec3(0, 0, 0));
				//		bodyDef.qOrientation = glm::quat();

				/*nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());*/
				nPhysics::iShape* capsuleShape = physicsFactory->CreateShape(nPhysics::sBox());
				//nPhysics::iShape* cubeShape = physicsFactory->CreateShape()
				pFloorPanel->rigidBody = physicsFactory->CreateRigidBody(bodyDef, capsuleShape);
				pFloorPanel->bHasRigidBody = true;
				nPhysics::cRigidBodyDef* bodyDefCheck;
				bodyDefCheck = pFloorPanel->rigidBody->GetRigidBodyDef();
				//bodyDefCheck->qOrientation = glm::quat(glm::vec3(0, -1, -45));
				pFloorPanel->bodyDef = bodyDefCheck;
				//pViper->bIsImposter = true;
				//	camera1->AttachToGameObject(pFloorPanel);



				::g_vec_pGOs.push_back(pFloorPanel);
				physicsWorld->AddBody(pFloorPanel->rigidBody);

				//GoalPanel* pStart = new GoalPanel();
				//pStart->SetPosition(glm::vec3((float)row, 0.f, float(col)));
				//g_vec_pMaze.push_back(pStart);

				::gPlayer->bodyDef->position = glm::vec3(5.f*col, 20.f, 5.f*row);
				//glm::mat4 playerMat = glm::mat4();
				//playerMat[3].x = 5.f*col;
				//playerMat[3].y = 0.f;
				//playerMat[3].z = 5.f*row;
				//player->SetMatrix(playerMat);
				startCount++;
			}
			else
			{
				std::cout << pixelCount << ": Red: " << (int)p_theImages[pixelCount].redPixel << std::endl;
				std::cout << pixelCount << ": Green: " << (int)p_theImages[pixelCount].greenPixel << std::endl;
				std::cout << pixelCount << ": Blue: " << (int)p_theImages[pixelCount].bluePixel << std::endl;
			}



			pixelCount++;
			bytesRead += 3;	// Add three bytes
		}// for (unsigned long col





		//std::cout << "Floor Count: " << floorCount << std::endl;
		//	std::cout << "Wall Count: " << wallCount << std::endl;
		// We are at the end of the row and there will 
		//	still be bytes padding up to a multiple of 4...
		// Read and discard these...
		for (int discardedBytes = 0; discardedBytes < numberOfPaddingBytes; discardedBytes++)
		{
			char tempChar = ReadNextChar(pRawData, curIndex);
		}
		// How much have we read?
		// NOTE: This really isn't percent, but it does the job of 
		//	printing out fewer dots.
		int percentRead = static_cast<int>((static_cast<double>(bytesRead) / static_cast<double>(totalBytesInFile)) * 100);
		if ((percentRead % 25) == 0)
		{
			if (true)
			{
				std::cout << ".";
			}
		}
	}// for (unsigned long row...
	if (true)
	{
		std::cout << "Floor Count: " << floorCount << std::endl;
		std::cout << "Wall Count: " << wallCount << std::endl;
		std::cout << "Start Count: " << startCount << std::endl;
		std::cout << "Goal Count: " << goalCount << std::endl;
		std::cout << "complete." << std::endl;
	}

	cGameObject* pFloorPanelnew = new cGameObject();
	pFloorPanelnew->bIsVisible = true;
	pFloorPanelnew->bodyDef->staticBody = true;	// 
	pFloorPanelnew->bIsWireframe = false;

	//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
	//	pFloorPanel->bodyDef->position = glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);

	pFloorPanelnew->scale = 30.5f;	// Unit sphere (0.5 units raduis) 
									//std::string tempMeshName = it->child("Mesh").attribute("name").value();
	pFloorPanelnew->meshName = "Cube2.ply";// it->child("Mesh").attribute("name").value();
										   //pWizard->meshName = "Cube2.ply"; 
	pFloorPanelnew->bUseDebugColours = false;
	pFloorPanelnew->debugRGBA = glm::vec4(0.f, 0.f, 1.f, 1.f);
	pFloorPanelnew->texture00Name = "GridSmall.bmp";
	pFloorPanelnew->bIsVisible = true;

	nPhysics::cRigidBodyDef bodyDefNew;
	bodyDefNew.position = glm::vec3(15.f, 3.f, 15.f);
	bodyDefNew.staticBody = true;
	bodyDefNew.mass = 1.0f;
	bodyDefNew.velocity = glm::vec3(0.f);
	bodyDefNew.force = glm::vec3(0.f);
	bodyDefNew.qOrientation = glm::quat(glm::vec3(0, 0, 0));
	//		bodyDef.qOrientation = glm::quat();

	/*nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());*/
	nPhysics::iShape* capsuleShape = physicsFactory->CreateShape(nPhysics::sBox());
	//nPhysics::iShape* cubeShape = physicsFactory->CreateShape()
	pFloorPanelnew->rigidBody = physicsFactory->CreateRigidBody(bodyDefNew, capsuleShape);
	pFloorPanelnew->bHasRigidBody = true;
	nPhysics::cRigidBodyDef* bodyDefCheckNew;
	bodyDefCheckNew = pFloorPanelnew->rigidBody->GetRigidBodyDef();
	//bodyDefCheck->qOrientation = glm::quat(glm::vec3(0, -1, -45));
	pFloorPanelnew->bodyDef = bodyDefCheckNew;
	//pViper->bIsImposter = true;
	//	camera1->AttachToGameObject(pFloorPanel);



	::g_vec_pGOs.push_back(pFloorPanelnew);
	//	physicsWorld->AddBody(pFloorPanel->rigidBody);




	{	// Add a "debug sphere"
		cGameObject* pSphere = new cGameObject();
		pSphere->bodyDef->staticBody = false;	// 
		pSphere->bIsWireframe = true;
		// Pick a semi-random location
		//		pSphere->x = 0.0f;	pSphere->y = 0.0f;	pSphere->z = 0.0f;
		pSphere->bodyDef->position = glm::vec3(0.0f, 0.0f, 0.0f);

		pSphere->scale = 1.0f;	// Unit sphere (0.5 units raduis)

								//	pSphere->meshName = "Sphere_InvertedNormals.ply";
		pSphere->meshName = "Isoshphere_xyz_InvertedNormals.ply";

		pSphere->bIsVisible = false;

		::g_DebugSphereID = pSphere->getID();
		::g_vec_pGOs.push_back(pSphere);
	}


	//g_vec_pMaze


	// BMP file is read....
}






bool LoadBMP2(std::string fileName)
{
	std::ifstream theFile;
	//	if (this->m_bHave_cout_output)
	//	{
	std::cout << "Reading texture file: " << fileName;
	//	}
	theFile.open(fileName.c_str(), std::ios_base::binary);
	if (!theFile)
	{
		//	this->m_lastErrorNum = CTextureFromBMP::ERORR_FILE_WONT_OPEN;
		std::cout << "Something bad happen" << std::endl;
		return false;
	}
	// Scan to the end to get the size
	theFile.seekg(0, std::ios::end);
	std::ios::pos_type fileSize = theFile.tellg();
	// Return to start
	theFile.seekg(0, std::ios::beg);

	char* pRawData = new char[static_cast<unsigned int>(fileSize)];

	theFile.read(pRawData, fileSize);
	theFile.close();

	// Now go through and decode the BMP file.
	unsigned long curIndex = 0;
	char letter1 = ReadNextChar(pRawData, curIndex);
	char letter2 = ReadNextChar(pRawData, curIndex);
	if ((letter1 != 'B') && (letter2 != 'M'))
	{
		//this->m_lastErrorNum = CTextureFromBMP::ERROR_NOT_A_BMP_FILE;
		std::cout << "Something else bad happen" << std::endl;
		return false;
	}

	unsigned long height;
	unsigned long width;
	unsigned long originalHeight;
	unsigned long originalWidth;
	unsigned short reserved1;
	unsigned short reserved2;
	unsigned long fileSize2;
	unsigned long offsetInBits;
	unsigned long headerSize;
	unsigned long numberOfRows;
	unsigned long numberOfColumns;
	unsigned short numberOfPlanes;
	unsigned short bitPerPixel;	// 1, 4, 8, 15, 24, 32, or 64
	unsigned long compressionMode;	// 0 if NOT compressed (i.e. 24 bit bitmap)
	unsigned long imageSizeInBytes;	// Total bytes in image - may be zero
	unsigned long PixelsPerMeterX;
	unsigned long PixelsPerMeterY;
	unsigned long numberOfLookUpTableEntries;	// Could be 0 for 24 bit format
	unsigned long numberOfImportantColours;	// 0 if all colours are important




											// File is BMP
	fileSize2 = ReadNextUnsignedLong(pRawData, curIndex);
	reserved1 = ReadNextUnsignedShort(pRawData, curIndex);
	reserved2 = ReadNextUnsignedShort(pRawData, curIndex);
	offsetInBits = ReadNextUnsignedLong(pRawData, curIndex);
	headerSize = ReadNextUnsignedLong(pRawData, curIndex);
	numberOfRows = ReadNextUnsignedLong(pRawData, curIndex);
	height = originalHeight = numberOfRows;
	numberOfColumns = ReadNextUnsignedLong(pRawData, curIndex);
	width = originalWidth = numberOfColumns;
	numberOfPlanes = ReadNextUnsignedShort(pRawData, curIndex);
	bitPerPixel = ReadNextUnsignedShort(pRawData, curIndex);
	std::cout << "bitPerPixel: " << bitPerPixel << std::endl;
	// Is is a 24 bit bitmap?
	if (bitPerPixel != 24)
	{
		//this->m_lastErrorNum = CTextureFromBMP::ERROR_NOT_A_24_BIT_BITMAP;
		std::cout << "Not 24 bit bitmap" << std::endl;
		return false;
	}
	C24BitBMPpixel* p_theImages;
	compressionMode = ReadNextUnsignedLong(pRawData, curIndex);
	imageSizeInBytes = ReadNextUnsignedLong(pRawData, curIndex);
	PixelsPerMeterX = ReadNextUnsignedLong(pRawData, curIndex);
	PixelsPerMeterY = ReadNextUnsignedLong(pRawData, curIndex);
	numberOfLookUpTableEntries = ReadNextUnsignedLong(pRawData, curIndex);
	numberOfImportantColours = ReadNextUnsignedLong(pRawData, curIndex);

	// 
	long bytesPerRow = ((3 * numberOfRows + 3) / 4) * 4;
	long numberOfPaddingBytes = bytesPerRow - 3 * numberOfColumns;

	// Allocate enough space...
	p_theImages = new C24BitBMPpixel[numberOfRows * numberOfColumns];

	// Did we run out of memory?
	if (!p_theImages)
	{
		//this->m_lastErrorNum = CTextureFromBMP::ERROR_NOT_ENOUGHT_MEMORY_FOR_BITMAP;
		std::cout << "No memory badness" << std::endl;
		return false;
	}


	// Read the bitmap into memory...
	unsigned long bytesRead = 0;
	unsigned long totalBytesInFile = numberOfRows * numberOfColumns;
	unsigned long pixelCount = 0;

	//cGameObject* pFloorPanel = new cGameObject();
	//pFloorPanel->bIsVisible = true;
	//pFloorPanel->bodyDef->staticBody = true;	// 
	//pFloorPanel->bIsWireframe = false;
	
	//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
	//	pFloorPanel->bodyDef->position = glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);

	//pFloorPanel->scale = 4.f;	// Unit sphere (0.5 units raduis) 
								//std::string tempMeshName = it->child("Mesh").attribute("name").value();
	//pFloorPanel->meshName = "Cube2.ply";// it->child("Mesh").attribute("name").value();
	//									pWizard->meshName = "Cube2.ply"; 
	//pFloorPanel->lowDetailMesh = "Cube_1x1x1_N.ply";
	//pFloorPanel->usesLowDetailMesh = true;
	//pFloorPanel->bUseDebugColours = true;
	//pFloorPanel->debugRGBA = glm::vec4(1.f, 1.f, 1.f, 1.f);
	//pFloorPanel->texture00Name = "GridSmall.bmp";
	//pFloorPanel->bIsVisible = true;
	//pFloorPanel->bodyDef->position= glm::vec3((float)numberOfRows*.75f, 18.f, float(numberOfColumns)*.75f);
	//nPhysics::cRigidBodyDef bodyDefFloor;

	//bodyDefFloor.position = glm::vec3((float)numberOfRows*.75f , 18.f, float(numberOfColumns)*.75f);// glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);
	//bodyDefFloor.staticBody = true;
	//bodyDefFloor.mass = 1.0f;
	//bodyDefFloor.velocity = glm::vec3(0.f);
	//bodyDefFloor.force = glm::vec3(0.f);
	//bodyDefFloor.qOrientation = glm::quat(glm::vec3(0, 0, 0));
	////		bodyDef.qOrientation = glm::quat();
	//
	///*nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());*/
	//nPhysics::iShape* boxShape = physicsFactory->CreateShape(nPhysics::sBox((float)numberOfRows*1.5f, float(numberOfColumns)*1.5f,2.f));
	////nPhysics::iShape* cubeShape = physicsFactory->CreateShape()
	//nPhysics::iRigidBody* rigidBody = physicsFactory->CreateRigidBody(bodyDefFloor, boxShape);
	//pFloorPanel->bHasRigidBody = true;
	//nPhysics::cRigidBodyDef* bodyDefCheck;
	//bodyDefCheck = pFloorPanel->rigidBody->GetRigidBodyDef();
	//bodyDefCheck->qOrientation = glm::quat(glm::vec3(0, -1, -45));
	//pFloorPanel->bodyDef = bodyDefCheck;
	//pViper->bIsImposter = true;
	//	camera1->AttachToGameObject(pFloorPanel);



	//::g_vec_pGOs.push_back(pFloorPanel);
	//physicsWorld->AddBody(rigidBody);




	int floorCount = 0;
	int wallCount = 0;
	int startCount = 0;
	int goalCount = 0;
	for (unsigned long row = 0; row < numberOfRows; row++)
	{
		for (unsigned long col = 0; col < numberOfColumns; col++)
		{
			char theblue = ReadNextChar(pRawData, curIndex);
			char thegreen = ReadNextChar(pRawData, curIndex);
			char thered = ReadNextChar(pRawData, curIndex);

			p_theImages[pixelCount].redPixel = static_cast<unsigned char>(thered);
			p_theImages[pixelCount].greenPixel = static_cast<unsigned char>(thegreen);
			p_theImages[pixelCount].bluePixel = static_cast<unsigned char>(theblue);

			/*	std::cout << pixelCount<<": Red: " << (int) p_theImages[pixelCount].redPixel << std::endl;
			std::cout << pixelCount << ": Green: " << (int)p_theImages[pixelCount].greenPixel << std::endl;
			std::cout << pixelCount << ": Blue: " << (int)p_theImages[pixelCount].bluePixel << std::endl;*/
			//m_p_theImages[pixelCount].alphaPixel = 255;
			if ((int)p_theImages[pixelCount].redPixel>220 && (int)p_theImages[pixelCount].greenPixel>220 && (int)p_theImages[pixelCount].bluePixel > 220)
			{


				//cGameObject* pFloorPanel = new cGameObject();
				//pFloorPanel->bIsVisible = true;
				//pFloorPanel->bodyDef->staticBody = true;	// 
				//pFloorPanel->bIsWireframe = false;
				//
				////p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
				////	pFloorPanel->bodyDef->position = glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);

				//pFloorPanel->scale = 1.2f;	// Unit sphere (0.5 units raduis) 
				//							//std::string tempMeshName = it->child("Mesh").attribute("name").value();
				//pFloorPanel->meshName = "Cube2.ply";// it->child("Mesh").attribute("name").value();
				//									//pWizard->meshName = "Cube2.ply"; 
				//pFloorPanel->bUseDebugColours = false;
				//pFloorPanel->debugRGBA = glm::vec4(0.f, 0.f, 1.f, 1.f);
				//pFloorPanel->texture00Name = "GridSmall.bmp";
				//pFloorPanel->bIsVisible = true;

				//nPhysics::cRigidBodyDef bodyDef;
				//bodyDef.position = glm::vec3((float)row + row*.5f, 18.f, float(col) + col*.5f);// glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);
				//bodyDef.staticBody = true;
				//bodyDef.mass = 1.0f;
				//bodyDef.velocity = glm::vec3(0.f);
				//bodyDef.force = glm::vec3(0.f);
				//bodyDef.qOrientation = glm::quat(glm::vec3(0, 4, 0));
				////		bodyDef.qOrientation = glm::quat();

				///*nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());*/
				//nPhysics::iShape* capsuleShape = physicsFactory->CreateShape(nPhysics::sBox());
				////nPhysics::iShape* cubeShape = physicsFactory->CreateShape()
				//pFloorPanel->rigidBody = physicsFactory->CreateRigidBody(bodyDef, capsuleShape);
				//pFloorPanel->bHasRigidBody = true;
				//nPhysics::cRigidBodyDef* bodyDefCheck;
				//bodyDefCheck = pFloorPanel->rigidBody->GetRigidBodyDef();
				////bodyDefCheck->qOrientation = glm::quat(glm::vec3(0, -1, -45));
				//pFloorPanel->bodyDef = bodyDefCheck;
				////pViper->bIsImposter = true;
				////	camera1->AttachToGameObject(pFloorPanel);



				//::g_vec_pGOs.push_back(pFloorPanel);
				//physicsWorld->AddBody(pFloorPanel->rigidBody);




				floorCount++;
			}
			else if ((int)p_theImages[pixelCount].redPixel == 0 && (int)p_theImages[pixelCount].greenPixel == 0 && (int)p_theImages[pixelCount].bluePixel == 0)
			{
				cGameObject* pFloorPanel = new cGameObject();
				pFloorPanel->bIsVisible = true;
				pFloorPanel->bodyDef->staticBody = true;	// 
				pFloorPanel->bIsWireframe = false;

				//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
				//	pFloorPanel->bodyDef->position = glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);

				pFloorPanel->scale = 1.2f;	// Unit sphere (0.5 units raduis) 
											//std::string tempMeshName = it->child("Mesh").attribute("name").value();
				pFloorPanel->meshName = "Cube2.ply";// it->child("Mesh").attribute("name").value();
													//pWizard->meshName = "Cube2.ply"; 
				pFloorPanel->lowDetailMesh = "Cube_1x1x1_N.ply";
				pFloorPanel->usesLowDetailMesh = true;

				pFloorPanel->bUseDebugColours = false;
				pFloorPanel->debugRGBA = glm::vec4(0.f, 0.f, 1.f, 1.f);
				pFloorPanel->texture00Name = "GridSmall.bmp";
				pFloorPanel->bIsVisible = true;

				nPhysics::cRigidBodyDef bodyDef;
				bodyDef.position = glm::vec3((float)row + row*.5f, 19.f, float(col) + col*.5f);// glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);
				bodyDef.staticBody = true;
				bodyDef.mass = 1.0f;
				bodyDef.velocity = glm::vec3(0.f);
				bodyDef.force = glm::vec3(0.f);
				bodyDef.qOrientation = glm::quat(glm::vec3(0, 0, 0));
				//		bodyDef.qOrientation = glm::quat();

				/*nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());*/
				nPhysics::iShape* capsuleShape = physicsFactory->CreateShape(nPhysics::sBox());
				//nPhysics::iShape* cubeShape = physicsFactory->CreateShape()
				pFloorPanel->rigidBody = physicsFactory->CreateRigidBody(bodyDef, capsuleShape);
				pFloorPanel->bHasRigidBody = true;
				nPhysics::cRigidBodyDef* bodyDefCheck;
				bodyDefCheck = pFloorPanel->rigidBody->GetRigidBodyDef();
				//bodyDefCheck->qOrientation = glm::quat(glm::vec3(0, -1, -45));
				pFloorPanel->bodyDef = bodyDefCheck;
				//pViper->bIsImposter = true;
				//	camera1->AttachToGameObject(pFloorPanel);



				::g_vec_pGOs.push_back(pFloorPanel);
				physicsWorld->AddBody(pFloorPanel->rigidBody);

				wallCount++;
			}
			else if ((int)p_theImages[pixelCount].redPixel == 0 && (int)p_theImages[pixelCount].greenPixel == 128 && (int)p_theImages[pixelCount].bluePixel == 128)
			{
				cGameObject* pFloorPanel = new cGameObject();
				pFloorPanel->bIsVisible = true;
				pFloorPanel->bodyDef->staticBody = true;	// 
				pFloorPanel->bIsWireframe = false;

				//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
				//	pFloorPanel->bodyDef->position = glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);

				pFloorPanel->scale = 1.2f;	// Unit sphere (0.5 units raduis) 
											//std::string tempMeshName = it->child("Mesh").attribute("name").value();
				pFloorPanel->meshName = "Cube2.ply";// it->child("Mesh").attribute("name").value();
													//pWizard->meshName = "Cube2.ply"; 
				pFloorPanel->lowDetailMesh = "Cube_1x1x1_N.ply";
				pFloorPanel->usesLowDetailMesh = true;

				pFloorPanel->bUseDebugColours = false;
				pFloorPanel->debugRGBA = glm::vec4(0.f, 0.f, 1.f, 1.f);
				pFloorPanel->texture00Name = "GridSmall.bmp";
				pFloorPanel->bIsVisible = true;

				nPhysics::cRigidBodyDef bodyDef;
				bodyDef.position = glm::vec3((float)row + row*.5f, 18.f, float(col) + col*.5f);// glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);
				bodyDef.staticBody = true;
				bodyDef.mass = 1.0f;
				bodyDef.velocity = glm::vec3(0.f);
				bodyDef.force = glm::vec3(0.f);
				bodyDef.qOrientation = glm::quat(glm::vec3(0, 0, 0));
				//		bodyDef.qOrientation = glm::quat();

				/*nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());*/
				nPhysics::iShape* capsuleShape = physicsFactory->CreateShape(nPhysics::sBox());
				//nPhysics::iShape* cubeShape = physicsFactory->CreateShape()
				pFloorPanel->rigidBody = physicsFactory->CreateRigidBody(bodyDef, capsuleShape);
				pFloorPanel->bHasRigidBody = true;
				nPhysics::cRigidBodyDef* bodyDefCheck;
				bodyDefCheck = pFloorPanel->rigidBody->GetRigidBodyDef();
				//bodyDefCheck->qOrientation = glm::quat(glm::vec3(0, -1, -45));
				pFloorPanel->bodyDef = bodyDefCheck;
				//pViper->bIsImposter = true;
				//	camera1->AttachToGameObject(pFloorPanel);



				::g_vec_pGOs.push_back(pFloorPanel);
				physicsWorld->AddBody(pFloorPanel->rigidBody);

				startCount++;
			}
			else if ((int)p_theImages[pixelCount].redPixel == 0 && (int)p_theImages[pixelCount].greenPixel == 0 && (int)p_theImages[pixelCount].bluePixel == 255)
			{
				cGameObject* pFloorPanel = new cGameObject();
				pFloorPanel->bIsVisible = true;
				pFloorPanel->bodyDef->staticBody = true;	// 
				pFloorPanel->bIsWireframe = false;

				//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
				//	pFloorPanel->bodyDef->position = glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);

				pFloorPanel->scale = 1.2f;	// Unit sphere (0.5 units raduis) 
											//std::string tempMeshName = it->child("Mesh").attribute("name").value();
				pFloorPanel->meshName = "Cube2.ply";// it->child("Mesh").attribute("name").value();
													//pWizard->meshName = "Cube2.ply"; 
				pFloorPanel->lowDetailMesh = "Cube_1x1x1_N.ply";
				pFloorPanel->usesLowDetailMesh = true;

				pFloorPanel->bUseDebugColours = false;
				pFloorPanel->debugRGBA = glm::vec4(0.f, 0.f, 1.f, 1.f);
				pFloorPanel->texture00Name = "GridSmall.bmp";
				pFloorPanel->bIsVisible = true;

				nPhysics::cRigidBodyDef bodyDef;
				bodyDef.position = glm::vec3((float)row + row*.5f + numberOfRows*1.5f, 18.f, float(col) + col*.5f);// glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);
				bodyDef.staticBody = true;
				bodyDef.mass = 1.0f;
				bodyDef.velocity = glm::vec3(0.f);
				bodyDef.force = glm::vec3(0.f);
				bodyDef.qOrientation = glm::quat(glm::vec3(0, 0, 0));
				//		bodyDef.qOrientation = glm::quat();

				/*nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());*/
				nPhysics::iShape* capsuleShape = physicsFactory->CreateShape(nPhysics::sBox());
				//nPhysics::iShape* cubeShape = physicsFactory->CreateShape()
				pFloorPanel->rigidBody = physicsFactory->CreateRigidBody(bodyDef, capsuleShape);
				pFloorPanel->bHasRigidBody = true;
				nPhysics::cRigidBodyDef* bodyDefCheck;
				bodyDefCheck = pFloorPanel->rigidBody->GetRigidBodyDef();
				//bodyDefCheck->qOrientation = glm::quat(glm::vec3(0, -1, -45));
				pFloorPanel->bodyDef = bodyDefCheck;
				//pViper->bIsImposter = true;
				//	camera1->AttachToGameObject(pFloorPanel);



				::g_vec_pGOs.push_back(pFloorPanel);
				physicsWorld->AddBody(pFloorPanel->rigidBody);

				//GoalPanel* pStart = new GoalPanel();
				//pStart->SetPosition(glm::vec3((float)row, 0.f, float(col)));
				//g_vec_pMaze.push_back(pStart);

				::gPlayer->bodyDef->position = glm::vec3(5.f*col, 20.f, 5.f*row);
				//glm::mat4 playerMat = glm::mat4();
				//playerMat[3].x = 5.f*col;
				//playerMat[3].y = 0.f;
				//playerMat[3].z = 5.f*row;
				//player->SetMatrix(playerMat);
				startCount++;
			}
			else
			{
				std::cout << pixelCount << ": Red: " << (int)p_theImages[pixelCount].redPixel << std::endl;
				std::cout << pixelCount << ": Green: " << (int)p_theImages[pixelCount].greenPixel << std::endl;
				std::cout << pixelCount << ": Blue: " << (int)p_theImages[pixelCount].bluePixel << std::endl;
			}




			pixelCount++;
			bytesRead += 3;	// Add three bytes
		}// for (unsigned long col





		//std::cout << "Floor Count: " << floorCount << std::endl;
		//	std::cout << "Wall Count: " << wallCount << std::endl;
		// We are at the end of the row and there will 
		//	still be bytes padding up to a multiple of 4...
		// Read and discard these...
		for (int discardedBytes = 0; discardedBytes < numberOfPaddingBytes; discardedBytes++)
		{
			char tempChar = ReadNextChar(pRawData, curIndex);
		}
		// How much have we read?
		// NOTE: This really isn't percent, but it does the job of 
		//	printing out fewer dots.
		int percentRead = static_cast<int>((static_cast<double>(bytesRead) / static_cast<double>(totalBytesInFile)) * 100);
		if ((percentRead % 25) == 0)
		{
			if (true)
			{
				std::cout << ".";
			}
		}
	}// for (unsigned long row...
	if (true)
	{
		std::cout << "Floor Count: " << floorCount << std::endl;
		std::cout << "Wall Count: " << wallCount << std::endl;
		std::cout << "Start Count: " << startCount << std::endl;
		std::cout << "Goal Count: " << goalCount << std::endl;
		std::cout << "complete." << std::endl;
	}

	cGameObject* pFloorPanelnew = new cGameObject();
	pFloorPanelnew->bIsVisible = true;
	pFloorPanelnew->bodyDef->staticBody = true;	// 
	pFloorPanelnew->bIsWireframe = false;

	//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
	//	pFloorPanel->bodyDef->position = glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);

	pFloorPanelnew->scale = 30.5f;	// Unit sphere (0.5 units raduis) 
								//std::string tempMeshName = it->child("Mesh").attribute("name").value();
	pFloorPanelnew->meshName = "Cube2.ply";// it->child("Mesh").attribute("name").value();
										//pWizard->meshName = "Cube2.ply"; 
	pFloorPanelnew->bUseDebugColours = false;
	pFloorPanelnew->debugRGBA = glm::vec4(0.f, 0.f, 1.f, 1.f);
	pFloorPanelnew->texture00Name = "GridSmall.bmp";
	pFloorPanelnew->bIsVisible = true;

	nPhysics::cRigidBodyDef bodyDefNew;
	bodyDefNew.position = glm::vec3(15.f + numberOfRows*1.5f + 4.f, 3.f, 15.f);
	bodyDefNew.staticBody = true;
	bodyDefNew.mass = 1.0f;
	bodyDefNew.velocity = glm::vec3(0.f);
	bodyDefNew.force = glm::vec3(0.f);
	bodyDefNew.qOrientation = glm::quat(glm::vec3(0, 0, 0));
	//		bodyDef.qOrientation = glm::quat();

	/*nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());*/
	nPhysics::iShape* capsuleShape = physicsFactory->CreateShape(nPhysics::sBox());
	//nPhysics::iShape* cubeShape = physicsFactory->CreateShape()
	pFloorPanelnew->rigidBody = physicsFactory->CreateRigidBody(bodyDefNew, capsuleShape);
	pFloorPanelnew->bHasRigidBody = true;
	nPhysics::cRigidBodyDef* bodyDefCheckNew;
	bodyDefCheckNew = pFloorPanelnew->rigidBody->GetRigidBodyDef();
	//bodyDefCheck->qOrientation = glm::quat(glm::vec3(0, -1, -45));
	pFloorPanelnew->bodyDef = bodyDefCheckNew;
	//pViper->bIsImposter = true;
	//	camera1->AttachToGameObject(pFloorPanel);



	::g_vec_pGOs.push_back(pFloorPanelnew);
//	physicsWorld->AddBody(pFloorPanel->rigidBody);




	{	// Add a "debug sphere"
		cGameObject* pSphere = new cGameObject();
		pSphere->bodyDef->staticBody= false;	// 
		pSphere->bIsWireframe = true;
		// Pick a semi-random location
		//		pSphere->x = 0.0f;	pSphere->y = 0.0f;	pSphere->z = 0.0f;
		pSphere->bodyDef->position = glm::vec3(0.0f, 0.0f, 0.0f);

		pSphere->scale = 1.0f;	// Unit sphere (0.5 units raduis)

								//	pSphere->meshName = "Sphere_InvertedNormals.ply";
		pSphere->meshName = "Isoshphere_xyz_InvertedNormals.ply";

		pSphere->bIsVisible = false;

		::g_DebugSphereID = pSphere->getID();
		::g_vec_pGOs.push_back(pSphere);
	}


	//g_vec_pMaze


	// BMP file is read....
}


bool LoadEnemies()
{
	
	gEnemy1 = new cGameObject();
	gEnemy1->bIsVisible = true;
	gEnemy1->bodyDef->staticBody = false;	// 
	gEnemy1->bIsWireframe = false;
	//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
	//pFireBall->bodyDef->position = glm::vec3(0.0f, 0.0f, 0.0f);

	gEnemy1->scale = .40f;	// Unit sphere (0.5 units raduis)
	gEnemy1->meshName = "Isoshphere_xyz_InvertedNormals.ply";
	gEnemy1->bUseDebugColours = true;
	gEnemy1->debugRGBA = glm::vec4(1.f, 1.f, 1.f, 1.f);
	//gravityBall->texture00Name = "GravityBall.bmp";
	gEnemy1->bIsVisible = true;
	gEnemy1->bHasRigidBody = true;
	//gravityBall->amountReflective = 0.5f;
	//gravityBall->amountRefractive = 0.1f;
	nPhysics::cRigidBodyDef bodyDef;
	gEnemy1->hasUpdateFunction = true;

	bodyDef.position = glm::vec3(18.f,20.f,5.f);
	bodyDef.staticBody = false;
	bodyDef.mass = 1.0f;

	bodyDef.velocity = glm::vec3(0.f);// -glm::vec3(0.f, 0.f, 20.f)*gPlayer->bodyDef->qOrientation;
	bodyDef.force = glm::vec3(0.f);
	//	bodyDef.qOrientation = glm::quat();


	nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere(.4f));
	enemyBody = physicsFactory->CreateRigidBody(bodyDef, sphereShape);

	nPhysics::cRigidBodyDef* bodyDefCheck;
	bodyDefCheck = enemyBody->GetRigidBodyDef();

	gEnemy1->bodyDef = bodyDefCheck;

	::g_vec_pGOs.push_back(gEnemy1);
	physicsWorld->AddBody(enemyBody);




	gEnemy2 = new cGameObject();
	gEnemy2->bIsVisible = true;
	gEnemy2->bodyDef->staticBody = false;	// 
	gEnemy2->bIsWireframe = false;
	//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
	//pFireBall->bodyDef->position = glm::vec3(0.0f, 0.0f, 0.0f);

	gEnemy2->scale = .40f;	// Unit sphere (0.5 units raduis)
	gEnemy2->meshName = "Isoshphere_xyz_InvertedNormals.ply";
	gEnemy2->bUseDebugColours = true;
	gEnemy2->debugRGBA = glm::vec4(1.f, 1.f, 1.f, 1.f);
	//gravityBall->texture00Name = "GravityBall.bmp";
	gEnemy2->bIsVisible = true;
	gEnemy2->bHasRigidBody = true;
	//gravityBall->amountReflective = 0.5f;
	//gravityBall->amountRefractive = 0.1f;
	nPhysics::cRigidBodyDef bodyDef2;
	gEnemy2->hasUpdateFunction = true;

	bodyDef2.position = glm::vec3(40.f, 20.f, 17.5f);
	bodyDef2.staticBody = false;
	bodyDef2.mass = 1.0f;

	bodyDef2.velocity = glm::vec3(0.f);// -glm::vec3(0.f, 0.f, 20.f)*gPlayer->bodyDef->qOrientation;
	bodyDef2.force = glm::vec3(0.f);
	//	bodyDef.qOrientation = glm::quat();


	nPhysics::iShape* sphereShape2 = physicsFactory->CreateShape(nPhysics::sSphere(.4f));
	enemyBody2 = physicsFactory->CreateRigidBody(bodyDef2, sphereShape2);

	nPhysics::cRigidBodyDef* bodyDefCheck2;
	bodyDefCheck2 = enemyBody2->GetRigidBodyDef();

	gEnemy2->bodyDef = bodyDefCheck2;

	::g_vec_pGOs.push_back(gEnemy2);
	physicsWorld->AddBody(enemyBody2);











	//pViper->bIsImposter = true;
	return true;
}

bool LoadObjectives()
{
	
	//cGameObject* pFloorPanel 
	objectiveMarker = new cGameObject();
	objectiveMarker->bIsVisible = true;
	objectiveMarker->bodyDef->staticBody = true;	// 
	objectiveMarker->bIsWireframe = false;

	//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
	//	pFloorPanel->bodyDef->position = glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);

	objectiveMarker->scale = 1.2f;	// Unit sphere (0.5 units raduis) 
								//std::string tempMeshName = it->child("Mesh").attribute("name").value();
	objectiveMarker->meshName = "AztecCube2.ply";// it->child("Mesh").attribute("name").value();
										//pWizard->meshName = "Cube2.ply"; 
	objectiveMarker->lowDetailMesh = "Cube_1x1x1_N.ply";
	objectiveMarker->usesLowDetailMesh = true;

	objectiveMarker->bUseDebugColours = false;
	objectiveMarker->debugRGBA = glm::vec4(0.f, 0.f, 1.f, 1.f);
	objectiveMarker->texture00Name = "GridSmall.bmp";
	objectiveMarker->bIsVisible = true;

	nPhysics::cRigidBodyDef bodyDef;
	bodyDef.position = glm::vec3(50.f, 19.f, 7.f);
	bodyDef.staticBody = true;
	bodyDef.mass = 1.0f;
	bodyDef.velocity = glm::vec3(0.f);
	bodyDef.force = glm::vec3(0.f);
	bodyDef.qOrientation = glm::quat(glm::vec3(0, 0, 0));
	//		bodyDef.qOrientation = glm::quat();

	/*nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());*/
	nPhysics::iShape* capsuleShape = physicsFactory->CreateShape(nPhysics::sBox());
	//nPhysics::iShape* cubeShape = physicsFactory->CreateShape()
	objectiveMarker->rigidBody = physicsFactory->CreateRigidBody(bodyDef, capsuleShape);
	objectiveMarker->bHasRigidBody = true;
	nPhysics::cRigidBodyDef* bodyDefCheck;
	bodyDefCheck = objectiveMarker->rigidBody->GetRigidBodyDef();
	//bodyDefCheck->qOrientation = glm::quat(glm::vec3(0, -1, -45));
	objectiveMarker->bodyDef = bodyDefCheck;
	//pViper->bIsImposter = true;
	//	camera1->AttachToGameObject(pFloorPanel);



	::g_vec_pGOs.push_back(objectiveMarker);
	physicsWorld->AddBody(objectiveMarker->rigidBody);












	return true;
}

char ReadNextChar(char* data, unsigned long &index)
{
	char returnVal = static_cast<unsigned char>(data[index]);
	index++;
	return returnVal;
}

unsigned short ReadNextUnsignedShort(char* data, unsigned long &index)
{
	unsigned short usTheReturnVal = 0;
	usTheReturnVal = ShiftAndAdd_Byte_to_UShort(usTheReturnVal, static_cast<unsigned char>(data[index]), 0);
	index++;
	usTheReturnVal = ShiftAndAdd_Byte_to_UShort(usTheReturnVal, static_cast<unsigned char>(data[index]), 1);
	index++;
	return usTheReturnVal;
}

unsigned long ReadNextUnsignedLong(char* data, unsigned long &index)
{
	unsigned long ulTheReturnVal = 0;
	// Read four bytes, but return them in reverse order.
	ulTheReturnVal = ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<unsigned char>(data[index]), 0);
	index++;
	ulTheReturnVal = ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<unsigned char>(data[index]), 1);
	index++;
	ulTheReturnVal = ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<unsigned char>(data[index]), 2);
	index++;
	ulTheReturnVal = ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<unsigned char>(data[index]), 3);
	index++;
	return ulTheReturnVal;
}


unsigned long ShiftAndAdd_Byte_to_ULong(unsigned long theULong, char theByte, int bytesToShift)
{
	// Too many bytes to shift (there are only 4 bytes in a long, so max is 3)?
	if (bytesToShift > 3)
	{
		bytesToShift = 0;
	}
	unsigned short bitsToShift = bytesToShift * 8;	// 8 bits / byte.
													// Now, convert the uChar to a uLong, 
													//	shift it the number of bits,
													//	then or it (add it) to the long
	unsigned char uByte = theByte;
	theULong = theULong | (static_cast<unsigned long>(uByte) << bitsToShift);
	return theULong;
}

unsigned short ShiftAndAdd_Byte_to_UShort(unsigned short theUShort, char theByte, int bytesToShift)
{
	// Too many bytes to shift (there is only 2 bytes in a long, so max is 1)?
	if (bytesToShift > 1)
	{
		bytesToShift = 0;
	}
	unsigned short bitsToShift = bytesToShift * 8;	// 8 bits / byte.
													// Now, convert the uChar to a uLong, 
													//	shift it the number of bits,
													//	then or it (add it) to the long
	unsigned char uByte = theByte;
	theUShort = theUShort | (static_cast<unsigned long>(uByte) << bitsToShift);
	return theUShort;
}