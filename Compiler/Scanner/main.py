from scanner import Scanner

def read_optable(file_path):
    optable = {}
    with open(file_path, 'r', encoding='utf-8') as file:
        for line in file:
            columns = line.split()
            key, value = columns[0], int(columns[1])
            optable[key] = value
    return optable

if __name__ == "__main__":
    optable = read_optable('optable.tsv')

    sc = Scanner(optable, dict())
    sc.read_file('test3.txt')
    sc.read_tokens()
    sc.print_symboltable()
    from pprint import pprint
    pprint(sorted(sc.tokenizer.table))