# Or-Patterns
def or_pattern(x: int | str) -> str:
    match x:
        case 1 | 2 | 3:
            return "small number"
        case "a" | "b" | "c":
            return "small letter"
        case _:
            return "other"


# Tuple capture
def tuples(point: tuple[int, int]) -> str:
    match point:
        case (-1, -1) | (1, 1):
            return "-1/1"
        case (0, 0):
            return "0, 0"
        case (0, y):
            return f"0, {y}"
        case (x, y):
            return f"{x}, {y}"
