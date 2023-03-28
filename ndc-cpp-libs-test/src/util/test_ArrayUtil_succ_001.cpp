#include "../../pch.h"

using namespace nl;

// Define a test fixture class
class VectorTest : public ::testing::Test
{
protected:
  void SetUp() override
  {
    vec1 = {1, 2, 3, 4, 5};
    vec2 = {1, 2, 3, 4, 5, 5};
    vec3 = {"hello", "world", "hello", "gtest"};
  }

  std::vector<int> vec1;
  std::vector<int> vec2;
  std::vector<std::string> vec3;
};

// Test case for vector without duplicates
TEST_F(VectorTest, NoDuplicates)
{
  ASSERT_FALSE(util::isDuplicateElemInArray(vec1));
}

// Test case for vector with duplicates
TEST_F(VectorTest, WithDuplicates)
{
  ASSERT_TRUE(util::isDuplicateElemInArray(vec2));
}

// Test case for vector with duplicates of std::string type
TEST_F(VectorTest, WithDuplicatesOfString)
{
  ASSERT_TRUE(util::isDuplicateElemInArray(vec3));
}
