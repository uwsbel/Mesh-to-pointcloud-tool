//
// Mesh to pointcloud exporter
//
// Author: Charles Ricchio
//
// Copyright (c) 2015 UW SBEL
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file at the top level of the distribution
//
#pragma once

// C++ Includes
#include <vector>

// External libraries
#include "tinyobjloader/tiny_obj_loader.h"
#include "glm/glm.hpp"

#define EPSILON 0.000001

struct triangle {
	int a;
	int b;
	int c;
	void print() {
		printf("	Triangle: %d, %d, %d\n", a, b, c);
	}
};


struct vertex {
	glm::dvec3 position;
	glm::dvec3 normal;
	void print() {
		printf("	Vertex Position: %f, %f, %f\n", position.x, position.y, position.z);
		printf("	Vertex Normal: %f, %f, %f\n", normal.x, normal.y, normal.z);
	}	
};


struct mesh {
	std::vector<triangle> triangles;
	std::vector<vertex> vertices;

	void printInfo() {
		std::cout << "Num Triangles: " << triangles.size() << std::endl;
		std::cout << "Num Vertices: " << vertices.size() << std::endl;
	}
	void printVertices() {
		std::cout << "Vertices: " << std::endl;

		for (unsigned int i = 0; i < vertices.size(); ++i){
			printf("Vertices[%d]: \n",i);
			vertices[i].print();
		}
	}
	void printTriangles() {
		std::cout << "Triangles: " << std::endl;
		for (unsigned int i = 0; i < triangles.size(); ++i){
			printf("Triangle[%d]: \n",i);
			triangles[i].print();
		}
	}



};


glm::dvec3 getMinVert(std::vector<vertex> vertices) {
	glm::dvec3 minV = vertices[0].position;
	glm::dvec3 currV = vertices[0].position;
	for (unsigned int i = 0; i < vertices.size(); ++i){
		currV = vertices[i].position;
		if(minV.x > currV.x) minV.x = currV.x;
		if(minV.y > currV.y) minV.y = currV.y;
		if(minV.z > currV.z) minV.z = currV.z;
	}
	return minV;
}


glm::dvec3 getMaxVert(std::vector<vertex> vertices) {
	glm::dvec3 maxV = vertices[0].position;
	glm::dvec3 currV = vertices[0].position;
	for (unsigned int i = 0; i < vertices.size(); ++i){
		currV = vertices[i].position;
		if(maxV.x < currV.x) maxV.x = currV.x;
		if(maxV.y < currV.y) maxV.y = currV.y;
		if(maxV.z < currV.z) maxV.z = currV.z;
	}
	return maxV;
}

mesh loadMesh(const std::string& filename) {

	std::cout << filename << std::endl;
	std::vector<tinyobj::shape_t> model_shapes{ 0 };
	std::vector<tinyobj::material_t> model_mtls{ 0 };
	tinyobj::LoadObj(model_shapes, model_mtls, filename.c_str());

	mesh _ret;

	for (unsigned int i = 0; i < model_shapes[0].mesh.indices.size(); i += 3) {
		_ret.triangles.push_back(triangle{ model_shapes[0].mesh.indices[i], model_shapes[0].mesh.indices[i + 1], model_shapes[0].mesh.indices[i + 2] });
	}

	for (unsigned int i = 0; i < model_shapes[0].mesh.positions.size(); i += 3) {
		vertex v;
		v.position = glm::dvec3(model_shapes[0].mesh.positions[i], model_shapes[0].mesh.positions[i + 1], model_shapes[0].mesh.positions[i + 2]);
		if (model_shapes[0].mesh.normals.size() != 0) {
			v.normal = glm::dvec3(model_shapes[0].mesh.normals[i], model_shapes[0].mesh.normals[i + 1], model_shapes[0].mesh.normals[i + 2]);
		}
		_ret.vertices.push_back(std::move(v));
	}

	return _ret;

}


int triangle_intersection(const glm::dvec3   v1,  // Triangle vertices
                          const glm::dvec3   v2,
                          const glm::dvec3   v3,
                          const glm::dvec3    orig,  //Ray origin
                          const glm::dvec3    dir,  //Ray direction
                          double* out )
{

	// printf("Starting intersection\n");
	glm::dvec3 edge1, edge2;  //Edgedge1, Edgedge2
	glm::dvec3 pvec, qvec, tvec;
	double det, inv_det, u, v;
	double t;

  	//Find vectors for two edges sharing V1
  	edge1 = v2 - v1;
  	edge2 = v3 - v1;

  	//Begin calculating determinant - also used to calculate u parameter
  	pvec = glm::cross(dir, edge2);
  	//if determinant is near zero, ray lies in plane of triangle or ray is parallel to plane of triangle
  	det = glm::dot(edge1, pvec);
  	//NOT CULLING
  	if(det > -EPSILON && det < EPSILON){
  		// printf("det = %f , first IF!\n", det);
  		return 0;	
  	} 
  	inv_det = 1.0 / det;

  	//calculate distance from V1 to ray origin
  	tvec =  orig - v1;

  	//Calculate u parameter and test bound
  	u = glm::dot(tvec, pvec) * inv_det;
  	//The intersection lies outside of the triangle
  	if(u < 0.0 || u > 1.0) {
  		// printf("u = %f , second IF!\n", u);
  		return 0;
  	}

  	//Prepare to test v parameter
  	qvec = glm::cross(tvec, edge1);

  	//Calculate V parameter and test bound
  	v = glm::dot(dir, qvec) * inv_det;
  	//The intersection lies outside of the triangle
  	if(v < 0.0 || ((u + v)  > 1.0)) {
  		// printf("v = %f , v + u = %f, third IF!\n", u,u+v);
  		return 0;	
  	} 

  	t = glm::dot(edge2, qvec) * inv_det;
  	if(t > EPSILON) { //ray intersection
    	*out = t;
    	return 1;
  	}

  	// No hit, no win
  	return 0;
}


int isInsideMesh(mesh m,glm::dvec3 ray_origin)
{
	glm::dvec3 ray_dir1 = glm::dvec3(5,0.5,0.25);
	glm::dvec3 ray_dir2 = glm::dvec3(-3,0.7,10);
	triangle t;
	vertex v1, v2, v3;
	int t_inter1, t_inter2;
	double out;
	int intersectCounter1 = 0;
	int intersectCounter2 = 0;
	for (unsigned int i = 0; i < m.triangles.size(); ++i)
	{
		t = m.triangles[i]; 	
		v1 = m.vertices[t.a]; 	
		v2 = m.vertices[t.b]; 	
		v3 = m.vertices[t.c];

		t_inter1 = triangle_intersection(v1.position,v2.position,v3.position,ray_origin,ray_dir1,&out);	
		t_inter2 = triangle_intersection(v1.position,v2.position,v3.position,ray_origin,ray_dir2,&out);	
		if(t_inter1 == 1) { //|| t_inter2 == 1) {
			intersectCounter1 += 1;
		}
		if(t_inter2 == 1) { //|| t_inter2 == 1) {
			intersectCounter2 += 1;
		}
	}
	// printf("IntersectCounter = %d\n",intersectCounter);
	// if (intersectCounter == 8)
	// {
	// 	pri
	// }

	if(((intersectCounter1 % 2) == 1) && ((intersectCounter2 % 2) == 1) ) {
		return 1;
	}
	else {
		return 0;
	}
	return 0;
}