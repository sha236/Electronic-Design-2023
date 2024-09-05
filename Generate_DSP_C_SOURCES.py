import os

# Set the root directory of the DSP source files
dsp_source_path = 'DSP/Source'

# Initialize a list to hold the paths of the .c files
c_files_list = []

# Walk through the directory
for root, dirs, files in os.walk(dsp_source_path):
    for file in files:
        if file.endswith('.c'):
            # Construct the relative path
            relative_path = os.path.join(root, file)
            # Replace backslashes on Windows with forward slashes
            relative_path = relative_path.replace('\\', '/')
            c_files_list.append(relative_path)

# Sort the list of files for consistency
c_files_list.sort()

# Write the output to a file
with open('c_sources_list.txt', 'w') as f:
    for i, c_file in enumerate(c_files_list):
        # Add a backslash at the end of each line except the last
        end_char = ' \\\n' if i < len(c_files_list) - 1 else '\n'
        f.write(c_file + end_char)

print("C source files have been written to c_sources_list.txt")
