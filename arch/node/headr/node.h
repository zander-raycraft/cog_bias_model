#ifndef NODE_H
#define NODE_H

#include <vector>
/**
 * 
 * @class: NetworkNode -> base neuron for network
 *
 * @note: This node is made to be extendible to various number of inputs and outputs
 * 
 */

class NetworkNode 
{
    public:
        NetworkNode(int num_in); //dfault ctor

        /**
         * 
         * @breif: copy constructor for node
         * 
         * @param: base -> type: NetworkNode&, node to copy;
         * 
         */
        NetworkNode(const NetworkNode& base);

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
        double activation_func(double nodeInfo) noexcept;

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
         * @breif: friend function for the gtest file
         * 
         */
        friend void testNetworkNode(int inputNodes);

    private:
        std::vector<double> weightVec;
        double biasVal;
        double output;
};

#endif