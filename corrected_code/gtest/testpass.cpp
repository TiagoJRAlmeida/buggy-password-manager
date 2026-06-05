
#include <gtest/gtest.h>
#include <pwm.h>

TEST(test_pass, invalid_pass_length5) {
  ASSERT_EQ(PWM_INVALID_PASSWORD, pwm_is_valid_password("Aa9zx"));
}

TEST(test_pass, valid_pass_length6) {
  ASSERT_EQ(PWM_OK, pwm_is_valid_password("Aa9za1"));
}

TEST(test_pass, invvalid_pass_with_two_punct) {
  ASSERT_EQ(PWM_INVALID_PASSWORD, pwm_is_valid_password("Aa9za1!!"));
}

TEST(test_pass, blacklisted_pass1) {
  ASSERT_EQ(PWM_INVALID_PASSWORD, pwm_is_valid_password("A123zad!"));
}

TEST(test_pass, too_short_length5) {
  ASSERT_EQ(PWM_INVALID_PASSWORD, pwm_is_valid_password("Aa9zx"));
}

// Length 6 → minimum valid length (BUG: original uses <= instead of <, rejects
// this)
TEST(test_pass, min_valid_length6) {
  ASSERT_EQ(PWM_OK, pwm_is_valid_password("Aa9za1"));
}

// Length 12 → maximum valid length (BUG: original uses >= instead of >, rejects
// this)
TEST(test_pass, max_valid_length12) {
  ASSERT_EQ(PWM_OK, pwm_is_valid_password("Aa9za1Bb2cd!"));
}

// Length 13 → too long (above MAX=12)
TEST(test_pass, too_long_length13) {
  ASSERT_EQ(PWM_INVALID_PASSWORD, pwm_is_valid_password("Aa9za1Bb2cde!"));
}

// ─── Blacklist tests ────────────────────────────

// "123" → first entry in blacklist (BUG: original loop starts at i=1, skips it)
TEST(test_pass, blacklisted_123) {
  ASSERT_EQ(PWM_INVALID_PASSWORD, pwm_is_valid_password("A123zad!"));
}

// "321" → second entry (index 1), should be caught
TEST(test_pass, blacklisted_321) {
  ASSERT_EQ(PWM_INVALID_PASSWORD, pwm_is_valid_password("A321Zad!"));
}

// "abc" → blacklisted
TEST(test_pass, blacklisted_abc) {
  ASSERT_EQ(PWM_INVALID_PASSWORD, pwm_is_valid_password("AbcZ1!!"));
}

// "pass" → blacklisted
TEST(test_pass, blacklisted_pass) {
  ASSERT_EQ(PWM_INVALID_PASSWORD, pwm_is_valid_password("Pass1word!"));
}

// "bob" → blacklisted (last entry in list)
TEST(test_pass, blacklisted_bob) {
  ASSERT_EQ(PWM_INVALID_PASSWORD, pwm_is_valid_password("Bob123!!"));
}

// ─── Character class tests ────────────────────────────

// Missing uppercase letter
TEST(test_pass, missing_uppercase) {
  ASSERT_EQ(PWM_INVALID_PASSWORD, pwm_is_valid_password("aa9za1"));
}

// Missing lowercase letter
TEST(test_pass, missing_lowercase) {
  ASSERT_EQ(PWM_INVALID_PASSWORD, pwm_is_valid_password("AA9ZA1"));
}

// Missing digit (BUG: original digit check uses || instead of &&, always true)
TEST(test_pass, missing_digit) {
  ASSERT_EQ(PWM_INVALID_PASSWORD, pwm_is_valid_password("AaZzBb"));
}

// ─── Punctuation tests ────────────────────────────

// One punctuation character → valid
// (BUG: original logic is inverted — rejects first punctuation, accepts second)
TEST(test_pass, one_punct_valid) {
  ASSERT_EQ(PWM_OK, pwm_is_valid_password("Aa9za1!"));
}

// Two punctuation characters → invalid
TEST(test_pass, two_punct_invalid) {
  ASSERT_EQ(PWM_INVALID_PASSWORD, pwm_is_valid_password("Aa9z!!a1"));
}

// Each valid punctuation character: '.'
TEST(test_pass, punct_dot) {
  ASSERT_EQ(PWM_OK, pwm_is_valid_password("Aa9za1."));
}

// Each valid punctuation character: ':'
TEST(test_pass, punct_colon) {
  ASSERT_EQ(PWM_OK, pwm_is_valid_password("Aa9za1:"));
}

// Each valid punctuation character: ','
TEST(test_pass, punct_comma) {
  ASSERT_EQ(PWM_OK, pwm_is_valid_password("Aa9za1,"));
}

// Each valid punctuation character: '?'
TEST(test_pass, punct_question) {
  ASSERT_EQ(PWM_OK, pwm_is_valid_password("Aa9za1?"));
}

// Invalid character (not in allowed set) → invalid
TEST(test_pass, invalid_char) {
  ASSERT_EQ(PWM_INVALID_PASSWORD, pwm_is_valid_password("Aa9za1#"));
}

// ─── Valid password (no punctuation) ────────────────────────────────

TEST(test_pass, valid_no_punct) {
  ASSERT_EQ(PWM_OK, pwm_is_valid_password("Aa9Za1b"));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
