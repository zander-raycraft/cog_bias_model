#include "../headr/layer.h"
#include <random>

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
    // Initialize input vector to be size of size, "size", and each element set to 0
    LayerInputs(size, 0),
    // Initialize the weight vector to be size of size, "size", each index 0 to be made random in body
    LayerWeights(size, 0),
    // Initialize layer to be null
    prevLayer(prev)
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
            if(!isInputLayer && nodeType == NodeType::BaseNode)
            {
                // go through the output for each of the nodes in the last layer
                for(auto node : layerNodes)
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
            // TODO: LSTM Make the connection and establishment of the layers in the LSTM netowrk

        } else {
            throw std::invalid_argument("Layer constructor failed");
        }
}