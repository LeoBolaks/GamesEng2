#pragma once

enum class State { OFF, ON, PAUSED };
enum class Event { powerOn, powerOff, pause, resume };

class Machine {
public:
    Machine();
    State current() const;
    State transition(State s, Event e) const;
    void  step(Event e);
private:
    State current_;
};
