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

#include <glm/glm.hpp>

struct point {

	glm::dvec3 position;
	bool active = true;

};