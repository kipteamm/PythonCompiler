# Unbounded single type parameter on a function
def identity[T](x: T) -> T:
    return x

# Bounded type parameter
def add[T: int | float](x: T, y: T) -> T:
    return x + y


result = identity[str]("hello")
sum_mixed = add[float](5, 4.5)
