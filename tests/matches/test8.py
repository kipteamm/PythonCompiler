# Pattern Guards
def guard(point: tuple[int, int]) -> str:
    match point:
        case (x, y) if x == y:
            return "diagonal"
        case (x, y) if x > 0 and y > 0:
            return "quadrant 1"
        case (x, y):
            return "other point"
