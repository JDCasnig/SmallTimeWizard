#include "cLight.h"

cLight::cLight()
{
	position = glm::vec3(0.f);
	constAttenuation =0.f;
	linearAttenuation = 0.1f;
	quadAttenuation = 0.1f;

	colour= glm::vec3(1.f);

	specularParams= glm::vec4(0.f);
	typesAndParams = glm::vec4(0.f);

	powerStatus = true;
}

cLight::cLight(bool status)
{
	position = glm::vec3(0.f);
	constAttenuation = 0.f;
	linearAttenuation = 0.1f;
	quadAttenuation = 0.001f;

	colour = glm::vec3(1.f);

	specularParams = glm::vec4(0.f);
	typesAndParams = glm::vec4(0.f);
	powerStatus = status;
}

cLight::~cLight()
{

}