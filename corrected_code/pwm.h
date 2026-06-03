#ifndef __pwm_h__
#define __pwm_h__

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>

// Error codes
typedef enum {
  PWM_OK,
  PWM_NOT_INITIALIZED,
  PWM_NOT_SAVED,
  PWM_OPERATION_NOT_ALLOWED,
  PWM_OPERATION_NOT_RECOGNIZED,
  PWM_USER_NOT_FOUND,
  PWM_USER_ALREADY_EXISTS,
  PWM_INVALID_USER_ID,
  PWM_INVALID_PASSWORD,
  PWM_PASSWORD_MISMATCH,
  PWM_FILE_INACESSIBLE,
  PWM_FILE_ALREADY_EXISTS,
  PWM_FILE_CORRUPT,
  PWM_IO_ERROR,
  PWM_OS_ERROR,
  PWM_MEMORY_ALLOCATION_ERROR
} pwm_res_t;

// User and password validation
#define PWM_MIN_USER_ID_LEN 4
#define PWM_MAX_USER_ID_LEN 10
#define PWM_MIN_PASSWORD_LEN 6
#define PWM_MAX_PASSWORD_LEN 12

// Validation functions
// NOTE: Implemented on the file validation.c. Both functions analyzed and
// errors noted.
pwm_res_t pwm_is_valid_user(const char *user);
pwm_res_t pwm_is_valid_password(const char *password);

// Global definitions for salt and hash
// NOTE: unsigned char is an integer of 4 bytes, that is, from 0 to 255 (2^8 -
// 1). This means:
// - salt_t is an array of 4 integers of values between 0 to 255.
// - hash_t is an array of 16 integers of values between 0 to 255.
typedef unsigned char salt_t[4];
typedef unsigned char hash_t[16];

// Admin user name
// NOTE: Hardcoded credentials instead of using env variables. Not sure
// it is an error, but probably not best practice.
#define PWM_ADMIN_USER "admin"

// Data structures for internal representation
typedef struct _pwm_node_t {
  // NOTE: Why +1?
  char user[PWM_MAX_USER_ID_LEN + 1];
  salt_t salt;
  hash_t hash;
  // NOTE: This means this Struct is a node to a linked list.
  struct _pwm_node_t *next;
} pwm_node_t;

// NOTE: The Linked List in question. It saves a file name (probably the file
// containing the user ids, salts and hashed user passwords, same as etc/shadow)
// and the first node for the linked list of users, probaly inside that file.
struct _pwm_t {
  char *file;
  pwm_node_t *entries;
};
// NOTE: Could have applied this typedef directly to the struct for cleaner code
// but whatever, it works, this whole codebase is written like a first year
// student, I am getting used to it.
typedef struct _pwm_t *PWM;

// Core operations
// NOTE: Implemented on core.c; Will stop here for now. Tomorrow will continue
// from here and analyze this core.c file.
pwm_res_t pwm_init(char *file, char *password, PWM *pwm);
pwm_res_t pwm_open(char *file, char *password, PWM *pwm);
pwm_res_t pwm_save(PWM pwm);
pwm_res_t pwm_free(PWM pwm);

pwm_res_t pwm_match(PWM pwm, char *user, char *password);
pwm_res_t pwm_add(PWM pwm, char *user, char *password);
pwm_res_t pwm_update(PWM pwm, char *user, char *password);
pwm_res_t pwm_delete(PWM pwm, char *user);

typedef pwm_res_t pwm_iterator_t(char *user, salt_t salt, hash_t hash,
                                 void *arg);
pwm_res_t pwm_iterate(PWM pwm, pwm_iterator_t *iterator, void *arg);

// Command handling
#define PWM_MAX_COMMAND_ARGS 3

pwm_res_t pwm_handle_command(PWM *pwm, char *command_name, char **args,
                             int n_args);

// Interactive session
void pwm_interactive_session(void);

// UTILITY FUNCTIONS
pwm_res_t pwm_generate_salt(salt_t salt);
pwm_res_t pwm_hash_password(salt_t salt, char *password, hash_t hash);
int pwm_decode_hex_string(char *str, unsigned char *data, int expected_length);
void pwm_print_hex_string(FILE *fp, unsigned char *data, int length);

int pwm_split_line(char *string, char separator, char **fields, int max_fields);

void pwm_error(const char *fmt, ...);

#ifdef __cplusplus
}
#endif
#endif
