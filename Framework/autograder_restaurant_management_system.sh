#!/bin/bash

# ===============================================================================
# ENHANCED RESTAURANT MANAGEMENT SYSTEM AUTOGRADER
# With detailed debugging output for easier error identification
# ===============================================================================

echo "=========================================="
echo "  Restaurant Management Autograder"
echo "=========================================="
echo ""

# Initialize scores
compilation_score=0
copy_score=0
find_score=0
sum_score=0
add_score=0
order_score=0
sort_score=0

# Step 1: Check required files
echo "🔍 Checking required files..."
required_files=("EXPECTED_OUTPUT.txt" "TESTCASES.txt" "restaurant.h" 
                "copyString.c" "findMenuItem.c" "calculateSum.c" 
                "addMenuItem.c" "processOrder.c" "sortMenu.c" "driver.c")

missing_files=()
for file in "${required_files[@]}"; do
    if [ ! -f "$file" ]; then
        missing_files+=("$file")
    fi
done

if [ ${#missing_files[@]} -ne 0 ]; then
    echo "❌ ERROR: Missing files: ${missing_files[*]}"
    exit 1
fi

if [ ! -f "makefile" ] && [ ! -f "Makefile" ]; then
    echo "❌ ERROR: makefile not found!"
    exit 1
fi

echo "✅ All required files found"

# Step 2: Clean rebuild with timeout protection
echo "🔨 Building restaurant system..."
if timeout 10 make clean >/dev/null 2>&1; then
    echo "   Clean completed"
else
    echo "   Clean skipped (not critical)"
fi

if timeout 15 make >/dev/null 2>&1; then
    compilation_score=10
    echo "✅ Compilation successful"
else
    echo "❌ Compilation failed"
    make 2>&1 | head -10  # Show first 10 lines of errors
    exit 1
fi

# Check if executable was created
if [ ! -x "restaurant" ]; then
    echo "❌ ERROR: restaurant executable not created"
    exit 1
fi

# Step 3: Generate student output with timeout protection
echo "🚀 Running restaurant system..."
if timeout 20 ./restaurant TESTCASES.txt > STUDENT_OUTPUT_RAW.txt 2>&1; then
    echo "✅ Restaurant system executed"    
    # Remove trailing newline and create final output
    printf '%s' "$(cat STUDENT_OUTPUT_RAW.txt)" > STUDENT_OUTPUT.txt
    rm STUDENT_OUTPUT_RAW.txt
else
    exit_code=$?
    if [ $exit_code -eq 124 ]; then
        echo "❌ Restaurant system timed out (possible infinite loop)"
    else
        echo "❌ Restaurant system crashed or failed"
    fi
    echo "Attempting partial grading with available output..."
fi

echo ""

# Extract function data function
extract_function_data() {
    local function_prefix="$1"
    local file="$2"
    
    if [[ ! -f "$file" ]]; then
        echo ""
        return
    fi
    
    # Extract lines starting with the function prefix
    grep "^${function_prefix}" "$file" 2>/dev/null || echo ""
}

# Compare function data function  
# Compare function data with strict structure but lenient whitespace handling
compare_function_data() {
    local student_data="$1"
    local expected_data="$2"
    local function_name="$3"
    
    if [[ -z "$expected_data" ]]; then
        echo "0"
        return
    fi
    
    if [[ -z "$student_data" ]]; then
        echo "0"
        return
    fi
    
    # Convert to arrays for exact comparison, trimming whitespace
    local -a expected_lines
    local -a student_lines
    
    # Read expected lines and trim whitespace
    while IFS= read -r line; do
        # Trim leading and trailing whitespace
        line=$(echo "$line" | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')
        # Only add non-empty lines
        if [[ -n "$line" ]]; then
            expected_lines+=("$line")
        fi
    done <<< "$expected_data"
    
    # Read student lines and trim whitespace
    while IFS= read -r line; do
        # Trim leading and trailing whitespace
        line=$(echo "$line" | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')
        # Only add non-empty lines
        if [[ -n "$line" ]]; then
            student_lines+=("$line")
        fi
    done <<< "$student_data"
    
    # Must have exact same number of lines (STRICT)
    if [[ ${#student_lines[@]} -ne ${#expected_lines[@]} ]]; then
        echo "0"  # Fail if different number of lines
        return
    fi
    
    # If no lines to compare, return 100%
    if [[ ${#expected_lines[@]} -eq 0 ]]; then
        echo "100"
        return
    fi
    
    # Check each line matches exactly in order (STRICT) after trimming (LENIENT)
    local correct_count=0
    for ((i=0; i<${#expected_lines[@]}; i++)); do
        if [[ "${student_lines[i]}" == "${expected_lines[i]}" ]]; then
            correct_count=$((correct_count + 1))
        fi
    done
    
    # Calculate percentage
    local percentage=$(( (correct_count * 100) / ${#expected_lines[@]} ))
    echo "$percentage"
}

# Enhanced differences function with detailed line-by-line comparison
show_function_differences() {
    local student_data="$1"
    local expected_data="$2"
    local function_name="$3"
    
    echo ""
    echo "   🔍 DETAILED ANALYSIS for $function_name:"
    echo "   =========================================="
    
    # Convert to arrays for line-by-line comparison
    local -a expected_lines
    local -a student_lines
    
    # Read expected lines
    while IFS= read -r line; do
        if [[ -n "$line" ]]; then
            expected_lines+=("$line")
        fi
    done <<< "$expected_data"
    
    # Read student lines  
    while IFS= read -r line; do
        if [[ -n "$line" ]]; then
            student_lines+=("$line")
        fi
    done <<< "$student_data"
    
    echo "   📊 EXPECTED OUTPUT (${#expected_lines[@]} lines):"
    for i in "${!expected_lines[@]}"; do
        printf "   %2d: %s\n" $((i+1)) "${expected_lines[i]}"
    done
    
    echo ""
    echo "   📊 YOUR OUTPUT (${#student_lines[@]} lines):"
    for i in "${!student_lines[@]}"; do
        printf "   %2d: %s\n" $((i+1)) "${student_lines[i]}"
    done
    
    echo ""
    echo "   🔍 LINE-BY-LINE COMPARISON:"
    
    # Find maximum length for alignment
    local max_lines=${#expected_lines[@]}
    if [[ ${#student_lines[@]} -gt $max_lines ]]; then
        max_lines=${#student_lines[@]}
    fi
    
    local mismatches=0
    
    for ((i=0; i<max_lines; i++)); do
        local expected_line="${expected_lines[i]:-}"
        local student_line="${student_lines[i]:-}"
        
        if [[ "$expected_line" == "$student_line" ]]; then
            printf "   %2d: ✅ MATCH    '%s'\n" $((i+1)) "$expected_line"
        else
            printf "   %2d: ❌ DIFF     Expected: '%s'\n" $((i+1)) "$expected_line"
            printf "   %2s            Your:     '%s'\n" "" "$student_line"
            mismatches=$((mismatches + 1))
        fi
    done
    
    echo ""
    echo "   📈 SUMMARY: $mismatches mismatches out of $max_lines lines"
    
    if [[ $mismatches -gt 0 ]]; then
        echo "   💡 DEBUGGING HINTS:"
        
        # Provide specific hints based on function name
        case $function_name in
            "copyString")
                echo "      - Check if strings are copied correctly character by character"
                echo "      - Ensure null termination is handled properly"
                echo "      - Verify pointer arithmetic is used (no array indexing)"
                ;;
            "findMenuItem")
                echo "      - Check if linear search returns correct index"
                echo "      - Verify string comparison using strncmp()"
                echo "      - Ensure ITEM_NOT_FOUND (-1) is returned for missing items"
                ;;
            "calculateSum")
                echo "      - Check if pointer arithmetic is used for array traversal"
                echo "      - Verify accumulation logic"
                echo "      - Ensure proper handling of edge cases (empty array, negative count)"
                ;;
            "addMenuItem")
                echo "      - Check input validation (NULL pointers, empty strings, price range)"
                echo "      - Verify array capacity checking"
                echo "      - Ensure proper string copying with strncpy() and null termination"
                ;;
            "processOrder")
                echo "      - Check parallel array processing (prices × quantities)"
                echo "      - Verify tax calculation (subtotal × TAX_RATE)"
                echo "      - Ensure proper handling of zero quantities"
                echo "      - Check if all return values are set via pointers"
                ;;
            "sortMenu")
                echo "      - Check if function pointer detection works correctly"
                echo "      - Verify all three arrays are sorted simultaneously"
                echo "      - Ensure data consistency across arrays after sorting"
                ;;
        esac
    fi
    
    echo "   =========================================="
    echo ""
}

# Step 4: Compare and grade functions
functions=("COPY:copyString:10" "FIND:findMenuItem:10" "SUM:calculateSum:10" 
           "ADD:addMenuItem:15" "ORDER:processOrder:15" "SORT:sortMenu:20")

echo "🔍 Grading Function Implementations..."
echo ""

for func_info in "${functions[@]}"; do
    prefix=$(echo "$func_info" | cut -d':' -f1)
    function_name=$(echo "$func_info" | cut -d':' -f2)
    max_points=$(echo "$func_info" | cut -d':' -f3)

    echo "   Testing $function_name..."

    student_data=$(extract_function_data "$prefix" "STUDENT_OUTPUT.txt")
    expected_data=$(extract_function_data "$prefix" "EXPECTED_OUTPUT.txt")

    if [[ -n "$expected_data" ]]; then
        if [[ -n "$student_data" ]]; then
            match_percentage=$(compare_function_data "$student_data" "$expected_data" "$function_name")
            points=$(( (max_points * match_percentage) / 100 ))
        else
            match_percentage=0
            points=0
        fi
    else
        match_percentage=0
        points=0
        echo "   ⚠️  No expected data found for $function_name"
    fi

    # Store scores in corresponding variables
    case $prefix in
        "COPY") copy_score=$points ;;
        "FIND") find_score=$points ;;
        "SUM") sum_score=$points ;;
        "ADD") add_score=$points ;;
        "ORDER") order_score=$points ;;
        "SORT") sort_score=$points ;;
    esac

    # Show results with enhanced debugging for non-perfect scores
    if [[ $match_percentage -eq 100 ]]; then
        echo "   ✅ $function_name: Perfect implementation ($points/$max_points points)"
    elif [[ $match_percentage -ge 80 ]]; then
        echo "   🟡 $function_name: Good implementation ($match_percentage% match, $points/$max_points points)"
        show_function_differences "$student_data" "$expected_data" "$function_name"
    else
        echo "   ❌ $function_name: Issues detected ($match_percentage% match, $points/$max_points points)"
        show_function_differences "$student_data" "$expected_data" "$function_name"
    fi
done

# Step 5: Final results
total_score=$((compilation_score + copy_score + find_score + sum_score + add_score + order_score + sort_score))
max_total=90
percentage=$(( (total_score * 100) / max_total ))

echo ""
echo "=========================================="
echo "           AUTOGRADER RESULTS"
echo "=========================================="
echo "Compilation & Build:      $compilation_score/10 points"
echo "copyString:               $copy_score/10 points"  
echo "findMenuItem:             $find_score/10 points"
echo "calculateSum:             $sum_score/10 points"
echo "addMenuItem:              $add_score/15 points"
echo "processOrder:             $order_score/15 points"
echo "sortMenu:                 $sort_score/20 points"
echo "----------------------------------------"
echo "AUTOGRADER TOTAL:         $total_score/$max_total points"
echo "AUTOGRADER PERCENTAGE:    $percentage%"
echo "----------------------------------------"
echo "Manual Grading:           /10 points (Code Quality, Comments, README)"
echo "FINAL TOTAL:              /$((max_total + 10)) points"
echo "=========================================="

echo ""

# Grade classification
if [[ $total_score -eq $max_total ]]; then
    echo "🎉 PERFECT! All functions working correctly!"
elif [[ $total_score -ge 72 ]]; then  # 90% of 80
    echo "🌟 EXCELLENT! Outstanding implementation!"
elif [[ $total_score -ge 64 ]]; then  # 80% of 80
    echo "👍 VERY GOOD! Strong implementation!"
elif [[ $total_score -ge 56 ]]; then  # 70% of 80
    echo "✅ GOOD! Solid implementation!"
elif [[ $total_score -ge 48 ]]; then  # 60% of 80
    echo "⚠️  SATISFACTORY! Needs some improvement!"
else
    echo "❌ NEEDS SIGNIFICANT WORK!"
fi

echo ""

# Show debug files information
if [[ -f "STUDENT_OUTPUT.txt" ]]; then
    echo "📄 Debug files available for your reference:"
    echo "   - STUDENT_OUTPUT.txt (your program's output)"
    echo "   - EXPECTED_OUTPUT.txt (reference output)"  
    echo "   Run 'diff STUDENT_OUTPUT.txt EXPECTED_OUTPUT.txt' for detailed comparison"
fi

echo ""

# Exit with appropriate code
if [[ $percentage -ge 70 ]]; then
    exit 0
else
    exit 1
fi