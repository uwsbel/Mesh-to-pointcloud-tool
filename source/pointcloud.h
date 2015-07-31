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

#include "point.h"
#include <vector>
#include <iostream>

struct pointcloud {

	std::vector<std::vector<std::vector<point>>> points;

	point& at(unsigned int x, unsigned int y, unsigned int z) {
		return points[x][y][z];
	}

	point& at(unsigned int idx) {
		unsigned int x(0), y(0), z(0);
		//return (&(points.front().front().front()))[idx];
		if (idx < points.size()) {
			x = idx;
			y = 0;
			z = 0;
		}
		else {
			unsigned int idx0 = idx % points.size();
			unsigned int idx1 = idx / points.size();

			if (idx1 < points[0].size()) {
				x = idx0;
				y = idx1;
				z = 0;
			}
			else {
				unsigned int idx2 = 0;

				if (idx1 < (points[0].size() * points[0][0].size())) {
					idx2 = idx1 / points[0].size();
					idx1 = idx1 % points[0].size();

					x = idx0;
					y = idx1;
					z = idx2;
				}
				else {
					return points[0][0][0];
				}
			}
		}

		return points[x][y][z];
	}

	void place(unsigned int x, unsigned int y, unsigned int z, const point& P) {
		points[x][y][z] = P;
	}

	void place(unsigned int x, unsigned int y, unsigned int z, point&& P) {
		points[x][y][z] = P;
	}

	void fill(size_t x, size_t y, size_t z) {
		points.resize(x);
		for (unsigned int i = 0; i < points.size(); i++) {
			points[i].resize(y);
			for (unsigned int j = 0; j < points[i].size(); j++) {
				points[i][j].resize(z);
			}
		}
	}

	size_t size() {
		size_t _ret;

		_ret = points.size();
		_ret *= points[0].size();
		_ret *= points[0][0].size();
		

		return _ret;
	}

};


// Creates a cloud of unit size filled with points of defined distance spacing
pointcloud createUnitPointcloud(double distance) {

	pointcloud _ret;

	if (distance > 1) {
		std::cerr << "Point distance cannot be greater than one\n";
		return _ret;
	}

	size_t side_length = (size_t)(1.0 / distance) + 1;

	_ret.fill(side_length, side_length, side_length);

	for (unsigned int x = 0; x < _ret.points.size(); x++) {
		for (unsigned int y = 0; y < _ret.points[x].size(); y++) {
			for (unsigned int z = 0; z < _ret.points[x][y].size(); z++) {
				_ret.at(x, y, z).position = glm::dvec3((double)x * distance, (double)y * distance, (double)z * distance);
			}
		}
	}

	return _ret;
}

// Creates a cloud of side_length size filled with points of defined distance spacing
pointcloud createPointcloud(size_t side_length, double distance) {

	pointcloud _ret;

	if (distance > side_length) {
		std::cerr << "Point distance cannot be greater than side length\n";
		return _ret;
	}

	size_t sl = (size_t)((double)side_length / distance) + 1;

	_ret.fill(sl, sl, sl);
	double half_length = (double)side_length / 2.0;

	for (unsigned int x = 0; x < _ret.points.size(); x++) {
		for (unsigned int y = 0; y < _ret.points[x].size(); y++) {
			for (unsigned int z = 0; z < _ret.points[x][y].size(); z++) {
				_ret.at(x, y, z).position = glm::dvec3((double)x * distance - half_length, (double)y * distance - half_length, (double)z * distance - half_length);
			}
		}
	}

	return _ret;
}