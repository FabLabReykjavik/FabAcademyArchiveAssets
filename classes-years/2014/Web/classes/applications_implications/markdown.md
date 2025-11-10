import os
import re
from bs4 import BeautifulSoup

def parse_main_index(index_path):
    """Parse the main index.html for topic folder names (from <a href=...index.html>)."""
    with open(index_path, encoding="utf-8") as f:
        html = f.read()
    soup = BeautifulSoup(html, "html.parser")
    # Find all <a href="topic/index.html">topic</a>
    links = []
    for a in soup.find_all("a", href=True):
        href = a['href']
        # Match 'something/index.html' and extract 'something'
        m = re.match(r'([a-zA-Z0-9_]+)/index\.html', href)
        if m:
            topic = m.group(1)
            links.append((topic, a.text.strip() or topic))
    return links

def parse_topic_index(topic_folder):
    """Parse a subfolder's index.html for main link(s) and image(s)."""
    index_path = os.path.join(topic_folder, "index.html")
    if not os.path.exists(index_path):
        return None
    with open(index_path, encoding="utf-8") as f:
        html = f.read()
    soup = BeautifulSoup(html, "html.parser")

    md_parts = []

    # 1. All <a href=...> possibly wrapping <img>
    for a in soup.find_all("a", href=True):
        link = a['href']
        img = a.find("img")
        if img and img.get('src'):
            img_src = img['src']
            md_parts.append(f"[Original site]({link})\n\n![{os.path.basename(img_src)}]({img_src})")
        else:
            md_parts.append(f"[Original site]({link})")

    # 2. Standalone <img> not wrapped in <a>
    for img in soup.find_all("img"):
        parent_a = img.find_parent("a")
        if not parent_a and img.get('src'):
            img_src = img['src']
            md_parts.append(f"![{os.path.basename(img_src)}]({img_src})")

    if not md_parts:
        with open(index_path, encoding="utf-8") as f:
            md_parts = [f.read()]

    return "\n\n".join(md_parts)

def main():
    root = os.path.dirname(os.path.abspath(__file__))
    main_index = os.path.join(root, "index.html")
    if not os.path.exists(main_index):
        print("No index.html found in the current directory.")
        return

    # 1. Parse main index.html for topic folders
    topic_links = parse_main_index(main_index)
    if not topic_links:
        print("No topics found in main index.html.")
        return

    # 2. Write applicationsImplications.md (links point into folders)
    with open("applicationsImplications.md", "w", encoding="utf-8") as f:
        f.write("# Applications and Implications\n\n")
        for topic, _ in topic_links:
            f.write(f"- [{topic}]({topic}/{topic}.md)\n")

    print("Created applicationsImplications.md")

    # 3. For each topic, create topic/topic.md
    for topic, _ in topic_links:
        md_content = parse_topic_index(topic)
        if md_content:
            out_md = os.path.join(topic, f"{topic}.md")
            with open(out_md, "w", encoding="utf-8") as f:
                f.write(md_content)
            print(f"Created {out_md}")
        else:
            print(f"Warning: No index.html or content found for {topic}")

if __name__ == "__main__":
    try:
        from bs4 import BeautifulSoup
    except ImportError:
        print("This script requires BeautifulSoup. Install it with:\n  pip install beautifulsoup4")
        exit(1)
    main()
