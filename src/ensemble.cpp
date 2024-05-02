#include "ensemble.h"
#include "piece.h"


Ensemble::Ensemble() {
	models[Classic] = torch::jit::load("training\\classic.pt", torch::kCPU);
	models[Red] = torch::jit::load("training\\red.pt", torch::kCPU);
	models[Blue] = torch::jit::load("training\\blue.pt", torch::kCPU);
}

double Ensemble::forward(int* inputArray, int* essenceCounts)
{
	// transform input
	torch::Tensor inputTensor = torch::from_blob(inputArray, { 2, 8, 8 });
	std::vector<torch::jit::IValue> iValue;
	iValue.push_back(inputTensor);

	// calculate sum(w_i * x_i)
	double value = 0;
	for (int i = 0; i < 3; i++) {
		if (essenceCounts[i] == 0)
			continue;

		torch::Tensor tensor = models[i].forward(iValue).toTensor();
		double weight = (double)essenceCounts[i] / 8;
		value += weight * tensor.item<double>();
	}

	return value;
}
