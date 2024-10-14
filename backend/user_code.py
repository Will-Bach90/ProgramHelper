import sys
import json

def max_sum_non_adjacent(nums):
    incl = 0
    excl = 0
    
    for num in nums:
        new_excl = max(incl, excl)
        incl = excl + num
        excl = new_excl
    
    return max(incl, excl)

if __name__ == "__main__":
    # Debugging: Print the input file name
    print(f"Input file: {sys.argv[1]}")

    # Read the input from the file passed as an argument (e.g., "input.json")
    input_file = sys.argv[1]

    try:
        with open(input_file, 'r') as f:
            input_data = json.load(f)  # Load the JSON input from the file
            # Debugging: Print the input data
            print(f"Input data: {input_data}")
    except Exception as e:
        print(f"Error reading the input file: {e}")
        sys.exit(1)

    # Call the function and print the result
    result = max_sum_non_adjacent(input_data)
    print(f"Result: {result}")