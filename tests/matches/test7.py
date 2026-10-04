# Value Patterns vs Capture Patterns (Dot Notation)
# Attribute lookups (e.g., Color.RED) act as Value Patterns (equality check),
# whereas plain identifiers (e.g., RED) act as Capture Patterns (variable assignment).
class Color:
    RED = 1
    GREEN = 2

def value_vs_capture(val):
    match val:
        case Color.RED: # Value pattern: checks if val == Color.RED
            return "Is Red"
        case Color.GREEN: # Value pattern: checks if val == Color.GREEN
            return "Is Green"
        case RED: # Capture pattern: binds val to a local variable named RED!
            return f"Captured as local variable named RED: {RED}"
