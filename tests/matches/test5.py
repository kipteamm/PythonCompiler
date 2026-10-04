class Point:
    __match_args__ = ("x", "y") # Enables positional pattern matching
    def __init__(self, x, y):
        self.x = x
        self.y = y

class Circle:
    def __init__(self, radius):
        self.radius = radius


# Class / Instance Matching
def test_classes(shape):
    match shape:
        # Positional matching via __match_args__
        case Point(0, 0):
            return "Origin point"
        case Point(x, y):
            return f"Point at {x}, {y}"
        # Keyword matching
        case Circle(radius=r) if r > 10:
            return "Large circle"
        case Circle(radius=r):
            return f"Circle radius {r}"
        case int() | float():
            return "Primitive number"
        case _:
            return "Unknown object"


# As-Patterns
def test_as_pattern(data):
    match data:
        case [1 | 2 | 3 as num, str() as label]:
            return f"Number {num} with label '{label}'"
        case (Point() | Circle()) as shape:
            return f"Matched a geometric shape: {type(shape).__name__}"
        case _:
            return "No match"
