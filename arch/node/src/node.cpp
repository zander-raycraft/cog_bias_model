#include "../headr/node.h"
#include <cmath>
#include <stdexcept>
#include <random>
#include <Eigen/Dense>


// GLOBAL VARS
std::random_device seedGen;
std::mt19937 generate(seedGen());
std::uniform_real_distribution<> distribution(-1.0, 1.0);

/**
 * 
 * @breif: default constructor for the node, initializes all weights to be random
 * 
 * @param: inputs -> type: int, the number of input connections coming into this node
 * s
 */
NetworkNode::NetworkNode(int inputs)
        : weightVec(inputs), biasVal(0.0), output(0.0), numOutput(1)
{
    try
    {
        // Set the weights to random values
        if (inputs <= 0)
        {
            throw std::invalid_argument("Number of inputs must be greater than 0.");
        }
        for(int i = 0; i < inputs; i++)
        {
            weightVec[i] = distribution(generate);
        }
        biasVal = distribution(generate);

    } catch (const std::length_error& e)
    {
        throw std::invalid_argument("NetworkNode constructor failed");
    }
}

/**
 *
 * @breif: alternative constructor
 *
 * @param: inNum -> type: int, number of inputs
 * @param: outNum -> type: int, number of outputs
 *
 */
NetworkNode::NetworkNode(int inNum, int outNum)
        : weightVec(), biasVal(0.0), output(0.0), numOutput(outNum)
{
    try
    {
        if(inNum <= 0 || outNum <= 0)
        {
            throw std::invalid_argument("NetworkNode constructor failed");
        }

        // dynamic memory allocation for vec
        weightVec.reserve(inNum);
        // make weights random
        for(int i = 0; i < inNum; i++)
        {
            weightVec.push_back(distribution(generate));
        }
        biasVal = (distribution(generate));
    } catch (const std::length_error& e)
    {
        if(inNum <= 0)
        {
            throw std::invalid_argument("NetworkNode constructor failed");
        }
    }
}

/**
 *
 * @brief: copy constructor for node (deep copy, noexcept safe)
 *
 * @param: base -> type: const NetworkNode&, node to copy;
 *
 */
NetworkNode::NetworkNode(const NetworkNode& base) noexcept
        : weightVec(), biasVal(base.biasVal), output(base.output), numOutput(base.numOutput)
{
    try {
        // deep copy: done by allocating memory and then copying the data
        weightVec.reserve(base.weightVec.size());
        weightVec.insert(weightVec.end(), base.weightVec.begin(), base.weightVec.end());
    } catch (const std::exception& e) {
        // alloc fault: clear vec to original state
        weightVec.clear();
        throw std::runtime_error("NetworkNode constructor failed");
    }
}

/**
 *
 * @breif: assignment operator for the node class
 *
 * @param: base -> type: NetworkNode&, the node value being assigned to the caller node
 * @return: NetworkNode& -> modified network node address
 *
 */
NetworkNode& NetworkNode::operator=(const NetworkNode& base) noexcept {
    if (this != &base) {
        // Prepare newWeightVec and ensure it won't throw
        weightVec.clear();
        weightVec.reserve(base.weightVec.size());
        weightVec.insert(weightVec.end(), base.weightVec.begin(), base.weightVec.end());

        // Assign scalar values which are noexcept by default
        biasVal = base.biasVal;
        output = base.output;
        numOutput = base.numOutput;
    }
    return *this;
}

/**
 * 
 * @breif: destructor for the networkNode class
 */
NetworkNode::~NetworkNode() noexcept
{
    weightVec.clear();
}


/**
 * 
 * @breif: Calculates the output of the node for the inputs coming in
 * 
 * @param: inputs ->  type: vector<double>, input values for the node
 * @return: output -> type: double, calculated output post weighted sum, bias, and
 *                      activation function
 */
double NetworkNode::find_output(const std::vector<double>& inputs) noexcept {
    try {
        if (inputs.size() != weightVec.size()) {
            throw std::invalid_argument("Input vector size does not match weight vector size");
        }

        // Convert inputs and weight vector to Eigen vectors
        Eigen::VectorXd inputVec = Eigen::Map<const Eigen::VectorXd>(inputs.data(), inputs.size());
        Eigen::VectorXd weightVecEigen = Eigen::Map<const Eigen::VectorXd>(weightVec.data(), weightVec.size());
        double weightedSum = weightVecEigen.dot(inputVec) + biasVal;

        // Apply activation function and store the output
        output = activation_func(weightedSum);
        return weightedSum;

    } catch (const std::exception& e) {
        return 0.0;
    }
}

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






