def pytest_ignore_collect(collection_path, config):
    if collection_path.name == "conftest.py": return True
    if collection_path.name == "test.py": return True
    return False
