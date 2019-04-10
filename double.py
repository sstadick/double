import fileinput
from itertools import combinations

def main():
    [print(f"{a}{b}\n{a} {b}")for a, b in combinations((line.rstrip() for line in fileinput.input()), 2)]



if __name__ == '__main__':
    main()
