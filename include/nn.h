#pragma once

#include "board.h"
#include <torch/script.h>


class Ensemble {
public:
	Ensemble();

	double forward(Board board);
private:
	torch::jit::script::Module models[3];
};
