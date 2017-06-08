#include "GlobalStuff.h"
#include "Utilities.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "cGameObject.h"

// This will be drawn multiple times... 
unsigned int g_DebugSphereID = 0;
unsigned int g_SkyboxID = 0;

unsigned int g_1SidedQuadID = 0;
unsigned int g_2SidedQuadID = 0;




//static const struct
//{
//    float x, y;
//    float r, g, b;
//} vertices[6] =
struct vertXYRGB
{
    float x, y;				// 4 + 4 
    float r, g, b;			// 4 + 4 + 4  = 20 bytes
};

//struct vert_XYZ_RGB
//{
//    float x, y, z;				// 4 + 4 + 4
//    float r, g, b;			// 4 + 4 + 4  = 24 bytes
//};


void DrawDebugLightSpheres(void)
{
	//// And even more drawing...
	//cGameObject* pDebugBall = ::findObjectByID( ::g_DebugSphereID );

	//// Draw HOT PINK ball where the light is...
	//if ( ::bDrawLightDebugLocation )
	//{
	//	DrawDebugBall( glm::vec3( ::lightPos.x, ::lightPos.y, ::lightPos.z ),
	//	               glm::vec4( 255.0f/255.0f, 105.0f/255.0f, 180.0f/255.0f, 1.0f ), 0.25f );
	//}

	//const float BALLTRANSPARENCY = 0.75f;

	//if ( ::bDrawLightDebugSpheres )
	//{
	//	// Draw a red ball at 75% brightness
	//	float distanceAt75Percent = calcApproxDistFromAtten( 
	//					0.75f, // desired brighness
	//					::lightConstAtten, ::lightLinearAtten, ::lightQuadAtten, 
	//					0.0001f, // Accuracy (how close to 0.75 are we getting
	//					1000.0f );  // "Infinite" distance (so give up)
	//	DrawDebugBall( glm::vec3( ::lightPos.x, ::lightPos.y, ::lightPos.z ),
	//					glm::vec4( 1.0f, 0.0f, 0.0f, BALLTRANSPARENCY), distanceAt75Percent );

	//	// Draw a yellow ball at 50% brightness
	//	float distanceAt50Percent = calcApproxDistFromAtten( 
	//					0.50f, // desired brighness
	//					::lightConstAtten, ::lightLinearAtten, ::lightQuadAtten, 
	//					0.0001f, // Accuracy (how close to 0.75 are we getting
	//					1000.0f );  // "Infinite" distance (so give up)
	//	DrawDebugBall( glm::vec3( ::lightPos.x, ::lightPos.y, ::lightPos.z ),
	//					glm::vec4( 1.0f, 1.0f, 0.0f, BALLTRANSPARENCY), distanceAt50Percent );

	//	// Draw a green ball at 25% brightness
	//	float distanceAt25Percent = calcApproxDistFromAtten( 
	//					0.25f, // desired brighness
	//					::lightConstAtten, ::lightLinearAtten, ::lightQuadAtten, 
	//					0.0001f, // Accuracy (how close to 0.75 are we getting
	//					1000.0f );  // "Infinite" distance (so give up)
	//	DrawDebugBall( glm::vec3( ::lightPos.x, ::lightPos.y, ::lightPos.z ),
	//					glm::vec4( 0.0f, 1.0f, 0.0f, BALLTRANSPARENCY), distanceAt25Percent );

	//	// Draw a green ball at 00% brightness
	//	float distanceAt01Percent = calcApproxDistFromAtten( 
	//					0.01f, // desired brighness
	//					::lightConstAtten, ::lightLinearAtten, ::lightQuadAtten, 
	//					0.0001f, // Accuracy (how close to 0.75 are we getting
	//					1000.0f );  // "Infinite" distance (so give up)
	//	DrawDebugBall( glm::vec3( ::lightPos.x, ::lightPos.y, ::lightPos.z ),
	//					glm::vec4( 0.0f, 1.0f, 1.0f, BALLTRANSPARENCY), distanceAt01Percent );

	//}//if ( ::bDrawLightDebugSpheres )

	return;
}


void DrawFlameParticle( glm::vec3 position,
				    glm::vec4 colour, float scale )
{
	// And even more drawing...
	cGameObject* pDebugBall = ::findObjectByID( ::g_DebugSphereID );

	// Save old state
	//glm::vec3 oldPosition( pDebugBall->x, pDebugBall->y, pDebugBall->z );
	glm::vec3 oldPosition = pDebugBall->bodyDef->position;
	//glm::vec3 oldColour( pDebugBall->solid_R, pDebugBall->solid_G, pDebugBall->solid_B );
	glm::vec4 oldColour = pDebugBall->debugRGBA;
	float oldScale = pDebugBall->scale;
	bool oldIsVisible = pDebugBall->bIsVisible;
	bool oldIsWireframe = pDebugBall->bIsWireframe;
	bool oldbUseDebugColours = pDebugBall->bUseDebugColours;

	// Set values
	//pDebugBall->x = position.x; 
	//pDebugBall->y = position.y;	
	//pDebugBall->z = position.z;
	pDebugBall->bodyDef->position = position;
	//pDebugBall->solid_R = colour.x;
	//pDebugBall->solid_G = colour.y;
	//pDebugBall->solid_B = colour.z;
	pDebugBall->debugRGBA = colour;
	pDebugBall->bIsWireframe = false;
	pDebugBall->bIsVisible = true;
	pDebugBall->bUseDebugColours = false;
	pDebugBall->scale = scale;
	pDebugBall->texture00Name = "fireball.bmp";
	pDebugBall->bIsImposter = true;
	glm::mat4x4 matModel = glm::mat4x4(1.0f);	// Identity
	DrawObject( pDebugBall, matModel, false );

	// Restore state...
	//pDebugBall->x = oldPosition.x;
	//pDebugBall->y = oldPosition.y;
	//pDebugBall->z = oldPosition.z;
	pDebugBall->bodyDef->position = oldPosition;
	//pDebugBall->solid_R = oldColour.x;
	//pDebugBall->solid_G = oldColour.y;
	//pDebugBall->solid_B = oldColour.z;
	pDebugBall->debugRGBA = oldColour;
	pDebugBall->scale = oldScale;
	pDebugBall->bIsVisible = oldIsVisible;
	pDebugBall->bIsWireframe = oldIsWireframe;
	pDebugBall->bUseDebugColours = oldbUseDebugColours;
	
	return;
}


void DrawGravityParticle(glm::vec3 position,
	glm::vec4 colour, float scale)
{
	// And even more drawing...
	cGameObject* pDebugBall = ::findObjectByID(::g_DebugSphereID);

	// Save old state
	//glm::vec3 oldPosition( pDebugBall->x, pDebugBall->y, pDebugBall->z );
	glm::vec3 oldPosition = pDebugBall->bodyDef->position;
	//glm::vec3 oldColour( pDebugBall->solid_R, pDebugBall->solid_G, pDebugBall->solid_B );
	glm::vec4 oldColour = pDebugBall->debugRGBA;
	float oldScale = pDebugBall->scale;
	bool oldIsVisible = pDebugBall->bIsVisible;
	bool oldIsWireframe = pDebugBall->bIsWireframe;
	bool oldbUseDebugColours = pDebugBall->bUseDebugColours;

	// Set values
	//pDebugBall->x = position.x; 
	//pDebugBall->y = position.y;	
	//pDebugBall->z = position.z;
	pDebugBall->bodyDef->position = position;
	//pDebugBall->solid_R = colour.x;
	//pDebugBall->solid_G = colour.y;
	//pDebugBall->solid_B = colour.z;
	pDebugBall->debugRGBA = colour;
	pDebugBall->bIsWireframe = false;
	pDebugBall->bIsVisible = true;
	pDebugBall->bUseDebugColours = false;
	pDebugBall->scale = scale;
	pDebugBall->texture00Name = "GravityBall.bmp";
	//pDebugBall->bIsImposter = true;
	glm::mat4x4 matModel = glm::mat4x4(1.0f);	// Identity
	DrawObject(pDebugBall, matModel, false);

	// Restore state...
	//pDebugBall->x = oldPosition.x;
	//pDebugBall->y = oldPosition.y;
	//pDebugBall->z = oldPosition.z;
	pDebugBall->bodyDef->position = oldPosition;
	//pDebugBall->solid_R = oldColour.x;
	//pDebugBall->solid_G = oldColour.y;
	//pDebugBall->solid_B = oldColour.z;
	pDebugBall->debugRGBA = oldColour;
	pDebugBall->scale = oldScale;
	pDebugBall->bIsVisible = oldIsVisible;
	pDebugBall->bIsWireframe = oldIsWireframe;
	pDebugBall->bUseDebugColours = oldbUseDebugColours;

	return;
}



void DrawQuad( bool bTwoSided, glm::vec3 position, glm::vec3 rotation, std::string texture, float scale,
			   glm::vec4 debugColour, bool bUseDebugColour, 
			   bool bIsWireFrame )
{
	cGameObject* pQuad = 0;

	if ( bTwoSided )	{	pQuad = ::findObjectByID( ::g_2SidedQuadID ); }
	else				{	pQuad = ::findObjectByID( ::g_1SidedQuadID ); }

	// Save old state
//	glm::vec3 oldPosition( pQuad->x, pQuad->y, pQuad->z );
	glm::vec3 oldPosition = pQuad->bodyDef->position;
//	glm::vec3 oldColour( pQuad->solid_R, pQuad->solid_G, pQuad->solid_B );
	glm::vec4 oldColour = pQuad->debugRGBA;
	float oldScale = pQuad->scale;
	bool oldIsVisible = pQuad->bIsVisible;
	bool oldIsWireframe = pQuad->bIsWireframe;
	bool oldbUseDebugColours = pQuad->bUseDebugColours;
//	glm::vec3 oldPreRotation( pQuad->pre_Rot_X, pQuad->pre_Rot_Y, pQuad->pre_Rot_Z );
//	glm::vec3 oldPostRotation( pQuad->post_Rot_X, pQuad->post_Rot_Y, pQuad->post_Rot_Z );
	glm::quat oldOrientation = pQuad->bodyDef->qOrientation;

	// Set values
	//pQuad->x = position.x; 
	//pQuad->y = position.y;	
	//pQuad->z = position.z;
	pQuad->bodyDef->position = position;
	pQuad->bUseDebugColours = bUseDebugColour;
	//pQuad->solid_R = debugColour.x;
	//pQuad->solid_G = debugColour.y;
	//pQuad->solid_B = debugColour.z;
	pQuad->debugRGBA = debugColour;
	pQuad->bIsWireframe = bIsWireFrame;
	pQuad->bIsVisible = true;
	pQuad->scale = scale;
	// Added for the imposter
//	pQuad->post_Rot_X = rotation.x;
//	pQuad->post_Rot_Y = rotation.y;
//	pQuad->post_Rot_Z = rotation.z;

	glm::mat4x4 matModel = glm::mat4x4(1.0f);	// Identity

	DrawObject( pQuad, matModel, false );

	// Restore state...
	//pQuad->x = oldPosition.x;
	//pQuad->y = oldPosition.y;
	//pQuad->z = oldPosition.z;
	pQuad->bodyDef->position = oldPosition;
	pQuad->bUseDebugColours = oldbUseDebugColours;
	//pQuad->solid_R = oldColour.x;
	//pQuad->solid_G = oldColour.y;
	//pQuad->solid_B = oldColour.z;
	pQuad->debugRGBA = oldColour;
	pQuad->bIsWireframe = oldIsWireframe;
	pQuad->bIsVisible = oldIsVisible;
	pQuad->scale = oldScale;
///	pQuad->post_Rot_X = oldPostRotation.x;
//	pQuad->post_Rot_Y = oldPostRotation.y;
//	pQuad->post_Rot_Z = oldPostRotation.z;
	pQuad->bodyDef->qOrientation = oldOrientation;


	return;
}



void SetUpTextureBindings( cGameObject* pCurGO )
{
	// Get texture number


	std::string textureName = pCurGO->texture00Name;
//	GLuint texture00Number = ::g_pTextureManager->getTextureIDFromName("brickTexture_square_1024pixels.bmp");
//	//GLuint texture00Number = ::g_pTextureManager->getTextureIDFromName("smoketex.bmp");
//	// Texture binding...
//	GLuint texture00Unit = 0;		// Texture unit go from 0 to 79
//
//	glActiveTexture( texture00Unit + GL_TEXTURE0 );		// GL_TEXTURE0 = 33984
//	glBindTexture( GL_TEXTURE_2D, texture00Number );
////	glBindTextureUnit( texture00Unit, texture00Number );			// OpenGL 4.5+ only
//	// Set texture unit in the shader, too
//	glUniform1i( UniformLoc_texSamp2D_00, texture00Unit );


	GLuint texture01Number = ::g_pTextureManager->getTextureIDFromName(textureName);
	GLuint texture01Unit = 1;		// Texture unit go from 0 to 79
	glActiveTexture( texture01Unit + GL_TEXTURE0 );		// GL_TEXTURE0 = 33984
	glBindTexture( GL_TEXTURE_2D, texture01Number );
////	glBindTextureUnit( 1, texture01Number );			// OpenGL 4.5+ only
//	// Set texture unit in the shader, too
	glUniform1i( UniformLoc_texSamp2D_01, texture01Unit );

	{
		GLuint textureNumber = ::g_pTextureManager->getTextureIDFromName("FenceActual.bmp");
		GLuint textureUnit = 2;		// Texture unit go from 0 to 79
		glActiveTexture( textureUnit + GL_TEXTURE0 );		// GL_TEXTURE0 = 33984
		glBindTexture( GL_TEXTURE_2D, textureNumber );
	////	glBindTextureUnit( 1, texture01Number );			// OpenGL 4.5+ only
	//	// Set texture unit in the shader, too
		glUniform1i( UniformLoc_texSamp2D_02, textureUnit );
	}
	{
		GLuint textureNumber = ::g_pTextureManager->getTextureIDFromName("FenceAlphaMask.bmp");
		GLuint textureUnit = 3;		// Texture unit go from 0 to 79
		glActiveTexture( textureUnit + GL_TEXTURE0 );		// GL_TEXTURE0 = 33984
		glBindTexture( GL_TEXTURE_2D, textureNumber );
	////	glBindTextureUnit( 1, texture01Number );			// OpenGL 4.5+ only
	//	// Set texture unit in the shader, too
		glUniform1i( UniformLoc_texSamp2D_03, textureUnit );
	}



	glUniform1i( UniformLoc_bUseTextureAsDiffuse, TRUE );		// 1

	return;
}

unsigned int g_NumberOfIndicesRendered = 0;


void DrawObject( cGameObject* pCurGO, 
				 glm::mat4x4 matModel,		// NOT passing by reference 
				 bool bUseLighting /*= true*/ )
{
	if ( ! pCurGO->bIsVisible )
	{
		// Don't draw it
		return;
	}

	std::string meshModelName;
	if (!pCurGO->usesLowDetailMesh)
	{
	meshModelName = pCurGO->meshName;
	}
	else
	{
		if (glm::distance(pCurGO->bodyDef->position, camera1->position) > 30.f)
		{
			meshModelName = pCurGO->lowDetailMesh;
		}
		else
		{
			meshModelName = pCurGO->meshName;
		}
	}
	GLuint VAO_ID = 0;
	int numberOfIndices = 0;
	float unitScale = 1.0f;
	if ( ! ::g_pTheMeshTypeManager->LookUpMeshInfo( meshModelName,
													VAO_ID, 
													numberOfIndices,
													unitScale ) )
	{	// Skip the rest of the for loop, but continue
		return;
	}

//			glm::mat4x4 mvp(1.0f);		// Model, View, Projection matrix

//			mat4x4_identity(m);


//	glm::mat4x4 matModel = glm::mat4x4(1.0f);


	// Pre-rotation
	// Translation
	// Post-rotation
	// Scale

	// Pre in that it's BEFORE the translation
//	matModel = glm::rotate( matModel, pCurGO->pre_Rot_X, glm::vec3( 1.0f, 0.0f, 0.0f) );
//	matModel = glm::rotate( matModel, pCurGO->pre_Rot_Y, glm::vec3( 0.0f, 1.0f, 0.0f) );
//	matModel = glm::rotate( matModel, pCurGO->pre_Rot_Z, glm::vec3( 0.0f, 0.0f, 1.0f) );

//	matModel = glm::translate(matModel, glm::vec3( pCurGO->x, pCurGO->y, pCurGO->z ));
	matModel = glm::translate(matModel, pCurGO->bodyDef->position );
	// Pre in that it's AFTER the translation
//	matModel = glm::rotate( matModel, pCurGO->post_Rot_X, glm::vec3( 1.0f, 0.0f, 0.0f) );
//	matModel = glm::rotate( matModel, pCurGO->post_Rot_Y, glm::vec3( 0.0f, 1.0f, 0.0f) );
//	matModel = glm::rotate( matModel, pCurGO->post_Rot_Z, glm::vec3( 0.0f, 0.0f, 1.0f) );
	glm::mat4 matRotation( pCurGO->bodyDef->qOrientation );
	matModel = matModel * matRotation;

	//matModel *= matRotation;	// or this

	//
	float actualScale = pCurGO->scale * unitScale;

	matModel = glm::scale(matModel, glm::vec3( actualScale, actualScale, actualScale ));


	if ( pCurGO->bIsWireframe )
	{	// Turn off backface culling
		// Enable "wireframe" polygon mode
		glPolygonMode( GL_FRONT_AND_BACK,	// GL_FRONT_AND_BACK is the only thing you can pass here
						GL_LINE );			// GL_POINT, GL_LINE, or GL_FILL
		glDisable( GL_CULL_FACE );	
	}
	else
	{	// "Regular" rendering: 
		// Turn on backface culling
		// Turn polygon mode to solid (Fill)
		glCullFace( GL_BACK );		// GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK
		glEnable( GL_CULL_FACE );
		glPolygonMode( GL_FRONT_AND_BACK,	// GL_FRONT_AND_BACK is the only thing you can pass here
					   GL_FILL );			// GL_POINT, GL_LINE, or GL_FILL
	}

	SetUpTextureBindings( pCurGO );

	//**********************************************************
	glUniform1f( UniformLoc_amountReflective, pCurGO->amountReflective );
	glUniform1f( UniformLoc_amountRefractive, pCurGO->amountRefractive );
	glUniform1f( UniformLoc_amountOfDiffuse, pCurGO->amountOfDiffuse );
	glUniform1f( UniformLoc_indexOfRefraction, pCurGO->indexOfRefraction );
	//**********************************************************


	if ( pCurGO->bIsImposter )
	{
		glUniform1i( UniformLoc_bIsImposterBlackIsAlpha, TRUE );
	}
	else
	{		
		glUniform1i( UniformLoc_bIsImposterBlackIsAlpha, FALSE );
	}


	if ( bUseLighting )
	{
		glUniform1i( UniformLoc_bUseLighting, TRUE );
	}
	else 
	{		
		glUniform1i( UniformLoc_bUseLighting, FALSE );
	}

		

//			mat4x4_mul(mvp, p, m);
		// This is being calculated in the shader now...
//			mvp = p * v * m;

//			glUseProgram(program);
	::g_pTheShaderManager->useShaderProgram("simple");


	glUniformMatrix4fv( locID_matModel, 1, GL_FALSE, 
				        (const GLfloat*) glm::value_ptr(matModel) );



	// Setting the uniform colours
	//glUniform4f( UniformLoc_ID_objectColour, 
	//				pCurGO->solid_R, pCurGO->solid_G,  pCurGO->solid_B, 1.0f ); 
	glUniform4f( UniformLoc_ID_objectColour, 
				 pCurGO->debugRGBA.r, pCurGO->debugRGBA.g, pCurGO->debugRGBA.b, pCurGO->debugRGBA.a );
	if ( pCurGO->bIsWireframe )
	{
		glUniform1i( UniformLoc_ID_isWireframe, TRUE );		// 1
	}
	else 
	{
		glUniform1i( UniformLoc_ID_isWireframe, FALSE );	// 0
	}
	
	if ( pCurGO->bUseDebugColours )
	{
		glUniform1i( UniformLoc_ID_bUseDebugDiffuseNoLighting, TRUE );		// 1
	}
	else 
	{
		glUniform1i( UniformLoc_ID_bUseDebugDiffuseNoLighting, FALSE );	// 0
	}

	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawArrays(GL_TRIANGLES, 0, numberofVerts);

	// Drawing indirectly from the index buffer

	// "turns on" transparency
	glEnable( GL_BLEND );		// Enables "blending"
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );


	glUniform1f( UniformLoc_alpha, 0.0f );
	glUniform1f( UniformLoc_alpha, pCurGO->alpha );

	::g_NumberOfIndicesRendered += numberOfIndices;

	glBindVertexArray( VAO_ID );
	glDrawElements( GL_TRIANGLES, 
					numberOfIndices, 
					GL_UNSIGNED_INT,	// Each index is how big?? 
					(GLvoid*) 0 );		// Starting point in buffer
	glBindVertexArray(0);


	// Draw any children
	for ( int index = 0; index != pCurGO->vec_pChildren.size(); index++ )
	{
		DrawObject( pCurGO->vec_pChildren[index], 
				    matModel,		// Parent's final world matrix
				    bUseLighting );
	}

	// Draw all the debug lines from the "debug line" VAO
	unsigned int LineBuffer_VAO_ID = 0;
	unsigned int numberOfVerticesToDraw = 0;
	if ( ::g_pTheMeshTypeManager->GetDebugLineBufferForRender( LineBuffer_VAO_ID, 
		                                                       numberOfVerticesToDraw ) )
	{
		if ( numberOfVerticesToDraw > 0 )
		{
			::g_pTheShaderManager->useShaderProgram("simple");


			matModel = glm::mat4x4(1.0f);
			glUniformMatrix4fv( locID_matModel, 1, GL_FALSE, 
								(const GLfloat*) glm::value_ptr(matModel) );


			glUniform1i( UniformLoc_ID_bIsLineDebugDrawingPass, TRUE ); 

			glPolygonMode( GL_FRONT_AND_BACK,	// GL_FRONT_AND_BACK is the only thing you can pass here
							GL_LINE );			// GL_POINT, GL_LINE, or GL_FILL
			glDisable( GL_CULL_FACE );	

			//glUniform1i( UniformLoc_ID_bUseDebugDiffuseNoLighting, FALSE );		// 1

			//glUniform4f( UniformLoc_ID_objectColour, 
			//			 1.0f, 1.0f, 1.0f, 1.0f );
			//glUniform1i( UniformLoc_ID_isWireframe, FALSE );		// 1

			// "Point to" (bind) the VAO with the lines in it
			glBindVertexArray( LineBuffer_VAO_ID );
			glDrawArrays( GL_LINES, 
						  0,	/*start from start of buffer*/
						  numberOfVerticesToDraw );
			// Point to (bind) to nothing
			glBindVertexArray( 0 );

			glUniform1i( UniformLoc_ID_bIsLineDebugDrawingPass, FALSE ); 

		}
	}//if ( ! ::g_pTheMeshTypeManager->GetDebugLineBufferForRender



	return;
}



