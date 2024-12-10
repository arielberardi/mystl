#include "Source/Vector.hpp"

#include <gtest/gtest.h>

using TVector = MySTL::Vector<int>;

TEST(Vector, AppliesRuleOfFive) {
  EXPECT_TRUE(std::is_constructible_v<TVector>);
  EXPECT_TRUE(std::is_destructible_v<TVector>);
  EXPECT_TRUE(std::is_copy_assignable_v<TVector>);
  EXPECT_TRUE(std::is_copy_constructible_v<TVector>);
  EXPECT_TRUE(std::is_move_assignable_v<TVector>);
  EXPECT_TRUE(std::is_move_constructible_v<TVector>);
}

TEST(Vector, ConstructAndRepeatAnObject) {
  TVector v(3, 1);

  EXPECT_EQ(v.size(), 3);
  for (size_t i{}; i < v.size(); ++i) {
    EXPECT_EQ(v[0], 1);
  }
}

TEST(Vector, ConstructWithInitializerList) {
  TVector v{0, 1, 2, 3, 4};
  EXPECT_EQ(v.size(), 5);
}

TEST(Vector, ReserveMemoryAndNotChangeSize) {
  TVector v{};
  v.reserve(10);
  EXPECT_EQ(v.capacity(), 10);
  EXPECT_EQ(v.size(), 0);
}

TEST(Vector, ResizeWithDefaultElement) {
  TVector v{2, 1};

  EXPECT_EQ(v.front(), 2);
  EXPECT_EQ(v.back(), 1);

  v.resize(10);

  EXPECT_EQ(v.capacity(), 16);
  EXPECT_EQ(v.size(), 10);
  EXPECT_EQ(v.front(), 2);
  EXPECT_EQ(v.back(), 0);
}

TEST(Vector, ClearSetSizeToZero) {
  TVector v{2, 1};

  EXPECT_EQ(v.size(), 2);
  v.clear();
  EXPECT_EQ(v.size(), 0);
}

TEST(Vector, PushBackItem) {
  TVector v{2, 1};

  v.push_back(3);

  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v.back(), 3);
}

TEST(Vector, PopBackItem) {
  TVector v{2, 1, 3};

  v.pop_back();

  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v.back(), 1);
}

TEST(Vector, EmplaceBackItem) {
  TVector v{2, 1};

  v.emplace_back(3);

  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v.back(), 3);
}

TEST(Vector, ShrinkCapacityToFitSize) {
  TVector v{2, 1};

  v.reserve(10);
  EXPECT_EQ(v.capacity(), 10);

  v.shrink_to_fit();
  EXPECT_EQ(v.capacity(), v.size());
}

TEST(Vector, EmplaceItemInSelectedPosition) {
  TVector v{2, 1};

  v.emplace(1, 3);

  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v.front(), 2);
  EXPECT_EQ(v.back(), 1);
  EXPECT_EQ(v[1], 3);
}

TEST(Vector, InsertAItemInSelectedPosition) {
  TVector v{2, 1};

  v.insert(1, 3);

  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v.front(), 2);
  EXPECT_EQ(v.back(), 1);
  EXPECT_EQ(v[1], 3);
}

TEST(Vector, InsertManyItemInSelectedPosition) {
  TVector v{2, 1};

  v.insert(1, 3, 3);

  EXPECT_EQ(v.size(), 5);
  EXPECT_EQ(v.front(), 2);
  EXPECT_EQ(v.back(), 1);
  EXPECT_EQ(v[1], 3);
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(v[3], 3);
}

TEST(Vector, AtMethodThrowsOutOfBoundError) {
  TVector v{2, 1};
  EXPECT_THROW((void)v.at(3), std::runtime_error);
}

TEST(Vector, Empty) {
  TVector v;
  EXPECT_TRUE(v.empty());
}

TEST(Vector, AssignmentWithInitializerList) {
  TVector v;
  v = {1, 2, 3, 4};

  EXPECT_EQ(v.size(), 4);
  EXPECT_EQ(v.front(), 1);
  EXPECT_EQ(v.back(), 4);
}
