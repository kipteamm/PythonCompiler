import subprocess
import sys
import os

def main():
    if len(sys.argv) < 3:
        print("Usage: run_test.py <compiler_path> <source_file>")
        sys.exit(1)

    compiler_path = sys.argv[1]
    source_file = sys.argv[2]

    if source_file.endswith("run_test.py"): return
    if source_file.endswith("test.py"): return

    base_path, _ = os.path.splitext(source_file)
    expected_ast = base_path + ".ast"
    expected_error = base_path + ".error"
    actual_ast = "../output/ast.dot"

    # Run the compiler
    cmd = [compiler_path, source_file]
    result = subprocess.run(cmd, capture_output=True, text=True)

    # Check for expected errors
    if result.returncode != 0:
        if not os.path.exists(expected_error):
            print(f"Compiler failed unexpectedly with: {result.stderr}")
            sys.exit(1)

        with open(expected_error, "r") as f:
            expected = f.read().strip()

        actual = (result.stderr or result.stdout).strip()

        if expected != actual:
            print(f"Error mismatch!\nExpected:\n{expected}\nActual:\n{actual}")
            sys.exit(1)

        sys.exit(0)

    if not os.path.exists(actual_ast):
        print(f"Expected AST file {actual_ast} was not generated.")
        sys.exit(1)

    with open(expected_ast, "r") as f_exp, open(actual_ast, "r") as f_act:
        if f_exp.read().strip() != f_act.read().strip():
            print(f"AST mismatch for {source_file}")
            sys.exit(1)

    sys.exit(0)


if __name__ == "__main__":
    main()
