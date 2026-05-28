#ifndef YADRO_TUPLE_H
#define YADRO_TUPLE_H

#include <cstddef>

namespace TUPLE_IMPL {

/// structure for storing an element in a tuple
/// @tparam Index position in the tuple
/// @tparam Element type of the stored element
template <size_t Index, typename Element> class TupleElement {
public:
    /// Default constructor
    constexpr TupleElement() = default;

    /// Constructor
    /// @param[in] value value for storage
    constexpr TupleElement(const Element& value) : value_(value) {}

    Element value_; ///< stored value
};

/// Recursive implementation of a tuple
/// @tparam Index current position in the tuple
/// @tparam Head type of the first element in the remaining list
/// @tparam Tail types of the remaining elements
template <size_t Index, typename Head, typename... Tail>
class TupleImpl : public TupleElement<Index, Head>, public TupleImpl<Index + 1, Tail...> {
public:
    /// Default Constructor
    constexpr TupleImpl() = default;

    /// Constructor.
    /// @param[in] h value for the current head element
    /// @param[in] t values for the remaining tail elements
    constexpr TupleImpl(const Head& h, const Tail&... t)
        : TupleElement<Index, Head>(h), TupleImpl<Index + 1, Tail...>(t...) {}

    /// Returns a mutable reference to the current head element
    /// @return reference to the stored value at current index
    constexpr Head& GetHead() { return static_cast<TupleElement<Index, Head>&>(*this).value_; }

    /// Returns a const reference to the current head element
    /// @return const reference to the stored value at current index
    constexpr const Head& GetHead() const { return static_cast<const TupleElement<Index, Head>&>(*this).value_; }

    /// Returns a mutable reference to the tail of the tuple
    /// @return reference to the remaining tuple elements
    constexpr TupleImpl<Index + 1, Tail...>& GetTail() { return static_cast<TupleImpl<Index + 1, Tail...>&>(*this); }

    /// Returns a const reference to the tail of the tuple
    /// @return const reference to the remaining tuple elements
    constexpr const TupleImpl<Index + 1, Tail...>& GetTail() const {
        return static_cast<const TupleImpl<Index + 1, Tail...>&>(*this);
    }
};

/// Base case specialization for a single-element tuple
/// @tparam Index position of the element in the tuple
/// @tparam Head type of the single element
template <size_t Index, typename Head> class TupleImpl<Index, Head> : public TupleElement<Index, Head> {
public:
    /// Default constructor
    constexpr TupleImpl() : TupleElement<Index, Head>(){};

    /// constructor
    /// @param[in] head value for the element
    constexpr TupleImpl(const Head& head) : TupleElement<Index, Head>(head) {}

    /// Returns a mutable reference to the element
    /// @return reference to the stored value
    constexpr Head& GetHead() { return this->value_; }

    /// Returns a const reference to the element
    /// @return const reference to the stored value
    constexpr const Head& GetHead() const { return this->value_; }
};

/// Helper template for index-based element access
/// @tparam Index target index to retrieve
/// @tparam CurrentIndex current index being examined in recursion
/// @tparam Head type of the first element in the remaining list
/// @tparam Tail types of the remaining elements
template <size_t Index, size_t CurrentIndex, typename Head, typename... Tail> struct GetHelper {
    /// Retrieves a mutable reference to the element at specified index
    /// @param tupleImpl reference to the tuple implementation
    /// @return mutable reference to the element
    static constexpr auto& Get(TupleImpl<CurrentIndex, Head, Tail...>& tupleImpl) {
        if constexpr (Index == CurrentIndex) {
            return tupleImpl.GetHead();
        } else {
            return GetHelper<Index, CurrentIndex + 1, Tail...>::Get(tupleImpl.GetTail());
        }
    }

    /// Retrieves a const reference to the element at specified index
    /// @param tupleImpl reference to the tuple implementation
    /// @return const reference to the element
    static constexpr const auto& Get(const TupleImpl<CurrentIndex, Head, Tail...>& tupleImpl) {
        if constexpr (Index == CurrentIndex) {
            return tupleImpl.GetHead();
        } else {
            return GetHelper<Index, CurrentIndex + 1, Tail...>::Get(tupleImpl.GetTail());
        }
    }
};

} // namespace TUPLE_IMPL

/// compile-time tuple container
/// @tparam Elements types of elements stored in the tuplpe
template <typename... Elements> class Tuple : public TUPLE_IMPL::TupleImpl<0, Elements...> {
public:
    /// Default constrcutor
    constexpr Tuple() : TUPLE_IMPL::TupleImpl<0, Elements...>() {}

    /// Constructor
    /// @param[in] args values for all elements
    constexpr Tuple(const Elements&... args) : TUPLE_IMPL::TupleImpl<0, Elements...>(args...){};
};

/// Deduction quide for Tuple
/// @tparam Types types of elements stored in the tuplpe
template <typename... Types> Tuple(Types...) -> Tuple<Types...>;

/// Get mutable reference to the element in the tuple
/// @tparam Index index of the element
/// @tparam Elements types of elements stored in the tuplpe
/// @param[in] tuple Reference to the tuple
/// @return mutable reference to the element
template <size_t Index, typename... Elements> constexpr auto& Get(Tuple<Elements...>& tuple) {
    return TUPLE_IMPL::GetHelper<Index, 0, Elements...>::Get(tuple);
}

/// Get const reference to the element in the tuple
/// @tparam Index index of the element
/// @tparam Elements types of elements stored in the tuplpe
/// @param[in] tuple Reference to the tuple
/// @return const reference to the element
template <size_t Index, typename... Elements> constexpr const auto& Get(const Tuple<Elements...>& tuple) {
    return TUPLE_IMPL::GetHelper<Index, 0, Elements...>::Get(tuple);
}

#endif // YADRO_TUPLE_H