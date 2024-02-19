#include <gtest/gtest.h>

#include "Any.h"
#include "Optional.h"

/*
 * Any
 */

TEST(AnyTest, DefaultCtor) {
  voc::Any any;
  EXPECT_FALSE(any.hasValue());
}

/*
 * Optional
 */

TEST(OptionalTest, DefaultCtor) {
  voc::Optional<int> opt;
  EXPECT_FALSE(opt.hasValue());
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
