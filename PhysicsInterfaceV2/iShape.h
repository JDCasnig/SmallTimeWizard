#ifndef _iShape_HG_
#define _iShape_HG_

#include "GLMIncludes.h"

namespace nPhysics
{
	// base class/interface for all shapes

	enum eShapeType
	{
		GHOST_SHAPE = 0, // yet to be implemented
		CIRCLE_SHAPE = 1,
		PLANE_SHAPE = 2,
		BOX_SHAPE = 3,
		HEMICIRCLE_SHAPE = 4
	};

	/*maybe these should be in a glm extension folder?*/

	struct sCircle
	{
		sCircle() : Radius(1.f) {}
		sCircle(float radius) : Radius(radius) {}
		float Radius;
	};
	struct sPlane
	{
		sPlane() : Normal(0.f, 1.f, 0.f) {}
		sPlane(const glm::vec3& normal) : Normal(normal) {}
		sPlane(float nX, float nY, float nZ) : Normal(nX, nY, nZ) {}
		glm::vec3 Normal;
	};
	struct sBox
	{
		sBox() : width(1.f), length(1.f) {}
		sBox(float side) : width(side), length(side) {}
		sBox(float width, float length, float height) : width(width), length(length) {}
		float width;
		float length;
	};
	struct sHemicircle
	{
		sHemicircle() : Radius(1.f), Normal(glm::vec3(0.f, 0.f, 1.f)) {}
		sHemicircle(float radius) : Radius(radius), Normal(glm::vec3(0.f, 0.f, 1.f)) {}
		sHemicircle(glm::vec3 normal) : Radius(1.f), Normal(normal) {}
		sHemicircle(float radius, glm::vec3 normal) : Radius(radius), Normal(normal) {}
		float Radius;
		glm::vec3 Normal;
	};

	class iShape
	{
	public:
		virtual ~iShape() {}
		// Use GetShapeType()
		// if this isn't the right shape, it will do nothing
		// and return false
		virtual bool GetCircle(sCircle& circleOut) = 0;
		virtual bool GetPlane(sPlane& planeOut) = 0;
		virtual bool GetBox(sBox& boxOut) = 0;
		virtual bool GetCapsule(sHemicircle& HemicircleOut) = 0;
		// eShapeType
		// what shape am I?
		virtual	eShapeType GetShapeType() = 0;


		/*private:
		eShapeType mShapeType;*/
	};
}

#endif