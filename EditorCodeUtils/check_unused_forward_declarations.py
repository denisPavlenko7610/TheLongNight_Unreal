import re
from pathlib import Path

root = Path("Source/TheLongNight_Unreal")

if not root.exists():
    print(f"[Error] The specified path does not exist: {root.resolve()}")
    print("Make sure you are running the script from the project root folder (where the .uproject file is located).")
    exit(1)

print(f"Scanning folder: {root.resolve()}\n")

for path in root.rglob("*.h"):
    try:
        text = path.read_text(encoding="utf-8-sig", errors="ignore")
    except OSError as e:
        print(f"[Skipped] Win32 error while reading file: {path}")
        print(f"            Details: {e}\n")
        continue

    forwards = re.findall(r"^\s*class\s+([A-Za-z_]\w*)\s*;\s*$", text, flags=re.MULTILINE)

    unused = []
    for name in forwards:
        count = len(re.findall(rf"\b{name}\b", text))
        if count <= 1:
            unused.append(name)

    if unused:
        print(f"File: {path}")
        for name in unused:
            print(f"  -> Possibly unused: class {name};")
        print("-" * 40)
