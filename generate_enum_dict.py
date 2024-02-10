import os
import re

import os
import re

def parse_enum(enum_string):
    # Split the enum string into lines
    enum_name, enum_body = enum_string.split('{')
    enum_body, type_def_name = enum_body.split('}')
    if 'typedef' in enum_name:
        _, enum_name = enum_name.split('typedef')
    enum_name = enum_name.replace('enum','')
    enum_name = enum_name.strip()
    #remove the word enum from name
    enum_values = {}
    for val,line in enumerate(enum_body.split(',')):
        # Remove any comments
        line = re.sub(r'//.*', '', line)
        line = re.sub(r'/\*.*\*/', '', line)
        line = line.strip()
        # Skip empty lines
        if not line:
            continue
        #remove brackets
        line = line.replace('{','')
        line = line.replace('}','')
        #check if there is explict value assignment
        if '=' in line:
            # Split the line into name and value
            name, value = line.split('=')
            value = value.strip()
            # Add the name and value to the dictionary
            val = value

        name = line.strip()
        enum_values[name] = val

    # Print debug for when the enum_name contains a whitespace
    print("Enum names with whitespace: ",)
    if ' ' in enum_name:
        print(f'enum_name: {enum_name}')
    return enum_name, enum_values

def find_enums(source_dir):
    # Regular expression to match enum definitions
    enum_pattern = re.compile(r'(?:typedef\s+)?enum\s+(\w+)\s*\{([^}]*)\}\s*;?', re.DOTALL)    
    # Dictionary to hold the enums
    enums = {}

    # Walk through all files in the source directory
    for root, dirs, files in os.walk(source_dir):
        for file in files:
            print(f'Processing {file}...')
            with open(os.path.join(root, file), 'r') as f:
                content = f.read()

                # Find all enum definitions in the file
                for match in enum_pattern.finditer(content):
                    enum_string = match.group()
                    enum_name, enum_values = parse_enum(enum_string)
                    print(f'Found enum: {enum_name}')
                    enums[enum_name] = enum_values

    return enums

def generate_enum_dict_file(enums, filename):
    with open(filename, 'w') as f:
        # Add the #include directive for the string.h header file
        f.write('#include <string.h>\n\n')
        # Add the #include directive for the corresponding header file
        header_filename = filename.replace('_dict.c', '.h')
        header_filename = header_filename.split('source/')[-1]
        f.write(f'#include "{header_filename}"\n\n')

        for i, (enum_name, enum_values) in enumerate(enums.items()):
            f.write(f'char* {enum_name}_dict[{len(enum_values)}] = {{\n')
            for value_name in enum_values.keys():
                f.write(f'    "{value_name}",\n')
            f.write('};\n\n')

def generate_getter_functions_file(enums, builtin_types, filename):
    with open(filename, 'w') as f:
        # Add the #include directive for the stdio.h header file
        f.write('#include <stdio.h>\n\n')
        f.write('#include <string.h>\n\n')
        
        # Add the #include directive for the corresponding header file
        header_filename = filename.replace('_getter.c', '.h')
        header_filename = header_filename.split('source/')[-1]
        f.write(f'#include "{header_filename}"\n\n')
        
        f.write('char type_val_string[1000];\n\n')

        # Generate the getter functions for the enums
        type_to_index = {}  # Database for storing the index of each type
        for i, (enum_name, enum_values) in enumerate(enums.items()):
            f.write(f'char* get_{enum_name}_name(void* p_type) {{\n')
            f.write( '    int type = *(int*)p_type;\n')
            f.write(f'    if (type < 0 || type >= sizeof({enum_name}_dict) / sizeof({enum_name}_dict[0])) {{\n')
            f.write('        return "Invalid type";\n')
            f.write('    }\n')
            f.write(f'    return {enum_name}_dict[type];\n')
            f.write('}\n\n')
            type_to_index[enum_name] = i  # Save the index of the enum type

        # Generate the getter functions for the built-in types
        def format_from_type(type_name):
            if type_name in ['int', 'short']:
                return '%d'
            if type_name in ['long']:
                return '%ld'
            if type_name in ['long long']:
                return '%lld'
            elif type_name in ['float', 'double']:
                return '%f'
            elif type_name == 'char':
                return '%c'
            else:
                return 'unknown'
        for i, type_name in enumerate(builtin_types, start=len(enums)):
            no_underscore_type_name = type_name.replace(' ', '_')
            f.write(f'char* get_{no_underscore_type_name}_name(void* p_type) {{\n')
            f.write(f'    {type_name} val = *({type_name} *)p_type;\n')
            f.write(f'    sprintf(type_val_string, "{format_from_type(type_name)}\\n", val);\n')
            f.write(f'    return type_val_string;\n')
            f.write('}\n\n')
            type_to_index[no_underscore_type_name] = i  # Save the index of the built-in type

        # Generate the global array of getter function pointers
        f.write('getter_func_t g_a_getters[] = {\n')
        for type_name, index in type_to_index.items():
            f.write(f'    get_{type_name}_name,\n')
        f.write('};\n\n')

        # Generate the typename_to_idx function
        f.write('int typename_to_idx(const char *type_name) {\n')
        for type_name, index in type_to_index.items():
            f.write(f'    if (strcmp(type_name, "{type_name}") == 0) return {index};\n')
        f.write('    return -1;  // Return -1 if type_name is not found\n')
        f.write('}\n')

def generate_header_file(enums, builtin_types, filename):
    # The header file shold be written to the include directory
    # The include directory is the same as the source directory
    filename = filename.replace('source', 'include')
    with open(filename, 'w') as f:
        # Generate the _Generic macro
        f.write('#define typename(x) _Generic((x), \\\n')
        for type_name in builtin_types:
            f.write(f'    {type_name}: "{type_name}", \\\n')
        for enum_name in enums.keys():
            f.write(f'    enum {enum_name}: "{enum_name}", \\\n')
        f.write('    default: "unknown")\n')

        # Generate the function pointer typedef
        f.write('typedef char* (*getter_func_t)(void*);\n')

        # Generate the extern declaration for the type_val_string
        f.write('extern char type_val_string[1000];\n\n')

        # Generate the extern declarations for the getter functions and the getters array
        for enum_name in enums.keys():
            f.write(f'extern char* get_{enum_name}_name(void* p_type);\n')
            f.write(f'extern char* {enum_name}_dict[{len(enums[enum_name])}];\n')  # Add extern declaration for the dictionary

        f.write('extern getter_func_t g_a_getters[];\n')

        # Generate the typename_to_idx function declaration
        f.write('int typename_to_idx(const char *type_name);\n')

import argparse

if __name__ == "__main__":
    # Create the parser
    parser = argparse.ArgumentParser(description="Generate a dictionary of enums from source files.")

    # Add the arguments
    parser.add_argument('--SourceDir', metavar='source_dir', type=str, nargs='*', help='the source directories to scan for enums', default=['./source', './include'])
    parser.add_argument('--OutputFile', metavar='output_file', type=str, help='the output file', default='./generated_enum_dict.c')
    
    # Parse the arguments
    args = parser.parse_args()

    print(f'python3 {__file__} --SourceDir {" ".join(args.SourceDir)} --OutputFile {args.OutputFile}')

    # Modify the source directory to be absolute path
    # The absolute path is based on the directory of the script file
    # This is in the location of the build directory
    # Construct the absolute path to the source directory
    args.SourceDir = [os.path.abspath(os.path.join(os.path.dirname(__file__), source_dir)) for source_dir in args.SourceDir]
    
    # Run the function with each source directory argument
    for source_dir in args.SourceDir:
        print(f'Processing source directory: {source_dir}')
        enums = find_enums(source_dir)
        for enum_name, enum_values in enums.items():
            print(f'{enum_name}: {enum_values}')

        # Generate the C file
        builtin_types = ['int', 'float', 'double', 'char', 'short', 'long', 'long long']

        #modify filename to have "_dict" and "_getter" suffix
        dict_filename = args.OutputFile.replace('.txt', '_dict.c')
        generate_enum_dict_file(enums, dict_filename)
        getter_filename = args.OutputFile.replace('.txt', '_getter.c')
        generate_getter_functions_file(enums, builtin_types, getter_filename)
        header_filename = args.OutputFile.replace('.txt', '.h')
        generate_header_file(enums, builtin_types, header_filename)