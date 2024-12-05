#include "../headr/layer.h"
#include <random>
#include <iostream>

/**
 * @brief Default constructor
 * @param size -> int, number of nodes in the layer
 * @param nodeType -> NodeType, type of nodes in the layer
 */
template <typename NodeType>
NetworkLayer<NodeType>::NetworkLayer(int size, NodeType nodeType, bool isInputLayer,
                                     NetworkLayer<NodeType>* prev) noexcept :

// Initialize the vector to be of size, "size", and have each element be a node with default of one input
        layerNodes(size, NetworkNode<NodeType>(1)),
        // Initialize the output vector to be of size, "size", and each element set to 0
        LayerOutputVec(size, 0),
        // Initialize the weight vector to be size of size, "size", each index 0 to be made random in body
        LayerWeights(size, 0),
        // Initialize layer to be null
        prevLayer(prev),
        informationMatrix(10, std::vector<double>(3, 0))
{
    try
    {
        if(size != 0)
        {
            // randomize the weights
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(-1.0, 1.0);
            for(double& weight : LayerWeights)
            {
                weight = dis(gen);
            }

            // connecting of layers in the base neural network
            if(!isInputLayer)
            {
                if constexpr (std::is_same<NodeType, BaseNode>::value)
                {
                    // go through the output for each of the nodes in the last layer
                    for(auto& node : layerNodes)
                    {
                        // input value for node = size of output vec in prev layer
                        node.getInputs().resize(prevLayer->LayerOutputVec.size());
                        // weight vec and input vec are the same for nodes
                        for(int i = 0; i < node.getWeightVecSize(); i++)
                        {
                            node.setInputs(i, prevLayer->LayerOutputVec[i]);
                        }
                    }
                }
                else if constexpr (std::is_same<NodeType, LstmNode>::value)
                {
                    //format the inputs with funciton
                    dataLoadLstm(prevLayer->informationMatrix);
                }
            }
            // NOTE: When calling this, you should call the dataFit funciton
        }

    } catch(const std::invalid_argument& ia)
    {
        throw std::invalid_argument("NetworkLayer constructor failed");
    }
}


/**
 *
 * @breif this function modifies the LSTM nodes to have properly formatted data between layers
 * @param values -> std::vector<std::vector<double>> values from the datafile to be run through the model
 * @return void
 *
 */
template <typename NodeType>
void NetworkLayer<NodeType>::dataLoadLstm(std::vector<std::vector<double>> values)
{
    if constexpr (std::is_same<NodeType, LstmNode>::value)
    {
        // Validate input dimensions
        if (values.size() != 3 || values[0].empty())
        {
            throw std::invalid_argument("NetworkLayer dataLoad failed: input dimensions invalid or empty values");
        }

        const std::vector<double>& layerOutputs = values[0];
        const std::vector<double>& stm = values[1];
        const std::vector<double>& ltm = values[2];

        // Calculate averages for STM and LTM
        double stmSum = std::accumulate(stm.begin(), stm.end(), 0.0);
        double ltmSum = std::accumulate(ltm.begin(), ltm.end(), 0.0);

        double stmAvg = stmSum / stm.size();
        double ltmAvg = ltmSum / ltm.size();

        // Update each node in the layer
        for (size_t nodeIdx = 0; nodeIdx < layerNodes.size(); ++nodeIdx)
        {
            auto& node = layerNodes[nodeIdx];

            // Update the inputs for the node
            node.getInputs().resize(layerOutputs.size());
            for (size_t i = 0; i < layerOutputs.size(); ++i)
            {
                node.setInputs(i, layerOutputs[i]);
            }

            // Update STM and LTM in the LstmNode struct inside the node
            auto& nodeInternal = static_cast<LstmNode&>(node.getNode()); // Get the internal LstmNode

            std::cout << "Before modification: LongTermState = " << nodeInternal.LongTermState << ", ShortTermState = " << nodeInternal.ShortTermState << std::endl;

            nodeInternal.LongTermState = ltmAvg;
            nodeInternal.ShortTermState = stmAvg;

            std::cout << "After modification: LongTermState = " << nodeInternal.LongTermState << ", ShortTermState = " << nodeInternal.ShortTermState << std::endl;
        }
    }
    else
    {
        throw std::logic_error("dataLoadLstm is only valid for LstmNode layers");
    }
}

template class NetworkLayer<BaseNode>;
template class NetworkLayer<LstmNode>;