// UnityWrapperTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "UnityWrapper.h"
#include <iostream>
#include <stdlib.h>
#include "sl\Camera.hpp"
#include <ctime>
#include <vector>

using namespace std;

int loopTime = 5;

int main()
{
	cout << "Starting" << endl;
	cout << sl::errorCode2str((sl::ERROR_CODE)startSpatialMapping()) << endl;
	time_t start = time(0);
	int timeLeft = loopTime;
	cout << "Main loop start" << endl;
	// run for 10 seconds;
	while (timeLeft > 0)
	{
		mappingLoop();
		time_t end = time(0);
		time_t timeTaken = end - start;
		timeLeft = loopTime - timeTaken;
		cout << setw(3) << timeLeft << '\r' << flush;
	}

	stopSpatialMappingAndSave();
	cout << "Main loop done" << endl;
	int verticesSize = getMeshSize();
	sl::float3* vertices = new sl::float3[verticesSize];
	sl::float3* normals = new sl::float3[verticesSize];
	sl::float2* uvs = new sl::float2[verticesSize];

	int trianglesSize = getTrianglesSize();
	sl::uint3* triangles = new sl::uint3[trianglesSize];

	

	getVertices(vertices, verticesSize);
	getNormals(normals, verticesSize);
	getUVs(uvs, verticesSize);
	getTriangles(triangles, trianglesSize);

	sl::Mesh mesh;
	mesh.vertices.resize(verticesSize);
	mesh.normals.resize(verticesSize);
	mesh.uv.resize(verticesSize);
	mesh.triangles.resize(trianglesSize);

	memcpy(&mesh.vertices[0], &vertices[0], sizeof(sl::float3) * verticesSize);
	memcpy(&mesh.normals[0], &normals[0], sizeof(sl::float3) * verticesSize);
	memcpy(&mesh.uv[0], &uvs[0], sizeof(sl::float2) * verticesSize);
	memcpy(&mesh.triangles[0], &triangles[0], sizeof(sl::uint3) * trianglesSize);

	//mesh.vertices = vector<sl::float3>(vertices, vertices + sizeof(sl::float3) * verticesSize);
	/*mesh.normals = vector<sl::float3>(normals, normals + sizeof(sl::float3) * verticesSize);
	mesh.uv = vector<sl::float2>(uvs, uvs + sizeof(sl::float2) * verticesSize);
	mesh.triangles = vector<sl::uint3>(triangles, triangles + sizeof(sl::uint3) * trianglesSize);*/

	mesh.save("hackyObject.obj");

	/*int width = getTextureWidth();
	int height = getTextureHeight();

	sl::uchar1* texture = new sl::uchar1[width * height * 3];
	getTexture(texture, width, height);*/
	/*for (int i = 0; i < 100; i++) 
	{
		cout << (float)texture[i].r << " : " << (float)texture[i].y << " : " << (float)texture[i].z << endl;
	}*/
	

	/*vector<float3> v(vertices, vertices + size);
	cout << v.size() << endl;*/


	//deleteVerticesArray(vertices);
	cout << "Done" << endl;
	getchar();

	//delete[] vertices;
	//delete[] normals;
	//delete[] uvs;
	//delete[] triangles;

    return 0;
}

