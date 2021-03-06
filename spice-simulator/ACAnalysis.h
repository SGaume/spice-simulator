#pragma once

#include <cmath>
#include <complex>
#include <iostream>

#include "Eigen/Dense"

#include "Component.h"
#include "DCOpPoint.h"

std::vector<std::vector<Eigen::Vector3d>> runACAnalysis(int outputNode, int inputSource, double startFreq, double stopFreq, int pointsPerDecade,
	std::vector<Component*> components, int highest_node);