# Multiple type parameters on a generic class
# Bounded type parameters
class Pair[T, U: int | str]:
    def __init__(self, first: T, second: U):
        self.first = first
        self.second = second
