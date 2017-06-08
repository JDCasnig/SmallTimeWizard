#include "GlobalStuff.h"


nPhysics::iPhysicsWorld* physicsWorld = NULL;
nPhysics::iPhysicsFactory* physicsFactory = NULL;
nPhysics::iRigidBody* gPlayerBody = NULL;
cGameObject* gPlayer = NULL;

cLight* gLight = new cLight();
cLightManager* gLightManager = new cLightManager();


GLint locID_lightPosition = -1;
GLint locID_lightDiffuse = -1;
GLint locID_lightAttenuation = -1;

GLint locID2_lightPosition = -1;
GLint locID2_lightDiffuse = -1;
GLint locID2_lightAttenuation = -1;
GLint UniformLoc_Sheen = -1;
float sheen = .1f;
void CreateFireBall();
void DetonateFireBall();
void CreateConeOfCold();
void CreateGravityBall();
void CreateForceWave();
void DetonateGravityBall();
float playerHealth=30.f;
cGameObject* gEnemy1 = NULL;
cGameObject* objectiveMarker=NULL;
bool magicVision = false;
cGameObject* gEnemy2 = NULL;
nPhysics::iRigidBody* enemyBody2 = NULL;

nPhysics::iRigidBody* enemyBody=NULL;
cGameObject* fireBall = NULL;
bool fireBallExists = false;

bool bGravityBallExists=false;
cGameObject* gravityBall;
nPhysics::iRigidBody* gravityBallBody;

// We should place this into a class...
float g_CameraX = 6.9f;
float g_CameraY = 6.5f;
float g_CameraZ = 28.7f;	
float g_CameraMovementSpeed = 0.1f;

float g_TargetX = 0.0f;
float g_TargetY = 0.0f;
float g_TargetZ = 0.0f;


//glm::vec3 lightPos(5.4f, 5.0f, 19.4f);		// I just picked these values after moving the light around until it looked "nice"
//oat lightConstAtten = 0.1f;
//float lightLinearAtten = 0.000477747f;
//float lightQuadAtten = 0.000000683441f;
bool bDrawLightDebugSpheres = false;
bool bDrawLightDebugLocation = true;

GLuint g_FBO = 0;
GLuint g_FBO_colourTexture = 0;
GLuint g_FBO_depthTexture = 0;
GLint g_FBO_SizeInPixes = 1024;

GLint UniformLoc_tex2ndPass = -1;				// Actual texture sampler
GLint UniformLoc_bIsSecondRenderPass = -1;


GLint locID_matModel = -1;		// 
GLint locID_matView = -1;
GLint locID_matProj = -1;

GLint UniformLoc_ID_objectColour = -1;
GLint UniformLoc_ID_isWireframe = -1;
GLint UniformLoc_ID_bUseDebugDiffuseNoLighting = -1;
GLint UniformLoc_ID_bIsLineDebugDrawingPass = -1;

GLint UniformLoc_alpha = -1;

GLint UniformLoc_texSamp2D_00 = -1;
GLint UniformLoc_texSamp2D_01 = -1;
GLint UniformLoc_texSamp2D_02 = -1;
GLint UniformLoc_texSamp2D_03 = -1;

GLint UniformLoc_texCube01 = -1;
GLint UniformLoc_bIsSkybox = -1;

GLint UniformLoc_amountReflective = -1;		// 0 - 1.0   (no reflection to 100%)
GLint UniformLoc_amountRefractive = -1;		// 0 - 1.0   (no refraction to 100%)
GLint UniformLoc_amountOfDiffuse = -1;		// 0 - 1.0  (underlying "colour")
GLint UniformLoc_indexOfRefraction = -1;	// 1.0f to something


GLint UniformLoc_bUseTextureAsDiffuse = -1;
GLint UniformLoc_bUseLighting = -1;

GLint UniformLoc_bIsImposterBlackIsAlpha = -1;
cGameObject* g_pExplosion = NULL;

std::vector< cGameObject* > g_vec_pGOs;
std::vector< cContact > g_vec_Contacts;

cMeshTypeManager* g_pTheMeshTypeManager = 0;

cShaderManager* g_pTheShaderManager = 0;

cParticleEmitter* g_FireParticles = 0;
cParticleEmitter* g_GravityParticles=0;

cBasicTextureManager* g_pTextureManager = 0;


// TODO: REMOVE THIS!!!!
//cGameObject* g_pObjectToRotate = 0;

std::string g_coolSoundEffect = "";


cCamera* camera1 = new cCamera();
cCamera* camera2 = new cCamera();
cCamera* camera3 = new cCamera();

