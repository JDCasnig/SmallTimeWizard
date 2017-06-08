#include <glad/glad.h>		
#include <GLFW/glfw3.h>		
#include <iostream>

#include "GlobalStuff.h"

//#include "linmath.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>
#include <stdio.h>
#include <sstream>		// "String Stream", used for setting the windows title

#include <vector>

#include "vert_XYZ_RGB.h"


#include "Utilities.h"
#include <cBulletFactory.h>
#include <iPhysicsFactory.h>
#include <iPhysicsWorld.h>

#include <Windows.h>		// Be


DWORD WINAPI g_ParticleUpdateThreadFunction(LPVOID lpParameter)
{
	// Get the appropriate pointer type
	cParticleEmitter* pEmitter = static_cast<cParticleEmitter*>(lpParameter);

	//::g_pParticles->Update( deltaTime );
	// For now, pick a static amount of time...
	// (you might change the emitter to have its own timer, or something)
	while (pEmitter->bKeepThreadAlive)
	{
		pEmitter->Update(0.01f);

		if (!pEmitter->bIsLocked())
		{
			pEmitter->Lock();
			pEmitter->updateParticleCopy();
			pEmitter->UnLock();
		}
		Sleep(0);
	}

	return 0;
}


extern unsigned int g_SkyboxID;


bool LoadObjectsIntoScene( std::vector< cGameObject* > &vec_pGOs, std::string sceneFile );






static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}




void SetUpFBO(void)
{
	//glCreateFramebuffers( 1, &g_FBO );		// 4.5 only
	glGenFramebuffers(1, &g_FBO);			// every other GL
	glBindFramebuffer(GL_FRAMEBUFFER, g_FBO);

	glGenTextures(1, &g_FBO_colourTexture);
	glBindTexture(GL_TEXTURE_2D, g_FBO_colourTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, g_FBO_SizeInPixes, g_FBO_SizeInPixes);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &g_FBO_depthTexture);
	glBindTexture(GL_TEXTURE_2D, g_FBO_depthTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, g_FBO_SizeInPixes, g_FBO_SizeInPixes);

	glFramebufferTexture(GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		g_FBO_colourTexture, 0);
	glFramebufferTexture(GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		g_FBO_depthTexture, 0);

	static const GLenum draw_bufers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, draw_bufers);

	// Point back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return;
}




std::vector< cParticleEmitter::cParticle > g_vecParticlesToRender;

int main(void)
{
	GLFWwindow* window;
	srand(NULL);
	// Added an "index_buffer" variable here
	//GLuint vertex_buffer, index_buffer, vertex_shader, fragment_shader, program;
	//GLuint vertex_shader, fragment_shader; //, program;
//    GLint mvp_location;//, vpos_location, vcol_location;

	//GLint locID_matModel = -1;		// 
	//GLint locID_matView = -1;
	//GLint locID_matProj = -1;

	//GLint locID_lightPosition = -1;
	//GLint locID_lightDiffuse = -1;
	//GLint locID_lightAttenuation = -1;

	//GLint locID2_lightPosition = -1;
	//GLint locID2_lightDiffuse = -1;
	//GLint locID2_lightAttenuation = -1;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	window = glfwCreateWindow(1200, 800, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);


	std::cout << glGetString(GL_VENDOR) << " "
		<< glGetString(GL_RENDERER) << ", "
		<< glGetString(GL_VERSION) << std::endl;
	std::cout << "Shader language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	//	std::cout << glGetString(GL_EXTENSIONS) << std::endl;

	::g_pTheShaderManager = new cShaderManager();

	cShaderManager::cShader verShader;
	cShaderManager::cShader fragShader;

	//verShader.fileName = "simpleVert.glsl";
	//fragShader.fileName = "simpleFrag.glsl";
	verShader.fileName = "phongVert.glsl";
	fragShader.fileName = "phongFrag.glsl";
	::g_pTheShaderManager->setBasePath("assets//shaders//");
	if (!::g_pTheShaderManager->createProgramFromFile("simple", verShader, fragShader))
	{	// Oh no!
		std::cout << ::g_pTheShaderManager->getLastError() << std::endl;
		std::cout.flush();
		// TODO: Shut it all down... 
		return -1;
	}

	SetUpFBO();

	physicsFactory = new cBulletFactory();
	physicsWorld = physicsFactory->CreatePhysicsWorld();


	


	// Entire loading the model and placing ito the VAOs 
	// is now in the cMeshTypeManager
	::g_pTheMeshTypeManager = new cMeshTypeManager();


	// Note: this returns a bool, which you might want to check...
	// (We won't right now, though)
	GLuint shadProgID = ::g_pTheShaderManager->getIDFromFriendlyName("simple");
	::g_pTheMeshTypeManager->SetBaseFilePath("assets//models//");

	::g_pTheMeshTypeManager->ScaleEverythingToBoundingBoxOnLoad(true, 10.0f);

	// Objects we can use for imposters
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer(shadProgID, "1x1_2Tri_Quad_1_Sided_XY_Plane.ply");
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer(shadProgID, "1x1_2Tri_Quad_2_Sided_XZ_Plane.ply");
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer(shadProgID, "1x1_2Tri_Quad_2_Sided_XY_Plane.ply");

	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer(shadProgID, "ArrowWithHead_xyz_nxyz_ASCII_Pos_Z_1_unit_long.ply");
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer(shadProgID, "LowerResArrow_Pos_Z_1_unit_long.ply");
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer(shadProgID, "Long_bar_x_aligned_x10.ply");
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer(shadProgID, "Long_bar_y_aligned_x10.ply");
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer(shadProgID, "Long_bar_z_aligned_x10.ply");

	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer(shadProgID, "2x2_plane_y_up.ply");


	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer(shadProgID, "Sphere_N.ply");					// "Sphere.ply");
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer(shadProgID, "wizard.ply");
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer(shadProgID, "AztecCube2.ply");
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer(shadProgID, "Isoshphere.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer(shadProgID, "Isoshphere_xyz_InvertedNormals.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer(shadProgID, "Cube2.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer(shadProgID, "Cube2.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"
	//::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Seafloor2_N.ply" );

	// Load the seafloor "dynamically"


//	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "dragon_vrip_res2_N.ply" );

	// Create the Debug line drawing thing
	const unsigned int NUMBEROFLINESINBUFFER = 1'000;
	::g_pTheMeshTypeManager->CreateDebugLineBuffer(NUMBEROFLINESINBUFFER,
		shadProgID);

	// Create a particle emitter
	::g_FireParticles = new cParticleEmitter();

	cParticleEmitter::sParticleDesc emitDescript;
	// Change stuff in the description, perhaps?
	emitDescript.lifetimeMin = 2.0f;
	emitDescript.lifetimeMax = 4.0f;
	emitDescript.particlesCreatedPerUpdate = 3;
	// You would change this "force" to be in the direction
	// you wanted to travel (or whatever)
	emitDescript.ongoingForce = glm::vec3(0.0f, 0.5f, 0.0f);

	emitDescript.initVelMin = glm::vec3(-0.1f, 0.0f, -0.1f);
	emitDescript.initVelMax = glm::vec3(0.1f, 0.0f, 0.1f);


	::g_FireParticles->allocateParticles(15, emitDescript /*, true*/);

	::g_GravityParticles = new cParticleEmitter();

	cParticleEmitter::sParticleDesc emitDescript2;
	// Change stuff in the description, perhaps?
	emitDescript2.lifetimeMin = 2.0f;
	emitDescript2.lifetimeMax = 4.0f;
	emitDescript2.particlesCreatedPerUpdate = 1;
	// You would change this "force" to be in the direction
	// you wanted to travel (or whatever)
	emitDescript2.ongoingForce = glm::vec3(0.0f, 0.f, 0.0f);

	emitDescript2.initVelMin = glm::vec3(-0.1f, -.10f, -0.1f);
	emitDescript2.initVelMax = glm::vec3(0.1f, 0.10f, 0.1f);


	::g_GravityParticles->allocateParticles(15, emitDescript2 /*, true*/);



	::g_pTextureManager = new cBasicTextureManager();


	::g_pTextureManager = new cBasicTextureManager();



	::g_pTextureManager->SetBasePath("assets/textures");
	if (!::g_pTextureManager->Create2DTextureFromBMPFile("brickTexture_square_1024pixels.bmp", true))
	{
		std::cout << "WARNING: Didn't load texture" << std::endl;
	}
	else
	{
		std::cout << "Texture loaded OK" << std::endl;
	}

	if (!::g_pTextureManager->Create2DTextureFromBMPFile("GridSmall.bmp", true))
	{
		std::cout << "WARNING: Didn't load smoketex.bmp texture" << std::endl;
	}
	else
	{
		std::cout << "Texture loaded OK" << std::endl;
	}

	::g_pTextureManager->Create2DTextureFromBMPFile("FenceActual.bmp", true);

	if (!::g_pTextureManager->Create2DTextureFromBMPFile("smoketex.bmp", true))
	{
		std::cout << "WARNING: Didn't load smoketex.bmp texture" << std::endl;
	}
	else
	{
		std::cout << "Texture loaded OK" << std::endl;
	}

	if (!::g_pTextureManager->Create2DTextureFromBMPFile("fireball.bmp", true))
	{
		std::cout << "WARNING: Didn't load fireball.bmp texture" << std::endl;
	}
	else
	{
		std::cout << "Texture loaded OK" << std::endl;
	}

	if (!::g_pTextureManager->Create2DTextureFromBMPFile("Stonefloor.bmp", true))
	{
		std::cout << "WARNING: Didn't load Stonefloor.bmp texture" << std::endl;
	}
	else
	{
		std::cout << "Texture loaded OK" << std::endl;
	}

	if (!::g_pTextureManager->Create2DTextureFromBMPFile("GravityBall.bmp", true))
	{
		std::cout << "WARNING: Didn't load Stonefloor.bmp texture" << std::endl;
	}
	else
	{
		std::cout << "Texture loaded OK" << std::endl;
	}

	if (!::g_pTextureManager->Create2DTextureFromBMPFile("PurpleFire.bmp", true))
	{
		std::cout << "WARNING: Didn't load Stonefloor.bmp texture" << std::endl;
	}
	else
	{
		std::cout << "Texture loaded OK" << std::endl;
	}


	//::g_pTextureManager->Create2DTextureFromBMPFile("smoketex.bmp", true);

	if (!::g_pTextureManager->CreateNewCubeTextureFromBMPFiles("spacebox",
		"MyFirstSpaceBox_right1_posX.bmp", "MyFirstSpaceBox_left2_negX.bmp",
		"MyFirstSpaceBox_top3_posY.bmp", "MyFirstSpaceBox_bottom4_negY.bmp",
		"MyFirstSpaceBox_front5_posZ.bmp", "MyFirstSpaceBox_back6_negZ.bmp"))
	{
		std::cout << "Didn't load skybox" << std::endl;
		std::cout << ::g_pTextureManager->getLastError(true);
	}
	else
	{
		std::cout << "skybox loaded OK" << std::endl;
	}




	// Shader loading and building was here...

//    mvp_location = glGetUniformLocation(shadProgID, "MVP");

	// glGetActiveUniforms... 
	locID_matModel = glGetUniformLocation(shadProgID, "matModel");
	locID_matView = glGetUniformLocation(shadProgID, "matView");
	locID_matProj = glGetUniformLocation(shadProgID, "matProj");

	gLightManager = new cLightManager();
	cLight* firstTempLight = new cLight();
	firstTempLight->position = glm::vec3(0.f, 30.f, 0.f);
	firstTempLight->quadAttenuation = .001f;
	/*gLightManager->AddLight(new cLight());*/
	gLightManager->AddLight(firstTempLight);
	cLight* tempLight = new cLight();
	tempLight->colour = glm::vec3(1.f, 1.f, 1.f);
	tempLight->linearAttenuation = 0.1f;
	tempLight->quadAttenuation = 0.1f;
	tempLight->position = glm::vec3(30.f, 30.f, 30.f);
	gLightManager->AddLight(tempLight);

	locID_lightPosition = glGetUniformLocation(shadProgID, "theLights[0].position");
	locID_lightDiffuse = glGetUniformLocation(shadProgID, "theLights[0].diffuse");
	locID_lightAttenuation = glGetUniformLocation(shadProgID, "theLights[0].attenuation");// = 0.0f;		// Can set to 1.0 to "turn off" a light


	locID2_lightPosition = glGetUniformLocation(shadProgID, "theLights[1].position");
	locID2_lightDiffuse = glGetUniformLocation(shadProgID, "theLights[1].diffuse");
	locID2_lightAttenuation = glGetUniformLocation(shadProgID, "theLights[1].attenuation");// = 0.0f;		// Can set to 1.0 to "turn off" a light

	if (!LoadObjectsIntoScene(::g_vec_pGOs, "level1.xml"))
	{
		std::cout << "WARNING: Could not load all models into the scene." << std::endl;
	}


	//GLuint UniformLoc_ID_objectColour = 0;
	//GLuint UniformLoc_ID_isWireframe = 0;

	UniformLoc_ID_objectColour = glGetUniformLocation(shadProgID, "objectColour");
	UniformLoc_ID_isWireframe = glGetUniformLocation(shadProgID, "isWireframe");
	UniformLoc_ID_bUseDebugDiffuseNoLighting = glGetUniformLocation(shadProgID, "bUseDebugDiffuseNoLighting");

	UniformLoc_ID_bIsLineDebugDrawingPass = glGetUniformLocation(shadProgID, "bIsLineDebugDrawingPass");

	UniformLoc_alpha = glGetUniformLocation(shadProgID, "alpha");

	UniformLoc_texSamp2D_00 = glGetUniformLocation(shadProgID, "texSamp2D_00");
	UniformLoc_texSamp2D_01 = glGetUniformLocation(shadProgID, "texSamp2D_01");
	UniformLoc_texSamp2D_02 = glGetUniformLocation(shadProgID, "texSamp2D_02");
	UniformLoc_texSamp2D_03 = glGetUniformLocation(shadProgID, "texSamp2D_03");

	UniformLoc_texCube01 = glGetUniformLocation(shadProgID, "texCubeMap01");
	UniformLoc_bIsSkybox = glGetUniformLocation(shadProgID, "bIsCubeMap");

	UniformLoc_bUseTextureAsDiffuse = glGetUniformLocation(shadProgID, "bUseTextureAsDiffuse");
	UniformLoc_bUseLighting = glGetUniformLocation(shadProgID, "bUseLighting");

	UniformLoc_bIsImposterBlackIsAlpha = glGetUniformLocation(shadProgID, "bIsImposterBlackIsAlpha");

	UniformLoc_amountReflective = glGetUniformLocation(shadProgID, "amountReflective");		// 0 - 1.0   (no reflection to 100%)
	UniformLoc_amountRefractive = glGetUniformLocation(shadProgID, "amountRefractive");		// 0 - 1.0   (no refraction to 100%)
	UniformLoc_amountOfDiffuse = glGetUniformLocation(shadProgID, "amountOfDiffuse");		// 0 - 1.0  (underlying "colour")
	UniformLoc_indexOfRefraction = glGetUniformLocation(shadProgID, "indexOfRefraction");	// 1.0f to something

	UniformLoc_tex2ndPass = glGetUniformLocation(shadProgID, "tex2ndPass");
	UniformLoc_bIsSecondRenderPass = glGetUniformLocation(shadProgID, "bIsSecondRenderPass");
	UniformLoc_Sheen = glGetUniformLocation(shadProgID, "sheen");
	// Note that GLFW would have had to create a "depth buffer" also
	// (which it does, apparently...)
	glEnable(GL_DEPTH_TEST);
	// Start the particle update thread here....
	DWORD hThreadID;	// Will HAVE the ID in it
	LPDWORD phThread;	// WIll POINT to the variable holding the thread id
	phThread = &hThreadID;	// Set pointer to point to variable

	HANDLE hThreadHandle = CreateThread(
		NULL,
		0, // stack size
		g_ParticleUpdateThreadFunction,	// DoTheWork,
										// pass a pointer to the valid object we want the function to update
		static_cast<void*>(::g_FireParticles),
		NULL,
		(DWORD*)&phThread);


	DWORD hThreadID2;	// Will HAVE the ID in it
	LPDWORD phThread2;	// WIll POINT to the variable holding the thread id
	phThread2 = &hThreadID2;	// Set pointer to point to variable

	HANDLE hThreadHandle2 = CreateThread(
		NULL,
		0, // stack size
		g_ParticleUpdateThreadFunction,	// DoTheWork,
										// pass a pointer to the valid object we want the function to update
		static_cast<void*>(::g_GravityParticles),
		NULL,
		(DWORD*)&phThread2);
	//::g_pParticles->Update( deltaTime );
	cAverageSample timerAverage(100);
	double lastTime = glfwGetTime();
	nPhysics::cRigidBodyDef bodyDefFloor;

	bodyDefFloor.position = glm::vec3(10.f, 18.f, 10.f);// glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);glm::vec3(0.0f, 0.0f, 0.0f);
	bodyDefFloor.staticBody = true;
	bodyDefFloor.mass = 1.0f;
	bodyDefFloor.velocity = glm::vec3(0.f);
	bodyDefFloor.force = glm::vec3(0.f);
	bodyDefFloor.qOrientation = glm::quat(glm::vec3(0, 0, 0));
	//		bodyDef.qOrientation = glm::quat();

	/*nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());*/
	nPhysics::iShape* boxShape = physicsFactory->CreateShape(nPhysics::sBox(40.f, 40.f, 2.f));
	//nPhysics::iShape* cubeShape = physicsFactory->CreateShape()
	nPhysics::iRigidBody* rigidBody = physicsFactory->CreateRigidBody(bodyDefFloor, boxShape);
	physicsWorld->AddBody(rigidBody);
	while (!glfwWindowShouldClose(window))
	{
		if (::gPlayer->bodyDef->position.y<10.f || playerHealth<0.f)
		{
			physicsWorld->MoveBodyToPosition(::gPlayerBody, glm::vec3(5.f, 20.f, 5.f));
			::playerHealth = 30.f;
			physicsWorld->MoveBodyToPosition(::enemyBody, glm::vec3(18.f, 20.f, 5.f));
			physicsWorld->MoveBodyToPosition(::enemyBody2, glm::vec3(8.f, 20.f, 19.f));
			//::gPlayer->bodyDef->position = glm::vec3(5.f, 20.f, 5.f);
		}

		float winDistance;
		winDistance = glm::distance(::gPlayer->bodyDef->position, objectiveMarker->bodyDef->position);
		

		if (winDistance<5.f)
		{
			physicsWorld->MoveBodyToPosition(::gPlayerBody, glm::vec3(5.f, 20.f, 5.f));
			::playerHealth = 30.f;
			physicsWorld->MoveBodyToPosition(::enemyBody, glm::vec3(18.f, 20.f, 5.f));
			physicsWorld->MoveBodyToPosition(::enemyBody2, glm::vec3(8.f, 20.f, 19.f));
			//::gPlayer->bodyDef->position = glm::vec3(5.f, 20.f, 5.f);
			std::cout << "Congratulations! You have freed yourself, and won the game!" << std::endl;
		}




		float ratio;
		int width, height;

		gEnemy1->Update();
		gEnemy2->Update();
		// *****************************************************************
		glUniform1i(UniformLoc_bIsSecondRenderPass, FALSE);		// 
																// Point rendering to the off-screen frame buffer object
		glBindFramebuffer(GL_FRAMEBUFFER, ::g_FBO);

		// Clear the offscreen framebuffer
		glViewport(0, 0, g_FBO_SizeInPixes, g_FBO_SizeInPixes);
		GLfloat	zero = 0.0f;
		GLfloat one = 1.0f;
		glClearBufferfv(GL_COLOR, 0, &zero);
		glClearBufferfv(GL_DEPTH, 0, &one);

		width = g_FBO_SizeInPixes;
		height = g_FBO_SizeInPixes;

		// *****************************************************************
		::gLightManager->UpdateLights();

		/////////////////////////////////
		// *****************************************************************

		//// Move this line to later in the code when we draw the quad
		//// on the ACTUAL framebuffer we are "presenting" to the screen
		//glfwGetFramebufferSize(window, &width, &height);

		ratio = width / (float)height;

		glViewport(0, 0, width, height);


		::g_coolSoundEffect = "";

		//		float ratio;
			//	int width, height;
				//        mat4x4 m, p, mvp;
		glm::mat4x4 matProjection;		// Was just "p"



		// The "view" matrix is (usually) only updated once per "scene"
		// The "projection" matrix only needs to be updated when the window size 
		//    changes, but here we are updating it every "scene"
		// (a "scene" is when ALL the game objects are drawn)

		glm::mat4x4 matView(1.0f);	// "View" (or camera) matrix

//		mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		matProjection = glm::perspective(0.6f, ratio, 0.01f, 10000.0f);

		//v = glm::lookAt( glm::vec3( 0.0f, 0.0f, -10.0f), // Eye
		matView = glm::lookAt(
			camera1->position, // Eye
			camera1->cameraTarget, // At (target)
			glm::vec3(0.0f, 1.0f, 0.0f));	// Up

//		const float TIMESTEPCONST = 0.01f;	// 10ms

		static const float MAXDELTATIME = 0.1f;	// 100 ms
		float deltaTime = static_cast<float>(glfwGetTime() - lastTime);
		lastTime = glfwGetTime();
		if (deltaTime > MAXDELTATIME)
		{
			deltaTime = MAXDELTATIME;
		}
		timerAverage.AddSample(deltaTime);
		deltaTime = static_cast<float>(timerAverage.getAverage());
		physicsWorld->TimeStep(deltaTime);
		camera1->Update();

		if (fireBallExists)
		{
			
			::g_FireParticles->position = fireBall->bodyDef->position;// +glm::vec3(0.f, 1.f, 0.f);
			//::g_FireParticles->allocateParticles(15, emitDescript /*, true*/);
		}
		else
		{
			::g_FireParticles->position = gPlayer->bodyDef->position + glm::vec3(0.f, 1.f, 0.f);
		}

		if (bGravityBallExists)
		{
			::g_GravityParticles->position = gravityBall->bodyDef->position;// +glm::vec3(0.f, 1.f, 0.f);
		}
		else
		{
			::g_GravityParticles->position = gPlayer->bodyDef->position + glm::vec3(0.f, 1.f, 0.f);
		}
		// Inidcate where the eye is (for the imposters)
		::g_FireParticles->particleSettings.imposterEyeLocation = camera1->position;

		//::g_pParticles->particleSettings.imposterEyeLocation.x = ::g_CameraX;
		//::g_pParticles->particleSettings.imposterEyeLocation.y = ::g_CameraY;
		//::g_pParticles->particleSettings.imposterEyeLocation.z = ::g_CameraZ;

		::g_FireParticles->particleSettings.ongoingForce = glm::vec3(0.0f, 3.0f, 0.0f);

		::g_FireParticles->particleSettings.initVelMin = glm::vec3(-.1f, .050f, -.1f);
		::g_FireParticles->particleSettings.initVelMax = glm::vec3(.10f, .10f, .10f);

	//	::g_FireParticles->particleSettings.lifetimeMax = 10.0f;


		::g_GravityParticles->particleSettings.imposterEyeLocation = camera1->position;

		//::g_pParticles->particleSettings.imposterEyeLocation.x = ::g_CameraX;
		//::g_pParticles->particleSettings.imposterEyeLocation.y = ::g_CameraY;
		//::g_pParticles->particleSettings.imposterEyeLocation.z = ::g_CameraZ;

	//	::g_GravityParticles->particleSettings.ongoingForce = glm::vec3(0.0f, 0.0f, 0.0f);

	//	::g_GravityParticles->particleSettings.initVelMin = glm::vec3(-.1f, .050f, -.1f);
	//	::g_GravityParticles->particleSettings.initVelMax = glm::vec3(.10f, .10f, .10f);

	//	::g_GravityParticles->particleSettings.lifetimeMax = 10.0f;
		
		if (::g_pExplosion != NULL)
		{
			::g_pExplosion->scale*=1.1;
			if (::g_pExplosion->scale > 2.f)
			{
				for (unsigned int i = 0; i < ::g_vec_pGOs.size(); i++)
				{
					if (::g_vec_pGOs[i] == ::g_pExplosion)
					{
						::g_vec_pGOs.erase(::g_vec_pGOs.begin() + i);
						::g_pExplosion = NULL;
						break;
					}
				}
				
			}
		}

		


		// update the bunny rotation
//		::g_vec_pGOs[0]->pre_Rot_X += 0.01f;
//		::g_vec_pGOs[0]->post_Rot_X += 0.01f;

		// Only needs to be set if window is created and-or 
		// is resized (you could put this in the window re-size
		// callback in GLFW or freeGLUT)
		glUniformMatrix4fv(locID_matProj, 1, GL_FALSE,
			(const GLfloat*)glm::value_ptr(matProjection));

		// This is set once at the start of the "scene" draw.
		glUniformMatrix4fv(locID_matView, 1, GL_FALSE,
			(const GLfloat*)glm::value_ptr(matView));

		//lightPos = skyCrane->position;


		//glUniform4f(locID_lightPosition, lightPos.x, lightPos.y, lightPos.z, 1.0f);
		//glUniform4f(locID_lightDiffuse, 1.0, 0.0f, 0.0f, 1.0f);

		//glUniform4f(locID_lightAttenuation, ::lightConstAtten, ::lightLinearAtten, ::lightQuadAtten, 1.0f);

		::gLightManager->SetSceneLighting();

		//glUniform4f(locID_lightPosition, gLight->position.x, gLight->position.y,gLight->position.z, 1.0f);
		//glUniform4f(locID_lightDiffuse, gLight->colour.x,gLight->colour.y,gLight->colour.z, 1.0f);

		//glUniform4f(locID_lightAttenuation,gLight->constAttenuation, gLight->linearAttenuation, gLight->quadAttenuation, 1.0f);
		if (fireBall != NULL)
		{
			if (fireBall->bodyDef->collisionFlag)
			{
				//fireBall->texture00Name = "GridSmall.bmp";

				//fireBall->bodyDef->collisionFlag = false;
				DetonateFireBall();
			}
		}


		if (gravityBall != NULL)
		{
			if (gravityBall->bodyDef->collisionFlag)
			{
				gravityBall->texture00Name = "GridSmall.bmp";
				gravityBall->bodyDef->collisionFlag = false;
			}
		}


		// Order the object by distance from the camera
		// Left for the students to do, for the time being...

		// Sort objects by distance from camera
		std::vector< cGameObject* > vecGO_Sorted;
		vecGO_Sorted.reserve(::g_vec_pGOs.size());
		// Copy to vector
		for (std::vector<cGameObject*>::iterator itGO = ::g_vec_pGOs.begin(); itGO != ::g_vec_pGOs.end(); itGO++)
		{
			vecGO_Sorted.push_back(*itGO);
		}

		// Do one pass of bubble sort, becuase the objects are likely to be in 
		// almost the correct order from frame to frame		
		glm::vec3 eye = camera1->position;
		for (int index = 0; index != vecGO_Sorted.size() - 1; index++)
		{
			cGameObject* p1 = vecGO_Sorted[index];
			cGameObject* p2 = vecGO_Sorted[index + 1];

			// Use glm distance 
			// (note, you can use a "squared distance" function
			//  that does not do the square root, as it's faster)
			//glm::vec3 p1Pos(p1->x, p1->y, p1->z);
			//glm::vec3 p2Pos(p2->x, p2->y, p2->z);
			//if ( glm::distance( eye, p1Pos ) < glm::distance( eye, p2Pos ) )

			if (glm::distance(eye, p1->bodyDef->position) < glm::distance(eye, p2->bodyDef->position))
			{	// p1 is closer than p2, but 
				// we are drawing from back to front, so switch them
				vecGO_Sorted[index] = p2;
				vecGO_Sorted[index + 1] = p1;
			}

		}//for ( int index = 0

		// Copy order back to main vectro
		for (int index = 0; index != vecGO_Sorted.size(); index++)
		{
			::g_vec_pGOs[index] = vecGO_Sorted[index];
		}

		::g_NumberOfIndicesRendered = 0;

		// Start of Draw Scene
		//		for ( int index = 0; index != ::g_vec_pGOs.size(); index++)
		for (int index = 0; index != vecGO_Sorted.size(); index++)
		{
			//cGameObject* pCurGO = ::g_vec_pGOs[index];
			cGameObject* pCurGO = vecGO_Sorted[index];

			// Set identity matrix for all "parent" or "root" objects
			glm::mat4x4 matModel = glm::mat4x4(1.0f);

			// Draw an object.... 
			DrawObject(pCurGO, matModel);

		}

		// for ( int index = 0;.... (bottom of "render scene" loop)
// End of Draw Scene

//		// Draw the particles...
//		int numParticles = ::g_pParticles->getParticlesCopy( ::g_vecParticlesToRender );
//		for ( int index = 0; index != numParticles; index++ )
//		{
//			DrawQuad( true, 
//			          ::g_vecParticlesToRender[index].position, 
//					  ::g_vecParticlesToRender[index].rotation,
//					  "",					/* texture name */
//					  1.0f,					/* scale */
//					  glm::vec3( 1.0f, 1.0f, 0.0f ), false /*useDebugColours*/, 
//					  false /*isWireframe*/);
//		}
		::g_FireParticles->Lock();

		int numParticles = ::g_FireParticles->getParticlesCopy(::g_vecParticlesToRender);

		for (int index = 0; index != numParticles; index++)
		{
			DrawFlameParticle(::g_vecParticlesToRender[index].position,
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), .1f);
		}

		::g_FireParticles->UnLock();

		::g_GravityParticles->Lock();

		numParticles = 0;

		numParticles = ::g_GravityParticles->getParticlesCopy(::g_vecParticlesToRender);

		for (int index = 0; index != numParticles; index++)
		{
			DrawGravityParticle(::g_vecParticlesToRender[index].position,
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), .1f);
		}

		::g_GravityParticles->UnLock();

		// *********************************
		DrawDebugLightSpheres();
		// *********************************

	//// Adjust the transparency of the objects
	//	for ( std::vector<cGameObject*>::iterator itpGO = g_vec_pGOs.begin();
	//			itpGO != g_vec_pGOs.end(); itpGO++ )
	//	{
	//		(*itpGO)->alpha += 0.01f;
	//		// To high alpha?
	//		if ( (*itpGO)->alpha >= 1.0f )
	//		{	// Yes, so make it 0.0f
	//			(*itpGO)->alpha = 0.0f;
	//		}
	//	}


		// Draw skybox
		cGameObject* pSkybox = ::findObjectByID(::g_SkyboxID);
		pSkybox->bIsVisible = true;
		// Move skybox to the same location as the player
		glm::mat4 matSkybox(1.0f);
		matSkybox = glm::translate(matSkybox, camera1->position);
		glUniform1i(UniformLoc_bIsSkybox, TRUE);
		DrawObject(pSkybox, matSkybox, false);
		pSkybox->bIsVisible = false;
		glUniform1i(UniformLoc_bIsSkybox, FALSE);


		// Show or "present" what we drew...

		if (magicVision)
		{
			::objectiveMarker->debugRGBA = glm::vec4(0.f, .5, .5f, .5f);
			::objectiveMarker->bUseDebugColours = true;
			glm::mat4x4 matModel = glm::mat4x4(1.0f);
			sheen *= 1.1f;
			if (sheen > .4f)
			{
				sheen = .1f;
			}
			// Draw an object.... 
			DrawObject(::objectiveMarker, matModel);
		}
		else
		{
			objectiveMarker->bUseDebugColours = false;
		}




		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Set the uniform to the 2nd pass
		glUniform1i(UniformLoc_bIsSecondRenderPass, TRUE);		// 
		glUniform1f(UniformLoc_Sheen, sheen);
		
		//GLuint texture00Number = ::g_pTextureManager->getTextureIDFromName("brickTexture_square_1024pixels.bmp");
		GLuint texture00Number = ::g_pTextureManager->getTextureIDFromName("GridSmall.bmp");
		// Texture binding...
		GLuint texture00Unit = 0;		// Texture unit go from 0 to 79

		glActiveTexture(texture00Unit + GL_TEXTURE0);		// GL_TEXTURE0 = 33984
		glBindTexture(GL_TEXTURE_2D, texture00Number);
		//	glBindTextureUnit( texture00Unit, texture00Number );			// OpenGL 4.5+ only
		// Set texture unit in the shader, too
		glUniform1i(UniformLoc_texSamp2D_00, texture00Unit);
	// Set the texture for the 2nd pass
	// Pick texture unit 007
	glActiveTexture(GL_TEXTURE7);		// GL_TEXTURE0 = 33984
										//		// ****JUST FOR TESTING, USE THE OBAMA TEXTURE****
										//		GLuint ObamaTextureID = ::g_pTextureManager->getTextureIDFromName( "obama_smile.bmp" );
										//		glBindTexture( GL_TEXTURE_2D, ObamaTextureID );
										//		// ***********************************************
	glBindTexture(GL_TEXTURE_2D, g_FBO_colourTexture);
	glUniform1i(UniformLoc_tex2ndPass, 7);

	
	matView = glm::lookAt(
		glm::vec3(0.f, 0.f, -25.f), // Eye
		glm::vec3(0.f), // At (target)
		glm::vec3(0.0f, 1.0f, 0.0f));	// Up

	glUniformMatrix4fv(locID_matView, 1, GL_FALSE,
		(const GLfloat*)glm::value_ptr(matView));

	//glm::vec3 oldCamera = camera3->position;
	////glm::vec3 oldCamera(::g_CameraX, ::g_CameraY, ::g_CameraZ);

	//::g_CameraX = 0.0f;
	//::g_CameraY = 0.0f;
	//::g_CameraZ = 1.0f;

	// This will be the 2nd pass, drawing only the quad...
	DrawQuad(true /*2 sided*/,
		//glm::vec3(oldCamera.x, oldCamera.y, oldCamera.z - 30.0f),		/*position*/
		//oldCamera,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),		/*rotation*/
		"",								/*textureName*/
		15.0f,								/*scale*/
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),	/*debug colour*/
		false,								/*use debug colour*/
		false);			/*wireframe*/
	//glActiveTexture(GL_TEXTURE20);		// GL_TEXTURE0 = 33984
	//									//		// ****JUST FOR TESTING, USE THE OBAMA TEXTURE****
	//									//		GLuint ObamaTextureID = ::g_pTextureManager->getTextureIDFromName( "obama_smile.bmp" );
	//									//		glBindTexture( GL_TEXTURE_2D, ObamaTextureID );
	//									//		// ***********************************************
	//glBindTexture(GL_TEXTURE_2D, g_FBO_colourTexture);
	//glUniform1i(UniformLoc_tex2ndPass, 20);
	//if (ScopeSelection != 1 && ScopeSelection != 0)
	//{
	//	DrawQuad(true /*2 sided*/,
	//		//glm::vec3(oldCamera.x, oldCamera.y, oldCamera.z - 30.0f),		/*position*/
	//		//oldCamera,
	//		glm::vec3(-6.0f, -6.0f, -1.0f),
	//		glm::vec3(0.0f, 0.0f, 0.0f),		/*rotation*/
	//		"",								/*textureName*/
	//		4.0f,								/*scale*/
	//		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),	/*debug colour*/
	//		false,								/*use debug colour*/
	//		false);

	//}
	//::g_CameraX = oldCamera.x;
	//::g_CameraY = oldCamera.y;
	//::g_CameraZ = oldCamera.z;

	// ***********************************************
	// ***********************************************
	// ***********************************************

	// Show or "present" what we drew...
	glfwSwapBuffers(window);
	//::g_CameraX = oldCamera.x;
	//::g_CameraY = oldCamera.y;
	//::g_CameraZ = oldCamera.z;


	glfwPollEvents();
	}

	// Bye bye...
	delete ::g_pTheMeshTypeManager;

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

//float distanceBetweenSpheres( cGameObject* pBallA, cGameObject* pBallB )
//{
//	float deltaX = pBallA->position.x - pBallB->position.x;	//	pBallA->x - pBallB->x;
//	float deltaY = pBallA->position.y - pBallB->position.y;	//	pBallA->y - pBallB->y;
//	float deltaZ = pBallA->position.z - pBallB->position.z;	//	pBallA->z - pBallB->z;
//
//	return sqrt( deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ);
//
//}
//
//float distanceBetweenPoints( glm::vec3 P1, glm::vec3 P2 )
//{
//	float deltaX = P1.x - P2.x;
//	float deltaY = P1.y - P2.y;
//	float deltaZ = P1.z - P2.z;
//
//	return sqrt( deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ);
//
//}
//
//// Narrow phase 
//bool testSphereSphereCollision( cGameObject* pBallA, cGameObject* pBallB )
//{
//	float totalRadius = pBallA->radius + pBallB->radius;
//	if ( distanceBetweenSpheres(pBallA, pBallB) <= totalRadius )
//	{
//		return true;
//	}
//	return false;
//}

// Search by ID, then returns pointer to object
cGameObject* findObjectByID( unsigned int ID )
{
	for ( int index = 0; index != g_vec_pGOs.size(); index++ )
	{
		if ( ::g_vec_pGOs[index]->getID() == ID )
		{	// Found it! 
			return ::g_vec_pGOs[index];
		}
	}
	// Didn't find it
	return 0;
}







