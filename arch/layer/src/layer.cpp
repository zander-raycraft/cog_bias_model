#include "../headr/layer.h"
#include <stdexcept>

/**
 * @brief Default constructor
 * @param size -> int, number of nodes in the layer
 * @param nodeType -> NodeType, type of nodes in the layer
 */
//template <typename NodeType>
//NetworkLayer<NodeType>::NetworkNode(int size, NodeType nodeType) :
//    layerNodes(size, NetworkNode<NodeType>(1)),
//    LayerOutputVec(size, 0.0),
//    LayerInputVec(1, 0.0)