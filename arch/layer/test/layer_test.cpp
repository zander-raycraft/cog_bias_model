#include "../headr/layer.h"
#include <gtest/gtest.h>

class LayerTest : public ::testing::Test{};

/**
 * @brief: Tests for the NetworkLayer constructor
 */
TEST_F(LayerTest, ConstructorTests)
{
    // Test 1: Check the creation of a BaseNode layer
    int numNodesBase = 5;
    try {
        NetworkLayer<BaseNode> baseLayer(numNodesBase, BaseNode(), true, nullptr);
        EXPECT_EQ(baseLayer.getPrivMemberLayerNodes().size(), numNodesBase) << "Failed to create BaseNode layer with correct number of nodes";
    } catch (const std::exception& e) {
        FAIL() << "BaseNode layer constructor threw an exception: " << e.what();
    }

    // Test 2: Check the creation of an LstmNode layer
    int numNodesLstm = 3;
    try {
        NetworkLayer<LstmNode> lstmLayer(numNodesLstm, LstmNode(), true, nullptr);
        EXPECT_EQ(lstmLayer.getPrivMemberLayerNodes().size(), numNodesLstm) << "Failed to create LstmNode layer with correct number of nodes";
    } catch (const std::exception& e) {
        FAIL() << "LstmNode layer constructor threw an exception: " << e.what();
    }

    // Test 3: Check weights are initialized properly for BaseNode layer
    NetworkLayer<BaseNode> baseLayer(numNodesBase, BaseNode(), true, nullptr);
    try {
        for (const auto& weight : baseLayer.getPrivMemberLayerWeights()) {
            EXPECT_GE(weight, -1.0) << "Weight less than -1 in BaseNode layer";
            EXPECT_LE(weight, 1.0) << "Weight greater than 1 in BaseNode layer";
        }
    } catch (const std::exception& e) {
        FAIL() << "Weight initialization check threw an exception: " << e.what();
    }

    // Test 4: Check previous layer linkage
    try {
        NetworkLayer<BaseNode> prevLayer(numNodesBase, BaseNode(), true, nullptr);
        NetworkLayer<BaseNode> currentLayer(numNodesLstm, BaseNode(), false, &prevLayer);
        EXPECT_EQ(currentLayer.getPrivMemberPrevLayer(), &prevLayer) << "Previous layer not correctly linked";
    } catch (const std::exception& e) {
        FAIL() << "Previous layer linkage check threw an exception: " << e.what();
    }
}
