#ifndef _cLightManager_HG_
#define _cLightManager_HG_


#include "cLight.h"
#include <vector>

class cLightManager
{
public:
	cLightManager();
	~cLightManager();
	void AddLight(cLight* lightToAdd);
	void RemoveLight(cLight* lightToRemove);
	void UpdateLights();
	void SetSceneLighting();


private:
	std::vector<cLight*> mlightVec;







};

#endif // !_cLightManager_HG_
