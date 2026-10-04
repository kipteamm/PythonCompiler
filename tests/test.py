# Literal & Wildcard Matching
def literals(val: int | str | bool) -> str:
    match val:
        case None:
            return "none match"
        case 0:
            return "zero"
        case "hello":
            return "string match"
        case True:
            return "bool match"
        case _:
            return "wildcard match"
