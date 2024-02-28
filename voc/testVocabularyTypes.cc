#ifndef VOC_ANY_TEST
#define VOC_ANY_TEST 1 // for testing the Any class
#endif
#ifndef VOC_OPTIONAL_TEST
#define VOC_OPTIONAL_TEST 1 // for testing the Optional class
#endif

#ifndef DEBBUG
#define DEBBUG 0 // for testing function that does not get tested in the main test
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

TEST(AnyTest, List_Of_Any_But_Different_Type)
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

  std::vector<voc::Any> any_list;
  any_list.push_back(42);
  any_list.push_back(3.14);
  any_list.push_back(std::string("The cake is a lie!"));
  any_list.push_back(Point(42, 24));
  std::cout << "any_list.size(): " << any_list.size() << std::endl;
  std::cout << "element of any_list :" << std::endl;
  for (auto &any : any_list)
  {
    if (any.getType() == typeid(int))
    {
      std::cout << "\tint: " << voc::anyCast<int>(any) << std::endl;
    }
    else if (any.getType() == typeid(double))
    {
      std::cout << "\tdouble: " << voc::anyCast<double>(any) << std::endl;
    }
    else if (any.getType() == typeid(std::string))
    {
      std::cout << "\tstring: " << voc::anyCast<std::string>(any) << std::endl;
    }
    else if (any.getType() == typeid(Point))
    {
      auto point = voc::anyCast<Point>(any);
      std::cout << "\tPoint: " << point.x << "x" << point.y << std::endl;
    }
  }

  EXPECT_TRUE(any_list.size() == 4);
  EXPECT_TRUE(any_list[0].getType() == typeid(int));
  EXPECT_TRUE(any_list[1].getType() == typeid(double));
  EXPECT_TRUE(any_list[2].getType() == typeid(std::string));
  EXPECT_TRUE(any_list[3].getType() == typeid(Point));
  EXPECT_TRUE(voc::anyCast<int>(any_list[0]) == 42);
  EXPECT_TRUE(voc::anyCast<double>(any_list[1]) == 3.14);
  EXPECT_TRUE(voc::anyCast<std::string>(any_list[2]) == "The cake is a lie!");
  EXPECT_TRUE(voc::anyCast<Point>(any_list[3]) == Point(42, 24));
}
#endif // VOC_ANY_TEST

#if VOC_OPTIONAL_TEST
/****************************
 * TESTS FOR OPTIONAL CLASS *
 ****************************/

TEST(OptionalTest, DefaultConstructor)
{
  voc::Optional<int> opt;
  EXPECT_FALSE(opt.hasValue());
}

TEST(OptionalTest, BoolConversion)
{
  voc::Optional<int> opt;
  EXPECT_FALSE(static_cast<bool>(opt)); // false because no value
  opt = 42;
  EXPECT_TRUE(static_cast<bool>(opt)); // true because value
}

TEST(OptionalTest, GetValue)
{
  voc::Optional<int> opt;
  opt = 42;
  EXPECT_EQ(opt.getValue(), 42);
  const voc::Optional<int> opt_const = 42;
  EXPECT_EQ(opt_const.getValue(), 42);
}

TEST(OptionalTest, GetValueOr)
{
  voc::Optional<int> opt;
  EXPECT_EQ(opt.getValueOr(42), 42);
  opt = 24;
  EXPECT_EQ(opt.getValueOr(42), 24);
}

TEST(OptionalTest, Clear)
{
  voc::Optional<int> opt(42);
  EXPECT_TRUE(opt.hasValue());
  opt.clear();
  EXPECT_FALSE(opt.hasValue());
}

TEST(OptionalTest, ArrowOperator)
{
  struct Point
  {
    int x;
    int y;
  };
  voc::Optional<Point> opt;
  opt = Point{42, 24};
  EXPECT_EQ(opt->x, 42);
  EXPECT_EQ(opt->y, 24);
}

TEST(OptionalTest, DereferenceOperator)
{
  struct Point
  {
    int x;
    int y;
  };
  voc::Optional<Point> opt;
  opt = Point{42, 24};
  EXPECT_EQ((*opt).x, 42);
  EXPECT_EQ((*opt).y, 24);
}

TEST(OptionalTest, CaseOfUsage)
{
  struct Point
  {
    int x;
    int y;

    Point(int x, int y) : x(x), y(y) {}
  };

  voc::Optional<Point> opt;
  if (opt)
  {
    std::cout << "This will not be printed" << std::endl;
  }
  else
  {
    std::cout << "This will be printed" << std::endl;
  }
  // This will be printed
  EXPECT_FALSE(static_cast<bool>(opt));
  opt = Point{42, 24};
  if (opt)
  {
    std::cout << "This will be printed" << std::endl;
  }
  else
  {
    std::cout << "This will not be printed" << std::endl;
  }
  // This will be printed
  EXPECT_TRUE(static_cast<bool>(opt));
  std::cout << opt->x << "x" << opt->y << std::endl;
  // 42x24
  EXPECT_EQ(opt->x, 42);
  EXPECT_EQ(opt->y, 24);
  auto opt_cleared = voc::makeOptional<Point>(42, 42);
  opt_cleared.clear();
  std::cout << opt_cleared.hasValue() << std::endl;
  // 0 (no data)
  EXPECT_FALSE(opt_cleared.hasValue());
  try
  {
    auto point = opt_cleared.getValue();
    std::cout << point.x << "x" << point.y << std::endl;
  }
  catch (const std::exception &e)
  {
    std::cout << e.what() << std::endl; // throw std::runtime_error, no initialized
  }
}

#endif // VOC_OPTIONAL_TEST

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
