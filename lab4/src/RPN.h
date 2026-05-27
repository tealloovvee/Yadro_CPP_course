#ifndef YADRO_RPN_H
#define YADRO_RPN_H

#include <array>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

namespace RPN {
constexpr size_t STACK_SIZE = 256; ///< Maximum stack capacity

/// Converts a fixed-length character sequence to an integer
/// @param[in] data pointer to the start of the number string
/// @param[in] size number of characters to parse
/// @return parsed integer value
constexpr int64_t ParseInt(const char* data, size_t size) {
    int64_t result = 0;
    for (size_t i = 0; i < size; ++i) {
        const char c = data[i];
#ifndef NDEBUG
        if (c < '0' || '9' < c) {
            throw std::logic_error("the string is not a number");
        }
#endif
        result = result * 10 + (c - '0');
    }
    return result;
}

/// Computes integer exponentiation
/// @param[in] base base number
/// @param[in] exp exponent
/// @return base raised to the power of exp
constexpr int64_t PowInt(int64_t base, int64_t exp) {
    int64_t result = 1;
    for (int64_t i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

/// Performs arithmetic operation on two operands
/// @param[in] a left operand
/// @param[in] b right operand
/// @param[in] op operator
/// @return result of a op b
constexpr int64_t ApplyOp(int64_t a, int64_t b, char op) {
    switch (op) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
        case '%':
            return a % b;
        case '^':
            return PowInt(a, b);
        default:
            throw std::logic_error("Unknown operator");
    }
}

/// Compile-time stack wrapper using a fixed-size array
class Stack {
public:
    /// Default constructor
    constexpr Stack() : data_(){};

    /// Returns the top element
    /// @return top element
    [[nodiscard]] constexpr int64_t Top() const { return data_[end_ - 1]; }

    /// Removes the top element
    constexpr void Pop() {
        assert(end_ > 0);
        --end_;
    }

    /// Adds an element to the top of the stack
    /// @param[in] val element to insert
    constexpr void Push(int64_t val) {
        assert(end_ < STACK_SIZE);
        data_[end_] = val;
        ++end_;
    }

    /// Returns the current number of elements in the stack
    /// @return stack size
    [[nodiscard]] constexpr size_t Size() const { return end_; }

private:
    int end_{0};                           ///< Index of the next free position
    std::array<int64_t, STACK_SIZE> data_; ///< storage
};

} // namespace RPN

/// Reverse polish notation evaluation
constexpr int64_t operator""_rpn(const char* str, size_t len) {
    RPN::Stack stack;
    size_t start = 0;
    for (size_t i = 0; i <= len; ++i) {
        if (str[i] == ' ' || i == len) {
            if (i > start) {
                const size_t token_len = i - start;
                const char op = str[start];
                if (token_len == 1 && (op == '+' || op == '-' || op == '*' || op == '/' || op == '%' || op == '^')) {
                    assert(stack.Size() >= 2);
                    const int64_t b = stack.Top();
                    stack.Pop();
                    const int64_t a = stack.Top();
                    stack.Pop();
                    stack.Push(RPN::ApplyOp(a, b, op));
                } else {
                    stack.Push(RPN::ParseInt(str + start, token_len));
                }
            }
            start = i + 1;
        }
    }
    assert(stack.Size() == 1);
    return stack.Top();
}

#endif // YADRO_RPN_H