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
 *  LAST SESSION: I got halfway through the layer.cpp constructor, I went back and revamped the logic for the nodes
 *                  in the layer class and fixed the math for the LSTM node, now I need to make the connection and
 *                  establishment of the LSTM layer then finish out the rest of the layer class and Gtests
 *
 *
 *                  think about applying information entropy -> if oyu have exp listening to harmonic freqeuncies, you can
 *                  get more information from listening to jazz cords
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
    NetworkLayer(int size, NodeType nodeType, bool isInputLayer, NetworkLayer<NodeType>* prev = nullptr) noexcept;

    /**
    *
    * @brief: copy constructor, deep copy
    *
    * @param: copyLayer -> type: NetworkLayer&, layer to copy;
    *
    */
    //NetworkLayer(const NetworkLayer& copyLayer) noexcept;

    /**
    *
    * @brief: assignment operator
    *
    * @param: copyLayer -> type: NetworkLayer&, layer to copy;
    *
    */
    //NetworkLayer& operator=(const NetworkLayer& copyLayer) noexcept;

    /**
    * @brief Destructor
    */
    //~NetworkLayer() noexcept;

    /**
    * @brief calculates the output for the layer
    *
    * @param inputVals -> const std::vector<std::vector<std::pair<double, double>>>&,
     *                  input matrix where each element is a pair of input value and weight.
    * @return updates the internal output vector (void)
    */
    //void calculateLayerOutput(const std::vector<std::vector<std::pair<double, double>>>& inputVals) noexcept;

    /**
    * @breif sets the input for the layer
    *
    * @params inputs -> const std::vector<double>&, input values to the layer
    * @returns updates the internal input vector (void)
    */
    //void setInput(const std::vector<double>& inputs) noexcept;

    /**
     * @brief Updates weights for the layer during backpropagation
     *
     * @param weightUpdates -> const std::vector<std::vector<double>>&,
     *                         matrix of weight updates.
     */
    //void updateWeights(const std::vector<std::vector<double>>& weightUpdates) noexcept;

    /**
    * @breif set biases the bias for the layer
    *
    * @params biases -> const std::vector<double>&, biases for the layer
    * @returns updates the internal input vector (void)
    */
    //void setBias(const std::vector<double> biases) noexcept;

    /**
     *
     * @breif this is a helper function for the ctor for the inputLayer
     * @param values -> std::vec<double> values from the datafile to be run through the model
     * @returns void
     *
     */
     //void setInputLayer(std::vector<double> values) noexcept;

     /**
      *
      * @breif this function modifies the LSTM nodes to have properly formatted data between layers
      * @param values -> std::vector<std::vector<double>> values from the datafile to be run through the model
      * @return void
      *
      */
     void dataLoadLstm(std::vector<std::vector<double>> values);

     const std::vector<NetworkNode<NodeType>>& getPrivMemberLayerNodes() const noexcept{ return layerNodes; }
     std::vector<double> getPrivMemberLayerWeights() const noexcept { return LayerWeights; }
     NetworkLayer* getPrivMemberPrevLayer() const noexcept { return prevLayer; }



private:
        std::vector<NetworkNode<NodeType>> layerNodes;
        std::vector<double> LayerOutputVec;
        std::vector<double> LayerWeights;
        NetworkLayer* prevLayer;
        std::vector<std::vector<double>> informationMatrix;
};

#endif