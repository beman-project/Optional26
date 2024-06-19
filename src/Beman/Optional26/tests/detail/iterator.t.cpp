/**
 * This file contains tests for beman::optional::detail::normal_iterator.
 */
#include <Beman/Optional26/detail/iterator.hpp>

#include <gtest/gtest.h>
#include <tests/test_types.hpp>

#include <concepts>
#include <type_traits>
#include <set>
#include <vector>

namespace beman::optional::test {

// Dummy containers helper.
struct container {};

} // namespace beman::optional::test

using namespace beman::optional::test;

TEST(NormalIteratorTest, IteratorConcepts) {
    const auto test = [](auto&& it) {
        // The iterator type of it.
        using iterator = typename std::remove_reference_t<decltype(it)>;

        static_assert(std::input_iterator<iterator>);
        static_assert(std::forward_iterator<iterator>);
        static_assert(std::bidirectional_iterator<iterator>);
        static_assert(std::random_access_iterator<iterator>);
        static_assert(std::contiguous_iterator<iterator>);
    };

    // non-const iterators
    test(beman::optional::detail::normal_iterator<int*, container>{});
    test(beman::optional::detail::normal_iterator<empty*, container>{});
    test(beman::optional::detail::normal_iterator<no_default_ctor*, container>{});
    test(beman::optional::detail::normal_iterator<base*, container>{});
    test(beman::optional::detail::normal_iterator<derived*, container>{});

    // const iterators
    test(beman::optional::detail::normal_iterator<const int*, container>{});
    test(beman::optional::detail::normal_iterator<const empty*, container>{});
    test(beman::optional::detail::normal_iterator<const no_default_ctor*, container>{});
    test(beman::optional::detail::normal_iterator<const base*, container>{});
    test(beman::optional::detail::normal_iterator<const derived*, container>{});
}

TEST(NormalIterator, DereferenceOperator) {
    std::vector<int> v{10, 20, 30, 40, 50};
    auto             it = beman::optional::detail::normal_iterator<int*, decltype(v)>{v.data()};

    EXPECT_EQ(*it, 10);
    *it = 100;
    EXPECT_EQ(*it, 100);

    it += 2;
    EXPECT_EQ(*it, 30);

    *it = 300;
    EXPECT_EQ(*it, 300);
}

TEST(NormalIteratorTest, ForwardIterator) {
    std::vector<int>       v{10, 20, 30, 40, 50};
    const std::vector<int> cv{10, 20, 30, 40, 50};

    const auto test = [](auto&& it) {
        EXPECT_EQ(*it, 10);

        ++it; // prefixed increment
        EXPECT_EQ(*it, 20);

        it++; // postfixed increment
        EXPECT_EQ(*it, 30);

        it++;
        EXPECT_EQ(*it, 40);

        ++it;
        EXPECT_EQ(*it, 50);
    };

    test(beman::optional::detail::normal_iterator<int*, decltype(v)>{v.data()});
    test(beman::optional::detail::normal_iterator<int*, decltype(v)>{v.data()});

    test(beman::optional::detail::normal_iterator<const int*, decltype(cv)>{cv.data()});
    test(beman::optional::detail::normal_iterator<const int*, decltype(cv)>{cv.data()});
}

TEST(NormalIteratorTest, BidirectionalIterator) {
    std::vector<int>       v{10, 20, 30, 40, 50};
    const std::vector<int> cv{10, 20, 30, 40, 50};
    const auto             test = [](auto&& it) {
        it++;
        it++;
        EXPECT_EQ(*it, 30);

        --it; // prefixed decrement
        EXPECT_EQ(*it, 20);

        it--; // postfixed decrement
        EXPECT_EQ(*it, 10);
    };

    test(beman::optional::detail::normal_iterator<int*, decltype(v)>{v.data()});
    test(beman::optional::detail::normal_iterator<int*, decltype(v)>{v.data()});

    test(beman::optional::detail::normal_iterator<const int*, decltype(cv)>{cv.data()});
    test(beman::optional::detail::normal_iterator<const int*, decltype(cv)>{cv.data()});
}

TEST(NormalIteratorTest, RandomAccessIterator) {
    std::vector<int>       v{10, 20, 30, 40, 50};
    const std::vector<int> cv{10, 20, 30, 40, 50};
    const auto             test = [](auto&& it) {
        EXPECT_EQ(it[0], 10);
        EXPECT_EQ(it[1], 20);
        EXPECT_EQ(it[2], 30);
        EXPECT_EQ(it[3], 40);
        EXPECT_EQ(it[4], 50);

        it += 2;
        EXPECT_EQ(*it, 30);

        it -= 1;
        EXPECT_EQ(*it, 20);

        it = it + 2;
        EXPECT_EQ(*it, 40);

        it = it - 1;
        EXPECT_EQ(*it, 30);
    };

    test(beman::optional::detail::normal_iterator<int*, decltype(v)>{v.data()});
    test(beman::optional::detail::normal_iterator<int*, decltype(v)>{v.data()});

    test(beman::optional::detail::normal_iterator<const int*, decltype(cv)>{cv.data()});
    test(beman::optional::detail::normal_iterator<const int*, decltype(cv)>{cv.data()});
}

TEST(NormalIteratorTest, ContainerType) {
    // Test for equality.
    constexpr const auto test_eq = [](auto&& container1, auto&& container2) {
        // The container types.
        using containter_type1 = typename std::remove_reference_t<decltype(container1)>;
        using containter_type2 = typename std::remove_reference_t<decltype(container2)>;

        auto it1 = beman::optional::detail::normal_iterator<int*, containter_type1>{};
        auto it2 = beman::optional::detail::normal_iterator<int*, containter_type2>{};

        static_assert(std::is_same_v<decltype(it1), decltype(it2)>);
    };

    test_eq(container{}, container{});
    test_eq(std::vector<int>{}, std::vector<int>{});
    test_eq(std::set<int>{}, std::set<int>{});

    // Test for inequality.
    constexpr const auto test_neq = [](auto&& container1, auto&& container2) {
        // The container types.
        using containter_type1 = typename std::remove_reference_t<decltype(container1)>;
        using containter_type2 = typename std::remove_reference_t<decltype(container2)>;
        using pointer_type1    = typename containter_type1::pointer;
        using pointer_type2    = typename containter_type2::pointer;

        auto it1 = beman::optional::detail::normal_iterator<pointer_type1, containter_type1>{};
        auto it2 = beman::optional::detail::normal_iterator<pointer_type1, containter_type2>{};

        static_assert(!std::is_same_v<decltype(it1), decltype(it2)>);
    };

    test_neq(std::vector<int>{}, std::vector<double>{});
    test_neq(std::vector<int>{}, std::set<int>{});
    test_neq(std::set<int>{}, std::vector<int>{});
}