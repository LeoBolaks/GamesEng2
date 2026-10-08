// Lab 05 - Testing a Three State Machine
// Name  :
// StudentID:

#include <gtest/gtest.h>
#include "machine.hpp"

// -------- State coverage --------
// Visit OFF, ON, PAUSED in a single sequence.
TEST(Machine, VisitsAllThreeStates) {
    Machine m;
    EXPECT_EQ(m.current(), State::OFF);
    // TODO: drive the machine to ON, then to PAUSED, checking with EXPECT_EQ after each step.
    FAIL() << "not implemented";
}

// -------- Transition coverage: one test per transition --------
TEST(Machine, OffPowerOnGoesToOn) {
    Machine m;
    EXPECT_EQ(m.transition(State::OFF, Event::powerOn), State::ON);
}

TEST(Machine, OnPowerOffGoesToOff) {
    // TODO
    FAIL() << "not implemented";
}

TEST(Machine, OnPauseGoesToPaused) {
    // TODO
    FAIL() << "not implemented";
}

TEST(Machine, PausedResumeGoesToOn) {
    // TODO
    FAIL() << "not implemented";
}

TEST(Machine, PausedPowerOffGoesToOff) {
    // TODO
    FAIL() << "not implemented";
}
