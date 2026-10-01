#pragma once

// It is essentially a very basic implementation of grid formation. 
// We walk along a path in a grid like pattern, marking and mapping rows. 
// Each vector then returns and maps the score, thus clamping its position on the grid. 

#include <vector>

// A rectangular region of integer cells. Rows may differ in length.
using Grid = std::vector<std::vector<int>>;

// Clamp a raw score into the reportable band [0, 100].
// Called from the switch inside scanRegion.
int adjust(int raw);

// Walk every cell of the region, accumulate a running total, then map that
// total onto a reported score.
int scanRegion(const Grid& grid);
