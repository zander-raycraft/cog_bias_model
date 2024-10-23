#include <gtest/gtest.h>
#include "../headr/node.h"
#include <cstdlib>
#include <ctime>

// Test fixture
class NodeTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        // Set a random seed before running tests to ensure different random values
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
    }
};

/**
 * @breif: test for the default constructor
 */
TEST_F(NodeTest, DefaultConstructor)
{
    int numInputs = 5;
    NetworkNode node(numInputs);

    // Test 1: Check number of inputs
    EXPECT_EQ(node.getWeightVecSize(), numInputs) << "Failed to set the number of inputs";

    // Test 2: Check that each weight within range [-1, 0]
    for (int i = 0; i < numInputs; i++)
    {
        double weight = node.getWeightVecElement(i);
        EXPECT_GE(weight, -1.0) << "Weight less than -1";
        EXPECT_LE(weight, 1.0) << "Weight greater than 1";
    }

    // Test 3: check to see if the bias values are within [-1, 1]
    EXPECT_GE(node.getBiasVal(), -1.0) << "Bias less than -1";
    EXPECT_LE(node.getBiasVal(), 1.0) << "Bias greater than 1";

    // Test 4: check to see if correct initial val of output
    EXPECT_LE(node.getOutput(), 0) << "Output not correctly initialized";

    // Test 5: check to see if output number is correctly initialized to one
    EXPECT_LE(node.getOutputNum(), 1) << "Output not correctly initialized";

    // Test 6: check to see if an invalid argument is thrown
//    EXPECT_THROW(NetworkNode(0), std::invalid_argument) << "Invalid argument not thrown for 0 input";
//    EXPECT_THROW(NetworkNode(-1), std::invalid_argument) << "Invalid argument not thrown for -1 input";
}

/**
* @breif: test for alternate constructor
*/
TEST_F(NodeTest, AlternateConstructor)
{
     // Test network
     int numInputs = 5;
     int numOutputs = 3;
     auto* node = new NetworkNode(numInputs, numOutputs);

     // Test 1: check number of inputs
     EXPECT_EQ(node->getWeightVecSize(), numInputs) << "Failed to set the number of inputs";

     // Test 2: check number of outputs
     EXPECT_EQ(node->getOutputNum(), numOutputs) << "Failed to set the number of outputs";

     // Test 3: check that each weight within range [-1, 0]
     for (int i = 0; i < numInputs; i++)
     {
         double weight = node->getWeightVecElement(i);
         EXPECT_GE(weight, -1.0) << "Weight less than -1";
         EXPECT_LE(weight, 1.0) << "Weight greater than 1";
     }

     // Test 4: check to see if the bias values are within [-1, 1]
     EXPECT_GE(node->getBiasVal(), -1.0) << "Bias less than -1";
     EXPECT_LE(node->getBiasVal(), 1.0) << "Bias greater than 1";

     // Test 5: check to see if correct initial val of output
     EXPECT_LE(node->getOutput(), 0) << "Output not correctly initialized";

     // Test 6: check and see if only one param is passed that the output num is 1;
     delete node;
     auto* oneValNode = new NetworkNode(3);
     EXPECT_EQ(oneValNode->getOutputNum(), 1) << "Failed to set the number of outputs";
     delete oneValNode;

     // Test 7: check to see if an invalid argument is thrown
     EXPECT_THROW(NetworkNode(0, 1), std::invalid_argument) << "Invalid argument not thrown for 0 input";
     EXPECT_THROW(NetworkNode(-1, 1), std::invalid_argument) << "Invalid argument not thrown for -1 input";
     EXPECT_THROW(NetworkNode(1, 0), std::invalid_argument) << "Invalid argument not thrown for 0 output";
     EXPECT_THROW(NetworkNode(1, -1), std::invalid_argument) << "Invalid argument not thrown for -1 output";
}

/**
* @breif: test to copy constructor
*/
TEST_F(NodeTest, CopyConstructor)
{
    // Test network
    int numInputs = 5;
    int numOutputs = 3;
    auto* node = new NetworkNode(numInputs, numOutputs);
    auto* oneValNode = new NetworkNode(*node);

    // Test 1: check number of inputs
    EXPECT_EQ(oneValNode->getWeightVecSize(), numInputs) << "Failed to set the number of inputs";

    // Test 2: check number of outputs
    EXPECT_EQ(oneValNode->getOutputNum(), numOutputs) << "Failed to set the number of outputs";

    // Test 3: check that each weight within range [-1, 0]
    for (int i = 0; i < numInputs; i++)
    {
        double weight = oneValNode->getWeightVecElement(i);
        EXPECT_GE(weight, -1.0) << "Weight less than -1";
        EXPECT_LE(weight, 1.0) << "Weight greater than 1";
    }

    // Test 4: check to see if the bias values are within [-1, 1]
    EXPECT_GE(oneValNode->getBiasVal(), -1.0) << "Bias less than -1";
    EXPECT_LE(oneValNode->getBiasVal(), 1.0) << "Bias greater than 1";

    // Test 5: check to see if correct initial val of output
    EXPECT_LE(oneValNode->getOutput(), 0) << "Output not correctly initialized";

    // Test 6: self assessment
    *oneValNode = *oneValNode;
    EXPECT_EQ(oneValNode->getWeightVecSize(), numInputs) << "Failed in self-assignment scenario";

    // Test 7: make sure deep copy does not get affected by changes in the original
    node->set(0.5);
    EXPECT_NE(oneValNode->getBiasVal(), 0.5) << "Failed in self-assignment scenario";
    delete oneValNode;
    delete node;
}

/**
* @breif: test to assignment operator
*/
TEST_F(NodeTest, AssignmentOperator)
{
    // Test 1: check values
    auto* nonCopyNode = new NetworkNode(5);
    auto* copyNode = new NetworkNode(7);

    *copyNode = *nonCopyNode;
    EXPECT_EQ(copyNode->getWeightVecSize(), 5) << "Failed in assignment scenario - weight size"; //Vec size
    EXPECT_EQ(copyNode->getOutputNum(), 1) << "Failed in assignment scenario - output num"; //Output num

    // bias check
    EXPECT_GE(copyNode->getBiasVal(), -1.0) << "Bias less than -1 after assignment";
    EXPECT_LE(copyNode->getBiasVal(), 1.0) << "Bias greater than 1 after assignment";

    EXPECT_EQ(copyNode->getOutput(), 0.0) << "Failed in assignment scenario - output"; //Output
    delete nonCopyNode;
    delete copyNode;

    // Test 2: Checking assignment operator from different constructor calls
    auto* oneParamNode = new NetworkNode(3);
    auto* twoParamNode = new NetworkNode(5, 7);
    *oneParamNode = *twoParamNode;
    EXPECT_EQ(oneParamNode->getWeightVecSize(), 5) << "Failed in double parameter constructor - vec size"; //Vec size
    EXPECT_EQ(oneParamNode->getOutputNum(), 7) << "Failed in double parameter constructor - output"; //Output num

    EXPECT_GE(oneParamNode->getBiasVal(), -1.0) << "Bias less than -1 after assignment [double input]";
    EXPECT_LE(oneParamNode->getBiasVal(), 1.0) << "Bias greater than 1 after assignment [double input]";

    EXPECT_EQ(oneParamNode->getOutput(), 0.0) << "Failed in double parameter constructor - output"; //Output
    delete oneParamNode;
    delete twoParamNode;

    // Test 3: self assignment
    auto* selfNode = new NetworkNode(5);
    *selfNode = *selfNode;
    EXPECT_EQ(selfNode->getWeightVecSize(), 5) << "Failed in self-assignment scenario"; //Vec size
    EXPECT_EQ(selfNode->getOutputNum(), 1) << "Failed in self-assignment scenario"; //Output num

    EXPECT_GE(selfNode->getBiasVal(), -1.0) << "Bias less than -1 after assignment [self-assignment]";
    EXPECT_LE(selfNode->getBiasVal(), 1.0) << "Bias greater than 1 after assignment [self-assignment]";

    EXPECT_EQ(selfNode->getOutput(), 0.0) << "Failed in self-assignment scenario"; //Output
    delete selfNode;

    // Test 4: check if it is copying a smaller vector
    auto* smallVecNode = new NetworkNode(3);
    auto* bigVecNode = new NetworkNode(5, 7);
    *smallVecNode = *bigVecNode;
    EXPECT_EQ(smallVecNode->getWeightVecSize(), 5) << "Failed in assigning smaller value - vec size"; //Vec size
    EXPECT_EQ(smallVecNode->getOutputNum(), 7) << "Failed in assigning smaller value - output num"; //Output num

    EXPECT_GE(smallVecNode->getBiasVal(), -1.0) << "Bias less than -1 after assignment [smaller vector]";
    EXPECT_LE(smallVecNode->getBiasVal(), 1.0) << "Bias greater than 1 after assignment [smaller vector]";

    EXPECT_EQ(smallVecNode->getOutput(), 0.0) << "Failed in assigning smaller value - output"; //Output
    delete smallVecNode;
    delete bigVecNode;

}