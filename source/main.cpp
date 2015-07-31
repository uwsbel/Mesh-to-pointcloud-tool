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

#include "pointcloud.h"
#include "mesh.h"
#include "mold_pointcloud.h"

#include <iostream>
#include <fstream>

int main(int argc, char** args) {

	double res_tol = 0.05;

	pointcloud pc = createPointcloud(2, res_tol);
	mesh m = loadMesh("box.obj");

	std::cout << "Pointcloud Size: " << pc.size() << " points\n";

	auto v = moldPointcloud(m, pc, res_tol);

	unsigned int inactive = 0;

	std::ofstream out("out.csv");
	out << "x,y,z\n";

	for (auto p : v) {
		//std::cout << p.position.x << " " << p.position.y << " " << p.position.z << "\n";
		out << p.position.x << "," << p.position.y << "," << p.position.z << "\n";
	}

	size_t inaa = pc.size() - v.size();

	std::cout << "Finished writing to file with " << inaa << " inactive points\n";

	out.close();

	float n;
	std::cin >> n;

	return 0;
}