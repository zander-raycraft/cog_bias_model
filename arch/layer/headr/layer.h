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
          * @params inputs -> const std::vector<double>&, input values to the layer
          * @return updates the internal output vector (void)
          */
          void calculateLayerOutput(const std::vector<double>& inputs) noexcept;

          /**
           * @breif sets the input for the layer
           *
           * @params inputs -> const std::vector<double>&, input values to the layer
           * @returns updates the internal input vector (void)
           */
           void setInput(const std::vector<double>& inputs) noexcept;

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
        std::vector<std::shared_ptr<std::vector<std::pair<double, double>>>> LayerInputVec;

};

#endif