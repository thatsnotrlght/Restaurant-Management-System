/*
 * addMenuItem()
 *  
 * OBJECTIVE: Add new menu item to arrays and update count using call-by-reference
 * 
 * PARAMETERS:
 * - names[][MAX_NAME_LENGTH]: 2D array to store item names
 * - categories[][MAX_CATEGORY_LENGTH]: 2D array to store categories  
 * - prices: 1D array to store prices (float*)
 * - count: pointer to current number of items (int*)
 * - name: name of new item to add (const char*)
 * - category: category of new item (const char*)
 * - price: price of new item (float)
 * 
 * RETURNS:
 * - OPERATION_SUCCESS (1) if item added successfully
 * - OPERATION_FAILURE (0) if operation failed
 * 
 * REQUIREMENTS:
 * - Implement safe string copying using strncpy() to prevent buffer overflow
 * - Validate all inputs before processing
 * - Use proper pointer dereferencing for count modification
 * - Ensure proper null termination of copied strings
 * 
 * STRING SAFETY CONCEPTS:
 * - Using strncpy() for bounded string copying
 * - Manual null termination after strncpy()
 * - Buffer overflow prevention with length limits
 * 
 * RELEVANT CONSTANTS:
 * - OPERATION_SUCCESS, OPERATION_FAILURE (return values)
 * - MAX_MENU_ITEMS (array capacity limit)
 * - MIN_PRICE, MAX_PRICE (price validation range)
 * - MAX_NAME_LENGTH, MAX_CATEGORY_LENGTH (string buffer limits)
 */

#include "restaurant.h"

int addMenuItem(char names[][MAX_NAME_LENGTH], 
               char categories[][MAX_CATEGORY_LENGTH],
               float* prices, int* count,
               const char* name, const char* category, float price) {
    
    // TODO: Implement addMenuItem with proper validation and string safety
    //
    // 1. INPUT VALIDATION:
    //    - Check all pointer parameters for NULL
    //    - Verify name and category are not empty strings (check first character)
    //    - Return OPERATION_FAILURE if any validation fails
    //
    // 2. CAPACITY AND PRICE VALIDATION:
    //    - Check if array has space available (use MAX_MENU_ITEMS constant)
    //    - Validate price is within acceptable range (use MIN_PRICE and MAX_PRICE constants)
    //    - Return OPERATION_FAILURE if validation fails
    //
    // 3. SAFE STRING COPYING:
    //    - Determine the correct index for the new item
    //    - Use strncpy() to copy strings safely with proper length limits
    //    - Ensure null termination after each strncpy() call
    //    - Copy both name and category using this approach
    //
    // 4. ADD PRICE AND UPDATE COUNT:
    //    - Store the price in the appropriate array position
    //    - Update the count to reflect the new item
    //    - Return OPERATION_SUCCESS
    //
    // String Safety Tips: 
    // - Use strncpy(dest, src, max_length-1) to leave space for null terminator
    // - Always manually set the last character to '\0' after strncpy()
    // Remember: Use dereferencing (*) to access/modify count value through pointer
    
    // Your implementation here:
    if (names == NULL || categories == NULL || prices == NULL || 
    name == NULL || category == NULL || count == NULL) {
    return OPERATION_FAILURE;
    }

    if (*name == '\0' || *category == '\0') {
        return OPERATION_FAILURE;
    }

    if (price < MIN_PRICE || price > MAX_PRICE) {
        return OPERATION_FAILURE;
    }

    if (*count >= MAX_MENU_ITEMS) {
        return OPERATION_FAILURE;
    }

    int index = *count;
    strncpy(names[index], name, MAX_NAME_LENGTH - 1);
    names[index][MAX_NAME_LENGTH - 1] = '\0';

    strncpy(categories[index], category, MAX_CATEGORY_LENGTH - 1);
    categories[index][MAX_CATEGORY_LENGTH - 1] = '\0';

    prices[index] = price;
    (*count)++;

    return OPERATION_SUCCESS;
}