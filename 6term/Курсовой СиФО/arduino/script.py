import os

CODE_FILENAME = "code.txt"
OUTPUT_FILENAME = "numbered_code.txt"

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    code_path = os.path.join(script_dir, CODE_FILENAME)
    output_path = os.path.join(script_dir, OUTPUT_FILENAME)
    if not os.path.isfile(code_path):
        print(f"Файл {CODE_FILENAME} не найден в директории скрипта.")
        return
    with open(code_path, "r", encoding="utf-8") as infile, open(output_path, "w", encoding="utf-8") as outfile:
        for idx, line in enumerate(infile, 1):
            numbered_line = f"{idx:03d} {line.rstrip()}\n"
            outfile.write(numbered_line)
    print(f"Файл {OUTPUT_FILENAME} успешно создан.")

if __name__ == "__main__":
    main()