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

/**
 * @brief: Tests for the dataLoadLstm function
 */
TEST_F(LayerTest, DataLoadLstmTests)
{
    // Test 1: Valid data loading for LstmNode
    int numNodesLstm = 3;
    NetworkLayer<LstmNode> lstmLayer(numNodesLstm, LstmNode(), true, nullptr);

    std::vector<std::vector<double>> validData = {
            {1.0, 2.0, 3.0}, // Layer outputs
            {0.5, 0.6, 0.7}, // STM
            {0.8, 0.9, 1.0}  // LTM
    };

    try {
        lstmLayer.dataLoadLstm(validData);

        // Check that each node in the layer has updated STM and LTM states
        for (size_t nodeIdx = 0; nodeIdx < lstmLayer.getPrivMemberLayerNodes().size(); ++nodeIdx) {
            auto& node = lstmLayer.getPrivMemberLayerNodes()[nodeIdx];
            auto& nodeInternal = node.getNode(); // Access internal LstmNode

            EXPECT_NEAR(nodeInternal.ShortTermState, 0.6, 1e-6) << "STM average not set correctly for node " << nodeIdx;
            EXPECT_NEAR(nodeInternal.LongTermState, 0.9, 1e-6) << "LTM average not set correctly for node " << nodeIdx;

            // Verify inputs are set correctly
            const auto& inputs = node.getInputs();
            ASSERT_EQ(inputs.size(), validData[0].size()) << "Input vector size mismatch for node " << nodeIdx;
            for (size_t i = 0; i < inputs.size(); ++i) {
                EXPECT_NEAR(inputs[i], validData[0][i], 1e-6) << "Mismatch in input value at index " << i;
            }
        }
    } catch (const std::exception& e) {
        FAIL() << "Valid data load threw an exception: " << e.what();
    }

    // Test 2: Empty data throws exception
    std::vector<std::vector<double>> emptyData;
    EXPECT_THROW({
                     lstmLayer.dataLoadLstm(emptyData);
                 }, std::invalid_argument) << "Empty data did not throw an exception";

    // Test 3: Data with empty rows throws exception
    std::vector<std::vector<double>> dataWithEmptyRows = {
            {.7, .7, .7},
            {0.5, 0.6, 0.7},
            {0.8, 0.9, 1.0}
    };
    EXPECT_THROW({
                     lstmLayer.dataLoadLstm(dataWithEmptyRows);
                 }, std::invalid_argument) << "Data with empty rows did not throw an exception";

    // Test 4: Logic error for BaseNode
    NetworkLayer<BaseNode> baseLayer(3, BaseNode(), true, nullptr);
    EXPECT_THROW({
                     baseLayer.dataLoadLstm(validData);
                 }, std::logic_error) << "Logic error not thrown for BaseNode";
}

