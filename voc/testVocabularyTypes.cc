#ifndef VOC_ANY_TEST
#define VOC_ANY_TEST 1 // for testing the Any class
#endif
#ifndef VOC_OPTIONAL_TEST
#define VOC_OPTIONAL_TEST 1 // for testing the Optional class
#endif

#ifndef DEBUG
#define DEBUG 1 // for testing function that does not get tested in the main test
#endif

#include <gtest/gtest.h>

#include "Any.h"
#include "Optional.h"

#if VOC_ANY_TEST
/****************************
 * TESTS FOR ANY CLASS      *
 ****************************/

TEST(AnyConstructorTest, Constructor)
{
  voc::Any a(42);
  EXPECT_EQ(voc::anyCast<int>(a), 42);
}

TEST(AnyConstructorTest, CopyConstructor)
{
  voc::Any a(42);
  voc::Any b(a);
  EXPECT_EQ(voc::anyCast<int>(b), 42);
}

TEST(AnyConstructorTest, MoveConstructor)
{
  voc::Any a(42);
  voc::Any b(std::move(a));
  EXPECT_EQ(voc::anyCast<int>(b), 42);
}

TEST(AnyConstructorTest, DefaultConstructor)
{
  voc::Any any;
  EXPECT_FALSE(any.hasValue());
}

/*
Any test suite
*/
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

/*
Any Lvalue test suite
*/
TEST(AnyLvalueTest, LvalueTest)
{
  int lvalue = 42;
  voc::Any a(lvalue);
  EXPECT_EQ(voc::anyCast<int>(a), 42);
}

TEST(AnyLvalueTest, LvalueAssignmentTest)
{
  int lvalue = 42;
  voc::Any a;
  a = lvalue; // assign lvalue to Any
  EXPECT_EQ(voc::anyCast<int>(a), 42);
}

TEST(AnyLvalueTest, LvalueMakeAnyTest)
{
  int lvalue = 42;
  voc::Any a = voc::makeAny<int>(lvalue);
  EXPECT_EQ(voc::anyCast<int>(a), 42);
}

/*
Any Rvalue test suite
*/
TEST(AnyRvalueTest, RvalueTest)
{
  voc::Any a(42); // 42 is an rvalue
  EXPECT_EQ(voc::anyCast<int>(a), 42);
}

TEST(AnyRvalueTest, RvalueAssignmentTest)
{
  voc::Any a;
  a = 42; // assign rvalue to Any
  EXPECT_EQ(voc::anyCast<int>(a), 42);
}

TEST(AnyRvalueTest, RvalueMakeAnyTest)
{
  voc::Any a = voc::makeAny<int>(42); // 42 is an rvalue
  EXPECT_EQ(voc::anyCast<int>(a), 42);
}

/*
Any cast test suite
*/
TEST(AnyCastTest, AnyCast)
{
  // L value
  voc::Any any(42);
  int value = voc::anyCast<int>(any);
  EXPECT_EQ(value, 42);
  // R value
  int value2 = voc::anyCast<int>(voc::Any(42));
  EXPECT_EQ(value2, 42);
}

TEST(AnyCastTest, AnyCastConst)
{
  const voc::Any any(42);
  const int value = voc::anyCast<int>(any);
  EXPECT_EQ(value, 42);
  const int value2 = voc::anyCast<int>(voc::Any(42));
  EXPECT_EQ(value2, 42);
}

TEST(AnyCastTest, AnyCastPointer)
{
  int original = 42;
  voc::Any any(&original);
  int *value = voc::anyCast<int *>(&any);
  ASSERT_NE(value, nullptr);
  EXPECT_EQ(*value, 42);
}

/*
Any mutant test suite
*/
TEST(AnyMutantTest, MutantTest_Constructor)
{
  voc::Any a(42);
  EXPECT_NE(voc::anyCast<int>(a), 43); // mutant: change 42 to 43 in constructor
}

TEST(AnyMutantTest, MutantTest_CopyConstructor)
{
  voc::Any a(42);
  voc::Any b(a);
  EXPECT_NE(voc::anyCast<int>(b), 43); // mutant: change 42 to 43 in copy constructor
}

TEST(AnyMutantTest, MutantTest_MoveConstructor)
{
  voc::Any a(42);
  voc::Any b(std::move(a));
  EXPECT_NE(voc::anyCast<int>(b), 43); // mutant: change 42 to 43 in move constructor
}

TEST(AnyMutantTest, MutantTest_CopyAssignment)
{
  voc::Any a(42);
  voc::Any b;
  b = a;
  EXPECT_NE(voc::anyCast<int>(b), 43); // mutant: change 42 to 43 in copy assignment
}

TEST(AnyMutantTest, MutantTest_MoveAssignment)
{
  voc::Any a(42);
  voc::Any b;
  b = std::move(a);
  EXPECT_NE(voc::anyCast<int>(b), 43); // mutant: change 42 to 43 in move assignment
}

TEST(AnyMutantTest, MutantTest_AnyCastNonPointerType)
{
  voc::Any a(42);
  EXPECT_NE(voc::anyCast<int>(a), 43); // mutant: change 42 to 43 in anyCast
}

TEST(AnyMutantTest, MutantTest_AnyCastPointerType)
{
  int original = 42;
  voc::Any a(&original);
  int *value = voc::anyCast<int *>(&a);
  EXPECT_NE(*value, 43); // mutant: change 42 to 43 in anyCast
}

TEST(AnyMutantTest, MutantTest_AnyCastConstType)
{
  const voc::Any any(42);
  const int value = voc::anyCast<int>(any);
  EXPECT_NE(value, 43); // mutant: change 42 to 43 in anyCast
}

TEST(AnyMutantTest, MutantTest_HasValue)
{
  voc::Any a(42);
  a.clear();
  EXPECT_FALSE(a.hasValue()); // mutant: change clear to not clear the value
}

TEST(AnyMutantTest, MutantTest_GetType)
{
  voc::Any a(42);
  EXPECT_NE(a.getType(), typeid(double)); // mutant: change int to double in getType
}

TEST(AnyMutantTest, MutantTest_MakeAny)
{
  voc::Any a = voc::makeAny<int>(42);
  EXPECT_NE(voc::anyCast<int>(a), 43); // mutant: change 42 to 43 in makeAny
}

#endif // VOC_ANY_TEST

#if VOC_OPTIONAL_TEST
/****************************
 * TESTS FOR OPTIONAL CLASS *
 ****************************/

/*
Optional constructor suite
*/
TEST(OptionalConstructorTest, DefaultConstructor)
{
  voc::Optional<int> opt;
  EXPECT_FALSE(opt.hasValue());
}

TEST(OptionalConstructorTest, ValueConstructor)
{
  voc::Optional<int> opt(42);
  EXPECT_TRUE(opt.hasValue());
  EXPECT_EQ(opt.getValue(), 42);
}

TEST(OptionalConstructorTest, CopyConstructor)
{
  voc::Optional<int> opt1(42);
  voc::Optional<int> opt2(opt1);
  EXPECT_TRUE(opt2.hasValue());
  EXPECT_EQ(opt2.getValue(), 42);
}

TEST(OptionalConstructorTest, MoveConstructor)
{
  voc::Optional<int> opt1(42);
  voc::Optional<int> opt2(std::move(opt1));
  EXPECT_TRUE(opt2.hasValue());
  EXPECT_EQ(opt2.getValue(), 42);
}

TEST(OptionalConstructorTest, CopyAssignment)
{
  voc::Optional<int> opt1(42);
  voc::Optional<int> opt2;
  opt2 = opt1;
  EXPECT_TRUE(opt2.hasValue());
  EXPECT_EQ(opt2.getValue(), 42);
}

TEST(OptionalConstructorTest, MoveAssignment)
{
  voc::Optional<int> opt1(42);
  voc::Optional<int> opt2;
  opt2 = std::move(opt1);
  EXPECT_TRUE(opt2.hasValue());
  EXPECT_EQ(opt2.getValue(), 42);
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

/*
Optinal Lvalue test suite
*/
TEST(OptionalLvalueTest, LvalueTest)
{
  int lvalue = 42;
  voc::Optional<int> opt(lvalue);
  EXPECT_TRUE(opt.hasValue());
  EXPECT_EQ(opt.getValue(), 42);
}

TEST(OptionalLvalueTest, LvalueAssignmentTest)
{
  int lvalue = 42;
  voc::Optional<int> opt;
  opt = lvalue; // assign lvalue to Optional
  EXPECT_TRUE(opt.hasValue());
  EXPECT_EQ(opt.getValue(), 42);
}

TEST(OptionalLvalueTest, LvalueMakeOptionalTest)
{
  int lvalue = 42;
  auto opt = voc::makeOptional<int>(lvalue);
  EXPECT_TRUE(opt.hasValue());
  EXPECT_EQ(opt.getValue(), 42);
}

/*
Optinal Rvalue test suite
*/
TEST(OptionalRvalueTest, RvalueTest)
{
  voc::Optional<int> opt(42); // 42 is an rvalue
  EXPECT_TRUE(opt.hasValue());
  EXPECT_EQ(opt.getValue(), 42);
}

TEST(OptionalRvalueTest, RvalueAssignmentTest)
{
  voc::Optional<int> opt;
  opt = 42; // assign rvalue to Optional
  EXPECT_TRUE(opt.hasValue());
  EXPECT_EQ(opt.getValue(), 42);
}

TEST(OptionalRvalueTest, RvalueMakeOptionalTest)
{
  auto opt = voc::makeOptional<int>(42); // 42 is an rvalue
  EXPECT_TRUE(opt.hasValue());
  EXPECT_EQ(opt.getValue(), 42);
}

/*
Optinal mutant test suite
*/
TEST(OptionalMutantTest, MutantTest_Constructor) {
  voc::Optional<int> opt(42);
  EXPECT_NE(opt.getValue(), 43); // mutant: change 42 to 43 in constructor
}

TEST(OptionalMutantTest, MutantTest_CopyConstructor) {
  voc::Optional<int> opt1(42);
  voc::Optional<int> opt2(opt1);
  EXPECT_NE(opt2.getValue(), 43); // mutant: change 42 to 43 in copy constructor
}

TEST(OptionalMutantTest, MutantTest_MoveConstructor) {
  voc::Optional<int> opt1(42);
  voc::Optional<int> opt2(std::move(opt1));
  EXPECT_NE(opt2.getValue(), 43); // mutant: change 42 to 43 in move constructor
}

TEST(OptionalMutantTest, MutantTest_CopyAssignment) {
  voc::Optional<int> opt1(42);
  voc::Optional<int> opt2;
  opt2 = opt1;
  EXPECT_NE(opt2.getValue(), 43); // mutant: change 42 to 43 in copy assignment
}

TEST(OptionalMutantTest, MutantTest_MoveAssignment) {
  voc::Optional<int> opt1(42);
  voc::Optional<int> opt2;
  opt2 = std::move(opt1);
  EXPECT_NE(opt2.getValue(), 43); // mutant: change 42 to 43 in move assignment
}

TEST(OptionalMutantTest, MutantTest_GetValue) {
  voc::Optional<int> opt(42);
  EXPECT_NE(opt.getValue(), 43); // mutant: change 42 to 43 in getValue
}

TEST(OptionalMutantTest, MutantTest_GetValueOr) {
  voc::Optional<int> opt;
  EXPECT_NE(opt.getValueOr(42), 43); // mutant: change 42 to 43 in getValueOr
}

TEST(OptionalMutantTest, MutantTest_Clear) {
  voc::Optional<int> opt(42);
  opt.clear();
  EXPECT_FALSE(opt.hasValue()); // mutant: change clear to not clear the value
}

TEST(OptionalMutantTest, MutantTest_MakeOptional) {
  auto opt = voc::makeOptional<int>(42);
  EXPECT_NE(opt.getValue(), 43); // mutant: change 42 to 43 in makeOptional
}

#endif // VOC_OPTIONAL_TEST

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
