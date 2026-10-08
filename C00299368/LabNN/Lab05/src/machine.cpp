//Lab05 - Three State Machine
#include "machine.hpp"

Machine::Machine() : current_(State::OFF) {}
State Machine::current() const { return current_; }

State Machine::transition(State s, Event e) const {
    // TODO: fill in the transitions per the table in the lab overview.
    // Any (state, event) not in the table returns s unchanged.
    switch (s) {
        case State::OFF:
            // TODO: powerOn -> ON
            if (e == Event::powerOn) return State::ON;
            break;
        case State::ON:
            // TODO: powerOff -> OFF
            // TODO: pause    -> PAUSED
            if (e == Event::powerOff) return State::OFF;
            else if (e == Event::pause) return State::PAUSED;
            break;
        case State::PAUSED:
            // TODO: resume   -> ON
            // TODO: powerOff -> OFF
            if (e == Event::resume) return State::ON;
            else if (e == Event::powerOff) return State::OFF;
            break;
    }
    return s;
}

void Machine::step(Event e) { current_ = transition(current_, e); }
