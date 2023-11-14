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
    import sys
    optable = read_optable('optable.tsv')

    sc = Scanner(optable)
    sc.read_file('test/noerror.txt')
    original_stdout = sys.stdout 	

    with open('result/noerror_result.txt', 'w') as f:
        sys.stdout = f
        sc.read_tokens()
        sc.print_symboltable()
        sys.stdout = original_stdout 