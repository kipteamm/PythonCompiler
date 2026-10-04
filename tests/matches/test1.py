# Literal & Wildcard Matching
def literals(val: int | str | bool) -> str:
    match val:
        case 0:
            return "zero"
        case "hello":
            return "string match"
        case True:
            return "bool match"
        case None:
            return "none match"
        case _:
            return "wildcard match"


# Variable Capture (Capture Pattern)
def capture(x: int) -> str:
    match x:
        case 100:
            return "exact 100"
        case other_val:
            # 'other_val' captures 'x'
            return f"captured {other_val}"
