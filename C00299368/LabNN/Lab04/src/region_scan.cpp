#include "region_scan.hpp"

int adjust(int raw) {
    if (raw < 0) {
        return 0;
    }
    if (raw > 100) {
        return 100;
    }
    return raw;
}

int scanRegion(const Grid& grid) {
    int sum = 0;

    for (int r = 0; r < static_cast<int>(grid.size()); ++r) {
        for (int c = 0; c < static_cast<int>(grid[r].size()); ++c) {
            sum += grid[r][c];
        }
    }

    int score;
    if (sum > 10) {
        score = sum;
    } else {
        score = -sum;
    }

    switch (sum % 3) {
        case 0:
            score = adjust(score);
            break;
        default:
            break;
    }

    return score;
}
