/* Matthieu Bidault */

#include <gtest/gtest.h>

#include "Any.h"
#include "Optional.h"

/*
 * Any
 */

TEST(AnyTest, DefaultCtor)
{
  voc::Any any;
  EXPECT_FALSE(any.hasValue());
}

TEST(AnyTest, ChangingType)
{
  voc::Any any(42);
  const auto &type = any.getType();
  EXPECT_TRUE(any.hasValue());
  EXPECT_EQ(type, typeid(int));

  any = std::string("hello");
  const auto &type2 = any.getType();
  EXPECT_TRUE(any.hasValue());
  EXPECT_EQ(type2, typeid(std::string));
}

TEST(AnyTest, Clear)
{
  voc::Any any(42);
  any.clear();
  EXPECT_FALSE(any.hasValue());
}

/* TEST(AnyTest, CopyConstructor)
{
  voc::Any any(42);
  voc::Any any2 = any;
  EXPECT_TRUE(any2.hasValue());
  EXPECT_EQ(any2.getType(), typeid(int));
  int value = voc::anyCast<int>(any2);
  EXPECT_EQ(value, 42);
} */

TEST(AnyTest, CopyAssignment)
{
  voc::Any any(42);
  voc::Any any2;
  any2 = any;
  EXPECT_TRUE(any2.hasValue());
  EXPECT_EQ(any2.getType(), typeid(int));
  int value = voc::anyCast<int>(any2);
  EXPECT_EQ(value, 42);
}

TEST(AnyTest, MoveConstructor)
{
  voc::Any any(42);
  voc::Any any2 = std::move(any);
  EXPECT_TRUE(any2.hasValue());
  EXPECT_EQ(any2.getType(), typeid(int));
  EXPECT_FALSE(any.hasValue());
  int value = voc::anyCast<int>(any2);
  EXPECT_EQ(value, 42);
}

TEST(AnyTest, MoveAssignment)
{
  voc::Any any(42);
  voc::Any any2;
  any2 = std::move(any);
  EXPECT_TRUE(any2.hasValue());
  EXPECT_EQ(any2.getType(), typeid(int));
  EXPECT_FALSE(any.hasValue());
  int value = voc::anyCast<int>(any2);
  EXPECT_EQ(value, 42);
}

TEST(AnyTest, CreateInPlace)
{
  voc::Any any(voc::InPlaceType<std::vector<int>>, 42);
  EXPECT_TRUE(any.hasValue());
  EXPECT_EQ(any.getType(), typeid(std::vector<int>));
  std::vector<int> vec = voc::anyCast<std::vector<int>>(any);
  EXPECT_EQ(vec.size(), 42);
}

TEST(AnyTest, MakeAny)
{
  auto any = voc::makeAny<int>(42);
  EXPECT_TRUE(any.hasValue());
  EXPECT_EQ(any.getType(), typeid(int));
  int value = voc::anyCast<int>(any);
  EXPECT_EQ(value, 42);
}

TEST(AnyTest, AnyCastConstLValue)
{
  const voc::Any any(42);
  int value = voc::anyCast<int>(any);
  EXPECT_EQ(value, 42);
  value = 69;
  int value2 = voc::anyCast<int>(any);
  EXPECT_EQ(value2, 42);
}

TEST(AnyTest, AnyCastLValue)
{
  voc::Any any(42);
  int value = voc::anyCast<int>(any);
  EXPECT_EQ(value, 42);
  value = 69;
  int value2 = voc::anyCast<int>(any);
  EXPECT_EQ(value2, 42);
}

TEST(AnyTest, AnyCastRValue)
{
  int value = voc::anyCast<int>(voc::Any(42));
  EXPECT_EQ(value, 42);
}

TEST(AnyTest, AnyCastConstPointer)
{
  const voc::Any any(42);
  const int *value = voc::anyCast<int>(&any);
  EXPECT_EQ(*value, 42);
  value = nullptr;
  const int *value2 = voc::anyCast<int>(&any);
  EXPECT_EQ(*value2, 42);
}

TEST(AnyTest, AnyCastPointer)
{
  int original = 42;
  voc::Any any(&original);
  int *value = voc::anyCast<int*>(&any);
  EXPECT_EQ(*value, 42);
  *value = 69;
  int *value2 = voc::anyCast<int*>(&any);
  EXPECT_EQ(*value2, 69);
}

/*
 * Optional
 */

TEST(OptionalTest, DefaultCtor)
{
  voc::Optional<int> opt;
  EXPECT_FALSE(opt.hasValue());
}

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
