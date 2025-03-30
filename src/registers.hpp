#pragma once

#include <string>
#include <unordered_map>
#include <stdexcept>

namespace Register
{
    enum Name
    {
        X0,
        X1,
        X2,
        X3,
        X4,
        X5,
        X6,
        X7,
        X8,
        X9,
        X10,
        X11,
        X12,
        X13,
        X14,
        X15,
        X16,
        X17,
        X18,
        X19,
        X20,
        X21,
        X22,
        X23,
        X24,
        X25,
        X26,
        X27,
        X28,
        X29,
        X30,
        XZR,
        NONE
    };

    inline Name from_string(std::string str)
    {
        // Convert to uppercase in-place
        for (char &c : str)
            c = std::toupper(c);

        static const std::unordered_map<std::string, Name> lookup = {
            {"X0", Name::X0},
            {"X1", Name::X1},
            {"X2", Name::X2},
            {"X3", Name::X3},
            {"X4", Name::X4},
            {"X5", Name::X5},
            {"X6", Name::X6},
            {"X7", Name::X7},
            {"X8", Name::X8},
            {"X9", Name::X9},
            {"X10", Name::X10},
            {"X11", Name::X11},
            {"X12", Name::X12},
            {"X13", Name::X13},
            {"X14", Name::X14},
            {"X15", Name::X15},
            {"X16", Name::X16},
            {"IP0", Name::X16},
            {"X17", Name::X17},
            {"IP1", Name::X17},
            {"X18", Name::X18},
            {"X19", Name::X19},
            {"X20", Name::X20},
            {"X21", Name::X21},
            {"X22", Name::X22},
            {"X23", Name::X23},
            {"X24", Name::X24},
            {"X25", Name::X25},
            {"X26", Name::X26},
            {"X27", Name::X27},
            {"X28", Name::X28},
            {"SP", Name::X28},
            {"X29", Name::X29},
            {"FP", Name::X29},
            {"X30", Name::X30},
            {"LR", Name::X30},
            {"XZR", Name::XZR}};

        auto it = lookup.find(str);
        if (it != lookup.end())
            return it->second;

        return NONE;
    }

    inline std::string to_string(Name reg)
    {
        static const char *names[] = {
            "X0", "X1", "X2", "X3", "X4", "X5", "X6", "X7",
            "X8", "X9", "X10", "X11", "X12", "X13", "X14", "X15",
            "X16", "X17", "X18", "X19", "X20", "X21", "X22", "X23",
            "X24", "X25", "X26", "X27", "X28", "X29", "X30", "XZR", "NONE"};

        int index = static_cast<int>(reg);
        if (index < 0 || index >= static_cast<int>(NONE) + 1)
            return "UNKNOWN";

        return names[index];
    }

} // namespace Register
