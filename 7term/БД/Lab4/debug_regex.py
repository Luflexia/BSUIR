import re

def test_regex():
    pattern = r'^\s*select\s*\*\s*from\s*"?([^"]+)"?\s*(?:where\s*([^\s=]+)\s*(=|contains|startswith)\s*\'?([^\']*)\'?\s*)?$'
    
    queries = [
        'SELECT * FROM "Станция" WHERE Город = \'Москва\'',
        'SELECT * FROM "Станция"',
        'select * from Станция where Город = Москва',
        'SELECT * FROM "Сотрудник" WHERE Должность contains \'Машинист\''
    ]

    print(f"Pattern: {pattern}")
    for q in queries:
        print(f"\nTesting query: '{q}'")
        m = re.match(pattern, q.strip(), flags=re.IGNORECASE)
        if m:
            print("MATCHED!")
            print(f"Table: {m.group(1)}")
            print(f"Col: {m.group(2)}")
            print(f"Op: {m.group(3)}")
            print(f"Val: {m.group(4)}")
        else:
            print("NO MATCH")

if __name__ == "__main__":
    test_regex()
