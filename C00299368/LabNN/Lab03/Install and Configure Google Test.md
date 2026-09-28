# Testing C++ with GoogleTest — a handbook for Games Engineering II

This is a reference for the practical side of the module. It covers how to set up your machine, how GoogleTest and GoogleMock work, how the two connect to CMake and VSCode, and how to write the kinds of tests each week's lab asks for. Read it once from start to finish early in the term, then come back to the section you need when a lab or an error has you stuck.


---

## How the labs map to this handbook

Each week adds one idea on top of the last. The framework work starts in Lab 3; Labs 1 and 2 use the plain compiler on purpose, so that you meet the ideas before the tool.

| Week | Lecture | Lab | What you build | New testing idea |
|------|---------|-----|----------------|------------------|
| 1 | Why test | Debugging | fix four planted bugs | read the compiler, fix one thing at a time |
| 2 | Graph and branch coverage | Classifier | test a three-branch function | count branches by hand, no framework |
| 3 | Tooling | Calculator | your first GoogleTest project | `TEST`, `EXPECT_EQ`, CMake with FetchContent |
| 4 | Finite state machines | Machine | a three-state machine | one test per transition |
| 5 | Logic coverage | Predicate | a truth table for `A or B` | parameterised tests, `TEST_P` |
| 6 | Logic coverage examples | Discount | `(A and B) or C` with a fixture | `TEST_F` fixtures and `gcov` coverage |
| 7 | Active clause coverage, MC/DC | Payment gate | CACC pairs | pairs of checks that flip one clause |
| 8 | Input space partitioning | Inventory | Each Choice and Pairwise suites | data-driven `TEST_P`, a faulty build to catch |
| 9 | Design for testability | State and Observer | notification behaviour | GoogleMock: `MOCK_METHOD`, `EXPECT_CALL` |
| 10 | Mutation testing | Mutation | kill generated mutants | `mutate.py`, the mutation score |

---

## Part 1. Setting up your machine

### What you need

Four things: a C++ compiler, CMake, a build tool called Ninja, and VSCode with two extensions. You do **not** need to download GoogleTest yourself. CMake pulls it in for you the first time you build a project, and the rest of this section explains how.

The lab machines use MSYS2 with the UCRT64 toolchain, which gives you GCC (the `g++` compiler) on Windows. The steps below assume that. If you work on macOS or Linux the ideas are identical; only the install commands differ, and there is a short note at the end of this part.

### Installing the toolchain on Windows

MSYS2 is already installed on the lab machines under `C:\msys64`. Open the application called **MSYS2 UCRT64** from the Start menu. This is a terminal. Paste this line and press Enter:

```
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-gdb
```

That installs the compiler (`gcc`), CMake, Ninja, and the debugger (`gdb`) into the UCRT64 environment. Answer yes if it asks.

Now make sure Windows can find these tools from anywhere. Add `C:\msys64\ucrt64\bin` to your `PATH`:

1. Press the Windows key, type "environment variables", open **Edit the system environment variables**.
2. Click **Environment Variables**, find **Path** under your user variables, click **Edit**, then **New**, and add `C:\msys64\ucrt64\bin`.
3. Click OK on everything, then close and reopen VSCode so it picks up the change.

To check it worked, open a normal terminal (or the VSCode terminal) and run `g++ --version`, `cmake --version`, and `ninja --version`. Each should print a version rather than "command not found".

### VSCode extensions

Install two extensions from the Extensions panel (the icon that looks like four squares):

- **C/C++** by Microsoft. This gives you IntelliSense, code navigation, and the debugger integration.
- **CMake Tools** by Microsoft. This drives CMake from inside the editor, so you configure, build, and run tests with a click instead of typing commands.

That is the whole setup. You do it once.

### Why there is no GoogleTest to install

GoogleTest is not a program you install on your machine like an application. It is a small library that becomes part of your project when you build it. CMake has a feature called **FetchContent** that downloads a pinned version of GoogleTest the first time you configure a project and compiles it alongside your code. Every lab's `CMakeLists.txt` already contains the few lines that do this, so there is nothing for you to set up. The only thing to know is that the very first build of a fresh project needs an internet connection and takes about a minute longer while it downloads. After that it is cached inside the project's `build` folder and works offline.

GoogleMock, which you use in Lab 9, ships inside the same GoogleTest download. There is nothing separate to fetch or install for it.

### On macOS or Linux

The tools are the same, only the install differs. On Ubuntu or Debian: `sudo apt install g++ cmake ninja-build gdb`. On macOS with Homebrew: `brew install cmake ninja`, and use the clang compiler that comes with Xcode command line tools (`xcode-select --install`). Everything else in this handbook applies unchanged.

---

## Part 2. How a lab project is put together

### The three pieces

From Lab 3 onward, every lab is a small CMake project with the same shape:

```
Lab3/
├─ CMakeLists.txt      The main CMake Manifest file that handles the entire build for that lab
├─ src/               the code under test (the "system under test", or SUT)
│  ├─ calculator.hpp
│  └─ calculator.cpp
└─ test/              your tests. You will find a starter test harness already written, that could be expanded to cover the lab objectives for the week.
   └─ lab03_tests.cpp
```

`src` holds the thing you are testing. `test` holds the tests. `CMakeLists.txt` tells the build how to turn both into a test program.

### The CMakeLists.txt, line by line

Here is the whole build recipe for Lab 3. It is short, and once you understand these lines you understand every lab's build, because they are all variations on this.

```cmake
cmake_minimum_required(VERSION 3.14)
project(lab03_calculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(calculator STATIC src/calculator.cpp src/calculator.hpp)
target_include_directories(calculator PUBLIC src)

add_executable(lab03_tests test/lab03_tests.cpp)
target_link_libraries(lab03_tests PRIVATE calculator GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(lab03_tests)
```

Reading it top to bottom:

- `cmake_minimum_required` and `project` name the project and say it is C++.
- The two `set` lines fix the language to C++17, which is what the module uses.
- The `FetchContent` block is the part that brings in GoogleTest. It declares where to get it (a pinned version, 1.14.0, so everyone uses the same one), and `FetchContent_MakeAvailable` downloads and builds it. `gtest_force_shared_crt` is a Windows detail that keeps GoogleTest's runtime settings in step with yours; leave it as is.
- `enable_testing()` turns on CMake's test runner, `ctest`.
- `add_library(calculator ...)` compiles the code under test into a small library. `target_include_directories(... PUBLIC src)` means anything that links to it can `#include "calculator.hpp"`.
- `add_executable(lab03_tests ...)` builds your test file into a program.
- `target_link_libraries(... calculator GTest::gtest_main)` links that program against the code under test and against GoogleTest. `GTest::gtest_main` also provides a ready-made `main()`, so you never write one yourself.
- `gtest_discover_tests(lab03_tests)` runs the test program once at build time to find every test inside it and register each one with `ctest`, so they show up individually in VSCode's test panel.

A lab that uses mocks, like Lab 9, changes only the link line. Instead of `GTest::gtest_main` it links `GTest::gtest GTest::gmock GTest::gmock_main`, which adds GoogleMock and its `main()`. Everything else is the same.

### Building and running one lab

You have two ways to do this. Use the editor day to day, and keep the terminal commands for when you want to see exactly what is happening.

**In VSCode.** Open the lab folder with File, Open Folder. The first time, open the Command Palette with `Ctrl+Shift+P`, run **CMake: Select a Kit**, and pick the entry that mentions **ucrt64** GCC. CMake Tools then configures the project (the first time this downloads GoogleTest). Press `F7` to build. Open the **Testing** panel from the flask icon on the left to see every test; press the run arrow to run them all, or run one at a time.

**In the terminal**, from inside the lab folder:

```
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
```

The first line configures the project into a folder called `build`. The second compiles. The third runs the tests and prints the details of any that fail.

### Building all the labs together

The instructor setup puts a single top-level `CMakeLists.txt` in the `GamesENGGLabs` folder that includes every lab you have copied in and fetches GoogleTest once for all of them. If you open that folder instead of a single lab, configure and build once, and every lab's tests appear together. Adding next week's lab is a matter of copying its folder in and configuring again. You do not need this to do a single lab, but it is convenient when you want everything in one place.

---

## Part 3. Your first test, explained

Here is the worked example from Lab 3. It is the smallest complete GoogleTest you will write, and every later test is built from these same parts.

```cpp
#include <gtest/gtest.h>
#include "calculator.hpp"

TEST(Calculator, Add_ReturnsSumForPositive) {
    Calculator c;
    EXPECT_EQ(c.add(2, 3), 5);
}
```

- `#include <gtest/gtest.h>` brings in the framework.
- `TEST(Calculator, Add_ReturnsSumForPositive)` declares one test. The first name, `Calculator`, is the **test suite**, a group of related tests. The second, `Add_ReturnsSumForPositive`, is this **test's** name. Read together they make `Calculator.Add_ReturnsSumForPositive`, which is exactly how the test appears in the VSCode panel and in `ctest`. Name tests after what they check, not after the method, so a failure reads like a sentence.
- Inside, you set up whatever you need (`Calculator c;`) and then make one or more checks.
- `EXPECT_EQ(c.add(2, 3), 5)` checks that `c.add(2, 3)` equals `5`. If it does, the test passes quietly. If it does not, GoogleTest prints the file and line, the value it expected, and the value it got, then marks the test failed.

You never write a `main()`. `GTest::gtest_main` from the CMake file provides one that finds and runs every `TEST` in the program.

### The FAIL() lines in the starters

The starter files have tests whose body is `FAIL() << "not implemented";`. `FAIL()` forces a test to fail on purpose. The starters use it so that an unfinished test fails loudly and shows up red, instead of passing by accident and letting you think you are done. Your job in each lab is to replace those `FAIL()` bodies with real checks. There is also `SUCCEED()`, which does the opposite and is rarely needed.

---

## Part 4. Assertions: how you check things

An assertion is a single check inside a test. GoogleTest gives you two families of them, and the difference matters.

- `EXPECT_*` records a failure and **keeps going**. Use this by default, so one test can report several problems at once.
- `ASSERT_*` records a failure and **stops the test immediately**. Use this only when carrying on makes no sense, for example after checking that a pointer is not null before you dereference it.

The common ones:

| Check | Assertion | Reads as |
|-------|-----------|----------|
| equal | `EXPECT_EQ(a, b)` | a equals b |
| not equal | `EXPECT_NE(a, b)` | a is not b |
| true | `EXPECT_TRUE(x)` | x is true |
| false | `EXPECT_FALSE(x)` | x is false |
| less than | `EXPECT_LT(a, b)` | a < b |
| less or equal | `EXPECT_LE(a, b)` | a <= b |
| greater than | `EXPECT_GT(a, b)` | a > b |
| greater or equal | `EXPECT_GE(a, b)` | a >= b |
| C strings equal | `EXPECT_STREQ(a, b)` | two `char*` are equal |
| floating point | `EXPECT_NEAR(a, b, eps)` | a and b within eps |

For every `EXPECT_` there is a matching `ASSERT_`.

Two things trip people up. First, do not compare floating-point numbers with `EXPECT_EQ`, because rounding makes `0.1 + 0.2` not exactly `0.3`. Use `EXPECT_NEAR` with a small tolerance, or `EXPECT_FLOAT_EQ`. Second, you can attach your own message to any assertion with `<<`, which is worth doing when a failure needs context:

```cpp
EXPECT_EQ(inv.addItem("sword", 1000, 10), AddResult::QTY_TOO_LARGE)
    << "qty 1000 is the boundary and must be rejected";
```

When an assertion on an `enum class` fails, GoogleTest prints the underlying number rather than the name. That is normal. The custom message is how you make the failure readable.

---

## Part 5. The techniques, week by week

This is the part to reread before each lab. Each subsection shows the shape the lab expects, using code close to what you will actually have in front of you.

### Weeks 1 and 2: testing before the framework

The first two labs do not use GoogleTest at all. That is deliberate. In Lab 1 you fix planted bugs with nothing but the compiler, so you get used to reading `g++` output and fixing one thing at a time. In Lab 2 you test a function that returns one of three values by writing a small `main()` that calls it with chosen inputs and prints whether each result matched. You then count how many of the function's branches your inputs actually reached.

The point of these two weeks is the idea of **coverage**: a test is only as good as the parts of the code it makes run. A function with three `return` statements has three branches, and a test that only ever hits one of them has left two untested. You will meet this idea again in the framework, but it is easier to see first with a function you can hold in your head.

### Week 3: your first framework project

Covered in Part 3 above. The lab is small on purpose. The new thing is the toolchain, not the theory: CMake, FetchContent, `TEST`, `EXPECT_EQ`, and the green bar. Get comfortable with configure, build, run before the later labs add ideas on top.

### Week 4: testing a state machine

Lab 4 gives you a machine with three states and a set of events that move between them. In code the states and events are enums, and a transition function says which state you land in:

```cpp
enum class State { OFF, ON, PAUSED };
enum class Event { powerOn, powerOff, pause, resume };

State transition(State s, Event e);
```

There are two things to cover here, and it helps to name them. **State coverage** means every state is visited by some test. **Transition coverage** means every arrow between states is taken by some test, which is the stronger goal. The clean way to get transition coverage is one small test per transition:

```cpp
TEST(Machine, OffPowerOnGoesToOn) {
    EXPECT_EQ(transition(State::OFF, Event::powerOn), State::ON);
}

TEST(Machine, OnPauseGoesToPaused) {
    EXPECT_EQ(transition(State::ON, Event::pause), State::PAUSED);
}
```

One test, one arrow, a name that says which arrow. When a transition test fails you know exactly which move in the machine is wrong.

### Week 5: parameterised tests and truth tables

Lab 5 tests a two-clause predicate, `eligible(a, b)` that returns `a or b`. A predicate with two boolean inputs has four rows in its truth table, and writing four nearly identical tests by hand is repetitive. GoogleTest's **parameterised tests** let you write the test once and feed it a list of rows. This is `TEST_P`.

```cpp
#include <gtest/gtest.h>
#include "eligible.hpp"

struct Row { bool a; bool b; bool expected; };

class EligibleTest : public ::testing::TestWithParam<Row> {};

TEST_P(EligibleTest, MatchesTruthTable) {
    Row r = GetParam();
    EXPECT_EQ(eligible(r.a, r.b), r.expected);
}

INSTANTIATE_TEST_SUITE_P(TruthTable, EligibleTest, ::testing::Values(
    Row{ false, false, false },
    Row{ false, true,  true  },
    Row{ true,  false, true  },
    Row{ true,  true,  true  }
));
```

The moving parts:

- A `struct Row` holds one line of the truth table: the inputs and the expected result.
- The test class inherits `::testing::TestWithParam<Row>`. Inside the test you read the current row with `GetParam()`.
- `TEST_P` (P for parameterised) is the test body, run once per row.
- `INSTANTIATE_TEST_SUITE_P` supplies the rows through `::testing::Values(...)`. The first argument, `TruthTable`, is just a label that appears in the test names.

The lecture idea this serves is the difference between **predicate coverage** (the predicate is seen both true and false) and **clause coverage** (each clause is seen both true and false). Writing the full truth table gives you clause coverage; the lab asks you to say which smaller set of rows would have given only predicate coverage, and why that is weaker.

### Week 6: fixtures and coverage

Lab 6 steps up to a three-clause predicate, `(isMember and spendOverThreshold) or hasCoupon`, and introduces two things.

The first is a **fixture**, written with `TEST_F` (F for fixture). A fixture is a class that holds setup shared by a group of tests, so you do not repeat it. Anything you put in the fixture is fresh for each test.

```cpp
class DiscountTest : public ::testing::Test {
protected:
    Discount d;              // a fresh Discount for every test
    void SetUp() override {  // runs before each test, if you need it
    }
    void TearDown() override { // runs after each test, if you need it
    }
};

TEST_F(DiscountTest, CouponAloneIsEligible) {
    EXPECT_TRUE(d.eligible(false, false, true));
}
```

`SetUp` runs before each test and `TearDown` after, which is where you would open a file or reset shared state in a larger system. For a small predicate the fixture is mostly there so you meet the pattern, because real test suites lean on it heavily.

The second new thing is **measuring** coverage rather than reasoning about it. You rebuild with coverage instrumentation and run `gcov`, which reports which lines and branches ran. The recipe is in Part 9. Paste the summary into your report and check it matches what you expected from your tests.

### Week 7: MC/DC

Lab 7 uses a five-clause gate, something like `(A or B) and C and G and D`. Testing every combination of five inputs would be 32 rows, and most of them tell you nothing. **Modified Condition/Decision Coverage** asks for something sharper: for each clause, show a pair of cases where flipping only that clause flips the whole result, with everything else held still. That pair proves the clause independently affects the outcome.

In GoogleTest a CACC pair is just two checks that differ in one input:

```cpp
TEST(Gate, C_DeterminesOutcome) {
    // hold A,B,G,D so the result tracks C, then flip C
    EXPECT_TRUE (gate(true, false, true,  true, true));
    EXPECT_FALSE(gate(true, false, false, true, true));
}
```

The subtle case is the `(A or B)` part. To show that `A` decides the result you must hold `B` false, because if `B` is already true the `or` is satisfied and `A` no longer changes anything. The lab walks you through finding these pairs. The testing mechanics are easy; the thinking is the lecture.

### Week 8: input space partitioning

Lab 8 tests `Inventory::addItem(name, qty, weightGrams)`, which returns a result code such as `OK`, `NEGATIVE_QTY`, `QTY_TOO_LARGE`, or `TOO_HEAVY`. Rather than test random inputs, you split each input into meaningful blocks (an empty name, a normal name; a negative quantity, a normal one, one over the limit; and so on) and then build test suites that cover those blocks. This is again a job for `TEST_P` with a table of rows, exactly like Week 5 but with richer rows:

```cpp
struct Row { std::string name; int qty; int weight; AddResult expected; std::string label; };

class InventoryTest : public ::testing::TestWithParam<Row> {
protected:
    Inventory inv;
};

TEST_P(InventoryTest, Behaves) {
    Row r = GetParam();
    EXPECT_EQ(inv.addItem(r.name, r.qty, r.weight), r.expected) << r.label;
}

INSTANTIATE_TEST_SUITE_P(EachChoice, InventoryTest, ::testing::Values(
    Row{ "",      10, 500,   AddResult::EMPTY_NAME,   "empty name"   },
    Row{ "sword", -1, 500,   AddResult::NEGATIVE_QTY, "negative qty" },
    Row{ "sword", 500, 500,  AddResult::OK,           "all normal"   }
    // ... one row per block
));
```

The lab compares two strategies. **Each Choice** uses every block at least once. **Pairwise** covers every pair of blocks across two inputs, which needs more rows but catches faults that depend on a combination. It ships a deliberately broken copy of the code where a boundary is wrong, and the payoff is watching a complete pairwise suite catch the bug while a thinner suite misses it. You build the tests against both the correct and the faulty code and compare.

### Week 9: mocks with GoogleMock

Lab 9 is where GoogleMock arrives, and it deserves its own part below. The short version: some behaviour is not a return value you can check with `EXPECT_EQ`. When the machine changes state it is supposed to **notify** its observers, and the thing you want to test is that the right notifications happened, in the right number and order. A mock is a fake observer that records the calls made to it and lets you state, in advance, exactly what you expect. Part 6 covers it in full using the Lab 9 code.

### Week 10: mutation testing

Lab 10 turns the question around. All term you have been asking "do my tests pass?". Mutation testing asks "are my tests any good?". A script, `mutate.py`, makes small changes to the code under test, one at a time, such as turning an `or` into an `and` or a `<` into a `<=`. Each changed version is a **mutant**. Your test suite is run against each mutant. If a mutant makes a test fail, your tests **killed** it, which is good, because they noticed the change. If every test still passes, the mutant **survived**, which means your tests could not tell the difference and are weaker than they look. The **mutation score** is the percentage of mutants killed. The lab asks you to extend your suite until the score is high, and to reason about any survivor that is genuinely equivalent to the original.

---

## Part 6. GoogleMock in depth

This is the one part of the framework that looks unfamiliar at first, so it is worth going slowly. Everything here is grounded in the Lab 9 code.

### The problem it solves

Lab 9 has a `MachineContext` that keeps a current state and a list of observers. When it changes state it calls `onStateChange(from, to)` on each observer. The interface for an observer is:

```cpp
enum class State { OFF, ON, PAUSED };

class IMachineObserver {
public:
    virtual ~IMachineObserver() = default;
    virtual void onStateChange(State from, State to) = 0;
};
```

You want to test that driving the machine produces the right notifications. You could hand-write an observer that stores every call in a vector and then check the vector, but GoogleMock does that bookkeeping for you and gives you a clear way to state expectations.

### Writing a mock

A mock is a class that implements the interface with mock methods. One macro does it:

```cpp
#include <gmock/gmock.h>

class MockObserver : public IMachineObserver {
public:
    MOCK_METHOD(void, onStateChange, (State from, State to), (override));
};
```

`MOCK_METHOD` takes the return type, the method name, the parameter list in its own set of brackets, and any qualifiers such as `(override)`. That single line gives you a working observer whose calls GoogleMock can watch.

### Setting expectations

You state what you expect **before** you exercise the code, then run it. GoogleMock checks the expectations when the mock is destroyed at the end of the test. Here is the worked example from the lab, using a fixture so every test gets a fresh machine and a fresh mock:

```cpp
using ::testing::_;
using ::testing::InSequence;

class MachineTest : public ::testing::Test {
protected:
    MachineContext ctx;
    MockObserver   obs;
    void SetUp() override    { ctx.addObserver(&obs); }
    void TearDown() override { ctx.removeObserver(&obs); }
};

TEST_F(MachineTest, PowerOn_FiresOneNotification) {
    EXPECT_CALL(obs, onStateChange(State::OFF, State::ON)).Times(1);
    ctx.handleEvent(Event::powerOn);
    EXPECT_EQ(ctx.current(), State::ON);
}
```

Read `EXPECT_CALL(obs, onStateChange(State::OFF, State::ON)).Times(1)` as: I expect `onStateChange` to be called on `obs`, with arguments `OFF` and `ON`, exactly once. The call to `ctx.handleEvent(Event::powerOn)` is what should trigger it. If the notification does not happen, or happens with different arguments, or happens twice, the test fails.

### The parts of EXPECT_CALL

An expectation has three kinds of detail you can set.

**Matchers** describe the arguments. You can give exact values, as above, or use a matcher. `::testing::_` means "any value", so `EXPECT_CALL(obs, onStateChange(_, _))` matches any state change. Other matchers include `Eq(x)`, `Ne(x)`, `Gt(x)`, and more, but exact values and `_` cover most of what you need this term.

**Cardinality** says how many times. `.Times(1)` for exactly once, `.Times(0)` for never (useful when a wrong event should be ignored), `.Times(2)` and so on. There are also `AtLeast(n)` and `AtMost(n)`.

**Order.** By default the order of separate expectations does not matter. When it does, wrap them in an `InSequence` block and they must happen in the order written:

```cpp
TEST_F(MachineTest, PowerOnThenPause_FiresTwoInOrder) {
    InSequence seq;
    EXPECT_CALL(obs, onStateChange(State::OFF, State::ON));
    EXPECT_CALL(obs, onStateChange(State::ON,  State::PAUSED));

    ctx.handleEvent(Event::powerOn);
    ctx.handleEvent(Event::pause);
}
```

The `Times(0)` case is worth calling out, because it is how you test that nothing happened. If an event is meant to be ignored in a given state, you expect zero calls and then send the event:

```cpp
TEST_F(MachineTest, ResumeFromOff_FiresZero) {
    EXPECT_CALL(obs, onStateChange(_, _)).Times(0);
    ctx.handleEvent(Event::resume);   // OFF ignores resume
}
```

### Return values and actions

The Lab 9 observer returns `void`, so there is nothing to return. When a mock method does return something, you say what with `.WillOnce(::testing::Return(value))` or `.WillRepeatedly(...)`. You will not need this for the observer, but you will see it in the wider world, so it is worth knowing it exists.

### Why this is a design lesson, not just a testing trick

The reason you can test the notifications at all is that `MachineContext` talks to observers through an interface rather than to one fixed class. That seam is what lets you slot a mock in. The lecture calls this designing for testability, and Lab 9 is the point where you feel why it matters: code written with these seams is code you can pin down with a test, and code without them often is not.

---

## Part 7. Running and filtering tests

### From VSCode

The **Testing** panel (the flask icon) lists every test after a build. The controls at the top run all tests, and each test has its own run and debug buttons. A green tick means pass, a red cross means fail; click a failed test to jump to the assertion that failed. If the panel is empty, build the project first, because tests are registered at build time.

### From the terminal with ctest

`ctest` is CMake's test runner. From inside the `build` folder, or with `--test-dir build` from the project folder:

```
ctest --output-on-failure        run all, show details of failures
ctest -N                         list the tests without running them
ctest -R Machine                 run only tests whose name matches "Machine"
ctest --rerun-failed             run only the tests that failed last time
ctest -j 4                       run tests in parallel using 4 cores
```

### From the test program directly

The compiled test program takes its own flags, which are handy when you want to focus on one test while writing it:

```
./lab09_tests --gtest_list_tests                 print every test's name
./lab09_tests --gtest_filter=MachineTest.*       run only that suite
./lab09_tests --gtest_filter=*PowerOn*           run tests matching a pattern
./lab09_tests --gtest_repeat=100                 run 100 times, to catch flakiness
./lab09_tests --gtest_shuffle                    run in random order
./lab09_tests --gtest_brief=1                    print only failures
```

On Windows the program is `lab09_tests.exe` and lives under the build folder, for example `build\Lab9\lab09_tests.exe` when built through the top-level project, or `build\lab09_tests.exe` when you built that lab on its own.

---

## Part 8. Debugging

### Reading a failure

A GoogleTest failure tells you where and what. A typical one looks like:

```
lab03_tests.cpp:16: Failure
Expected equality of these values:
  c.add(-4, -6)
    Which is: -10
  10
[  FAILED  ] Calculator.Add_ReturnsSumForNegatives
```

It names the file and line, shows the expression you tested and the value it produced, and shows what you compared against. Most of the time the fix is obvious from those three lines. Here the expected value in the test was wrong, not the code.

### Stepping through a test in the debugger

When the failure is not obvious, run the test under the debugger and watch it happen.

1. Open the test file and click in the margin to the left of a line number to set a **breakpoint** (a red dot). Put it on the first line of the failing test.
2. In the **Testing** panel, use the debug icon next to that test, or in the CMake Tools status bar choose the test target and use **Debug** rather than run.
3. Execution stops at your breakpoint. Use Step Over (`F10`) to run a line at a time, Step Into (`F11`) to go inside a function call, and Continue (`F5`) to run to the next breakpoint. Hover over a variable to see its value, or watch it in the panel on the left.

This is worth practising once on a passing test so the mechanics are familiar before you need them on a failing one. The debugger you installed with `pacman` (`gdb`) is what CMake Tools drives underneath.

### Common build and link errors

| Message | What it means | Fix |
|---------|---------------|-----|
| `No CMAKE_C_COMPILER could be found` | no compiler kit selected | select the ucrt64 GCC kit, delete `build`, configure again |
| `undefined reference to 'testing::...'` or to `main` | the test did not link GoogleTest | check the `target_link_libraries` line includes `GTest::gtest_main` |
| `undefined reference to` a GoogleMock symbol | mock code but gmock not linked | link `GTest::gmock GTest::gmock_main` (see Lab 9's CMake) |
| `gtest/gtest.h: No such file or directory` | project not configured, or wrong folder | configure from the project root before building |
| download or timeout during first configure | FetchContent cannot reach the internet | connect once; after the first build it is cached in `build/_deps` |
| tests build but none appear in the panel | not built yet, or discovery did not run | build first, then refresh the Testing panel |

### Common mistakes when writing tests

- Comparing floating-point values with `EXPECT_EQ`. Use `EXPECT_NEAR`.
- Forgetting the `INSTANTIATE_TEST_SUITE_P` line for a `TEST_P`. Without it the parameterised test never runs.
- Setting a GoogleMock expectation **after** calling the code. Expectations must be set before the code that should satisfy them.
- Putting `ASSERT_*` inside a helper function and expecting it to stop the test. `ASSERT_` only returns from the function it is in, so in a helper it will not end the test as you might expect. Prefer `EXPECT_` in helpers.
- Tests that depend on the order they run in, or on state left by a previous test. Each test should stand alone; a fixture with fresh members is the usual way to guarantee that.

---

## Part 9. Measuring coverage with gcov (Lab 6)

Coverage instrumentation is a separate build. Configure a second build folder with the coverage flags, run the tests to produce coverage data, then read it with `gcov`:

```
cmake -S . -B build-cov -G Ninja -DCMAKE_CXX_FLAGS="--coverage" -DCMAKE_EXE_LINKER_FLAGS="--coverage"
cmake --build build-cov
ctest --test-dir build-cov
gcov -r build-cov/CMakeFiles/discount.dir/src/discount.cpp.gcno
```

The last command prints, per source file, the percentage of lines executed and can show which lines never ran. Line coverage tells you a line was reached; it does not tell you every branch outcome was tested, which is why branch coverage, and the logic-coverage work from Weeks 5 to 7, is the sharper measure. Paste the summary into your Lab 6 report.

---

## Quick reference

**A minimal test**

```cpp
#include <gtest/gtest.h>
#include "thing.hpp"

TEST(SuiteName, DoesTheExpectedThing) {
    EXPECT_EQ(function(input), expected);
}
```

**Assertions**: `EXPECT_EQ NE TRUE FALSE LT LE GT GE`, `EXPECT_NEAR(a,b,eps)` for floats, `EXPECT_STREQ` for C strings. Swap `EXPECT` for `ASSERT` to stop the test on failure. Add context with `<< "message"`.

**Parameterised** (`TEST_P`): a `struct` for a row, a class extending `TestWithParam<Row>`, a `TEST_P` body using `GetParam()`, and `INSTANTIATE_TEST_SUITE_P(Label, Class, ::testing::Values(...))`.

**Fixture** (`TEST_F`): a class extending `::testing::Test` with members and optional `SetUp`/`TearDown`; tests use `TEST_F(FixtureName, TestName)`.

**Mock**: `MOCK_METHOD(ReturnType, name, (params), (override));` then `EXPECT_CALL(mock, name(args)).Times(n);` with `_` for any argument and an `InSequence` block for ordering.

**Build and run**

```
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
```

**CMake link lines**: plain tests link `GTest::gtest_main`; tests using mocks link `GTest::gtest GTest::gmock GTest::gmock_main`.

---

## Documentation and Resource Materials

The official GoogleTest documentation is thorough and searchable, that should cover most of your queries. 


