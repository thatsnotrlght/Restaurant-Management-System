/*
 * findMenuItem()
 * 
 * OBJECTIVE: Find menu item by name using linear search algorithm
 *            Demonstrate search algorithms with loop safety implementation
 * 
 * PARAMETERS:
 * - names[][MAX_NAME_LENGTH]: 2D array of menu item names
 * - count: number of items in the menu
 * - searchName: name of item to find (const char*)
 * 
 * RETURNS:
 * - Index of found item (0 to count-1) if found
 * - ITEM_NOT_FOUND (-1) if item not found
 * 
 * REQUIREMENTS:
 * - Use linear search algorithm with safe string comparison
 * - Implement loop safety protection against infinite loops
 * - Return immediately when item is found (early termination)
 * - Handle edge cases and invalid input appropriately
 * - Search is case-sensitive
 * 
 * SEARCH CONCEPTS:
 * - Linear search algorithm implementation
 * - Safe string comparison using strncmp()
 * - Early termination optimization
 * - Return value conventions for search results
 * 
 * LOOP SAFETY CONCEPTS:
 * - Infinite loop protection using iteration counters
 * - Breaking out of loops when safety limits exceeded
 * - Maximum iteration limits for safety
 * 
 * STRING SAFETY CONCEPTS:
 * - Using strncmp() for bounded string comparison
 * - Buffer-safe string operations
 * - Understanding string comparison return values
 * 
 * RELEVANT CONSTANTS:
 * - ITEM_NOT_FOUND: Return value for unsuccessful search
 * - MAX_LOOP_ITERATIONS: Maximum allowed loop iterations for safety
 * - MAX_NAME_LENGTH: Maximum length for string comparison bounds
 */

#include "restaurant.h"

int findMenuItem(char names[][MAX_NAME_LENGTH], int count, const char* searchName) {
    
    // TODO: Implement linear search with loop safety protection
    //
    // 1. INPUT VALIDATION:
    //    - Check for NULL pointers (names, searchName)
    //    - Validate count is positive
    //    - Return ITEM_NOT_FOUND for invalid input
    //
    // 2. INITIALIZE VARIABLES:
    //    - Declare counter variable for loop safety protection
    //    - Initialize appropriately
    //
    // 3. LINEAR SEARCH WITH SAFETY:
    //    - Loop through all menu items
    //    - Implement safety check: increment counter each iteration
    //    - Break loop if counter exceeds MAX_LOOP_ITERATIONS
    //    - Compare current item name with searchName using strncmp()
    //    - Return index immediately if match found (early termination)
    //
    // 4. RETURN RESULT:
    //    - Return ITEM_NOT_FOUND if no match found
    //
    // String Safety Tips:
    // - Use strncmp(str1, str2, max_length) for bounded comparison
    // - strncmp() returns 0 when strings are equal
    // Safety Tip: Check loop counter against MAX_LOOP_ITERATIONS constant
    
    // Your implementation here:
    
}