// Lab 04 - Prime Path Coverage
// Name  : Leo Bolaks
// StudentID: C00299368
//
// The unit under test is scanRegion( ) in src/region_scan.cpp. 
//
//
// Replace each FAIL( ) with real assertions. Add more TEST cases as you need
// them. A test "reaches" a node or edge by calling scanRegion( ) with a grid
// that forces execution down that part of the graph.

#include <gtest/gtest.h>
#include "region_scan.hpp"

// The following is an example test code provided as a starter code. 
// You can use the similar test format, and choosing the right assert from GoogleTest.
// Ideally, you should be able to write test cases for all FEASIBLE prime paths
// Just like we discussed in the lecture, Prime Path Coverage Subsumes Edge-Pair coverage, which in-turn subsumes Edge coverage.
// So in this lab, if all prime paths are covered (tests written for each), we automatically complete all the edges and the pairs.
TEST(ScanRegion, WorkedExample_PositiveMultipleOfThree) {
    // P03, P19
    Grid g = {{11, 11, 11}};
    EXPECT_EQ(scanRegion(g), 33);
}

// TODO(Task 2 - node coverage): add tests so that, between them, every node
// of the graph is executed at least once. One more grid alongside the worked
// example is enough. Think about which grid drives the false arm of the
// sum > 10 decision and the default arm of the switch.
// TEST(ScanRegion, NodeCoverage_SecondTest) {
//     FAIL() << "not implemented";
// }

// TODO(Task 3 - edge coverage): add the tests that cover the edges node
// coverage leaves out. The edge from the outer loop test straight to the code
// after the loops needs a grid the outer loop never enters.
// TEST(ScanRegion, EdgeCoverage_EmptyRegion) {
//     FAIL() << "not implemented";
// }

// TODO(Task 4 - prime path tour): using the prime path list in the overview,
// add tests whose executions tour the feasible prime paths. Label each test
// with the prime path numbers it covers. Three of the listed prime paths are
// infeasible; you do not write tests for those, but you must explain in your
// report why they cannot be toured.
TEST(ScanRegion, PrimePath_Example) {
    // P03, P22
    Grid g = {{-8}};
    EXPECT_EQ(scanRegion(g), 8);
    // This test covers nodes 1-6 and then 7 and the missing 9 and 12 nodes missing from the first top test
}


TEST(ScanRegion, If_Numbers_Less_Than_0_Non_Divisble_By_3) {
    Grid g = {{1, 4, 3}};
    EXPECT_EQ(scanRegion(g), -8);
}
TEST(ScanRegion, If_Numbers_Less_Than_0_Divisble_By_3) {
    Grid g = {{1, 4, 4}};
    EXPECT_EQ(scanRegion(g), 0);
}
TEST(ScanRegion, If_Numbers_Greater_Than_100_Non_Divisble_By_3) {
    Grid g = {{103, 4, 23}, {54, 65, 34}};
    EXPECT_EQ(scanRegion(g), 283);
}
TEST(ScanRegion, If_Numbers_Greater_Than_100_Divisble_By_3) {
    Grid g = {{103, 4, 10}, {54, 65, 34}};
    EXPECT_EQ(scanRegion(g), 100);
}
TEST(scanRegion, Test_Above_100) {
    int num = 105;
    EXPECT_EQ(adjust(num), 100);
}
TEST(scanRegion, Test_Big_Negative_Number) {
    int num = -987678;
    EXPECT_EQ(adjust(num), 0);
}

