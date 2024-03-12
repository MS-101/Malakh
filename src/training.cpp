#include <training.h>
#include <database.h>
#include <torch/torch.h>


// log_2(victory - (defeat + 0.5 * stalemate)) => value predicted by neural network
// Sum(w_i*a_i), where Sum(w_i) = 1 => interpolation between models in ensemble learning

void TrainingManager::trainModel(EssenceArgs essenceArgs)
{
	DatabaseConnection conn;
	int idEssenceConfig = conn.getIdEssenceConfig(essenceArgs);

	int page = 1;
	while (true) {
		auto rows = conn.getTrainingData(idEssenceConfig, page, 1000);
		if (rows.empty())
			break;

		for (const auto& row : rows) {
		}

		page++;
	}
}
