#include "ConductanceMatrix.h"
#include "Component.h"
#include "ParseFile.h"
#include "ACAnalysis.h"

int main() {
	std::ifstream infile;
	infile.open("../testCircuit.cir"); 
	if(!infile.is_open()){
		return EXIT_FAILURE;
	}

	//Here the highest node number is initialised and parsed into decode_file.
	//A vector of Component pointers is set to the output of decode_file
	int highest_node = 0; Command* command;
	std::vector<Component*> components = decode_file(infile, highest_node, command);
	infile.close();

	int outputNode = 2;

	std::vector<Eigen::Vector3d> results = runACAnalysis(outputNode, 100, 10000, 1000, components, highest_node);

	std::ofstream outfile("../output.csv");

	if (outfile.is_open()) {
		outfile << "Frequency / Hz, Amplitude / dB, Phase / Degrees" << std::endl;

		for (int i = 0; i < results.size(); i++) {
			double amplitude = 20 * log10(results[i](0));
			double phase = radians_to_degrees(results[i](1));
			double frequency = results[i](2);

			outfile << frequency << ", ";
			outfile << amplitude << ", ";
			outfile << phase << ", " << std::endl;
		}
	} else {
		std::cout << "Failed to open output.csv" << std::endl;
		return EXIT_FAILURE;
	}
}