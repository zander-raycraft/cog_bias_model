#ifndef LAYER_H
#define LAYER_H
#include "../../node/headr/node.h"
#include <vector>

/**
 *
 * @class: NetworkLayer -> base neuron layer for network
 *
 * @note: This is the layer class, you can define it as either an
 *          LSTM or Base layer made up of the respective nodes
 *
 * 1) First layer not getting any biases
 *
 * THINGS I NEED TO FIX
 *  LAST SESSION: I got the layer up, the .h, still need to do the .cpp and tests
 *                I need to fix the setInput thing and figure out algo for setting inputs in
 *                the matrix of pairs -> the idea is that each row represents the input values feeding into that node
 *                the size of the matrix will be size x num of inputs for first Node in layer, where the items in the
 *                matrix will be a double that thje first is the input and the second is the weight.
 *
 *                Make it to where the layers can be set up to not have biases if they are an input layer
 *                ???? maybe this can be done on the model level ?????
 *
 */
template <typename NodeType>
class NetworkLayer
{
    public:

    /**
     * @brief Default constructor
     *
     * @param size -> int, number of nodes in the layer
     * @param nodeType -> NodeType, type of nodes in the layer
     *
     * @notes It fills vec with size num of nodes of either base or LSTM, then it fills information matrix with bias and val
     */
    NetworkLayer(int size, NodeType nodeType);

    /**
    *
    * @brief: copy constructor, deep copy
    *
    * @param: copyLayer -> type: NetworkLayer&, layer to copy;
    *
    */
    NetworkLayer(const NetworkLayer& copyLayer) noexcept;

    /**
    *
    * @brief: assignment operator
    *
    * @param: copyLayer -> type: NetworkLayer&, layer to copy;
    *
    */
    NetworkLayer& operator=(const NetworkLayer& copyLayer) noexcept;

    /**
    * @brief Destructor
    */
    ~NetworkLayer() noexcept;

    /**
    * @brief calculates the output for the layer
    *
    * @param inputVals -> const std::vector<std::vector<std::pair<double, double>>>&,
     *                  input matrix where each element is a pair of input value and weight.
    * @return updates the internal output vector (void)
    */
    void calculateLayerOutput(const std::vector<std::vector<std::pair<double, double>>>& inputVals) noexcept;

    /**
    * @breif sets the input for the layer
    *
    * @params inputs -> const std::vector<double>&, input values to the layer
    * @returns updates the internal input vector (void)
    */
    void setInput(const std::vector<double>& inputs) noexcept;

    /**
     * @brief Updates weights for the layer during backpropagation
     *
     * @param weightUpdates -> const std::vector<std::vector<double>>&,
     *                         matrix of weight updates.
     */
    void updateWeights(const std::vector<std::vector<double>>& weightUpdates) noexcept;

    /**
    * @breif set biases the bias for the layer
    *
    * @params biases -> const std::vector<double>&, biases for the layer
    * @returns updates the internal input vector (void)
    */
    void setBias(const std::vector<double> biases) noexcept;


    private:
        std::vector<NetworkNode<NodeType>> layerNodes;
        std::vector<double> LayerOutputVec;
        std::vector<std::vector<std::pair<double, double>>> LayerInputMatrix;
};

#endif