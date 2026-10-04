# Complex Nesting
def test_nested(payload):
    match payload:
        case {"status": 200, "data": [{"id": first_id, "items": [head, *rest]}, *_]}:
            return f"Success! First payload item ID: {first_id}, head: {head}"
        case {"status": 404 | 500 as code, "error": msg}:
            return f"Error {code}: {msg}"
        case _:
            return "Invalid payload"
