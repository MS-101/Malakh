#include "nn.h"


Ensemble::Ensemble() {
	models[Classic] = torch::jit::load("training\\classic.pt");
	models[Red] = torch::jit::load("training\\red.pt");
	models[Blue] = torch::jit::load("training\\blue.pt");
}

double Ensemble::forward(Board board)
{
	// transform input
	int* inputArray = board.getInputArray();
	torch::Tensor inputTensor = torch::from_blob(inputArray, { 2, 8, 8 });
	std::vector<torch::jit::IValue> iValue;
	iValue.push_back(inputTensor);

	// calculate sum(w_i * x_i)
	double value = 0;
	for (int i = 0; i < 3; i++) {
		torch::Tensor tensor = models[i].forward(iValue).toTensor();
		double weight = (double)board.essenceCounts[i] / 8;
		value += weight * tensor.item<double>();
	}

	delete[] inputArray;
	return value;
}
