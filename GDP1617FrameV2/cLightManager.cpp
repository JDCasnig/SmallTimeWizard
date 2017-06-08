#include "cLightManager.h"
#include "GlobalStuff.h"
#include "include\glm\glm.hpp"
#include <glad/glad.h>		
#include <GLFW/glfw3.h>	
cLightManager::cLightManager()
{

}

cLightManager::~cLightManager()
{

}

void cLightManager::AddLight(cLight * lightToAdd)
{
	mlightVec.push_back(lightToAdd);
}

void cLightManager::RemoveLight(cLight * lightToRemove)
{
	for (unsigned int i = 0; i < mlightVec.size(); i++)
	{
		if (mlightVec[i] == lightToRemove)
		{
			
			mlightVec.erase(mlightVec.begin() + i);
		}
	}
	
}

void cLightManager::UpdateLights()
{
	mlightVec[0]->position = ::gPlayer->bodyDef->position+glm::vec3(0.f,4.f,0.f);
}

void cLightManager::SetSceneLighting()
{
	//if (mlightVec.size() < 15)
	//{
	//	for (unsigned int i = 0; i < mlightVec.size(); i++)
	//	{

	//	}
//	}

	glUniform4f(locID_lightPosition, mlightVec[0]->position.x, mlightVec[0]->position.y, mlightVec[0]->position.z, 1.0f);
	glUniform4f(locID_lightDiffuse, mlightVec[0]->colour.x, mlightVec[0]->colour.y, mlightVec[0]->colour.z, 1.0f);

	glUniform4f(locID_lightAttenuation, mlightVec[0]->constAttenuation, mlightVec[0]->linearAttenuation, mlightVec[0]->quadAttenuation, 1.0f);

	glUniform4f(locID2_lightPosition, mlightVec[1]->position.x, mlightVec[1]->position.y, mlightVec[1]->position.z, 1.0f);
	glUniform4f(locID2_lightDiffuse, mlightVec[1]->colour.x, mlightVec[1]->colour.y, mlightVec[1]->colour.z, 1.0f);

	glUniform4f(locID2_lightAttenuation, mlightVec[1]->constAttenuation, mlightVec[1]->linearAttenuation, mlightVec[1]->quadAttenuation, 1.0f);
}
