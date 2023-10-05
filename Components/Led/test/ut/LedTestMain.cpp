// ----------------------------------------------------------------------
// TestMain.cpp
// ----------------------------------------------------------------------

#include "LedTester.hpp"

TEST(Nominal, TestBlinking) {
    Components::LedTester tester;
    tester.testBlinking();
}

TEST(Nominal, TestBlinkInterval) {
    Components::LedTester tester;
    tester.testBlinkInterval();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
