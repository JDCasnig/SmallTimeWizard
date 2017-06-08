#ifndef _cLight_HG_
#define _cLight_HG_

#include "include\glm\glm.hpp"

class cLight
{
public:
	cLight();
	cLight(bool status);
	~cLight();

	glm::vec3 position;
	float constAttenuation;
	float linearAttenuation;
	float quadAttenuation;

	glm::vec3 colour;

	glm::vec4 specularParams;
	glm::vec4 typesAndParams;

	bool powerStatus;
};

#endif // !_cLight_HG_
