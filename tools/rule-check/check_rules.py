import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SOURCE_DIRECTORIES = ("SageQt/", "tests/")
SOURCE_PATTERNS = ("*.cpp", "*.h")
BACKSLASH_PAIR = "\\" * 2
HANDOFF_NEW = re.compile(
    r"(\w+)\s*=\s*new (QHBoxLayout|QVBoxLayout|QGridLayout|QFormLayout)\(\)|(\w+)\s*=\s*new (QSpacerItem|QStandardItem)\("
)
HANDOFF_CALL = r"(addLayout|addItem|appendRow|insertRow|setItem)\(\s*{name}\b"


def source_files():
    output = subprocess.run(
        ["git", "ls-files", *SOURCE_PATTERNS], cwd=ROOT, capture_output=True, text=True, check=True
    ).stdout
    return [path for path in output.split() if path.startswith(SOURCE_DIRECTORIES)]


def check_backslash_path(path, lines):
    return [
        (number, "경로 구분자 \\\\ 금지 — QFileInfo · QDir::toNativeSeparators (values-and-platform.md)")
        for number, line in enumerate(lines, start=1)
        if BACKSLASH_PAIR in line
    ]


def check_parentless_new(path, lines):
    findings = []
    for index, line in enumerate(lines):
        match = HANDOFF_NEW.search(line)
        if match is None:
            continue
        name = match.group(1) or match.group(3)
        next_line = lines[index + 1] if index + 1 < len(lines) else ""
        if re.search(HANDOFF_CALL.format(name=re.escape(name)), next_line) is None:
            findings.append(
                (index + 1, f"부모 없이 만든 {name}을 바로 다음 줄에서 넘기지 않음 (ownership-and-threads.md 규약 1)")
            )
    return findings


CHECKS = (check_backslash_path, check_parentless_new)


def main():
    sys.stdout.reconfigure(encoding="utf-8")
    findings = []
    for path in source_files():
        lines = (ROOT / path).read_text(encoding="utf-8").splitlines()
        for check in CHECKS:
            findings.extend((path, number, message) for number, message in check(path, lines))
    for path, number, message in findings:
        print(f"{path}:{number}: {message}")
    return 1 if findings else 0


if __name__ == "__main__":
    sys.exit(main())
