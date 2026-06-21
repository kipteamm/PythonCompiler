import os
import subprocess
import shutil  # Added to easily copy files
import pytest

def discover_test_files():
    test_files = []
    base_dir = os.path.dirname(__file__)

    for root, dirs, files in os.walk(base_dir):
        if root == base_dir: continue

        for file in files:
            if not file.endswith(".py"): continue
            if file.endswith("conftest.py"): continue
            if file.endswith("test.py"): continue
            test_files.append(os.path.join(root, file))

    return test_files


@pytest.mark.parametrize("source_file", discover_test_files())
def test_compiler_output(source_file, update_ast):
    base_path, _ = os.path.splitext(source_file)
    expected_ast = base_path + ".ast"
    expected_error = base_path + ".error"
    actual_ast = "output/ast.dot"

    # Run the compiler
    cmd = ["./cmake-build-debug/PythonCompiler.exe", source_file]
    result = subprocess.run(cmd, capture_output=True, text=True)

    if result.returncode != 0:
        assert os.path.exists(expected_error), f"Compiler failed unexpectedly with: {result.stderr}"

        with open(expected_error, "r") as f:
            expected = f.read().strip()

        actual = (result.stderr or result.stdout).strip()

        assert expected == actual, f"Error mismatch for {source_file}!"
        return

    assert os.path.exists(actual_ast), f"Expected AST file {actual_ast} was not generated."

    if update_ast:
        shutil.copyfile(actual_ast, expected_ast)
        return

    with open(expected_ast, "r") as f_exp, open(actual_ast, "r") as f_act:
        assert f_exp.read().strip() == f_act.read().strip(), f"AST mismatch for {source_file}"