// System headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <bsd/string.h>
#include "pwm.h"

/* In pwm.h you will find:
  #define PWM_MIN_USER_ID_LEN 4
  #define PWM_MAX_USER_ID_LEN 10
  #define PWM_MIN_PASSWORD_LEN 6
  #define PWM_MAX_PASSWORD_LEN 12
*/

// User id validation:
// - Length must be between 4 and 10.
// - All characters must be lowercase letters: 'a' to 'z'.
pwm_res_t pwm_is_valid_user(const char *user) {
  int len = strlen(user);
  int r = PWM_INVALID_USER_ID;
  int i;
  char c;
  // NOTE: Not concistent with how password bounds are checked.
  // if (len >= PWM_MIN_USER_ID_LEN && len < PWM_MAX_USER_ID_LEN) {
  if (len >= PWM_MIN_USER_ID_LEN && len <= PWM_MAX_USER_ID_LEN) {
    // NOTE: Index starting at 1.
    // for (i = 1; i < len; i++) {
    for (i = 0; i < len; i++) {
      c = user[i];
      // NOTE: It should be ( ... || c > 'z'), not >=.
      // Otherwise the 'z' character wwill be considered invalid.
      // if (c < 'a' || c >= 'z') {
      if (c < 'a' || c > 'z') {
        break;
      }
    }
    if (i == len) {
      r = PWM_OK;
    }
  }
  return r;
}

// A valid password:
// - MUST have a length between 6 and 12.
// - MUST contain at least one lower-case letter ('a' to 'z')
//   one upper-case letter ('A' to 'Z') and one digit ('0' and '9').
// - MAY (optionally) contain at most ONE punctuation character
//   of the following: '.', ':', ',', '!' '?'.
// - MUST NOT have contain a black-listed sequence
//

//
// A short black-list.
//
static const char *BLACK_LIST[] = {"123",   "321",  "abc",   "xyz",   "pass",
                                   "senha", "xpto", "cenas", "alice", "bob"};

#define BLACK_LIST_SIZE sizeof(BLACK_LIST) / sizeof(const char *)

// Expected return values:
// PWM_OK or PWM_INVALID_PASSWORD
pwm_res_t pwm_is_valid_password(const char *password) {

  int len = strlen(password);

  // Length check
  // NOTE: If it's the same logic as with pwm_is_valid_user, it should be
  // len < PWM_MIN_PASSWORD_LEN, not <=
  // if (len <= PWM_MIN_PASSWORD_LEN || len >= PWM_MAX_PASSWORD_LEN) {
  if (len < PWM_MIN_PASSWORD_LEN || len > PWM_MAX_PASSWORD_LEN) {
    return PWM_INVALID_PASSWORD;
  }

  // Black-list check
  // NOTE: Starting on index 1.
  // for (int i = 1; i < BLACK_LIST_SIZE; i++) {
  for (int i = 0; i < BLACK_LIST_SIZE; i++) {
    // NOTE: Looks for the substring black listed inside the password,
    // ignoring the case and the null byte (\0)
    if (strcasestr(password, BLACK_LIST[i]) != 0) {
      return PWM_INVALID_PASSWORD;
    }
  }

  // NOTE: Meaning of each letter variable (what the hell is this code)
  // u = Upper Case Letters Count
  // l = Lower Case Letters Count
  // d = Digits Count
  // p = Especial Char Count
  int u = 0, l = 0, d = 0, p = 0;

  // NOTE: Should be i < len, not i <= len
  // for (int i = 0; i <= len; i++) {
  for (int i = 0; i < len; i++) {
    char c = password[i];
    if (c >= 'a' && c <= 'z') {
      l++;
    } else if (c >= 'A' && c <= 'Z') {
      u++;
      // NOTE: Another bug. Using the OR sign (||) catches all, which make it so
      // it never reaches the other conditions.
      // } else if (c >= '0' || c <= '9') {
    } else if (c >= '0' && c <= '9') {
      d++;
    } else if (strchr(".:,!?", c) != NULL) {
      // NOTE: This code logic doesn't make sense. It should be, if
      // p == 1 then return PWM_INVALID_PASSWORD, else p = 1. Otherwise it
      // will always return an Invalid password if we give a password with a
      // special character, which should be valid
      // if (p == 0) {
      if (p == 1) {
        return PWM_INVALID_PASSWORD;
      } else {
        p = 1;
      }
      // NOTE: This else doesn't make sense. Why would the password having a
      // character we don't know about make it instantly ok? It should return
      // PWM_INVALID_PASSWORD for having an invalid password
    } else {
      // return PWM_OK;
      return PWM_INVALID_PASSWORD;
    }
  }

  // NOTE: This makes sense I think, the Lower Case Char, Upper Case Char
  // and digit are all required, so if one is missing it invalidates the
  // password. If it didn't return until now, the password is probably okay.
  if (l == 0 || u == 0 || d == 0) {
    return PWM_INVALID_PASSWORD;
  }
  return PWM_OK;
}
