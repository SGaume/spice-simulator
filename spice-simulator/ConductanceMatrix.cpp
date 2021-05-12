#include "ConductanceMatrix.h"

using namespace Eigen;

// matrixHandleTwoTerminalComponent Function
// Handler for updating the conductance matrix due to basic two terminal components, 
// the conduction matrix is passed by reference
void matrixHandleTwoTerminalComponent(MatrixXcd& conductanceMatrix, Component* component,
	std::vector<int> nodes, double angFreq) {

	if (nodes[0] != 0 && nodes[1 != 0]) {
		conductanceMatrix(nodes[0] - 1, nodes[1] - 1) -= component->getConductance(nodes[0], nodes[1], angFreq);
		conductanceMatrix(nodes[1] - 1, nodes[0] - 1) -= component->getConductance(nodes[1], nodes[0], angFreq);
	}

	if (nodes[0] != 0) {
		conductanceMatrix(nodes[0] - 1, nodes[0] - 1) += component->getConductance(nodes[0], nodes[1], angFreq);
	}

	if (nodes[1] != 0) {
		conductanceMatrix(nodes[1] - 1, nodes[1] - 1) += component->getConductance(nodes[1], nodes[0], angFreq);
	}
}

// matrixHandleVoltageSource Function
// Handler for updating the conductance matrix due to voltage source components,
// the conduction matrix is passed by reference
void matrixHandleVoltageSource(MatrixXcd& conductanceMatrix, Component* component) {
	std::vector<int> nodes = component->getNodes();
	int nodePlus = nodes[0];
	int nodeMinus = nodes[1];

	if (nodePlus == nodeMinus) return;

	if (nodePlus != 0 && nodeMinus != 0) {
		conductanceMatrix.row(nodePlus - 1) -= conductanceMatrix.row(nodePlus - 1);

		conductanceMatrix(nodePlus - 1, nodePlus - 1) = 1;
		conductanceMatrix(nodePlus - 1, nodeMinus - 1) = -1;
	} else if (nodePlus != 0 && nodeMinus == 0) {
		conductanceMatrix.row(nodePlus - 1) -= conductanceMatrix.row(nodePlus - 1);

		conductanceMatrix(nodePlus - 1, nodePlus - 1) = 1;
	} else if (nodePlus == 0 && nodeMinus != 0) {
		conductanceMatrix.row(nodeMinus - 1) -= conductanceMatrix.row(nodeMinus - 1);

		conductanceMatrix(nodeMinus - 1, nodeMinus - 1) = -1;
	}
}

// updateConductanceMatrix Function
// Updates the conductance matrix (passed by reference) with the effect of each
// component as this function is called for it
void updateConductanceMatrix(MatrixXcd& conductanceMatrix, Component* component, 
	bool& isVoltageSource, double angFreq) {
	if (typeid(*component) == typeid(DCCurrentSource) ||
		typeid(*component) == typeid(ACCurrentSource)) {

		return;
	} else if(typeid(*component) == typeid(DCVoltageSource) || 
		typeid(*component) == typeid(ACVoltageSource)) {

		isVoltageSource = true;
		return;
	} else {
		std::vector<int> nodes = component->getNodes();

		if (nodes.size() == 2) {
			matrixHandleTwoTerminalComponent(conductanceMatrix, component, nodes, angFreq);
		}
	}
}

// getConductanceMatrix Function
// Returns a matrix of complex doubles representing the conductance matrix in the
// nodal analysis equation
MatrixXcd getConductanceMatrix(std::vector<Component*> components, int numNodes, double angFreq) {
	MatrixXcd conductanceMatrix = MatrixXcd::Zero(numNodes, numNodes);
	bool isVoltageSource;
	std::vector<int> voltageSources;

	for (int i = 0; i < components.size(); i++) {
		isVoltageSource = false;
		updateConductanceMatrix(conductanceMatrix, components[i], isVoltageSource, angFreq);
		if (isVoltageSource) voltageSources.push_back(i);
	}

	for (int i = 0; i < voltageSources.size(); i++) {
		matrixHandleVoltageSource(conductanceMatrix, components[i]);
	}

	return conductanceMatrix;
}

// vectorHandleACCurrentSource Function
// Handler for updating the current vector with the effects of AC current sources,
// the current vector is passed by reference
void vectorHandleACCurrentSource(VectorXcd& currentVector, Component* component) {
	// Input at 0, Output at 1
	std::vector<int> nodes = component->getNodes();
	double amplitude = (component->getProperties())[0];
	double phase = (component->getProperties())[1];
	std::complex<double> current = std::polar(amplitude, phase);

	if (nodes[0] != 0) {
		currentVector(nodes[0] - 1) -= current;
	}

	if (nodes[1] != 0) {
		currentVector(nodes[1] - 1) += current;
	}
}

// vectorHandleVoltageSource Function
// Handler for updating the current vector with the effects of AC and DC voltage sources,
// the current vector is passed by reference
void vectorHandleVoltageSource(VectorXcd& currentVector, Component* component) {
	bool isDC = false;
	std::complex<double> voltage;
	std::vector<double> properties = component->getProperties();

	if (typeid(*component) == typeid(DCVoltageSource)) isDC = true;

	if (!isDC) {
		double amplitude = properties[0];
		double phase = properties[1];
		voltage = std::polar(amplitude, phase);
	} else {
		voltage = properties[0];
	}

	std::vector<int> nodes = component->getNodes();
	int nodePlus = nodes[0];
	int nodeMinus = nodes[1];

	if (nodePlus == nodeMinus) return;

	if (nodePlus == 0) {
		if (isDC) currentVector(nodeMinus - 1) = 0;
		else currentVector(nodeMinus - 1) = voltage;
	} else {
		if (isDC) currentVector(nodePlus - 1) = 0;
		else currentVector(nodePlus - 1) = voltage;
	}
}

// updateCurrentVector Function
// Helper function that updates the current vector (passed by reference), which is only
// affected by AC sources
void updateCurrentVector(VectorXcd& currentVector, Component* component, bool& isVoltageSource) {
	if (typeid(*component) == typeid(ACCurrentSource)) {
		vectorHandleACCurrentSource(currentVector, component);
	} else if (typeid(*component) == typeid(DCVoltageSource) ||
		typeid(*component) == typeid(ACVoltageSource)) {
		
		isVoltageSource = true;
	}
}

// getCurrentVector Function
// Returns a vector of complex doubles representing the current vector in the
// nodal analysis equation
VectorXcd getCurrentVector(std::vector<Component*> components, int numNodes) {
	VectorXcd currentVector = VectorXcd::Zero(numNodes);
	bool isVoltageSource;
	std::vector<int> voltageSources;

	for (int i = 0; i < components.size(); i++) {
		isVoltageSource = false;
		updateCurrentVector(currentVector, components[i], isVoltageSource);
		if (isVoltageSource) voltageSources.push_back(i);
	}

	for (int i = 0; i < voltageSources.size(); i++) {
		vectorHandleVoltageSource(currentVector, components[i]);
	}

	return currentVector;
}

// solveAtFrequency Function
// Solves for the voltage vector given a vector of pointers to Components, the number
// of nodes in the circuit and the frequency to solve for
VectorXcd solveAtFrequency(std::vector<Component*> components, int numNodes, double angularFrequency) {
	MatrixXcd conductanceMatrix = getConductanceMatrix(components, numNodes, angularFrequency);
	VectorXcd currentVector = getCurrentVector(components, numNodes);

	VectorXcd solution(numNodes);

	solution = conductanceMatrix.colPivHouseholderQr().solve(currentVector);

	return solution;
}