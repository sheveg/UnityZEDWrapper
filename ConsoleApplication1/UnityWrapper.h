// UnityWrapper.h - Contains declaration of Function class  
#pragma once  

#ifdef MATHLIBRARY_EXPORTS  
#define MATHLIBRARY_API __declspec(dllexport)   
#else  
#define UNITYWRAPPER_API __declspec(dllimport)   
#endif  
#include <sl\Camera.hpp>

extern "C" 
{
	UNITYWRAPPER_API int startSpatialMapping();

	UNITYWRAPPER_API void stopSpatialMappingAndSave();

	UNITYWRAPPER_API void stopSpatialMapping();

	UNITYWRAPPER_API sl::ERROR_CODE mappingLoop();

	UNITYWRAPPER_API sl::SPATIAL_MAPPING_STATE getMappingState();

	UNITYWRAPPER_API bool getVertices(sl::float3* vert, int size);

	UNITYWRAPPER_API bool getNormals(sl::float3* norm, int size);

	UNITYWRAPPER_API bool getUVs(sl::float2* uv, int size);

	UNITYWRAPPER_API bool getTriangles(sl::uint3* triangles, int size);

	UNITYWRAPPER_API int getMeshSize();

	UNITYWRAPPER_API int getTrianglesSize();

	UNITYWRAPPER_API bool getTexture(sl::uchar1* texture, int width, int height);

	UNITYWRAPPER_API int getTextureWidth();

	UNITYWRAPPER_API int getTextureHeight();
}