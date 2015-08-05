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

	std::string buff;
	std::string name;

	std::cout << "Enter filename of OBJ model\n> ";
	std::cin >> buff;

	auto found = buff.find(".obj");

	if (found == std::string::npos) {
		std::cout << "File must be OBJ\n";
		exit(1);
	}

	name = buff.substr(0, found);

	mesh m = loadMesh(buff);

	buff.clear();

	std::cout << "Enter grid resolution\n> ";
	std::cin >> buff;

	double res_tol = std::strtod(buff.c_str(), nullptr);
	buff.clear();

	std::cout << "Enter grid side length\n> ";
	std::cin >> buff;

	size_t len = std::atol(buff.c_str());
	buff.clear();

	pointcloud pc = createPointcloud(len, res_tol);

	std::cout << "Enter thread count\n> ";
	std::cin >> buff;

	size_t threads = std::atol(buff.c_str());
	buff.clear();
	
	std::cout << "Pointcloud Size: " << pc.size() << " points\n";

	auto v = moldPointcloud(m, pc, res_tol, threads);
	 
	unsigned int inactive = 0;

	std::ofstream out(name + ".csv");
	out << "x,y,z\n";

	for (auto p : v) {
		//std::cout << p.position.x << " " << p.position.y << " " << p.position.z << "\n";
		out << p.position.x << "," << p.position.y << "," << p.position.z << "\n";
	}

	size_t inaa = pc.size() - v.size();

	std::cout << "Finished writing to file with " << inaa << " inactive points\n";

	out.close();

	return 0;
}