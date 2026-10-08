// Lab 05 - Testing a Three State Machine
// Name  : Leo Bolaks
// StudentID: C00299368

#include <gtest/gtest.h>
#include "machine.hpp"

// -------- State coverage --------
// Visit OFF, ON, PAUSED in a single sequence.
TEST(Machine, VisitsAllThreeStates) {
    Machine m;
    EXPECT_EQ(m.current(), State::OFF);
    // TODO: drive the machine to ON, then to PAUSED, checking with EXPECT_EQ after each step.
    m.step(Event::powerOn);
    EXPECT_EQ(m.current(), State::ON);

    m.step(Event::pause);
    EXPECT_EQ(m.current(), State::PAUSED);

    m.step(Event::resume);
    EXPECT_EQ(m.current(), State::ON);
}

// -------- Transition coverage: one test per transition --------
TEST(Machine, OffPowerOnGoesToOn) {
    Machine m;
    EXPECT_EQ(m.transition(State::OFF, Event::powerOn), State::ON);
}

TEST(Machine, OnPowerOffGoesToOff) {
    Machine m;
    EXPECT_EQ(m.transition(State::ON, Event::powerOff), State::OFF);
}

TEST(Machine, OnPauseGoesToPaused) {
    Machine m;
    EXPECT_EQ(m.transition(State::ON, Event::pause), State::PAUSED);
}

TEST(Machine, PausedResumeGoesToOn) {
    Machine m;
    EXPECT_EQ(m.transition(State::PAUSED, Event::resume), State::ON);
}

TEST(Machine, PausedPowerOffGoesToOff) {
    Machine m;
    EXPECT_EQ(m.transition(State::PAUSED, Event::powerOff), State::OFF);
}
