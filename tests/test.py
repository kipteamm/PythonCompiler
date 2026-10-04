# Literal & Wildcard Matching
def literals(val: int | str | bool) -> str:
    match val:
        case 0:
            return "zero"
        # case "hello":
        #     return "string match"
        # case True:
        #     return "bool match"
        # case None:
        #     return "none match"
        # case _:
        #     return "wildcard match"
