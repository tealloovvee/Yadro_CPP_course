#include "bitset.h"

#include <algorithm>
#include <bit>
#include <cassert>
#include <new>

Bitset::Bitset(size_t maxValue) : size_(((maxValue) >> 3) + 1), nOnes(0) {
    data_ = static_cast<uint8_t*>(calloc(size_, 1));
    if (data_ == nullptr) {
        throw std::bad_alloc();
    }
}

Bitset::Bitset(const Bitset& other) : size_(other.size_), nOnes(other.nOnes) {
    data_ = static_cast<uint8_t*>(malloc(size_));
    if (data_ == nullptr) {
        throw std::bad_alloc();
    }
    memcpy(data_, other.data_, size_);
}

Bitset& Bitset::operator=(const Bitset& other) {
    if (this == &other) {
        return *this;
    }
    auto* const newData = static_cast<uint8_t*>(malloc(other.size_));
    if (newData == nullptr) {
        throw std::bad_alloc();
    }
    size_ = other.size_;
    free(data_);
    data_ = newData;
    memcpy(data_, other.data_, size_);
    nOnes = other.nOnes;
    return *this;
}

Bitset::Bitset(Bitset&& other) noexcept : data_(other.data_), size_(other.size_), nOnes(other.nOnes) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.nOnes = 0;
}

Bitset& Bitset::operator=(Bitset&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    free(data_);
    data_ = other.data_;
    other.data_ = nullptr;
    size_ = other.size_;
    other.size_ = 0;
    nOnes = other.nOnes;
    other.nOnes = 0;
    return *this;
}

void Bitset::set(size_t index, bool value) {
    const size_t offset = index >> 3;
    if (offset > size_) {
        ensure_capacity(2 * offset);
    }
    const uint8_t mask = 1 << (index & 0b111);
    const bool currentValue = static_cast<bool>(data_[offset] & mask);
    if (value == currentValue) {
        return;
    }
    if (value) {
        data_[offset] |= mask;
        ++nOnes;
    } else {
        data_[offset] &= ~mask;
        --nOnes;
    }
}

bool Bitset::operator[](size_t index) const {
    if (index >= (size_ << 3)) {
        return false;
    }
    return static_cast<bool>((data_[index >> 3] >> (index & 0b111)) & 1);
}

Bitset::Bitset(size_t size, bool dummy) : size_(size), nOnes(0) {
    data_ = static_cast<uint8_t*>(malloc(size_));
    if (data_ == nullptr) {
        throw std::bad_alloc();
    }
}

Bitset Bitset::union_with(const Bitset& other) const {
    Bitset result(std::max(size_, other.size_), true);
    const size_t smallestSize = std::min(size_, other.size_);
    for (size_t i = 0; i < smallestSize; ++i) {
        result.data_[i] = data_[i] | other.data_[i];
        result.nOnes += std::popcount(result.data_[i]);
    }
    if (smallestSize == size_) {
        memcpy(result.data_ + smallestSize, other.data_ + smallestSize, other.size_ - smallestSize);
        for (size_t i = smallestSize; i < other.size_; ++i) {
            result.nOnes += std::popcount(result.data_[i]);
        }
    } else {
        memcpy(result.data_ + smallestSize, data_ + smallestSize, size_ - smallestSize);
        for (size_t i = smallestSize; i < size_; ++i) {
            result.nOnes += std::popcount(result.data_[i]);
        }
    }
    return result;
}

Bitset Bitset::intersection_with(const Bitset& other) const {
    Bitset result(std::min(size_, other.size_), true);
    const size_t smallestSize = std::min(size_, other.size_);
    for (size_t i = 0; i < smallestSize; ++i) {
        result.data_[i] = data_[i] & other.data_[i];
        result.nOnes += std::popcount(result.data_[i]);
    }
    return result;
}

bool Bitset::is_subset(const Bitset& other) const {
    const size_t smallestSize = std::min(size_, other.size_);
    for (size_t i = 0; i < smallestSize; ++i) {
        if (static_cast<uint8_t>((~data_[i] | other.data_[i])) != 0xFF) {
            return false;
        }
    }
    for (size_t i = smallestSize; i < size_; ++i) {
        if (data_[i]) {
            return false;
        }
    }
    return true;
}

void Bitset::resize(size_t maxValue) {
    if (maxValue >= (size_ << 3)) {
        ensure_capacity((maxValue >> 3) + 1);
    } else if (maxValue + 1 < (size_ << 3)) {
        shrink(maxValue);
    }
}

void Bitset::ensure_capacity(size_t newSize) {
    assert(newSize > size_);
    auto* const newData = static_cast<uint8_t*>(malloc(newSize));
    if (newData == nullptr) {
        throw std::bad_alloc();
    }
    memcpy(newData, data_, size_);
    memset(newData + size_, 0, newSize - size_);
    free(data_);
    data_ = newData;
    size_ = newSize;
}

void Bitset::shrink(size_t maxValue) {
    const size_t newSize = (maxValue >> 3) + 1;
    assert(newSize <= size_);
    uint8_t* newData;
    if (newSize < size_) {
        newData = static_cast<uint8_t*>(malloc(newSize));
        if (newData == nullptr) {
            throw std::bad_alloc();
        }
    }
    for (size_t i = newSize; i < size_; ++i) {
        nOnes -= std::popcount(data_[i]);
    }
    const uint8_t mask = (1 << ((maxValue + 1) & 0b111)) - 1;
    nOnes -= std::popcount(static_cast<uint8_t>(~mask & data_[newSize - 1]));
    if (newSize < size_) {
        memcpy(newData, data_, newSize);
        free(data_);
        data_ = newData;
        size_ = newSize;
    }
    data_[newSize - 1] &= mask;
}