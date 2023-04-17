// ----------------------------------------------------------------------
// TestMain.cpp
// ----------------------------------------------------------------------

#include "Tester.hpp"

TEST(Nominal, TestBlinking) {
    Components::Tester tester;
    tester.testBlinking();
}

TEST(Nominal, TestBlinkInterval) {
    Components::Tester tester;
    tester.testBlinkInterval();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
