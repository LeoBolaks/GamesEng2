# Lab 05 — Report

**Name:** Leo Bolaks

**StudentID:** C00299368


## Coverage summary

State coverage:  3 / 3
Transition coverage:  5 / 5

## Report (250-500 words max)

    1. Explain the difference between state coverage and transition coverage in your own words. Could a test suite achieve 100 % state coverage without achieving 100 % transition coverage? 

    State coverage simply just tests for if each state actually functions and works, transition coverage is actually seeing if an event triggers the state to transition to a different state correctly. Yes a test suite could achieve 100% state coverage without achieving 100% transition coverage because some states can be can be triggered by multiple transition events and not every one of them might be tested for each state case, for example, if there are states like jump, fall, and move, you can go into the fall state after jumping, because then you'd start falling, but you can also go into the fall state from move if you walk off an edge, the fall state can be checked by jumping, but only 1 of the 2 transitions were checked to go into that state.

    2. Give one example of a bug in `transition()` that would slip past state coverage but be caught by transition coverage.

    A bug that could slip past is if you have an incorrect transition event triggering a wrong state, for example if in state ON, you have the event "pause" trigger the OFF state. State coverage wouldn't detect that because all it does it check if the state is working correctly, however in transition coverage you'd see that an incorrect event is triggering state OFF.

## Screenshots
To be provided as:
* `screenshots/vscode.png`
* `screenshots/terminal.png`
