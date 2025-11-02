/*####################################################
   DRIVER CODE
   💀 Students please don't modify this file!
####################################################*/

#include "restaurant.h"

/* ========================================
 * TIMEOUT AND SAFETY GLOBALS
 * ======================================== */
volatile int timeout_occurred = 0;

/* ========================================
 * GLOBAL TEST VARIABLES (for timeout context)
 * ======================================== */
static char g_test_input[MAX_NAME_LENGTH];
static char g_search_name[MAX_NAME_LENGTH];
static int g_expected_index;
static float g_prices[MAX_MENU_ITEMS];
static int g_prices_count;
static float g_expected_sum;
static char g_item_name[MAX_NAME_LENGTH];
static char g_item_category[MAX_CATEGORY_LENGTH];
static float g_item_price;
static char g_expected_result[20];
static float g_order_prices[MAX_MENU_ITEMS];
static int g_order_quantities[MAX_MENU_ITEMS];
static int g_order_size;
static float g_exp_subtotal, g_exp_tax, g_exp_total;
static char g_sort_type[20];

/* ========================================
 * TIMEOUT HANDLING IMPLEMENTATION
 * ======================================== */
void setup_timeout_handler(void)
{
    signal(SIGALRM, timeout_handler);
}

void timeout_handler(int sig)
{
    timeout_occurred = 1;
    (void)sig;
}

/* ========================================
 * UTILITY FUNCTIONS
 * ======================================== */
void safe_strncpy(char *dest, const char *src, size_t size)
{
    if (dest && src && size > 0)
    {
        strncpy(dest, src, size - 1);
        dest[size - 1] = '\0';
    }
}

int safe_strcmp(const char *str1, const char *str2)
{
    if (!str1 || !str2)
        return -1;
    return strcmp(str1, str2);
}

void parse_float_array(const char *input, float *array, int *count, int max_count)
{
    *count = 0;
    char *input_copy = malloc(strlen(input) + 1);
    strcpy(input_copy, input);

    char *token = strtok(input_copy, ",");
    while (token && *count < max_count)
    {
        array[*count] = atof(token);
        (*count)++;
        token = strtok(NULL, ",");
    }

    free(input_copy);
}

void parse_int_array(const char *input, int *array, int *count, int max_count)
{
    *count = 0;
    char *input_copy = malloc(strlen(input) + 1);
    strcpy(input_copy, input);

    char *token = strtok(input_copy, ",");
    while (token && *count < max_count)
    {
        array[*count] = atoi(token);
        (*count)++;
        token = strtok(NULL, ",");
    }

    free(input_copy);
}

/* ========================================
 * COMPARISON FUNCTIONS (PROVIDED FOR SORTING)
 * ======================================== */
int compareByName(const void *a, const void *b)
{
    return strcmp((const char *)a, (const char *)b);
}

int compareByPrice(const void *a, const void *b)
{
    float price_a = *((const float *)a);
    float price_b = *((const float *)b);

    if (price_a < price_b)
        return -1;
    if (price_a > price_b)
        return 1;
    return 0;
}

int compareByCategory(const void *a, const void *b)
{
    return strcmp((const char *)a, (const char *)b);
}

/* ========================================
 * ATOMIC TEST EXECUTION FUNCTIONS
 * These are the actual test implementations called by timeout wrapper
 * ======================================== */

void execute_copy_test(void)
{
    char source[MAX_NAME_LENGTH];
    char destination[MAX_NAME_LENGTH] = {0};

    safe_strncpy(source, g_test_input, MAX_NAME_LENGTH);

    copyString(destination, source);

    if (safe_strcmp(destination, source) == 0)
    {
        printf("COPY: PASS\n");
    }
    else
    {
        printf("COPY: FAIL Expected='%s' Got='%s'\n", source, destination);
    }
}

void execute_find_test(void)
{
    // Initialize test menu (using underscores to match TESTCASES.txt format)
    char names[MAX_MENU_ITEMS][MAX_NAME_LENGTH];
    safe_strncpy(names[0], "Pizza_Margherita", MAX_NAME_LENGTH);
    safe_strncpy(names[1], "Caesar_Salad", MAX_NAME_LENGTH);
    safe_strncpy(names[2], "Chocolate_Cake", MAX_NAME_LENGTH);
    safe_strncpy(names[3], "Iced_Tea", MAX_NAME_LENGTH);
    safe_strncpy(names[4], "Grilled_Chicken", MAX_NAME_LENGTH);
    int count = 5;

    int result = findMenuItem(names, count, g_search_name);

    if (result == g_expected_index)
    {
        printf("FIND: PASS %d\n", result);
    }
    else
    {
        printf("FIND: FAIL Expected=%d Got=%d\n", g_expected_index, result);
    }
}

void execute_sum_test(void)
{
    float result = calculateSum(g_prices, g_prices_count);

    if (fabs(result - g_expected_sum) < 0.01)
    {
        printf("SUM: PASS %.2f\n", result);
    }
    else
    {
        printf("SUM: FAIL Expected=%.2f Got=%.2f\n", g_expected_sum, result);
    }
}

void execute_add_test(void)
{
    char names[MAX_MENU_ITEMS][MAX_NAME_LENGTH];
    char categories[MAX_MENU_ITEMS][MAX_CATEGORY_LENGTH];
    float prices[MAX_MENU_ITEMS];
    int count = 3; // Start with 3 items

    // Initialize existing items
    safe_strncpy(names[0], "Pizza", MAX_NAME_LENGTH);
    safe_strncpy(categories[0], "Main", MAX_CATEGORY_LENGTH);
    prices[0] = 12.99;

    safe_strncpy(names[1], "Salad", MAX_NAME_LENGTH);
    safe_strncpy(categories[1], "Appetizer", MAX_CATEGORY_LENGTH);
    prices[1] = 8.50;

    safe_strncpy(names[2], "Cake", MAX_NAME_LENGTH);
    safe_strncpy(categories[2], "Dessert", MAX_CATEGORY_LENGTH);
    prices[2] = 6.75;

    int initial_count = count; // ✅ Save initial count

    int result = addMenuItem(names, categories, prices, &count,
                             g_item_name, g_item_category, g_item_price);

    if (strcmp(g_expected_result, "SUCCESS") == 0)
    {
        if (result == OPERATION_SUCCESS)
        {
            // ✅ Verify item was actually added
            int added_correctly = 1;

            // Check if count was incremented
            if (count != initial_count + 1)
            {
                added_correctly = 0;
            }

            // Check if name was copied
            if (added_correctly && strcmp(names[initial_count], g_item_name) != 0)
            {
                added_correctly = 0;
            }

            // Check if category was copied
            if (added_correctly && strcmp(categories[initial_count], g_item_category) != 0)
            {
                added_correctly = 0;
            }

            // Check if price was stored
            if (added_correctly && fabs(prices[initial_count] - g_item_price) > 0.01)
            {
                added_correctly = 0;
            }

            if (added_correctly)
            {
                printf("ADD: PASS\n");
            }
            else
            {
                printf("ADD: FAIL Item_not_added_correctly\n");
            }
        }
        else
        {
            printf("ADD: FAIL Expected=SUCCESS Got=FAILURE\n");
        }
    }
    else
    {
        if (result == OPERATION_FAILURE)
        {
            // ✅ Verify count wasn't changed for failed additions
            if (count == initial_count)
            {
                printf("ADD: PASS\n");
            }
            else
            {
                printf("ADD: FAIL Count_changed_on_failure\n");
            }
        }
        else
        {
            printf("ADD: FAIL Expected=FAILURE Got=SUCCESS\n");
        }
    }
}

void execute_order_test(void)
{
    float subtotal, tax, total;

    processOrder(g_order_prices, g_order_quantities, g_order_size,
                 &subtotal, &tax, &total);

    // ✅ Verify at least one non-zero value if inputs are valid
    if (g_order_size > 0)
    {
        int has_nonzero_quantity = 0;
        for (int i = 0; i < g_order_size; i++)
        {
            if (g_order_quantities[i] > 0)
            {
                has_nonzero_quantity = 1;
                break;
            }
        }

        // If there are non-zero quantities, total must be non-zero
        if (has_nonzero_quantity && total == 0.0)
        {
            printf("ORDER: FAIL Calculation_not_implemented\n");
            return;
        }
    }

    if (fabs(subtotal - g_exp_subtotal) < 0.01 &&
        fabs(tax - g_exp_tax) < 0.01 &&
        fabs(total - g_exp_total) < 0.01)
    {
        printf("ORDER: PASS %.2f,%.2f,%.2f\n", subtotal, tax, total);
    }
    else
    {
        printf("ORDER: FAIL Expected=%.2f,%.2f,%.2f Got=%.2f,%.2f,%.2f\n",
               g_exp_subtotal, g_exp_tax, g_exp_total, subtotal, tax, total);
    }
}

void execute_sort_test(void)
{
    char names[MAX_MENU_ITEMS][MAX_NAME_LENGTH];
    char categories[MAX_MENU_ITEMS][MAX_CATEGORY_LENGTH];
    float prices[MAX_MENU_ITEMS];
    int count = 5;

    // ✅ UNSORTED test data
    safe_strncpy(names[0], "Pizza_Margherita", MAX_NAME_LENGTH);
    safe_strncpy(categories[0], "Main", MAX_CATEGORY_LENGTH);
    prices[0] = 12.99;

    safe_strncpy(names[1], "Grilled_Chicken", MAX_NAME_LENGTH);
    safe_strncpy(categories[1], "Main", MAX_CATEGORY_LENGTH);
    prices[1] = 15.99;

    safe_strncpy(names[2], "Iced_Tea", MAX_NAME_LENGTH);
    safe_strncpy(categories[2], "Beverage", MAX_CATEGORY_LENGTH);
    prices[2] = 2.99;

    safe_strncpy(names[3], "Chocolate_Cake", MAX_NAME_LENGTH);
    safe_strncpy(categories[3], "Dessert", MAX_CATEGORY_LENGTH);
    prices[3] = 6.75;

    safe_strncpy(names[4], "Caesar_Salad", MAX_NAME_LENGTH);
    safe_strncpy(categories[4], "Appetizer", MAX_CATEGORY_LENGTH);
    prices[4] = 8.50;

    CompareFunction compare_func = NULL;
    if (strcmp(g_sort_type, "PRICE") == 0)
    {
        compare_func = compareByPrice;
    }
    else if (strcmp(g_sort_type, "NAME") == 0)
    {
        compare_func = compareByName;
    }
    else if (strcmp(g_sort_type, "CATEGORY") == 0)
    {
        compare_func = compareByCategory;
    }

    if (compare_func)
    {
        // Call student's sortMenu function
        sortMenu(names, categories, prices, count, compare_func);

        // ✅ NEW: Verify the data is actually sorted
        int is_sorted = 1;
        for (int i = 0; i < count - 1; i++)
        {
            int cmp_result = 0;

            if (strcmp(g_sort_type, "PRICE") == 0)
            {
                cmp_result = compareByPrice(&prices[i], &prices[i + 1]);
            }
            else if (strcmp(g_sort_type, "NAME") == 0)
            {
                cmp_result = compareByName(names[i], names[i + 1]);
            }
            else if (strcmp(g_sort_type, "CATEGORY") == 0)
            {
                cmp_result = compareByCategory(categories[i], categories[i + 1]);
            }

            if (cmp_result > 0)
            {
                is_sorted = 0;
                break;
            }
        }

        // Print result based on verification
        if (is_sorted)
        {
            printf("SORT_%s: PASS\n", g_sort_type);
            for (int i = 0; i < count; i++)
            {
                printf("ITEM: %s,%.2f\n", names[i], prices[i]);
            }
        }
        else
        {
            printf("SORT_%s: FAIL Data_not_sorted\n", g_sort_type);
            // Still print items for debugging
            for (int i = 0; i < count; i++)
            {
                printf("ITEM: %s,%.2f\n", names[i], prices[i]);
            }
        }
    }
    else
    {
        printf("SORT_%s: FAIL Invalid_sort_type\n", g_sort_type);
    }
}

/* ========================================
 * VALIDATION TEST EXECUTION FUNCTIONS
 * ======================================== */

void execute_find_zero_count_test(void)
{
    char names[MAX_MENU_ITEMS][MAX_NAME_LENGTH];
    int result = findMenuItem(names, 0, g_search_name); // count = 0

    if (result == g_expected_index)
    {
        printf("FIND_ZERO_COUNT: PASS %d\n", result);
    }
    else
    {
        printf("FIND_ZERO_COUNT: FAIL Expected=%d Got=%d\n", g_expected_index, result);
    }
}

void execute_find_large_count_test(void)
{
    char names[MAX_MENU_ITEMS][MAX_NAME_LENGTH];
    safe_strncpy(names[0], "Pizza_Margherita", MAX_NAME_LENGTH);
    safe_strncpy(names[1], "Caesar_Salad", MAX_NAME_LENGTH);

    // FIXED: Use a reasonable count that tests boundary conditions
    // but doesn't exceed actual array bounds
    int result = findMenuItem(names, MAX_MENU_ITEMS, g_search_name);

    if (result == g_expected_index)
    {
        printf("FIND_LARGE_COUNT: PASS %d\n", result);
    }
    else
    {
        printf("FIND_LARGE_COUNT: FAIL Expected=%d Got=%d\n", g_expected_index, result);
    }
}

void execute_sum_negative_count_test(void)
{
    float result = calculateSum(g_prices, -1); // Negative count

    if (fabs(result - g_expected_sum) < 0.01)
    {
        printf("SUM_NEGATIVE_COUNT: PASS %.2f\n", result);
    }
    else
    {
        printf("SUM_NEGATIVE_COUNT: FAIL Expected=%.2f Got=%.2f\n", g_expected_sum, result);
    }
}

void execute_add_at_capacity_test(void)
{
    char names[MAX_MENU_ITEMS][MAX_NAME_LENGTH];
    char categories[MAX_MENU_ITEMS][MAX_CATEGORY_LENGTH];
    float prices[MAX_MENU_ITEMS];
    int count = MAX_MENU_ITEMS; // Array at capacity

    // Fill array to capacity
    for (int i = 0; i < MAX_MENU_ITEMS; i++)
    {
        snprintf(names[i], MAX_NAME_LENGTH, "Item_%d", i);
        safe_strncpy(categories[i], "Main", MAX_CATEGORY_LENGTH);
        prices[i] = 10.0;
    }

    int result = addMenuItem(names, categories, prices, &count,
                             g_item_name, g_item_category, g_item_price);

    if (strcmp(g_expected_result, "SUCCESS") == 0)
    {
        if (result == OPERATION_SUCCESS)
        {
            printf("ADD_AT_CAPACITY: PASS\n");
        }
        else
        {
            printf("ADD_AT_CAPACITY: FAIL Expected=SUCCESS Got=FAILURE\n");
        }
    }
    else
    {
        if (result == OPERATION_FAILURE)
        {
            printf("ADD_AT_CAPACITY: PASS\n");
        }
        else
        {
            printf("ADD_AT_CAPACITY: FAIL Expected=FAILURE Got=SUCCESS\n");
        }
    }
}

void execute_order_negative_size_test(void)
{
    float order_prices[MAX_MENU_ITEMS] = {10.0};
    int order_quantities[MAX_MENU_ITEMS] = {1};
    float subtotal, tax, total;

    processOrder(order_prices, order_quantities, -1, &subtotal, &tax, &total); // Negative size

    if (fabs(subtotal - g_exp_subtotal) < 0.01 &&
        fabs(tax - g_exp_tax) < 0.01 &&
        fabs(total - g_exp_total) < 0.01)
    {
        printf("ORDER_NEGATIVE_SIZE: PASS %.2f,%.2f,%.2f\n", subtotal, tax, total);
    }
    else
    {
        printf("ORDER_NEGATIVE_SIZE: FAIL Expected=%.2f,%.2f,%.2f Got=%.2f,%.2f,%.2f\n",
               g_exp_subtotal, g_exp_tax, g_exp_total, subtotal, tax, total);
    }
}

void execute_sort_single_item_test(void)
{
    char names[MAX_MENU_ITEMS][MAX_NAME_LENGTH];
    char categories[MAX_MENU_ITEMS][MAX_CATEGORY_LENGTH];
    float prices[MAX_MENU_ITEMS];
    int count = 1; // Only one item

    safe_strncpy(names[0], "Single_Item", MAX_NAME_LENGTH);
    safe_strncpy(categories[0], "Main", MAX_CATEGORY_LENGTH);
    prices[0] = 15.99;

    CompareFunction compare_func = NULL;
    if (strcmp(g_sort_type, "PRICE") == 0)
    {
        compare_func = compareByPrice;
    }
    else if (strcmp(g_sort_type, "NAME") == 0)
    {
        compare_func = compareByName;
    }
    else if (strcmp(g_sort_type, "CATEGORY") == 0)
    {
        compare_func = compareByCategory;
    }

    if (compare_func)
    {
        sortMenu(names, categories, prices, count, compare_func);
        printf("SORT_SINGLE_ITEM: PASS\n");
        printf("ITEM: %s,%.2f\n", names[0], prices[0]);
    }
    else
    {
        printf("SORT_SINGLE_ITEM: FAIL Invalid_sort_type\n");
    }
}

void execute_sort_identical_values_test(void)
{
    char names[MAX_MENU_ITEMS][MAX_NAME_LENGTH];
    char categories[MAX_MENU_ITEMS][MAX_CATEGORY_LENGTH];
    float prices[MAX_MENU_ITEMS];
    int count = 3;

    // All items have identical prices
    safe_strncpy(names[0], "Item_A", MAX_NAME_LENGTH);
    safe_strncpy(names[1], "Item_B", MAX_NAME_LENGTH);
    safe_strncpy(names[2], "Item_C", MAX_NAME_LENGTH);

    safe_strncpy(categories[0], "Main", MAX_CATEGORY_LENGTH);
    safe_strncpy(categories[1], "Main", MAX_CATEGORY_LENGTH);
    safe_strncpy(categories[2], "Main", MAX_CATEGORY_LENGTH);

    prices[0] = prices[1] = prices[2] = 15.99; // Identical prices

    CompareFunction compare_func = NULL;
    if (strcmp(g_sort_type, "PRICE") == 0)
    {
        compare_func = compareByPrice;
    }

    if (compare_func)
    {
        sortMenu(names, categories, prices, count, compare_func);
        printf("SORT_IDENTICAL_VALUES: PASS\n");
        for (int i = 0; i < count; i++)
        {
            printf("ITEM: %s,%.2f\n", names[i], prices[i]);
        }
    }
    else
    {
        printf("SORT_IDENTICAL_VALUES: FAIL Invalid_sort_type\n");
    }
}

void execute_copy_max_length_test(void)
{
    char source[MAX_NAME_LENGTH];
    char destination[MAX_NAME_LENGTH] = {0};

    // Create a string that's exactly the maximum length
    strncpy(source, g_test_input, MAX_NAME_LENGTH - 1);
    source[MAX_NAME_LENGTH - 1] = '\0';

    copyString(destination, source);

    if (safe_strcmp(destination, source) == 0)
    {
        printf("COPY_MAX_LENGTH: PASS\n");
    }
    else
    {
        printf("COPY_MAX_LENGTH: FAIL Expected='%s' Got='%s'\n", source, destination);
    }
}

/* ========================================
 * SAFE EXECUTION WRAPPER
 * ======================================== */
int execute_with_timeout(void (*test_func)(void), int timeout_seconds)
{
    timeout_occurred = 0;

    alarm(timeout_seconds);
    test_func();
    alarm(0);

    return !timeout_occurred;
}

/* ========================================
 * PUBLIC TEST INTERFACE FUNCTIONS
 * ======================================== */

void test_copy_string(const char *test_input)
{
    safe_strncpy(g_test_input, test_input, MAX_NAME_LENGTH);

    if (!execute_with_timeout(execute_copy_test, 3))
    {
        printf("COPY: TIMEOUT\n");
    }
}

void test_find_menu_item(const char *search_name, int expected_index)
{
    safe_strncpy(g_search_name, search_name, MAX_NAME_LENGTH);
    g_expected_index = expected_index;

    if (!execute_with_timeout(execute_find_test, 3))
    {
        printf("FIND: TIMEOUT\n");
    }
}

void test_calculate_sum(const char *prices_str, float expected_sum)
{
    parse_float_array(prices_str, g_prices, &g_prices_count, MAX_MENU_ITEMS);
    g_expected_sum = expected_sum;

    if (!execute_with_timeout(execute_sum_test, 3))
    {
        printf("SUM: TIMEOUT\n");
    }
}

void test_add_menu_item(const char *name, const char *category, float price, const char *expected_result)
{
    safe_strncpy(g_item_name, name, MAX_NAME_LENGTH);
    safe_strncpy(g_item_category, category, MAX_CATEGORY_LENGTH);
    g_item_price = price;
    safe_strncpy(g_expected_result, expected_result, sizeof(g_expected_result));

    if (!execute_with_timeout(execute_add_test, 3))
    {
        printf("ADD: TIMEOUT\n");
    }
}

void test_process_order(const char *order_data, float exp_subtotal, float exp_tax, float exp_total)
{
    char *data_copy = malloc(strlen(order_data) + 1);
    strcpy(data_copy, order_data);

    char *prices_part = strtok(data_copy, ":");
    char *quantities_part = strtok(NULL, ":");

    int price_count = 0, qty_count = 0;

    if (prices_part)
    {
        parse_float_array(prices_part, g_order_prices, &price_count, MAX_MENU_ITEMS);
    }
    if (quantities_part)
    {
        parse_int_array(quantities_part, g_order_quantities, &qty_count, MAX_MENU_ITEMS);
    }

    g_order_size = (price_count < qty_count) ? price_count : qty_count;
    g_exp_subtotal = exp_subtotal;
    g_exp_tax = exp_tax;
    g_exp_total = exp_total;

    free(data_copy);

    if (!execute_with_timeout(execute_order_test, 3))
    {
        printf("ORDER: TIMEOUT\n");
    }
}

void test_sort_menu(const char *sort_type)
{
    safe_strncpy(g_sort_type, sort_type, sizeof(g_sort_type));

    if (!execute_with_timeout(execute_sort_test, 5))
    {
        printf("SORT_%s: TIMEOUT\n", sort_type);
    }
}

/* ========================================
 * VALIDATION TEST INTERFACE FUNCTIONS
 * ======================================== */

void test_find_zero_count(const char *search_name, int expected_index)
{
    safe_strncpy(g_search_name, search_name, MAX_NAME_LENGTH);
    g_expected_index = expected_index;

    if (!execute_with_timeout(execute_find_zero_count_test, 3))
    {
        printf("FIND_ZERO_COUNT: TIMEOUT\n");
    }
}

void test_find_large_count(const char *search_name, int expected_index)
{
    safe_strncpy(g_search_name, search_name, MAX_NAME_LENGTH);
    g_expected_index = expected_index;

    if (!execute_with_timeout(execute_find_large_count_test, 3))
    {
        printf("FIND_LARGE_COUNT: TIMEOUT\n");
    }
}

void test_sum_negative_count(const char *prices_str, float expected_sum)
{
    parse_float_array(prices_str, g_prices, &g_prices_count, MAX_MENU_ITEMS);
    g_expected_sum = expected_sum;

    if (!execute_with_timeout(execute_sum_negative_count_test, 3))
    {
        printf("SUM_NEGATIVE_COUNT: TIMEOUT\n");
    }
}

void test_add_at_capacity(const char *name, const char *category, float price, const char *expected_result)
{
    safe_strncpy(g_item_name, name, MAX_NAME_LENGTH);
    safe_strncpy(g_item_category, category, MAX_CATEGORY_LENGTH);
    g_item_price = price;
    safe_strncpy(g_expected_result, expected_result, sizeof(g_expected_result));

    if (!execute_with_timeout(execute_add_at_capacity_test, 3))
    {
        printf("ADD_AT_CAPACITY: TIMEOUT\n");
    }
}

void test_order_negative_size(const char *order_data, float exp_subtotal, float exp_tax, float exp_total)
{
    (void)order_data;
    g_exp_subtotal = exp_subtotal;
    g_exp_tax = exp_tax;
    g_exp_total = exp_total;

    if (!execute_with_timeout(execute_order_negative_size_test, 3))
    {
        printf("ORDER_NEGATIVE_SIZE: TIMEOUT\n");
    }
}

void test_sort_single_item(const char *sort_type)
{
    safe_strncpy(g_sort_type, sort_type, sizeof(g_sort_type));

    if (!execute_with_timeout(execute_sort_single_item_test, 3))
    {
        printf("SORT_SINGLE_ITEM: TIMEOUT\n");
    }
}

void test_sort_identical_values(const char *sort_type)
{
    safe_strncpy(g_sort_type, sort_type, sizeof(g_sort_type));

    if (!execute_with_timeout(execute_sort_identical_values_test, 3))
    {
        printf("SORT_IDENTICAL_VALUES: TIMEOUT\n");
    }
}

void test_copy_max_length(const char *test_input)
{
    safe_strncpy(g_test_input, test_input, MAX_NAME_LENGTH);

    if (!execute_with_timeout(execute_copy_max_length_test, 3))
    {
        printf("COPY_MAX_LENGTH: TIMEOUT\n");
    }
}

/* ========================================
 * MAIN TEST FILE PROCESSOR
 * ======================================== */
void process_test_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("ERROR: Cannot open test file %s\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = 0;

        if (line[0] == '#' || line[0] == '\0')
        {
            continue;
        }

        // Regular test cases
        if (strncmp(line, "TEST_COPY ", 10) == 0)
        {
            test_copy_string(line + 10);
        }
        else if (strncmp(line, "TEST_FIND ", 10) == 0)
        {
            char search_name[MAX_NAME_LENGTH];
            int expected_index;
            if (sscanf(line + 10, "%s %d", search_name, &expected_index) == 2)
            {
                test_find_menu_item(search_name, expected_index);
            }
        }
        else if (strncmp(line, "TEST_SUM ", 9) == 0)
        {
            char prices_str[256];
            float expected_sum;
            if (sscanf(line + 9, "%s %f", prices_str, &expected_sum) == 2)
            {
                test_calculate_sum(prices_str, expected_sum);
            }
        }
        else if (strncmp(line, "TEST_ADD ", 9) == 0)
        {
            char name[MAX_NAME_LENGTH], category[MAX_CATEGORY_LENGTH], result[20];
            float price;
            if (sscanf(line + 9, "%s %s %f %s", name, category, &price, result) == 4)
            {
                test_add_menu_item(name, category, price, result);
            }
        }
        else if (strncmp(line, "TEST_ORDER ", 11) == 0)
        {
            char order_data[256];
            float subtotal, tax, total;
            if (sscanf(line + 11, "%s %f,%f,%f", order_data, &subtotal, &tax, &total) == 4)
            {
                test_process_order(order_data, subtotal, tax, total);
            }
        }
        else if (strncmp(line, "TEST_SORT ", 10) == 0)
        {
            char sort_type[20];
            if (sscanf(line + 10, "%s", sort_type) == 1)
            {
                test_sort_menu(sort_type);
            }
        }

        // Validation test cases
        else if (strncmp(line, "TEST_FIND_ZERO_COUNT ", 21) == 0)
        {
            char search_name[MAX_NAME_LENGTH];
            int expected_index;
            if (sscanf(line + 21, "%s %d", search_name, &expected_index) == 2)
            {
                test_find_zero_count(search_name, expected_index);
            }
        }
        else if (strncmp(line, "TEST_FIND_LARGE_COUNT ", 22) == 0)
        {
            char search_name[MAX_NAME_LENGTH];
            int expected_index;
            if (sscanf(line + 22, "%s %d", search_name, &expected_index) == 2)
            {
                test_find_large_count(search_name, expected_index);
            }
        }
        else if (strncmp(line, "TEST_SUM_NEGATIVE_COUNT ", 24) == 0)
        {
            char prices_str[256];
            float expected_sum;
            if (sscanf(line + 24, "%s %f", prices_str, &expected_sum) == 2)
            {
                test_sum_negative_count(prices_str, expected_sum);
            }
        }
        else if (strncmp(line, "TEST_ADD_AT_CAPACITY ", 21) == 0)
        {
            char name[MAX_NAME_LENGTH], category[MAX_CATEGORY_LENGTH], result[20];
            float price;
            if (sscanf(line + 21, "%s %s %f %s", name, category, &price, result) == 4)
            {
                test_add_at_capacity(name, category, price, result);
            }
        }
        else if (strncmp(line, "TEST_ORDER_NEGATIVE_SIZE ", 25) == 0)
        {
            char order_data[256];
            float subtotal, tax, total;
            if (sscanf(line + 25, "%s %f,%f,%f", order_data, &subtotal, &tax, &total) == 4)
            {
                test_order_negative_size(order_data, subtotal, tax, total);
            }
        }
        else if (strncmp(line, "TEST_SORT_SINGLE_ITEM ", 22) == 0)
        {
            char sort_type[20];
            if (sscanf(line + 22, "%s", sort_type) == 1)
            {
                test_sort_single_item(sort_type);
            }
        }
        else if (strncmp(line, "TEST_SORT_IDENTICAL_VALUES ", 27) == 0)
        {
            char sort_type[20];
            if (sscanf(line + 27, "%s", sort_type) == 1)
            {
                test_sort_identical_values(sort_type);
            }
        }
        else if (strncmp(line, "TEST_COPY_MAX_LENGTH ", 21) == 0)
        {
            test_copy_max_length(line + 21);
        }
    }

    fclose(file);
}

/* ========================================
 * MAIN FUNCTION
 * ======================================== */
int main(int argc, char *argv[])
{
    setup_timeout_handler();

    const char *test_file = "TESTCASES.txt";
    if (argc > 1)
    {
        test_file = argv[1];
    }

    process_test_file(test_file);

    // Remove trailing newline by seeking back and truncating
    fflush(stdout);

    return 0;
}