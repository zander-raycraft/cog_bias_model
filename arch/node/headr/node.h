#ifndef NODE_H
#define NODE_H

#include <vector>



/**
 *
 * THINGS TO ADD:
 * - synthetic data learning (meta-learning)
 * - make sure data is neutral -> no introduction of biases during training
 * - weight freezing for post training
 *
 * NOTES FOR NEXT TIME I WORK ON THIS:
 * - I added the template type so all the tests are going to fail, fix that in node.h
 *  - it isnt broken it just isnt completed and I have to clock into work so I dont have time to fix it...
 */


/**
 *  @struct: BaseNode -> feedforward model node
 */
 struct BaseNode
         {
             static double activation_func(double nodeInfo) noexcept
             {
                 return std::tanh(nodeInfo);
             }

         };

/**
 * @struct: LstmNode -> LSTM node model
 *
 * @values:
 *     LongTermState -> type: double, the long term state of the node
 *     ShortTermStare -> type: double, the short term state of the node
 *     forgetVals -> type: vector<double>, the weights and bias of the forget gate
 *     inputVals -> type: vector<double>, the weights and bias of the input gate
 *     outputVals -> type: vector<double>, the weights and bias of the output gate
 */
 struct LstmNode:BaseNode
             {
                 double LongTermState = 0.0;
                 double ShortTermState = 0.0;
                 std::vector<double> forgetVals = std::vector<double>(3, 0.0);
                 std::vector<double> inputVals = std::vector<double>(3, 0.0);
                 std::vector<double> outputVals = std::vector<double>(3, 0.0);
             };


/**
 * 
 * @class: NetworkNode -> base neuron for network
 *
 * @note: This node is made to be extendible to various number of inputs and outputs
 * 
 */
//template<typename NodeType>
class NetworkNode 
{
    public:
        NetworkNode(int num_in); //dfault ctor

        /**
         *
         * @breif: alternate constructor with defaulted output to be one
         *
         * @param: inNum -> type: int, number of inputs
         * @param: outNum -> type: int, number of outputs
         *
         */
         NetworkNode(int inNum, int outNum);

        /**
         * 
         * @breif: copy constructor for node
         * 
         * @param: base -> type: NetworkNode&, node to copy;
         * 
         */
        NetworkNode(const NetworkNode& base) noexcept;

        /**
         * 
         * @breif: assignment operator for the node class
         * 
         * @param: base -> type: NetworkNode&, the node value being assigned to the caller node
         * @return: NetworkNode& -> modified network node address
         * 
         */
        NetworkNode& operator=(const NetworkNode& base) noexcept;

        /**
         * 
         * @breif: destructor for the networkNode class
         */
        ~NetworkNode() noexcept;


        /**
         * 
         * @breif: Calculates the output of the node for the inputs coming in
         * 
         * @param: inputs ->  type: vector<double>, input values for the node
         * @return: output -> type: double, calculated output post weighted sum, bias, and
         *                      activation function
         */
        double find_output(const std::vector<double>& inputs) noexcept;

        /**
         * 
         * @brief: Applies the activation function to the node's weighted sum and bias
         * 
         * @param nodeInfo -> type: double, the weighted sum of inputs plus bias
         * @return formattedOutput -> type: double, the result of applying the activation function
         */
        static double activation_func(double nodeInfo) noexcept;

        /**
         * 
         * @brief: getter function for the nodes output
         * 
         * @return nodeOutput -> type: double, the nodes outptut
         */
        double get() const noexcept;

        /**
         * @brief: set function for the biases
         */
        void set(double bias) noexcept;

        /**
         *
         * @brief: getter function for the size of the weight vector
         *
         * @return: size_t -> type: size_t, the size of the weight vector
         *
         */
        size_t getWeightVecSize() const noexcept { return weightVec.size(); }

        /**
         * @breif: getter function for the elements of the weight vector
         *
         * @param: index -> type: size_t, the index of the element
         * @return: double -> type: double, the value of the element
         *
         */
        double getWeightVecElement(size_t index) const noexcept { return weightVec.at(index); }

        /**
         *
         * @brief: getter function for the bias value
         *
         * @return: double -> type: double, the bias value
         *
         */
        double getBiasVal() const noexcept { return biasVal; }

        /**
         *
         * @brief: getter function for the output
         *
         * @return: double -> type: double, the output
         *
         */
        double getOutput() const noexcept { return output; }

        /**
         *
         * @berif: getter function for the number of outputs
         *
         * @return: int -> type: int, the number of outputs
         *
         */
         double getOutputNum() const noexcept { return numOutput; }

    private:
        //NodeType node;
        std::vector<double> weightVec;
        double biasVal;
        double output;
        int numOutput;
};

#endif