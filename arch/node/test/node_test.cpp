#include <gtest/gtest.h>
#include "../headr/node.h"

// Test fixture (if needed)
class NodeTest : public ::testing::Test {
protected:
};



void testNetworkNode(int numInputs) {
    NetworkNode node(numInputs);
    EXPECT_EQ(node.weightVec.size(), numInputs);
}

// Sample test
TEST_F(NodeTest, DefaultConstructor) {
    int numInputs = 5;
    NetworkNode node(numInputs);
    testNetworkNode(numInputs); 
}