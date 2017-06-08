#version 400

// FRAGMENT shader

uniform bool bUseDebugDiffuseNoLighting;

const int NUMBEROFLIGHTS = 2;

struct sLightDesc
{
	vec4 position;		// In the world
	vec4 diffuse;		// 1,1,1 (white colour light)
	vec4 attenuation;	// x = const, y = linear, z = quad, w = specular ratio
	vec4 specular;		// xyz = colour of the highlight, w is specular power
	                    // "shininess"  0.0, realy 1.0 to large, like 1000+
	vec4 typeAndParams;	// To be used soon. Specify type and attributes of type
};

uniform sLightDesc theLights[NUMBEROFLIGHTS];

// For spec:
uniform vec3 eyeLocation;
uniform sampler2D tex2ndPass;
uniform bool bIsSecondRenderPass;
uniform float alpha;		// a.k.a. transparency
uniform float sheen; //Magic Vision Glow Thing


// Texture "sampler"
// note that this ISN'T a 'texture', they are in 'texture units'
uniform sampler2D texSamp2D_00;			
uniform sampler2D texSamp2D_01;		
uniform sampler2D texSamp2D_02;			
uniform sampler2D texSamp2D_03;		

uniform samplerCube texCubeMap01;
uniform bool bIsCubeMap;

// Reflection and refraction
uniform float amountReflective;		// 0 - 1.0   (no reflection to 100%)
uniform float amountRefractive;		// 0 - 1.0   (no refraction to 100%)
uniform float amountOfDiffuse;		// 0 - 1.0  (underlying "colour")
uniform float indexOfRefraction;


// At least 80 texture units.
// Something like 16 to 32 on high end cards PER SHADER AT THE SAME TIME

uniform bool bUseTextureAsDiffuse;
uniform bool bUseLighting;
uniform bool bIsImposterBlackIsAlpha;

uniform bool bIsLineDebugDrawingPass;

// In from the vertex shader
in vec4 diffuseColour;	
in vec3 vertMV;			// view vector
in vec3 vertWorld;
in vec3 vertNorm;
//in vec3 viewVector;
in vec3 lightVector[NUMBEROFLIGHTS];
in vec3 lightHalfVector[NUMBEROFLIGHTS];
in vec3 text_STU;	// ADDED, texture coordinates (also called UV)


// out to the framebuffer
out vec4 out_Colour;

// This was taken out of the main portion. Just the light calculation
vec3 calcLightContribution( vec3 diffuseObjectColour, vec3 vecView, int lightIndex )
{
	// Can be used as an early exit for the shader
	// (if you add a "distance" value to each light, the distance 
	//  being how far the light has any influence. You'd set this to 
	//  a place where the light is essentially attenuated to "zero")
	float distanceFromLight = length(lightVector[lightIndex].xyz); 
	// Option: Add if statement to early exit if distance is "too far"
//	if ( distanceFromLight > lightVector[lightIndex].distance )
//	{
//		return vec3(0.0f, 0.0f, 0.0f);
//	}
	
	// The final attenuation is calculated. This is done here so 
	//  that you can also, optionally, early exit, say if the 
	//  attenuation is something very high (making the light 
	//  contribution essentially zero).
	float atten = 1.0f /( theLights[lightIndex].attenuation.x + 
	                      theLights[lightIndex].attenuation.y * distanceFromLight +
						  theLights[lightIndex].attenuation.z * distanceFromLight * distanceFromLight );

	
	// Normalize the light vector (which is the ray from the 
	//  vertex (world space) to the light (world space).
	// Since it's normalized, we only care about the
	//  direction, so essentially this is like a ray 
	//  cast from the origin (like the "normal" vector).
	vec3 lightVec = normalize(lightVector[lightIndex].xyz);
	
	// Diffuse contribution is the amount of light being reflected 
	//  (++INDEPENDENT of the eye (view) location++)
	//  from the LIGHT and the normal. Recal:
	//  - "lightVector" is ray from vertex (world space) to light (world space), normalized
	//  - "vertNormal" is normal of vertex (world space) with no translation
	// So both vectors are effectively coming from the origin (are only directions)
 	vec3 diffuse = max(0.0, dot(vertNorm.xyz, lightVec)) * diffuseObjectColour* theLights[lightIndex].diffuse.xyz;
				   	
	diffuse *= atten;
	
	// Specular: Phong
	// We do a simliar thing to the diffuse, but this time we are 
	//  seeing how much light is being reflected into THE EYE.
	// With diffuse, we don't care about the eye, as the colour
	//  we get is the same from every angle - it's only altered
	//  based on the light and normal position. 
	// The "most" light is reflected when the eye is directly
	//  in the path of the reflection vector, so let's calculate 
	//  that, first:
	//
	// The "view", "eye", or "camera" vector is the ray cast from 
	// 	the vertex, in "world" space, to the eye (also in "world" space).
	vec3 viewVector = normalize(eyeLocation - vertWorld);
	// The "light vector" is the ray from the vertex to the light.
	// (we've already calculated that)
	// 
	//  lightVector[index].xyz = theLights[index].position.xyz - vertWorld.xyz; (in VS)
	//  ---and---
	//	vec3 lightVec = normalize(lightVector[lightIndex].xyz); (FS: line 60)
	// 
	// Calculate the reflection vector from the light around normal:
	vec3 vecReflect = normalize(reflect(-lightVec,vertNorm));
	// The closer the angle between the eye and the vertex (the "viewVector") 
	//  and this reflection vector, the "brighter" the light is.
	//
	float specReflect = max(dot(vecReflect, viewVector), 0.0f);	// Silimar to diffuse, but uses view
	
	float lightSpecShinniness = theLights[lightIndex].specular.w;
//	lightSpecShinniness = 64.0f;
	float specPower = pow( specReflect, lightSpecShinniness );
	vec3 specular = specPower * theLights[lightIndex].specular.xyz;
	//vec3 specular = specPower * vec3(1.0f, 1.0f, 0.0f);
	specular *= atten;
	specular = clamp(specular, 0.0f, 1.0f);


//	return diffuse;
//	return specular;
//	return specReflect * vec3(1.0f, 1.0f, 1.0f);
//	return specReflect * vec3(1.0f, 1.0f, 1.0f) * atten;
	
	vec3 result = clamp(diffuse + specular, 0.0, 1.0f);
	
	return result;
}

void main()
{	

	if ( bIsSecondRenderPass )
	{
	
		vec3 results = texture( tex2ndPass, vec2(text_STU.x  ,  text_STU.y).xy  ).rgb;


		vec3 overlayResults = texture( texSamp2D_02, text_STU.xy ).rgb;
		if (results.r < 0.1f)
		{
			if (results.g > .35f && results.b > .35f)
			{
				float modifier =  0.2f;
	//			modifier/=10;
				results.r=0.f;
				results.g= results.g+sheen;
				results.b= results.g+sheen;
			}
		
		}
		//vec3 overlayResults = texture( texSamp2D_00, vec2(text_STU.x  ,  text_STU.y).xy  ).rgb;

	//	if (overlayResults.b < .1f)
	//	{
	//	results = overlayResults;//vec3(1.f,1.f,1.f);
	//	}

	
	//		float highest = results.r;
	//		if (highest < results.g)
	//		{
	//			highest = results.g;
	//		}

	//		if (highest < results.b)
	//		{
	//			highest = results.b;
	//		}
	
		out_Colour = vec4(results.rgb,1.f);
		
		return;
	}




	if ( bIsCubeMap )
	{
		vec3 diffuse0 = texture( texCubeMap01, vertNorm.xyz ).rgb;

		// the 1.5f is because the projector is dim
		out_Colour = vec4( diffuse0.rgb * 3.0f, 1.0f );
	
		return;
	}

	if ( bIsLineDebugDrawingPass )
	{
		out_Colour = diffuseColour;
		// HACK
		//out_Colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		return;
	}

	// Check to see if we are going to do the lighthing calculation
	if ( bUseDebugDiffuseNoLighting )
	{	// Bypass lighting entirely and set to uniform "debug" diffuse
		out_Colour.xyz = diffuseColour.xyz;
		//out_Colour.a = 1.0f;
		out_Colour.a = diffuseColour.a; 
		return;
	}

//	const float SLICEWIDTH = 0.1f;
//	if ( abs(vertWorld.x) - floor(abs(vertWorld.x)) < SLICEWIDTH )
//		discard;
//	if ( abs(vertWorld.y) - floor(abs(vertWorld.y)) < SLICEWIDTH )
//		discard;
//	if ( abs(vertWorld.z) - floor(abs(vertWorld.z)) < SLICEWIDTH )
//		discard;

	vec3 objectDiffuseColour = diffuseColour.xyz;

//uniform sampler2D texSamp2D_00; 
	
	float alphaOut = alpha;
	
	// are we using textures? 
	if ( bUseTextureAsDiffuse )
	{	// Replace the diffuse colour with what's in the texture
		//objectDiffuseColour = texture( texSamp2D_00, text_STU.xy ).rgb;
		
		vec3 diffuse0 = texture( texSamp2D_00, text_STU.xy ).rgb;
		vec3 diffuse1 = texture( texSamp2D_01, text_STU.xy ).rgb;
			
		vec3 diffuse2 = texture( texSamp2D_02, text_STU.xy ).rgb;	// Fench
		vec3 diffuse3 = texture( texSamp2D_03, text_STU.xy ).rgb;	// Fench alpha
		
		objectDiffuseColour = diffuse1;		// Alpha value for the fence
		
//		if ( diffuse3.r < 0.1f ) 
//		{
///			discard;
//		}
		// here take the transparency directly from the image
//		alphaOut = normalize(diffuse3.r + diffuse3.g + diffuse3.b );

		
		
//		diffuse0 *= 1.0f;
//		diffuse1 *= 1.0f;
		
//		objectDiffuseColour = mix(diffuse0, diffuse1, 0.5f);
		
		//vec3 diffuse = max(0.0, dot(vertNorm.xyz, lightVec)) 
		vec3 vecView = normalize(vertMV);
		float edgeValue;
		edgeValue = dot(vecView,vertNorm);

		if (edgeValue >.99f)
		{
		objectDiffuseColour*= vec3(0.01f);
		}
		else if (edgeValue >.9f)
		{
		objectDiffuseColour*= vec3(0.1f);
		}
		else if (edgeValue >.8f)
		{
		objectDiffuseColour*= vec3(0.6f);
		}
				else if (edgeValue >.5f)
		{
		objectDiffuseColour*= vec3(0.9f);
		}
	}
	
	if ( ! bUseLighting )
	{	// Don't perform the lighting calculation, just use existing diffuse
		out_Colour = vec4(objectDiffuseColour, alphaOut);	
	}
	else
	{	// Perform lighting calculation
		vec3 vecView = normalize(vertMV);
		
		// Do lighting calculation per light
		vec3 lightContrib = vec3(0.0f, 0.0f, 0.0f);
		for ( int index = 0; index != NUMBEROFLIGHTS; index++ )
		{
			//lightContrib += calcLightContribution( diffuseColour.xyz, vecView, index );
			lightContrib += calcLightContribution( objectDiffuseColour, vecView, index );
		}
		
		out_Colour = vec4(lightContrib, alphaOut);	
	}//if ( bUseLighting )

	if ( bIsImposterBlackIsAlpha )
	{
		// Treat anything that is close to "black" as transparent
		if (( (out_Colour.r + out_Colour.g + out_Colour.b)/3.0f) < 0.2f )
		{	// Fully transparent
			out_Colour.w = 0.0f;
		}
	}
	
	
	
	//// Reflection and refraction
	// amountReflective;		// 0 - 1.0   (no reflection to 100%)
	// amountRefractive;		// 0 - 1.0   (no refraction to 100%)
	// amountOfDiffuse;		// 0 - 1.0  (underlying "colour")
	// indexOfRefraction;	// 1.0f to something
	
	vec3 diffuse = out_Colour.rgb * amountOfDiffuse;	
	
	// Get reflected colour
	vec3 reflectColour = texture( texCubeMap01, vertNorm.xyz ).rgb;
	reflectColour *= amountReflective;
	
	// Get refraction colour
	vec3 vecIncident = eyeLocation - vertWorld;
	vec3 vecRefract = refract( vecIncident, vertNorm.xyz, indexOfRefraction );
	vec3 refractColour = texture( texCubeMap01, vecRefract ).rgb;	
	refractColour *= amountRefractive; 
	
	out_Colour.rgb = (diffuse + reflectColour + refractColour)/3.0f;
	
	
	out_Colour.rgb *= 2.0f;

	
//	out_Colour += diffuseColour;
	
}



