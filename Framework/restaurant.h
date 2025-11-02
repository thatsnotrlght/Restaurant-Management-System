/*####################################################
   HEADER FILE
   💀 Students please don't modify this file! 
####################################################*/

#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

/* ========================================
 * SYSTEM CONSTANTS AND LIMITS
 * ======================================== */
#define MAX_MENU_ITEMS 10

#define MAX_NAME_LENGTH 21          // 20 chars + null terminator
#define MAX_CATEGORY_LENGTH 16      // 15 chars + null terminator

#define MIN_PRICE 1.0
#define MAX_PRICE 50.0

#define TAX_RATE 0.10               

#define ITEM_NOT_FOUND -1
#define OPERATION_SUCCESS 1
#define OPERATION_FAILURE 0

/* ========================================
 * TIMEOUT AND SAFETY CONSTANTS
 * ======================================== */
#define MAX_EXECUTION_TIME 5        // Maximum seconds per function
#define MAX_LOOP_ITERATIONS 10000   // Maximum loop iterations
#define INPUT_BUFFER_SIZE 1024      // Input buffer size
#define OUTPUT_BUFFER_SIZE 2048     // Output buffer size

/* ========================================
 * FUNCTION POINTER TYPES
 * ======================================== */
typedef int (*CompareFunction)(const void* a, const void* b);

/* ========================================
 * TIMEOUT AND SAFETY MECHANISMS
 * ======================================== */
extern volatile int timeout_occurred;

void setup_timeout_handler(void);
void timeout_handler(int sig);

/* ========================================
 * STUDENT FUNCTION PROTOTYPES
 * 👍Students implement only these 6 functions
 * ======================================== */

/* FUNCTION 1: Multi-dimensional Arrays + Pointer Arithmetic
 * Copy string using pointer arithmetic only (no strcpy)
 * Tests: Character-by-character copying, pointer increment
 */
void copyString(char* dest, const char* src);

/* FUNCTION 2: Pointer Fundamentals + Linear Search
 * Find menu item using pointer-based search
 * Tests: Pointer arithmetic for traversal, search algorithm
 * Returns: index if found, ITEM_NOT_FOUND otherwise
 */
int findMenuItem(char names[][MAX_NAME_LENGTH], int count, const char* searchName);

/* FUNCTION 3: Pointer Fundamentals + 1D Array Traversal
 * Calculate sum of prices using pointer arithmetic
 * Tests: Pointer increment, dereferencing, accumulation
 */
float calculateSum(float* prices, int count);

/* FUNCTION 4: Function Parameters + Array Modification
 * Add new menu item with validation
 * Tests: Call-by-reference, array modification, bounds checking
 * Returns: OPERATION_SUCCESS or OPERATION_FAILURE
 */
int addMenuItem(char names[][MAX_NAME_LENGTH], 
               char categories[][MAX_CATEGORY_LENGTH],
               float* prices, int* count,
               const char* name, const char* category, float price);

/* FUNCTION 5: Function Parameters + Multiple Return Values
 * Process order and calculate totals using pointers
 * Tests: Multiple return values via pointer parameters
 */
void processOrder(float* prices, int* quantities, int menuSize,
                 float* subtotal, float* tax, float* total);

/* FUNCTION 6: Function Pointers + Strategy Pattern
 * Sort menu items using comparison function pointer
 * Tests: Function pointer usage, sorting algorithm
 */
void sortMenu(char names[][MAX_NAME_LENGTH], 
             char categories[][MAX_CATEGORY_LENGTH],
             float* prices, int count, CompareFunction compare);

/* ========================================
 * UTILITY AND FRAMEWORK FUNCTIONS
 * (⚠️Provided - students don't implement these)
 * ======================================== */
int safe_string_compare(const char* str1, const char* str2);
void clear_input_buffer(void);
void print_separator(void);
void log_function_call(const char* function_name);
void log_function_result(const char* function_name, int result);

/* ========================================
 * DRIVER-CONTROLLED DISPLAY FUNCTION
 * (⚠️Provided - students don't implement this)
 * ======================================== */
void driver_display_menu(char names[][MAX_NAME_LENGTH], 
                        char categories[][MAX_CATEGORY_LENGTH], 
                        float* prices, int count);

/* ========================================
 * COMPARISON FUNCTIONS FOR SORTING
 * (⚠️Provided - students don't implement these)
 * ======================================== */
int compareByName(const void* a, const void* b);
int compareByPrice(const void* a, const void* b);
int compareByCategory(const void* a, const void* b);

/* ========================================
 * TEST FRAMEWORK FUNCTIONS
 * (⚠️Framework functions - students don't modify)
 * ======================================== */
void initialize_test_data(char names[][MAX_NAME_LENGTH], 
                         char categories[][MAX_CATEGORY_LENGTH],
                         float* prices, int* count);

void run_test_suite(void);
void test_function_1(void);
void test_function_2(void);
void test_function_3(void);
void test_function_4(void);
void test_function_5(void);
void test_function_6(void);

/* ========================================
 * INPUT/OUTPUT PROCESSING
 * (⚠️Framework functions - students don't modify)
 * ======================================== */
int read_test_command(FILE* input, char* command, char* params);
void write_test_result(FILE* output, const char* test_name, const char* result);
void process_input_file(const char* input_filename, const char* output_filename);

#endif /* RESTAURANT_H */