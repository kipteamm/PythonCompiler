import pytest


def pytest_ignore_collect(collection_path, config):
    if collection_path.name == "conftest.py": return True
    if collection_path.name == "test.py": return True
    return False


def pytest_addoption(parser):
    parser.addoption(
        "--update-ast",
        action="store_true",
        default=False,
        help="Update the expected .ast files with the generated output instead of failing"
    )

@pytest.fixture
def update_ast(request):
    return request.config.getoption("--update-ast")
