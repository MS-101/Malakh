#include <training.h>
#include <fstream>
#include <sstream>
#include <memory>


NetImpl::NetImpl()
{
    inputLayer = register_module("input", torch::nn::Linear(13, 50));
    hiddenLayer1 = register_module("hidden1", torch::nn::Linear(50, 50));
    hiddenLayer2 = register_module("hidden2", torch::nn::Linear(50, 50));
    hiddenLayer3 = register_module("hidden3", torch::nn::Linear(50, 1));
}

torch::Tensor NetImpl::forward(torch::Tensor x)
{
    x = torch::relu(inputLayer->forward(x));
    x = torch::relu(hiddenLayer1->forward(x));
    x = torch::relu(hiddenLayer2->forward(x));
    x = hiddenLayer3->forward(x);
    return x;
}

TORCH_MODULE(Net);

CSVDataset::CSVDataset(const std::string& filename, int features, int labels) : filename(filename), features(features), labels(labels) {}

torch::data::Example<> CSVDataset::get(size_t index) {
    std::ifstream file(filename);

    std::string line;
    for (size_t i = 0; i < index + 2; ++i)
        std::getline(file, line);

    std::stringstream ss(line);
    std::string value;

    std::vector<float> features_vector;
    for (int i = 0; i < features; ++i) {
        std::getline(ss, value, ',');
        features_vector.push_back(std::stof(value));
    }

    std::vector<float> labels_vector;
    for (int i = 0; i < labels; ++i) {
        std::getline(ss, value, ',');
        labels_vector.push_back(std::stof(value));
    }

    file.close();

    auto features_tensor = torch::tensor(features_vector);
    auto labels_tensor = torch::tensor(labels_vector);

    return { features_tensor, labels_tensor };
}

torch::optional<size_t> CSVDataset::size() const {
    std::ifstream file(filename);
    size_t num_lines = std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');
    file.close();

    return num_lines > 0 ? num_lines - 1 : num_lines;
}

// log_2(victory - (defeat + 0.5 * stalemate)) => value predicted by neural network
// Sum(w_i*a_i), where Sum(w_i) = 1 => interpolation between models in ensemble learning

void TrainingManager::trainModel(std::string inputFile, std::string outputFile, int epochs)
{
	torch::Device device(torch::cuda::is_available() ? torch::kCUDA : torch::kCPU);

    Net model = Net();
    model->to(device);

    auto dataset = CSVDataset(inputFile, 13, 1).map(torch::data::transforms::Stack<>());

    int batchSize = 64;
    auto trainLoader = torch::data::make_data_loader<torch::data::samplers::RandomSampler>(
        std::move(dataset), batchSize
    );
	
	torch::optim::Adam optimizer(model->parameters(), 0.001);

	for (int i = 0; i < epochs; i++) {
        for (auto batch : *trainLoader) {
            auto data = batch.data.to(device);
            auto labels = batch.target.to(device);

            auto prediction = model->forward(data);
            auto loss = torch::mse_loss(prediction, batch.target);

            optimizer.zero_grad();
            loss.backward();
            optimizer.step();
        }
	}

	torch::save(model, outputFile);
}
