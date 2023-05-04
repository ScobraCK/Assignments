import sqlite3

class DNS():
    def __init__(self) -> None:
        self.con = sqlite3.connect('dns.db')
        self.cur = self.con.cursor()

        self.cur.execute("""
        CREATE TABLE IF NOT EXISTS domains (ip text PRIMARY KEY, dname text UNIQUE)
        """)

    def insert_domain(self, ip: str, dname: str):
        self.cur.execute(f"INSERT INTO domains VALUES (?, ?)", (ip, dname))
        self.con.commit()


if __name__ == "__main__":
    dns = DNS()
    try:
        dns.insert_domain('1.2.3.4', 'test.domain')
        dns.insert_domain('1.2.3.5', 'test.domain.2')
        dns.insert_domain('1.2.3.6', 'test.domain.3')
        dns.insert_domain('1.2.3.7', 'test.domain')
    except sqlite3.IntegrityError as e:
        print(f"Error: {e}")


    for row in dns.cur.execute("""SELECT * FROM domains"""):
        print(row)
    