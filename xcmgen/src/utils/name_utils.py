def kebab(s: str) -> str:
    kebab = []
    for i, char in enumerate(s):
        if char.isupper() and i > 0:
            kebab.append("-")
        kebab.append(char.lower())
    return "".join(kebab)
