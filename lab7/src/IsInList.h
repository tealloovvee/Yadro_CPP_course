#ifndef YADRO_ISINLIST_H
#define YADRO_ISINLIST_H

#include <algorithm>
#include <array>

namespace ISINLIST {

/// Array of intersting words
constexpr std::array<const char*, 5> interesting = {"init", "open", "read", "write", "close"};

/// Compare two strings
/// @return true, if are equal
constexpr bool IsEqual(const char* a, const char* b) {
    while (*a && *b && *a == *b) {
        ++a;
        ++b;
    }
    return *a == '\0' && *b == '\0';
}
} // namespace ISINLIST

/// Checks if a word is in an array
/// @return true, if word is in an array
constexpr bool IsInList(const char* name) {
    return std::any_of(ISINLIST::interesting.cbegin(), ISINLIST::interesting.cend(),
                       [name](const char* interestingName) { return ISINLIST::IsEqual(interestingName, name); });
}

#endif // YADRO_ISINLIST_H