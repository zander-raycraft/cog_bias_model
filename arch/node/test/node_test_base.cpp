#include <gtest/gtest.h>
#include "../headr/node.h"
#include <cstdlib>
#include <ctime>
#include <mach/mach.h>

// Test fixture
class NodeTest : public ::testing::Test {};

/**
 *
 * @brief Utility function to get the current memory usage of the process.
 *
 * @return Memory usage in bytes.
 */
size_t getCurrentRSS()
{
    struct mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info,
                  &infoCount) != KERN_SUCCESS)
    {
        return 0L; // Unable to get task info
    }
    return info.resident_size;
}

//ignore this line it is for github debugging

/**
 * @breif: test for the default constructor
 */
TEST_F(NodeTest, DefaultConstructor)
{
    int numInputs = 5;
    NetworkNode<BaseNode> node(numInputs);

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
//    EXPECT_THROW(NetworkNode<BaseNode>(0), std::invalid_argument) << "Invalid argument not thrown for 0 input";
//    EXPECT_THROW(NetworkNode<BaseNode>(-1), std::invalid_argument) << "Invalid argument not thrown for -1 input";
}

/**
* @breif: test for alternate constructor
*/
TEST_F(NodeTest, AlternateConstructor)
{
     // Test network
     int numInputs = 5;
     int numOutputs = 3;
     auto* node = new NetworkNode<BaseNode>(numInputs, numOutputs);

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
     auto* oneValNode = new NetworkNode<BaseNode>(3);
     EXPECT_EQ(oneValNode->getOutputNum(), 1) << "Failed to set the number of outputs";
     delete oneValNode;

     // Test 7: check to see if an invalid argument is thrown
     EXPECT_THROW(NetworkNode<BaseNode>(0, 1), std::invalid_argument) << "Invalid argument not thrown for 0 input";
     EXPECT_THROW(NetworkNode<BaseNode>(-1, 1), std::invalid_argument) << "Invalid argument not thrown for -1 input";
     EXPECT_THROW(NetworkNode<BaseNode>(1, 0), std::invalid_argument) << "Invalid argument not thrown for 0 output";
     EXPECT_THROW(NetworkNode<BaseNode>(1, -1), std::invalid_argument) << "Invalid argument not thrown for -1 output";
}

/**
* @breif: test to copy constructor
*/
TEST_F(NodeTest, CopyConstructor)
{
    // Test network
    int numInputs = 5;
    int numOutputs = 3;
    auto* node = new NetworkNode<BaseNode>(numInputs, numOutputs);
    auto* oneValNode = new NetworkNode<BaseNode>(*node);

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
    auto* nonCopyNode = new NetworkNode<BaseNode>(5);
    auto* copyNode = new NetworkNode<BaseNode>(7);

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
    auto* oneParamNode = new NetworkNode<BaseNode>(3);
    auto* twoParamNode = new NetworkNode<BaseNode>(5, 7);
    *oneParamNode = *twoParamNode;
    EXPECT_EQ(oneParamNode->getWeightVecSize(), 5) << "Failed in double parameter constructor - vec size"; //Vec size
    EXPECT_EQ(oneParamNode->getOutputNum(), 7) << "Failed in double parameter constructor - output"; //Output num

    EXPECT_GE(oneParamNode->getBiasVal(), -1.0) << "Bias less than -1 after assignment [double input]";
    EXPECT_LE(oneParamNode->getBiasVal(), 1.0) << "Bias greater than 1 after assignment [double input]";

    EXPECT_EQ(oneParamNode->getOutput(), 0.0) << "Failed in double parameter constructor - output"; //Output
    delete oneParamNode;
    delete twoParamNode;

    // Test 3: self assignment
    auto* selfNode = new NetworkNode<BaseNode>(5);
    *selfNode = *selfNode;
    EXPECT_EQ(selfNode->getWeightVecSize(), 5) << "Failed in self-assignment scenario"; //Vec size
    EXPECT_EQ(selfNode->getOutputNum(), 1) << "Failed in self-assignment scenario"; //Output num

    EXPECT_GE(selfNode->getBiasVal(), -1.0) << "Bias less than -1 after assignment [self-assignment]";
    EXPECT_LE(selfNode->getBiasVal(), 1.0) << "Bias greater than 1 after assignment [self-assignment]";

    EXPECT_EQ(selfNode->getOutput(), 0.0) << "Failed in self-assignment scenario"; //Output
    delete selfNode;

    // Test 4: check if it is copying a smaller vector
    auto* smallVecNode = new NetworkNode<BaseNode>(3);
    auto* bigVecNode = new NetworkNode<BaseNode>(5, 7);
    *smallVecNode = *bigVecNode;
    EXPECT_EQ(smallVecNode->getWeightVecSize(), 5) << "Failed in assigning smaller value - vec size"; //Vec size
    EXPECT_EQ(smallVecNode->getOutputNum(), 7) << "Failed in assigning smaller value - output num"; //Output num

    EXPECT_GE(smallVecNode->getBiasVal(), -1.0) << "Bias less than -1 after assignment [smaller vector]";
    EXPECT_LE(smallVecNode->getBiasVal(), 1.0) << "Bias greater than 1 after assignment [smaller vector]";

    EXPECT_EQ(smallVecNode->getOutput(), 0.0) << "Failed in assigning smaller value - output"; //Output
    delete smallVecNode;
    delete bigVecNode;
}

/**
* @breif: test for the destructor
*/
#ifdef __APPLE__

TEST_F(NodeTest, Destructor)
{
    /**
     *
     * NOTE: This test will not run and is designed not to run
     *       if you are not on macOS as the help is built for mac architecture
     *
     */

    // Test 1: Does the vec get removed from memory
    size_t memoryBefore = getCurrentRSS();

    auto* node = new NetworkNode<BaseNode>(5);
    size_t memoryAfterAllocation = getCurrentRSS();
    size_t allocatedMemory = memoryAfterAllocation - memoryBefore;

    ASSERT_EQ(node->getWeightVecSize(), 5) << "Failed to initialize weight vector size correctly";
    ASSERT_GE(node->getBiasVal(), -1.0) << "Bias value out of expected range";
    ASSERT_LE(node->getBiasVal(), 1.0) << "Bias value out of expected range";

    delete node;
    size_t memoryAfterDeletion = getCurrentRSS();
    size_t deallocatedMemory = memoryAfterAllocation - memoryAfterDeletion;
    // padding 1024 to account for system memory overhead
    ASSERT_LE(std::abs(static_cast<long>(deallocatedMemory - allocatedMemory)), 1024)
                                << "Memory was not properly deallocated after deletion.";
}
#endif

/**
 * @breif: test for find_output
 */
 TEST_F(NodeTest, FindOutput)
 {
     // Test 1: check if value is correctly calculated
     NetworkNode<BaseNode> node(3);
     std::vector<double> inputs = {0.5, -0.3, 0.7};
     double output = node.activation_func(node.find_output(inputs));

     // Manually calculate the expected output
     double expectedWeightedSum = 0;
     for (size_t i = 0; i < inputs.size(); ++i) {
         expectedWeightedSum += inputs[i] * node.getWeightVecElement(i);
     }
     expectedWeightedSum += node.getBiasVal();
     double expectedOutput = std::tanh(expectedWeightedSum);
     ASSERT_NEAR(output, expectedOutput, 1e-5) << "Failed to calculate correct output value";

     // Test 2: check what happens if input is smaller
     std::vector<double> mismatchedInputs = {0.5, -0.3};
     double outputMismatch = node.activation_func(node.find_output(mismatchedInputs));
     ASSERT_EQ(outputMismatch, 0.0) << "Failed to handle mismatched input vector size";

     // Test 3: All zero inputs
     std::vector<double> zeroInputs(3, 0.0); // {0.0, 0.0, 0.0}
     double outputZeroInputs = node.activation_func(node.find_output(zeroInputs));
     double expectedZeroOutput = std::tanh(node.getBiasVal());
     ASSERT_NEAR(outputZeroInputs, expectedZeroOutput, 1e-5) << "Failed to handle all zero inputs";

     // Test 4: All positive inputs
     std::vector<double> positiveInputs = {1.0, 1.0, 1.0};
     double outputPositive = node.activation_func(node.find_output(positiveInputs));
     double expectedPositiveSum = 0;
     for (size_t i = 0; i < positiveInputs.size(); ++i)
     {
         expectedPositiveSum += positiveInputs[i] * node.getWeightVecElement(i);
     }
     expectedPositiveSum += node.getBiasVal();
     double expectedPositiveOutput = std::tanh(expectedPositiveSum);
     ASSERT_NEAR(outputPositive, expectedPositiveOutput, 1e-5) << "Failed to handle all positive inputs";

     // Test 5: Extremely large inputs
     std::vector<double> largeInputs = {1e10, 1e10, 1e10};
     double outputLarge = node.activation_func(node.find_output(largeInputs));
     // Since this might cause overflow issues, check if the output is not NaN or infinity
     ASSERT_FALSE(std::isnan(outputLarge)) << "Output is NaN for extremely large inputs";
     ASSERT_FALSE(std::isinf(outputLarge)) << "Output is infinity for extremely large inputs";

     // Test 7: Inputs with NaN
     std::vector<double> nanInputs = {0.5, std::nan(""), 0.7};
     double outputNaN = node.activation_func(node.find_output(nanInputs));
     ASSERT_TRUE(std::isnan(outputNaN)) << "Failed to handle NaN input value";

     // Test 8: Inputs with infinity
     std::vector<double> infInputs = {0.5, std::numeric_limits<double>::infinity(), 0.7};
     double outputInf = node.find_output(infInputs);
     ASSERT_TRUE(std::isinf(outputInf)) << "Failed to handle infinity input value";
 }

 /**
  * @breif: test for activation function
  */
  TEST_F(NodeTest, ActivationFunction) {
    // Test 1: check if function is correct
    auto *node = new NetworkNode<BaseNode>(3);
    double inputZero = 0.0;
    double outputZero = node->activation_func(inputZero);
    ASSERT_NEAR(outputZero, 0.0, 1e-5) << "Failed to calculate tanh(0) correctly";

    // Test 2: Check for positive value
    double inputPositive = 1.0;
    double outputPositive = node->activation_func(inputPositive);
    ASSERT_NEAR(outputPositive, std::tanh(1.0), 1e-5) << "Failed to calculate tanh(1) correctly";

    // Test 3: Check for negative value
    double inputNegative = -1.0;
    double outputNegative = node->activation_func(inputNegative);
    ASSERT_NEAR(outputNegative, std::tanh(-1.0), 1e-5) << "Failed to calculate tanh(-1) correctly";

    // Test 4: Check for large positive value
    double inputLargePositive = 1000.0;
    double outputLargePositive = node->activation_func(inputLargePositive);
    ASSERT_NEAR(outputLargePositive, 1.0, 1e-5) << "Failed to handle large positive input correctly";

    // Test 5: Check for large negative value
    double inputLargeNegative = -1000.0;
    double outputLargeNegative = node->activation_func(inputLargeNegative);
    ASSERT_NEAR(outputLargeNegative, -1.0, 1e-5) << "Failed to handle large negative input correctly";
}

/**
 * @breif: test for the get function
 */
TEST_F(NodeTest, Get)
{
    // Test 1: check if function is correct after default initalization
    auto* node = new NetworkNode<BaseNode>(3);
    double output = node->get();
    ASSERT_EQ(output, 0.0) << "Failed to get the correct initial output value";

    // Test 2: Get value after calculating output
    std::vector<double> inputs = {0.5, -0.3, 0.7};
    node->find_output(inputs);
    double calculatedOutput = node->get();
    ASSERT_NEAR(calculatedOutput, std::tanh(node->find_output(inputs)), 1e-5) << "Failed to get the correct calculated output value";

    // Test 3: Get value after setting bias and recalculating output
    node->set(0.5);
    node->find_output(inputs);
    double outputAfterBiasSet = node->get();
    ASSERT_NEAR(outputAfterBiasSet, std::tanh(node->find_output(inputs)), 1e-5) << "Failed to get the correct output value after setting bias";

    delete node;
}

/**
 * @breif: test for the set function
 */
TEST_F(NodeTest, Set)
{
    // Test 1: check if function is correct after default initialization
    auto* node = new NetworkNode<BaseNode>(3);
    node->set(0.5);
    ASSERT_EQ(node->getBiasVal(), .5) << "Failed to set bias to a positive value correctly";

    // Test 2: Set value after calculating output
    double newBias = -0.75;
    node->set(newBias);
    ASSERT_EQ(node->getBiasVal(), newBias) << "Failed to set bias to a negative value correctly";

    // Test 3: Check if bias is set correctly to zero
    newBias = 0.0;
    node->set(newBias);
    ASSERT_EQ(node->getBiasVal(), newBias) << "Failed to set bias to zero correctly";
    delete node;
}

