#pragma once

#include <torch/script.h>


class Ensemble {
public:
	Ensemble();

	double forward(int* inputArray, int* essenceCounts);
private:
	torch::jit::script::Module models[3];
};
