'''
Trace Database Generator

The trace database is a CSV file with a row for each function in the C source files.
Each row contains the index of the function, the name of the function, and the path to the C source file.
'''

import csv
import glob
import os
import argparse
import pycparser
from pycparser import parse_file, c_ast

def find_functions(filename, include_dir):
    """
    Parse a C source file and return a list of function names.

    Args:
        filename (str): The path to the C source file.
        include_dir (str): The directory where the header files included in the C source file are located.

    Returns:
        list: A list of function names.
    """
    # Get the directory of the fake includes
    pycparser_dir = os.path.dirname(pycparser.__file__)
    fake_includes_dir = os.path.join(pycparser_dir, 'utils', 'fake_libc_include')
    print('Fake includes dir: ' + fake_includes_dir)
    # Add the include directories to cpp_args
    cpp_args = ['-I' + os.path.dirname(filename), '-I' + include_dir, '-I' + fake_includes_dir]
    ast = parse_file(filename, use_cpp=True, cpp_args=cpp_args)
    visitor = c_ast.NodeVisitor()
    functions = []

    class FuncDefVisitor(c_ast.NodeVisitor):
        def visit_FuncDef(self, node):
            functions.append(node.decl.name)

    visitor = FuncDefVisitor()
    visitor.visit(ast)
    return functions


def main():
    """
    Generate a trace database from C source files.

    The trace database is a CSV file with a row for each function in the C source files.
    Each row contains the index of the function, the name of the function, and the path to the C source file.
    """
    parser = argparse.ArgumentParser(description='Generate a trace database from C source files.')
    parser.add_argument('source_dir', help='The directory containing the C source files.')
    parser.add_argument('--output_dir', default='.', help='The directory to write the trace_db.csv file to. Defaults to the current directory.')

    args = parser.parse_args()

    source_dir = args.source_dir
    output_dir = args.output_dir
    include_dir = os.path.join(output_dir, 'include/')
    print('Include dir: ' + include_dir)
    output_file = os.path.join(output_dir, 'trace_db.csv')
    print('Output file: ' + output_file)
    print('Searching for C source files in ' + source_dir + '...')
    files = glob.glob(source_dir + '/**/*.c', recursive=True)
    print('Found ' + str(len(files)) + ' C source files.')

    with open(output_file, 'w', newline='', encoding='utf-8') as csvfile:
        print('Writing to ' + output_file)
        # print full path of created file:
        print(os.path.abspath(csvfile.name))
        fieldnames = ['index', 'function', 'file', 'is_enabled']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

        writer.writeheader()
        index = 0
        for file in files:
            functions = find_functions(file, include_dir=include_dir)
            for function in functions:
                writer.writerow({'index': index, 'function': function, 'file': file, 'is_enabled': 1})
                index += 1

if __name__ == "__main__":
    main()
    