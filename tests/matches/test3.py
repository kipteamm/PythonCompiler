# Fixed & Variable Length Sequences
def sequences(data):
    match data:
        case []:
            return "empty"
        case [x]:
            return f"single item: {x}"
        case [first, second]:
            return f"pair: {first}, {second}"
        case [head, *tail]:
            return f"head: {head}, tail: {tail}"
        case [*_, last]:
            return f"last item: {last}"
