#include <gtest/gtest.h>
#include <pwm.h>

TEST(test_user, short_length) {
  ASSERT_EQ(PWM_INVALID_USER_ID, pwm_is_valid_user("rui"));
}

TEST(test_user, invalid_char) {
  ASSERT_EQ(PWM_INVALID_USER_ID, pwm_is_valid_user("@rui"));
}

TEST(test_user, valid_user) {
  ASSERT_EQ(PWM_OK, pwm_is_valid_user("ruiruiruir"));
}

TEST(test_user, valid_user2) {
  ASSERT_EQ(PWM_OK, pwm_is_valid_user("ruiruirui"));
}

// ─── Length boundary tests ────────────────────────

// Length 3 → too short (below MIN=4)
TEST(test_user, too_short_length3) {
  ASSERT_EQ(PWM_INVALID_USER_ID, pwm_is_valid_user("abc"));
}

// Length 4 → minimum valid length
TEST(test_user, min_valid_length4) {
  ASSERT_EQ(PWM_OK, pwm_is_valid_user("abcd"));
}

// Length 10 → maximum valid length (BUG: original code uses < instead of <=,
// rejects this)
TEST(test_user, max_valid_length10) {
  ASSERT_EQ(PWM_OK, pwm_is_valid_user("abcdefghij"));
}

// Length 11 → too long (above MAX=10)
TEST(test_user, too_long_length11) {
  ASSERT_EQ(PWM_INVALID_USER_ID, pwm_is_valid_user("abcdefghijk"));
}

// ─── Character validation tests ────────────────────────

// First character is invalid (BUG: original loop starts at i=1, skips user[0])
TEST(test_user, invalid_first_char_digit) {
  ASSERT_EQ(PWM_INVALID_USER_ID, pwm_is_valid_user("1abc"));
}

// First character is uppercase (BUG: same as above)
TEST(test_user, invalid_first_char_upper) {
  ASSERT_EQ(PWM_INVALID_USER_ID, pwm_is_valid_user("Abcd"));
}

// Character 'z' in username (BUG: original uses c >= 'z' which excludes 'z')
TEST(test_user, valid_user_with_z) {
  ASSERT_EQ(PWM_OK, pwm_is_valid_user("abcz"));
}

// Invalid character '@' in middle
TEST(test_user, invalid_char_at_symbol) {
  ASSERT_EQ(PWM_INVALID_USER_ID, pwm_is_valid_user("ab@d"));
}

// Invalid character: digit in middle
TEST(test_user, invalid_char_digit_in_middle) {
  ASSERT_EQ(PWM_INVALID_USER_ID, pwm_is_valid_user("ab1d"));
}

// Invalid character: uppercase in middle
TEST(test_user, invalid_char_upper_in_middle) {
  ASSERT_EQ(PWM_INVALID_USER_ID, pwm_is_valid_user("abCd"));
}

// ─── Valid cases ───────────────────────────

// All lowercase, length 5
TEST(test_user, valid_user_length5) {
  ASSERT_EQ(PWM_OK, pwm_is_valid_user("abcde"));
}

// All lowercase, length 9
TEST(test_user, valid_user_length9) {
  ASSERT_EQ(PWM_OK, pwm_is_valid_user("abcdefghi"));
}

// Username ending with 'z'
TEST(test_user, valid_user_ends_with_z) {
  ASSERT_EQ(PWM_OK, pwm_is_valid_user("testz"));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
