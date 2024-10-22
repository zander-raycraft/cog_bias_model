#include "../headr/node.h"
#include <cmath>
#include <cstdlib>
#include <ctime>


/**
 * 
 * @breif: default constructor for the node, intializes all weights to be random
 * 
 * @param: inputs -> type: int, the number of input connections coming into this node
 * s
 */
NetworkNode::NetworkNode(int inputs)
{
    std::srand(std::time(0));
    for(int i = 0; i < inputs; i++)
    {
        weightVec.push_back((std::rand() / double(RAND_MAX)) * 2 - 1);
    }
    biasVal = ((std::rand() / double(RAND_MAX)) * 2 - 1);
};

/**
 * 
 * @breif: destructor for the networkNode class
 */
NetworkNode::~NetworkNode()
{

}

/**
 * 
 * @breif: Calculates the output of the node for the inputs coming in
 * 
 * @param: inputs ->  type: vector<double>, input values for the node
 * @return: output -> type: double, calculated output post weighted sum, bias, and
 *                      activation function
 */
double NetworkNode::find_output(const std::vector<double>& inputs) noexcept
{
    double sum = 0;
    for(int i = 0; i < inputs.size(); ++i)
    {
        sum += inputs[i] * weightVec[i];
    }
    sum += biasVal;
    return (output = activation_func(sum));
};

/**
 * 
 * @brief: Applies the tanh activation function to the node's weighted sum and bias
 * 
 * @param nodeInfo -> type: double, the weighted sum of inputs plus bias
 * @return formattedOutput -> type: double, the result of applying the activation function
 */
double NetworkNode::activation_func(double nodeInfo) noexcept
{
    return std::tanh(nodeInfo);
};

/**
 * 
 * @brief: getter function for the nodes output
 * 
 * @return nodeOutput -> type: double, the nodes outptut
 */
double NetworkNode::get() const noexcept
{
    return output;
};

/**
 * @brief: set function for the biases
 */
void NetworkNode::set(double newVal) noexcept
{
    biasVal = newVal;
}






