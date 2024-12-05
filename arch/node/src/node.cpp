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
template <typename NodeType>
NetworkNode<NodeType>::NetworkNode(int inputs)
        : weightVec(inputs), biasVal(0.0), output(0.0), numOutput(1), inputs(inputs, 0.0)
{
    try
    {
        if constexpr(std::is_same<NodeType, LstmNode>::value)
        {
            // Set the weights to random values - forget
            node.forgetVals.resize(2 * inputs + 1);
            for(auto& weight : node.forgetVals)
            {
                weight = distribution(generate);
            }

            // Set the weights to random values - input
            node.inputVals.resize(4 * inputs + 2);
            for(auto& weight : node.forgetVals)
            {
                weight = distribution(generate);
            }

            // Set the weights to random values - output
            node.outputVals.resize(2 * inputs + 1);
            for(auto& weight : node.forgetVals)
            {
                weight = distribution(generate);
            }
        }
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

// MODIFY THE LOGIC
template <typename NodeType>
NetworkNode<NodeType>::NetworkNode(int inNum, int outNum)
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
template <typename NodeType>
NetworkNode<NodeType>::NetworkNode(const NetworkNode& base) noexcept
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
template <typename NodeType>
NetworkNode<NodeType>& NetworkNode<NodeType>::operator=(const NetworkNode& base) noexcept {
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
template <typename NodeType>
NetworkNode<NodeType>::~NetworkNode() noexcept
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
template <typename NodeType>
double NetworkNode<NodeType>::find_output(const std::vector<double>& inputs,
                                          double agreSTM,
                                          double agreLTM) noexcept {
    try {
        if (inputs.size() != weightVec.size()) {
            throw std::invalid_argument("Input vector size does not match weight vector size");
        }

        if constexpr(std::is_same<NodeType, LstmNode>::value)
        {
            // aggregated value of LTM cell based on average of inputs from prev cells
            node.LongTermState = agreLTM;
            // aggregated value of STM cell based on average of inputs from prev cells
            node.ShortTermState = agreSTM;
            calcForgetGate();
            calcInputGate();
            output = calcOutputGate()[0];
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
template <typename NodeType>
double NetworkNode<NodeType>::activation_func(double nodeInfo) noexcept
{
    return std::tanh(nodeInfo);
};

/**
 * 
 * @brief: getter function for the nodes output
 * 
 * @return nodeOutput -> type: double, the nodes outptut
 */
template <typename NodeType>
double NetworkNode<NodeType>::get() const noexcept
{
    return output;
};

/**
 * @brief: set function for the biases
 */
template <typename NodeType>
void NetworkNode<NodeType>::set(double newVal) noexcept
{
    biasVal = newVal;
}

/**
* @breif calculates the forget gate
*
* @param: weightsAndBias -> std::vector<double>, the weights and biases of this gate
* @param: input -> int, the input value
* @param: LTST -> std::pair<double, double>, the long and short term state of the node
*
* @return: double -> the new LT memory cell
*/
template <typename NodeType>
double NetworkNode<NodeType>::calcForgetGate()
{
    if constexpr(std::is_same<NodeType, LstmNode>::value)
    {
        double runningSum = 0;
        double b1 = node.forgetVals[node.forgetVals.size() - 1];
        for(int i = 0; i < inputs.size(); i++)
        {
            double w1 = node.forgetVals[i * 2];
            double w2 = node.forgetVals[i * 2 + 1];
            runningSum += (w1 * inputs[i]) + (w2 * node.ShortTermState) + b1;
        }
        return(node.LongTermState *= (1.0/ (1.0 + std::exp(-runningSum))));

    } else {
        throw std::invalid_argument("Node is not an LstmNode");
    }
}

/**
* @breif calculates the input gate
*
* @param: input -> double, the input value
*
* @return: double -> the new LT memory cell
*/
template <typename NodeType>
double NetworkNode<NodeType>::calcInputGate()
{
    if constexpr(std::is_same<NodeType, LstmNode>::value)
    {
        // sig side calculation
        double b1 = node.inputVals[node.inputVals.size() - 2];
        double b2 = node.inputVals[node.inputVals.size() - 1];
        double runningSumSig = 0;
        for(int i = 0; i < inputs.size(); i++)
        {
            double w1 = node.inputVals[i * 4];
            double w2 = node.inputVals[i * 4 + 1];
            runningSumSig += (w1 * inputs[i]) + (w2 * node.ShortTermState) + b1;
        }
        runningSumSig = (1.0 / (1.0 + std::exp(-runningSumSig)));

        // tanh side calculation
        double runningSumTanh = 0;
        for(int i = 0; i < inputs.size(); i++)
        {
            double w3 = node.inputVals[i * 4 + 2];
            double w4 = node.inputVals[i * 4 + 3];
            runningSumTanh += (w3 * inputs[i]) + (w4 * node.ShortTermState) + b2;
        }
        runningSumTanh = std::tanh(runningSumTanh);
        return(node.LongTermState += (runningSumSig * runningSumTanh));

    } else {
        throw std::invalid_argument("Node is not an LstmNode");
    }
}

/**
* @breif calculates the input gate
*
* @param: input -> int, the input value
* @param: LTST -> std::pair<double, double>, the long and short term state of the node
*
* @return: std::vector<double> -> the new LT memory cell <output, LTM, STM>
*/
template <typename NodeType>
std::vector<double> NetworkNode<NodeType>::calcOutputGate()
{
    if constexpr(std::is_same<NodeType, LstmNode>::value)
    {
        double runningSum = 0;
        double b1 = node.outputVals[node.outputVals.size() - 1];
        for(int i = 0; i < inputs.size(); i++)
        {
            double w1 = node.outputVals[i * 2];
            double w2 = node.outputVals[i * 2 + 1];
            runningSum += (w1 * inputs[i]) + (w2 * node.ShortTermState) + b1;
        }
        double result = std::tanh(node.LongTermState) * (1.0/ (1.0 + std::exp(-runningSum)));
        node.ShortTermState = result;
        return(std::vector<double>{result, result});


    } else {
        throw std::invalid_argument("Node is not an LstmNode");
    }
}



template class NetworkNode<BaseNode>;
template class NetworkNode<LstmNode>;






