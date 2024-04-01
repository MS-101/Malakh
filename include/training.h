#pragma once

#include <torch/torch.h>


struct NetImpl : torch::nn::Module {
public:
    NetImpl();
    torch::Tensor forward(torch::Tensor x);
    torch::nn::Linear inputLayer{ nullptr };
    torch::nn::Linear hiddenLayer1{ nullptr };
    torch::nn::Linear hiddenLayer2{ nullptr };
    torch::nn::Linear hiddenLayer3{ nullptr };
};

class CSVDataset : public torch::data::Dataset<CSVDataset> {
private:
    std::string filename;
    int features;
    int labels;
public:
    explicit CSVDataset(const std::string& filename, int features, int labels);
    torch::data::Example<> get(size_t index) override;
    torch::optional<size_t> size() const override;
};

class TrainingManager {
public:
	static void trainModel(std::string inputFile, std::string outputFile, int epochs);
};