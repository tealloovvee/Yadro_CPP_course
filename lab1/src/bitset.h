#ifndef YADRO_BITSET_H
#define YADRO_BITSET_H

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

class Bitset {
public:
    /// Default constructor
    Bitset() : data_(nullptr), size_(0), nOnes(0) {}

    /// Constructor
    /// @param[in] maxValue the maximum value that ​​needs to be stored
    explicit Bitset(size_t maxValue);

    /// Copy constructor
    /// @param[in] other bitset object to copy
    Bitset(const Bitset& other);

    /// Copy assignment operator
    /// @param[in] other bitset object to copy
    Bitset& operator=(const Bitset& other);

    /// Move constructor
    /// @param[in,out] other bitset object to move
    Bitset(Bitset&& other) noexcept;

    /// Move assignment operator
    /// @param[in,out] other bitset object to move
    Bitset& operator=(Bitset&& other) noexcept;

    /// Destructor
    ~Bitset() { free(data_); }

    /// Set flag by index
    /// @param[in] index flag index
    /// @param[in] value the value set to the flag to
    void set(size_t index, bool value);

    /// Get flag state by index, or false if index is out of range
    /// @param[in] index flag index
    /// @return value of the flag
    bool operator[](size_t index) const;

    /// Get flag state by index, or false if index is out of range
    /// @param[in] index flag index
    /// @return value of the flag
    [[nodiscard]] bool test(size_t index) const { return (*this)[index]; }

    /// Get the current capacity
    /// @return number of elements in the bitset
    [[nodiscard]] size_t size() const { return size_ << 3; }

    /// Check if there are no non-zero values in the array
    /// @return true if all flags set to 0
    [[nodiscard]] bool empty() const { return nOnes == 0; }

    /// Get number of non-zero flags
    /// @return number of non-sero flags
    [[nodiscard]] size_t sum() const { return nOnes; }

    /// Reset all flags to 0
    void reset() {
        memset(data_, 0, size_);
        nOnes = 0;
    };

    /// Delete all flags and free memory
    void clear() {
        free(data_);
        data_ = nullptr;
        size_ = 0;
    }

    /// Get the union of 2 sets
    /// @param[in] other the second set to union with
    /// @return union of the two sets
    [[nodiscard]] Bitset union_with(const Bitset& other) const;

    /// Get the intersection of 2 sets
    /// @param[in] other the second set to intersection with
    /// @return intersection of the two sets
    [[nodiscard]] Bitset intersection_with(const Bitset& other) const;

    /// Checks if this set is a subset of another set. (i.e., does this \in other)
    /// @param[in] other the potential superset
    /// @return true if this set is a subset of other
    ///
    [[nodiscard]] bool is_subset(const Bitset& other) const;

    /// Set new size of the bitset
    /// @param[in] maxValue new maximum index in the bitset
    void resize(size_t maxValue);

private:
    uint8_t* data_; ///< pointer to memory
    size_t size_;   ///< size of memory in bytes
    size_t nOnes;   ///< number of enabled bits

    /// Constructor. Allocates memory without filling array with zeros
    /// @param[in] size size of allocated memory in bytes
    /// @param[in] dummy dummy parameter
    Bitset(size_t size, bool dummy);

    /// Ensure memory capacity
    /// @param[in] newSize new size of allocated memory in bytes
    void ensure_capacity(size_t newSize);

    /// Shrink with memory reallocation
    /// @param[in] maxValue the maximum value that needs to be stored
    void shrink(size_t maxValue);
};

#endif