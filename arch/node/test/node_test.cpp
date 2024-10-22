#include <gtest/gtest.h>
#include "../headr/node.h"

// Test fixture (if needed)
class NodeTest : public ::testing::Test {
protected:
};

// Sample test
TEST_F(NodeTest, DefaultConstructor) {
    int numInputs = 5;
    NetworkNode node(numInputs);
    testNetworkNode(numInputs); 
}

// No need for a main function, Google Test provides its own

void testNetworkNode(int numInputs) {
    NetworkNode node(numInputs);
    EXPECT_EQ(node.weightVec.size(), numInputs);
}
