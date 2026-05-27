#ifndef YADRO_LITERALS_H
#define YADRO_LITERALS_H

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <string_view>

namespace LITERALS {
constexpr size_t MAX_STRING_SIZE = 256; ///< Buffer size

/// Compile-time string
class alignas(128) String {
public:
    /// Default constructor
    constexpr String() : data_() {}

    /// Comparison operator
    /// @param[in] other other string object
    /// @return true if strings are equal
    constexpr bool operator==(std::string_view other) const {
        if (len_ != other.size() + 1) {
            return false;
        }
        for (size_t i = 0; i < len_ - 1; ++i) {
            if (data_[i] != other[i]) {
                return false;
            }
        }
        return true;
    }

    /// Provides access to character at specified position
    /// @param[in] i index
    /// @return reference to character at given index
    constexpr char& operator[](size_t i) { return data_[i]; }

    /// return C-style string
    /// @return pointer to internal buffer
    [[nodiscard]] constexpr const char* c_str() const { return data_.data(); }

    /// Returns reference to internal array
    /// @return reference to internal array
    constexpr std::array<char, MAX_STRING_SIZE>& GetData() { return data_; }

    /// Returns string size including 0-terminator
    /// @return size of string
    constexpr size_t& GetLen() { return len_; }

private:
    std::array<char, MAX_STRING_SIZE> data_; ///< buffer for string
    size_t len_{0};                          ///< string size including 0-terminator
};
} // namespace LITERALS

/// Converter string to upper case
consteval LITERALS::String operator""_to_upper(const char* str, size_t len) {
    assert(len < LITERALS::MAX_STRING_SIZE);
    LITERALS::String res;
    std::transform(str, str + len, res.GetData().begin(), [](char c) {
        { return 'a' <= c && c <= 'z' ? c - 'a' + 'A' : c; }
    });
    res[len] = '\0';
    res.GetLen() = len + 1;
    return res;
}
#endif // YADRO_LITERALS_H