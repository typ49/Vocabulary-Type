#ifndef VOC_ANY_TEST
#define VOC_ANY_TEST 1 // for testing the Any class
#endif
#ifndef VOC_OPTIONAL_TEST
#define VOC_OPTIONAL_TEST 0 // for testing the Optional class
#endif

#ifndef MY_TEST
#define MY_TEST 0 // for testing function that does not get tested in the main test
#endif

#include <gtest/gtest.h>

#include "Any.h"
#include "Optional.h"

#if VOC_ANY_TEST
/****************************
 * TESTS FOR ANY CLASS      *
 ****************************/

TEST(AnyTest, DefaultConstructor)
{
  voc::Any any;
  EXPECT_FALSE(any.hasValue());
}

TEST(AnyTest, BoolConversion)
{
  voc::Any any;
  EXPECT_FALSE(static_cast<bool>(any)); // false because no value
  any = 42;
  EXPECT_TRUE(static_cast<bool>(any)); // true because value
}

TEST(AnyTest, Clear)
{
  voc::Any any(42);
  EXPECT_TRUE(any.hasValue());
  any.clear();
  EXPECT_FALSE(any.hasValue());
}

TEST(AnyTest, GetType)
{
  voc::Any any(42);
  EXPECT_EQ(any.getType(), typeid(int));
  any = 42.0;
  EXPECT_EQ(any.getType(), typeid(double));
}

TEST(AnyTest, CaseOfUsage)
{

  struct Point
  {
    Point(int x1, int y1)
    {
      x = x1;
      y = y1;
    }
    int x;
    int y;
    bool operator==(const Point &other) const
    {
      return x == other.x && y == other.y;
    }
  };

  voc::Any any;
  any = 42;
  std::cout << voc::anyCast<int>(any) << std::endl;
  // 42
  EXPECT_TRUE(voc::anyCast<int>(any) == 42);
  any = 3.14;
  std::cout << voc::anyCast<double>(any) << std::endl;
  // 3.14
  EXPECT_TRUE(voc::anyCast<double>(any) == 3.14);
  any = std::string("The cake is a lie!");
  std::cout << voc::anyCast<std::string>(any) << std::endl;
  // The cake is a lie!
  EXPECT_TRUE(voc::anyCast<std::string>(any) == "The cake is a lie!");
  try
  {
    std::cout << voc::anyCast<bool>(any) << std::endl;
  }
  catch (const std::exception &e)
  {
    std::cout << e.what() << std::endl; // throw std::bad_cast, wrong type
  }
  voc::Any any_inplace(voc::InPlaceType<Point>, 42, 24);
  // Call the Point(int, int) constructor directly
  EXPECT_TRUE(voc::anyCast<Point>(any_inplace) == Point(42, 24));
  auto p = voc::anyCast<Point>(any_inplace);
  std::cout << "p : " << p.x << "x" << p.y << std::endl;
  // {42, 24}
  EXPECT_TRUE(p == Point(42, 24));
  auto any_cleared = voc::makeAny<Point>(42, 42);
  any_cleared.clear();
  std::cout << any_cleared.hasValue() << std::endl;
  // 0 (no data)
  EXPECT_FALSE(any_cleared.hasValue());
  try
  {
    auto point = voc::anyCast<Point>(any_cleared);
    std::cout << point.x << "x" << point.y << std::endl;
  }
  catch (const std::exception &e)
  {
    std::cout << e.what() << std::endl; // throw std::bad_cast, no initialized
  }
}
#endif // VOC_ANY_TEST

#if VOC_OPTIONAL_TEST
/****************************
 * TESTS FOR OPTIONAL CLASS *
 ****************************/

TEST(OptionalTest, DefaultCtor)
{
  voc::Optional<int> opt;
  EXPECT_FALSE(opt.hasValue());
}

#endif // VOC_OPTIONAL_TEST

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
