# Or-Patterns
def or_pattern(x):
    match x:
        case 1 | 2 | 3:
            return "small number"
        case "a" | "b" | "c":
            return "small letter"
        case _:
            return "other"


# Pattern Guards (if condition)
def guard(point):
    match point:
        case (x, y) if x == y:
            return "diagonal"
        case (x, y) if x > 0 and y > 0:
            return "quadrant 1"
        case (x, y):
            return "other point"
