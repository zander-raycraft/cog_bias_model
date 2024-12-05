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
                } else if(nodeType == NodeType::LstmNode)
                {
                    if(!isInputLayer)
                    {
                        //format the inputs with funciton
                        dataLoad(prevLayer->informationMatrix);

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
    if (!values.empty() || !values[0].empty())
    {
        std::vector<double> layerOutputs = values[0];
        std::vector<double> stm = values[1];
        std::vector<double> ltm = values[2];
        for(const auto& mRow : values)
        {
            layerOutputs.push_back(mRow[0]);
            stm.push_back(mRow[1]);
            ltm.push_back(mRow[2]);
        }
        //Calculate the average of the vector
        double stmSum = 0;
        double ltmSum = 0;
        for(int i = 0; i < stm.size(); i++)
        {
            stmSum += stm[i];
            ltmSum += ltm[i];
        }
        double stmAvg = stmSum/stm.size();
        double ltmAvg = ltmSum/ltm.size();

        for(auto& node : layerNodes)
        {
            auto nodeInternal = node.getNode();
            node.changeInputVecWhole(layerOutputs);
            nodeInternal.LongTermState = ltmAvg;
            nodeInternal.ShortTermState = stmAvg;
        }

    } else {
        throw std::invalid_argument("NetworkLayer dataLoad failed");
    }

}