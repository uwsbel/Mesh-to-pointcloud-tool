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

#include "mesh.h"
#include "pointcloud.h"
#include <thread>
#include <mutex>
#include <cstdint>

std::vector<point> moldPointcloud(mesh&, pointcloud&, double, size_t);
bool checkPointInTriangle(glm::dvec3&, vertex&, vertex&, vertex&, double);

std::vector<point> moldPointcloud(mesh& m, pointcloud& p, double tolerance, size_t threads) {

	std::vector<point> ret;
	std::mutex ret_mutex;

	triangle_list t_;

	t_ = m.triangles;


	auto _worker = [&ret, &ret_mutex, &m, &t_, &p, tolerance](unsigned int start, unsigned int end) -> void {

		std::vector<point> _points;

		for (unsigned int i = start; i < end; i++) {

			std::cout << "Checking point " << i << "\n";

			glm::dvec3 pos = p.at(i).position;

			for (auto t : t_) {
				vertex tp0 = m.vertices[t.a];
				vertex tp1 = m.vertices[t.b];
				vertex tp2 = m.vertices[t.c];

				if (checkPointInTriangle(pos, tp0, tp1, tp2, tolerance)) {
					_points.push_back(p.at(i));
					break;
				}
			}
		}

		ret_mutex.lock();
		for (auto p : _points) {
			ret.push_back(p);
		}
		ret_mutex.unlock();
	};

	std::vector<std::thread> _worker_threads;
	std::vector<std::vector<point>> _worker_rets;

	for (unsigned int i = 0; i < threads; i++) {
		float chunk_size = (float)p.size() / (float)threads;

		unsigned int start = i * chunk_size;
		unsigned int end = ((i + 1) * chunk_size) - 1;
		
		if (i == threads - 1) {
			end = p.size();
		}

		auto t = std::thread(_worker, start, end);
		_worker_threads.push_back(std::move(t));
	}





	for (unsigned int i = 0; i < _worker_threads.size(); i++) {
		_worker_threads[i].join();
	}

	/*for (unsigned int i = 0; i < p.size(); i++) {

		std::cout << "Checking point " << i << "\n";

		glm::dvec3 pos = p.at(i).position;

		for (unsigned int j = 0; j < t_.size(); j++) {
			vertex tp0 = m.vertices[t_[j].a];
			vertex tp1 = m.vertices[t_[j].b];
			vertex tp2 = m.vertices[t_[j].c];

			if (checkPointInTriangle(pos, tp0, tp1, tp2, tolerance)) {
				ret.push_back(p.at(i));
				break;
			}
			
		}

	}*/

	return ret;

}

void project(std::vector<vertex>& v, glm::dvec3& axis, double& min_out, double& max_out) {
	min_out = DBL_MAX;
	max_out = DBL_MIN;
	for (auto x : v) {
		double d0 = glm::dot(axis, x.position);
		if (d0 < min_out) {
			min_out = d0;
		}
		if (d0 > max_out) {
			max_out = d0;
		}
	}
}

struct plane_t {
	glm::dvec3 a, b, c;
	glm::dvec3 n;
};

struct plane_m {
	double a, b, c, d;
	glm::dvec3 center;
};

struct line_t {
	glm::dvec3 a;
	glm::dvec3 b;
};

double distancePoint_Plane(plane_m& p, glm::dvec3& r) {
	//return (glm::dot(glm::dvec3(p.a, p.b, p.c), r) + p.d);
	return glm::dot(glm::dvec3(p.a, p.b, p.c), r);
}

glm::dvec3 triangleCenter(vertex& a, vertex& b, vertex& c) {
	return a.position * ((b.position * b.position) + (c.position * c.position) - (a.position * a.position));
}

bool sameSide(glm::dvec3& p1, glm::dvec3& p2, glm::dvec3& a, glm::dvec3& b) {
	glm::dvec3 cp1 = glm::cross(b - a, p1 - a);
	glm::dvec3 cp2 = glm::cross(b - a, p2 - a);
	return (glm::dot(cp1, cp2) >= 0);
}

bool pointInTri(glm::dvec3& p, glm::dvec3& a, glm::dvec3& b, glm::dvec3& c) {
	return sameSide(p, a, b, c) && sameSide(p, b, a, c) && sameSide(p, c, a, b);
}

// Projects point onto triangle, verifies the point projected is within the triangle, and that
// the distance from the projected point to the given point is within the tolerable range
bool checkPointInTriangle(glm::dvec3& p, vertex& a, vertex& b, vertex& c, double tolerance) {

	glm::dvec3 n = glm::normalize(glm::cross(b.position - a.position, c.position - a.position));
	glm::dvec3 _n = -n;

	glm::dvec3 tCenter = triangleCenter(a, b, c);
	double tOriginDist = glm::length(tCenter);

	glm::dvec3 x1 = a.position - tCenter;

	plane_m tr = {
		n.x, n.y, n.z, tOriginDist,
		tCenter
	};

	plane_m tr2 = {
		_n.x, _n.y, _n.z, tOriginDist,
		tCenter
	};

	auto v_l = p - a.position;
	double d = distancePoint_Plane(tr, v_l);

	glm::dvec3 d_v = d * n;
	glm::dvec3 p_p = p - d_v;

	if (glm::abs(d) < tolerance && pointInTri(p_p, a.position, b.position, c.position)) {
		return true;
	}

	return false;
	
}