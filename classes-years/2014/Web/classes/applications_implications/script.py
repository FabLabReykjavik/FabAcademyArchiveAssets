import os

def scrape_index_files():
    root_folder = os.path.dirname(os.path.abspath(__file__))

    for dirpath, dirnames, filenames in os.walk(root_folder):
        for filename in filenames:
            if filename.lower() == 'index.html':
                full_path = os.path.join(dirpath, filename)
                rel_path = os.path.relpath(full_path, root_folder)
                print(f"\n=== {rel_path} ===")
                try:
                    with open(full_path, 'r', encoding='utf-8') as f:
                        print(f.read())
                except Exception as e:
                    print(f"Could not read {rel_path}: {e}")

if __name__ == "__main__":
    scrape_index_files()
