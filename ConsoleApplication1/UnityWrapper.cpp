// MathLibrary.cpp : Defines the exported functions for the DLL application.  
// Compile by using: cl /EHsc /DMATHLIBRARY_EXPORTS /LD MathLibrary.cpp  
// This wrapper extends the unity wrapper by the spatial mapping functions of the ZED C++ API. For now ,1st of August 2017,
// they decided for whatever reason to not include this functionality into Unity.

#include "stdafx.h"  
#include "UnityWrapper.h"
#include <stdlib.h>
#include <vector>

static sl::Camera zed;
static sl::SpatialMappingParameters spatial_mapping_params2;
static sl::MeshFilterParameters filter_params;
static sl::TRACKING_STATE tracking_state;
static sl::Pose pose;

bool mapping_is_started = false;

static sl::Mesh mesh;

static sl::SPATIAL_MAPPING_STATE mapping_state;

extern "C" 
{
	
	int startSpatialMapping() 
	{
		mesh.clear();
		// create initialization parameters
		sl::InitParameters parameters;
		parameters.depth_mode = sl::DEPTH_MODE_PERFORMANCE; // Use QUALITY depth mode to improve mapping results
		parameters.coordinate_units = sl::UNIT_METER;
		parameters.coordinate_system = sl::COORDINATE_SYSTEM_LEFT_HANDED_Y_UP; // Unity3D coordinate system
		parameters.camera_resolution = sl::RESOLUTION_HD720;
		// create spatial mapping parameters
		sl::SpatialMappingParameters spatial_mapping_params;
		spatial_mapping_params.range_meter.second = sl::SpatialMappingParameters::get(sl::SpatialMappingParameters::RANGE_MEDIUM);
		spatial_mapping_params.resolution_meter = sl::SpatialMappingParameters::get(sl::SpatialMappingParameters::RESOLUTION_LOW);
		spatial_mapping_params.save_texture = true;
		spatial_mapping_params.keep_mesh_consistent = false;
		// open zed camera
		sl::ERROR_CODE err = zed.open();
		if (err != sl::ERROR_CODE::SUCCESS) 
		{
			zed.close();
			return err;
		}

		// set mesh filter parameters
		filter_params.set(sl::MeshFilterParameters::FILTER_LOW);

		// Enable tracking and spatial mapping.
		// Note: spatial mapping needs the motion tracking to operate. Don't forget to enable it as well.
		zed.enableTracking();
		zed.enableSpatialMapping(spatial_mapping_params);
		mapping_is_started = true;
		zed.requestMeshAsync();

		return sl::ERROR_CODE::SUCCESS;
	}

	void stopSpatialMapping() 
	{
		mapping_is_started = false;
		// HOW TO GET TEXTURE	
		zed.extractWholeMesh(mesh);
		mesh.filter(filter_params);
		mesh.applyTexture();

		zed.disableSpatialMapping();
		zed.disableTracking();
		zed.close();
	}

	void stopSpatialMappingAndSave() 
	{
		mapping_is_started = false;
		zed.extractWholeMesh(mesh);
		mesh.filter(filter_params);
		mesh.applyTexture();
		mesh.save("randomShit.obj");

		zed.disableSpatialMapping();
		zed.disableTracking();
		zed.close();
	}

	sl::ERROR_CODE mappingLoop()
	{
		sl::ERROR_CODE err = sl::ERROR_CODE::ERROR_CODE_FAILURE;
		if (mapping_is_started)
		{
			if ((err = zed.grab()) == sl::SUCCESS)
			{
				zed.requestMeshAsync();
				if ((err = zed.getMeshRequestStatusAsync()) == sl::SUCCESS) {
					zed.retrieveMeshAsync(mesh);
				}
				else return err;		
			}
			else return err;
		}
		return err;
	}

	sl::SPATIAL_MAPPING_STATE getMappingState() 
	{
		mapping_state = zed.getSpatialMappingState();
		return mapping_state;
	}

	int getMeshSize()
	{
		return mesh.vertices.size() > 0 ? mesh.vertices.size() : -1;
	}

	int getTrianglesSize() 
	{
		return mesh.triangles.size() > 0 ? mesh.triangles.size() : -1;
	}

	bool getVertices(sl::float3* vert, int size) 
	{
		if (mesh.vertices.size() > 0) 
		{
			memcpy(vert, &(mesh.vertices[0]), sizeof(sl::float3) * size);
			return true;
		}
		else return false;
	}

	bool getNormals(sl::float3* norm, int size) 
	{
		if (mesh.normals.size() > 0)
		{
			memcpy(norm, &(mesh.normals[0]), sizeof(sl::float3) * size);
			return true;
		}
		else return false;
	}

	bool getUVs(sl::float2* uv, int size) 
	{
		if (mesh.uv.size() > 0)
		{
			memcpy(uv, &(mesh.uv[0]), sizeof(sl::float2) * size);
			return true;
		}
		else return false;
	}

	bool getTriangles(sl::uint3* triangles, int size) 
	{
		if (mesh.triangles.size() > 0)
		{
			memcpy(triangles, &(mesh.triangles[0]), sizeof(sl::uint3) * size);
			return true;
		}
		else return false;
	}

	bool getTexture(sl::uchar1* texture, int width, int height)
	{
		if (mesh.uv.size() > 0 && mesh.texture.data.isInit()) {
			sl::MEM memoryType = mesh.texture.data.getMemoryType();
			memcpy(texture, mesh.texture.data.getPtr<sl::uchar1>(memoryType),width * height * 3);
			return true;
		}
		return false;
	}

	int getTextureWidth()
	{
		if (mesh.uv.size() > 0) 
		{
			return mesh.texture.data.getWidth();
		}
		else return -1;
	}

	int getTextureHeight()
	{
		if (mesh.uv.size() > 0) 
		{
			return mesh.texture.data.getHeight();
		}
		else return -1;
	}
}