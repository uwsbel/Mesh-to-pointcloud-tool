#include <iostream>
#include <string>

#include "mesh.h"

int main(int argc, char* argv[])
{
	std::cout << "New program!" << std::endl;

	std::string filename = "samples/hmmwv_chassis_simple.obj";

	struct mesh m = loadMesh(filename);
	double delta = 0.05;
	std::cout << "Loaded mesh" << std::endl;
	// m.printInfo();
	// m.printVertices();
	// m.printTriangles();

	glm::dvec3 ray_origin = glm::dvec3(0.0,0.0,0.0);
	glm::dvec3 startCoords = getMinVert(m.vertices);
	glm::dvec3 endCoords = getMaxVert(m.vertices);
	std::vector<glm::dvec3> point_cloud;

	printf("start coords: %f, %f, %f\n", startCoords.x, startCoords.y, startCoords.z);
	printf("end coords: %f, %f, %f\n", endCoords.x, endCoords.y, endCoords.z);
	for(double x = startCoords.x; x < endCoords.x; x += delta){
		ray_origin.x = x;
		for(double y = startCoords.y; y < endCoords.y; y += delta){
			ray_origin.y = y;
			for(double z = startCoords.z; z < endCoords.z; z += delta){
				ray_origin.z = z;
				if(isInsideMesh(m,ray_origin)) {
					point_cloud.push_back(glm::dvec3(x,y,z));
				}

			}
		}
	}

	for (int i = 0; i < point_cloud.size(); ++i)
	{
		// printf("point_cloud[%d]: %f, %f, %f\n", i, point_cloud[i].x, point_cloud[i].y, point_cloud[i].z);
		printf("%f, %f, %f\n", point_cloud[i].x, point_cloud[i].y, point_cloud[i].z);
	}



	return 0;
}	
