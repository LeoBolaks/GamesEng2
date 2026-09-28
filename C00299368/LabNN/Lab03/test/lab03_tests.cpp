// Lab 03 - first GoogleTest project
// Name  : Leo Bolaks
// StudentID: C00299368

#include <gtest/gtest.h>
#include "calculator.hpp"

// First test case solved for the class

TEST(Calculator, Add_ReturnsSumForPositive) {
    Calculator c;
    EXPECT_EQ(c.add(2, 3), 5); // using a simple GoogleTest style assert to call out functions.
                               // This is still unit testing, but the idea is that it could be scaled up.
}

// TODO(1): Add_ReturnsSumForNegatives - e.g. add(-4, -6) == -10
TEST(Calculator, Add_ReturnsSumForNegatives) {
    Calculator c;
    EXPECT_EQ(c.add(-4, -6), -10);
}

// TODO(2): Subtract_ReturnsDifference - e.g. subtract(10, 4) == 6
TEST(Calculator, Subtract_ReturnsDifference) {
    Calculator c;
    EXPECT_EQ(c.subtract(10, 4), 6);
}

// TODO(3): Subtract_ReturnsZeroForEqualInputs - e.g. subtract(7, 7) == 0
TEST(Calculator, Subtract_ReturnsZeroForEqualInputs) {
    Calculator c;
    EXPECT_EQ(c.subtract(7, 7), 0);
}

// TODO(4): Add_HandlesZero - e.g. add(0, 42) == 42
TEST(Calculator, Add_HandlesZero) {
    Calculator c;
    EXPECT_EQ(c.add(0, 42), 42);
}
