/* Vital Focheux */

#include <gtest/gtest.h>

#include "Any.h"
#include "Optional.h"

#include <cmath>

namespace {

  voc::Optional<double> my_sqrt(double value) {
    if (value < 0) {
      return voc::Optional<double>();
    }
    return voc::Optional<double>(std::sqrt(value));
  }

  struct Point {
    Point(int x1, int y1){
      x = x1;
      y = y1;
    }

    int x;
    int y;

    bool operator==(const Point& other) const {
      return x == other.x && y == other.y;
    }
  };
};

voc::Optional<Point> couldCreatePoint(bool create){
  if(create){
    return voc::Optional<Point>(voc::InPlace, 42, 24);
  }
  return voc::Optional<Point>();
}

/*
 * Any
 */

TEST(AnyTest, DefaultCtor) {
  voc::Any any;
  EXPECT_FALSE(any.hasValue());
}

TEST(AnyTest, Get_Type){
  voc::Any any;
  EXPECT_EQ(any.getType(), typeid(void));
}

TEST(AnyTest, Get_Type_Change) {
  voc::Any any = 42;
  EXPECT_EQ(any.getType(), typeid(int));
  any = 3.14;
  EXPECT_EQ(any.getType(), typeid(double));
}

TEST(AnyTest, AnyCastConst){
  const voc::Any any = 42;
  try{
    auto c = voc::anyCast<int>(any);
    EXPECT_EQ(c, 42);
  }catch(const std::bad_cast& e){
    std::cout << e.what() << std::endl;
  }
  
}

TEST(AnyTest, AnyCastMove){
  voc::Any any = 42;
  try{
    auto c = voc::anyCast<int>(std::move(any));
    EXPECT_EQ(c, 42);
  }catch(const std::bad_cast& e){
    std::cout << e.what() << std::endl;
  }
}

TEST(AnyTest, AnyCast2){
  voc::Any any = 42;
  try{
    auto c = voc::anyCast<int>(any);
    EXPECT_EQ(c, 42);
  }catch(const std::bad_cast& e){
    std::cout << e.what() << std::endl;
  }
}

TEST(AnyTest, DefaultStringConstructor){
  voc::Any any(std::string("Hello"));
  EXPECT_TRUE(any.hasValue());
  EXPECT_EQ(any.getType(), typeid(std::string));
}

TEST(AnyTest, AnyCastPointer){
  const voc::Any any = 42;
  auto c = voc::anyCast<int>(&any);
  EXPECT_EQ(*c, 42);
}

TEST(AnyTest, CopyAssignmentOfAny){
  voc::Any any = 42;
  voc::Any any2;
  any2 = any;
  try{
    std::cout << "try" << std::endl;
    auto c = voc::anyCast<int>(any2);
    auto c2 = voc::anyCast<int>(any);
    EXPECT_EQ(c, 42);
    EXPECT_EQ(c2, 42);
    EXPECT_EQ(c, c2);
  } catch(const std::bad_cast& e){
    std::cout << e.what() << std::endl;
  }
  // try{
  //   auto c = voc::anyCast<int>(any);
  //   EXPECT_EQ(c, 42);
  // } catch(const std::bad_cast& e){
  //   std::cout << e.what() << std::endl;
  // }
}

/*
 * Optional
 */

TEST(OptionalTest, DefaultCtor) {
  voc::Optional<int> opt;
  EXPECT_FALSE(opt.hasValue());
}

TEST(Exemple, Exemple){
  voc::Any any;
  any = 42;
  EXPECT_EQ(voc::anyCast<int>(any), 42);

  any = 3.14;
  EXPECT_EQ(voc::anyCast<double>(any), 3.14);

  any = std::string("The cake is a lie!");
  EXPECT_EQ(voc::anyCast<std::string>(any), "The cake is a lie!");

  try{
    std::cout << voc::anyCast<bool>(any) << std::endl;
  } catch (const std::exception& e){
    EXPECT_STREQ(e.what(), "std::bad_cast");
  }

  voc::Any any_inplace(voc::InPlaceType<Point>, 42, 24);
  auto p = voc::anyCast<Point>(any_inplace);

  EXPECT_EQ(p.x, 42);
  EXPECT_EQ(p.y, 24);

  auto any_cleared = voc::makeAny<Point>(42, 42);
  any_cleared.clear();
  EXPECT_FALSE(any_cleared.hasValue());

  try{
    auto point = voc::anyCast<Point>(any_cleared);
    std::cout << point.x << "x" << point.y << std::endl;
  } catch (const std::exception& e){
    EXPECT_STREQ(e.what(), "std::bad_cast");
  }

  voc::Optional<double> opt = my_sqrt(-1.0);
  EXPECT_EQ(opt.getValueOr(-1.0), -1.0);

  opt = my_sqrt(9.0);
  EXPECT_EQ(opt.getValueOr(-1.0), 3.0);

  voc::Optional<Point> optPoint = couldCreatePoint(true);
  p = optPoint.getValue();

  EXPECT_EQ(p.x, 42);
  EXPECT_EQ(p.y, 24);

  auto opt2 = voc::makeOptional<Point>(42, 24);
  EXPECT_TRUE(p == opt2.getValue());

  // std::cout << " == " << std::endl;
  // EXPECT_TRUE(opt2 == optPoint);

  opt2.clear();
  EXPECT_FALSE(opt2);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
