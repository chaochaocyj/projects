import random
import random

random_numbers = [random.randint(1, 50000) for _ in range(300000)]
output_file_name = "input_30w.txt"
with open(output_file_name, 'w') as f:
    for item in random_numbers:
        f.write(f"{item}\n")
