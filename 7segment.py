invert = True

segments_to_pin = {
    "a": 7,
    "b": 6,
    "c": 4,
    "d": 3,
    "e": 4,
    "f": 5,
    "g": 1,
    "dp": 5,
}

figures = [
    ("a", "b", "c", "d", "e", "f"),  # 0
    ("b", "c"), # 1
    ("a", "b", "d", "e", "g"), # 2
    ("a", "b", "c", "d", "g"), # 3
    ("b", "c", "f", "g"), # 4
    ("a", "f", "g", "c", "d"), # 5
    ("a", "c", "d", "e", "f", "g"), # 6
    ("a", "b", "c"), # 7
    ("a", "b", "c", "d", "e", "f", "g"), # 8
    ("a", "b", "c", "d", "f", "g"), # 9
    ("a", "b", "c", "e", "f", "g"), # A
    ("c", "e", "f", "g"), # b
    ("d", "e", "g"), # c
    ("c", "d", "e", "f", "g"), # d
    ("a", "d", "e", "f", "g"), # E
    ("a", "e", "f", "g"), # F
]

def generate_pins():
    for figure in figures:
        out = 0

        for segment in figure:
            out |= 1 << segments_to_pin[segment]

        if invert:
            out = 0xff & ~out

        yield out

if __name__ == "__main__":
    print("{", ", ".join(map(hex, generate_pins())), "}")
