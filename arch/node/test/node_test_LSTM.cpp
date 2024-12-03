#include <gtest/gtest.h>
#include "../headr/node.h"
#include <ctime>

class NetworkNodeLstmTest : public ::testing::Test {};

/**
 * @brief Test initialization of NetworkNode with LstmNode template
 */
TEST_F(NetworkNodeLstmTest, Initialization)
{
    int numInputs = 5;
    NetworkNode<LstmNode> lstmNode(numInputs);

    // Test 1: Check weight vector size
    EXPECT_EQ(lstmNode.getWeightVecSize(), numInputs) << "Weight vector size mismatch.";

    // Test 2: Check initial bias value
    EXPECT_GE(lstmNode.getBiasVal(), -1.0) << "Bias less than -1.";
    EXPECT_LE(lstmNode.getBiasVal(), 1.0) << "Bias greater than 1.";

    // Test 3: Check default output
    EXPECT_EQ(lstmNode.get(), 0.0) << "Output not initialized to 0.";

    // Test 4: Check LstmNode-specific attributes
    auto& internalNode = lstmNode.getNode();
    EXPECT_EQ(internalNode.LongTermState, 0.0) << "Long-term state not initialized to 0.";
    EXPECT_EQ(internalNode.ShortTermState, 0.0) << "Short-term state not initialized to 0.";
    EXPECT_EQ(internalNode.forgetVals.size(), 3) << "Forget gate vector size mismatch.";
    EXPECT_EQ(internalNode.inputVals.size(), 3) << "Input gate vector size mismatch.";
    EXPECT_EQ(internalNode.outputVals.size(), 3) << "Output gate vector size mismatch.";
}

/**
 * @brief Test updates to gates and states in LstmNode
 */
TEST_F(NetworkNodeLstmTest, GateAndStateUpdates)
{
    NetworkNode<LstmNode> lstmNode(4);

    // Test 1: Update forget gate values
    lstmNode.getNode().forgetVals = {0.1, 0.2, 0.3};
    EXPECT_EQ(lstmNode.getNode().forgetVals[0], 0.1) << "Forget gate value update failed.";
    EXPECT_EQ(lstmNode.getNode().forgetVals[1], 0.2) << "Forget gate value update failed.";
    EXPECT_EQ(lstmNode.getNode().forgetVals[2], 0.3) << "Forget gate value update failed.";

    // Test 2: Update input gate values
    lstmNode.getNode().inputVals = {0.4, 0.5, 0.6};
    EXPECT_EQ(lstmNode.getNode().inputVals[0], 0.4) << "Input gate value update failed.";
    EXPECT_EQ(lstmNode.getNode().inputVals[1], 0.5) << "Input gate value update failed.";
    EXPECT_EQ(lstmNode.getNode().inputVals[2], 0.6) << "Input gate value update failed.";

    // Test 3: Update output gate values
    lstmNode.getNode().outputVals = {0.7, 0.8, 0.9};
    EXPECT_EQ(lstmNode.getNode().outputVals[0], 0.7) << "Output gate value update failed.";
    EXPECT_EQ(lstmNode.getNode().outputVals[1], 0.8) << "Output gate value update failed.";
    EXPECT_EQ(lstmNode.getNode().outputVals[2], 0.9) << "Output gate value update failed.";

    // Test 4: Update long-term state
    lstmNode.getNode().LongTermState = 1.5;
    EXPECT_EQ(lstmNode.getNode().LongTermState, 1.5) << "Long-term state update failed.";

    // Test 5: Update short-term state
    lstmNode.getNode().ShortTermState = -0.75;
    EXPECT_EQ(lstmNode.getNode().ShortTermState, -0.75) << "Short-term state update failed.";
}

/**
 * @brief Test find_output functionality for NetworkNode with LstmNode template
 */
TEST_F(NetworkNodeLstmTest, FindOutput)
{
    NetworkNode<LstmNode> lstmNode(3);
    std::vector<double> inputs = {0.4, -0.6, 0.8};

    // Test 1: Check output for valid inputs
    double weightedSum = lstmNode.find_output(inputs);
    EXPECT_NO_THROW(lstmNode.find_output(inputs)) << "find_output threw an exception for valid inputs.";

    // Test 2: Verify weighted sum calculation
    double expectedWeightedSum = 0.0;
    for (size_t i = 0; i < inputs.size(); ++i) {
        expectedWeightedSum += inputs[i] * lstmNode.getWeightVecElement(i);
    }
    expectedWeightedSum += lstmNode.getBiasVal();
    EXPECT_NEAR(weightedSum, expectedWeightedSum, 1e-5) << "Weighted sum calculation is incorrect.";

    // Test 3: Check activation function output
    double output = NetworkNode<LstmNode>::activation_func(weightedSum);
    EXPECT_NEAR(output, std::tanh(expectedWeightedSum), 1e-5) << "Activation function output is incorrect.";
}

/**
 * @brief Test edge cases for find_output in NetworkNode with LstmNode template
 */
TEST_F(NetworkNodeLstmTest, FindOutputEdgeCases)
{
    NetworkNode<LstmNode> lstmNode(3);

    // Test 1: Handle mismatched input size
    std::vector<double> inputs = {0.5, 0.6}; // Fewer inputs than weights
    EXPECT_EQ(lstmNode.find_output(inputs), 0.0) << "Did not handle mismatched input size correctly.";

    // Test 2: Handle all zero inputs
    inputs = {0.0, 0.0, 0.0};
    EXPECT_NO_THROW(lstmNode.find_output(inputs)) << "find_output threw exception for all zero inputs.";

    // Test 3: Handle large inputs
    inputs = {1e10, 1e10, 1e10};
    double output = lstmNode.find_output(inputs);
    EXPECT_FALSE(std::isnan(output)) << "Output is NaN for large inputs.";
    EXPECT_FALSE(std::isinf(output)) << "Output is infinity for large inputs.";
}

/**
 * @brief Test activation function behavior for NetworkNode with LstmNode template
 */
TEST_F(NetworkNodeLstmTest, ActivationFunction)
{
    // Test 1: Check tanh behavior for 0
    EXPECT_NEAR(NetworkNode<LstmNode>::activation_func(0.0), std::tanh(0.0), 1e-5) << "tanh(0) failed.";

    // Test 2: Check tanh behavior for positive value
    EXPECT_NEAR(NetworkNode<LstmNode>::activation_func(1.0), std::tanh(1.0), 1e-5) << "tanh(1) failed.";

    // Test 3: Check tanh behavior for negative value
    EXPECT_NEAR(NetworkNode<LstmNode>::activation_func(-1.0), std::tanh(-1.0), 1e-5) << "tanh(-1) failed.";

    // Test 4: Check tanh behavior for large positive value
    EXPECT_NEAR(NetworkNode<LstmNode>::activation_func(1000.0), 1.0, 1e-5) << "tanh(large positive) failed.";

    // Test 5: Check tanh behavior for large negative value
    EXPECT_NEAR(NetworkNode<LstmNode>::activation_func(-1000.0), -1.0, 1e-5) << "tanh(large negative) failed.";
}
