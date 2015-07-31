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

#include "tinyobjloader/tiny_obj_loader.h"
#include <glm/glm.hpp>
#include <vector>

typedef unsigned int index;

struct triangle {
	index a;
	index b;
	index c;
};

struct vertex {
	glm::dvec3 position;
	glm::dvec3 normal;
};

typedef std::vector<triangle> triangle_list;
typedef std::vector<vertex> vertex_list;

struct mesh {
	triangle_list triangles;
	vertex_list vertices;
};

mesh loadMesh(const std::string& Filename) {

	std::vector<tinyobj::shape_t> model_shapes;
	std::vector<tinyobj::material_t> model_mtls;
	tinyobj::LoadObj(model_shapes, model_mtls, Filename.c_str());

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