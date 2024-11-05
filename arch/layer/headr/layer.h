#ifndef LAYER_H
#define LAYER_H
#include "../../node/headr/node.h"
#include <vector>

/**
 * 
 * @class: NetworkLayer -> base neuron layer for network
 *
 * @note: This node is made to be extendible to various number of inputs and outputs
 *
 */


// default ctor
// copy constructor

class NetworkLayer
{
    public:
        //Default Constructor
        NetworkLayer(int size);

        /**
        *
        * @breif: copy constructor, deep copy
        *
        * @param: copyLayer -> type: NetworkLayer&, layer to copy;
        *
        */
        NetworkLayer(const NetworkLayer& copyLayer) noexcept;

        /**
        *
        * @breif: assignment operator
        *
        * @param: copyLayer -> type: NetworkLayer&, layer to copy;
        *
        */
        NetworkLayer& operator=(const NetworkLayer& copyLayer) noexcept;

    private:
};

#endif