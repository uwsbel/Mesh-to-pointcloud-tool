#include <iostream>
#include <string>
#include <fstream>

#include "mesh.h"


int main(int argc, char* argv[])
{
	std::cout << "New program!" << std::endl;

	std::string filename = "samples/L_share.obj";

	struct mesh m = loadMesh(filename);
	double delta = 0.001;
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
	int total_num = 0;
	for(double x = startCoords.x; x < endCoords.x; x += delta){
		ray_origin.x = x + 1e-9;
		for(double y = startCoords.y; y < endCoords.y; y += delta){
			ray_origin.y = y + 1e-9;
			for(double z = startCoords.z; z < endCoords.z; z += delta){
				ray_origin.z = z + 1e-9;
				if(isInsideMesh(m,ray_origin)) {
					point_cloud.push_back(glm::dvec3(x,y,z));
					total_num++;
					printf("current number of particles is: %d \n", total_num);
				}

			}
		}
	}

    /// write the Penetration into file
    std::ofstream myFile;
    myFile.open("./sphParticles.txt", std::ios::trunc);
    myFile.close();
    myFile.open("./sphParticles.txt", std::ios::app);
	myFile << "x" << "," << "y" << "," << "z" << "\n";
	for (int i = 0; i < point_cloud.size(); ++i)
	{
		// printf("point_cloud[%d]: %f, %f, %f\n", i, point_cloud[i].x, point_cloud[i].y, point_cloud[i].z);
		// printf("%f, %f, %f\n", point_cloud[i].x, point_cloud[i].y, point_cloud[i].z);
		myFile << point_cloud[i].x << "," << "\t" 
			   << point_cloud[i].y << "," << "\t" 
			   << point_cloud[i].z << "," << "\n";
	}
	myFile.close();



	return 0;
}	
