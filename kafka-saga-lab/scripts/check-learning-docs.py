#!/usr/bin/env python3
"""학습: 설명 누락을 기계적으로 찾되, 주석의 정확성을 테스트가 보장한다고 오해하지 않는다.

직접 관리하는 코드/설정에는 '학습' 주석과 파일 안내 링크를 요구한다.
JSON, Maven Wrapper 원본, 생성 산출물은 주석을 주입하지 않고 문서에서 역할을 설명한다.
"""
import os
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parent.parent
SKIP = {".git", ".codex", ".agents", ".mvn", ".vscode", "node_modules", "target", "dist", "data", "test-results", "playwright-report", "__pycache__"}
SUFFIXES = {".java", ".sql", ".yml", ".yaml", ".xml", ".js", ".jsx", ".css", ".html", ".py", ".sh"}
SPECIAL = {"Dockerfile", ".dockerignore", ".gitignore", ".env.example"}
index = (ROOT / "docs/file-guide.md").read_text()
linked = set(re.findall(r"\]\(\.\./([^)#]+)(?:#[^)]*)?\)", index))
missing = []
count = 0
for directory, subdirs, files in os.walk(ROOT):
    subdirs[:] = [name for name in subdirs if name not in SKIP]
    # IDE Java 업그레이드 도구가 생성한 관리 파일은 직접 작성한 학습 코드가 아니다.
    if Path(directory) == ROOT / ".github":
        subdirs[:] = [name for name in subdirs if name != "modernize"]
    for name in files:
        path = Path(directory) / name
        if path.suffix not in SUFFIXES and name not in SPECIAL:
            continue
        relative = path.relative_to(ROOT).as_posix()
        count += 1
        if "학습" not in path.read_text()[:3000]:
            missing.append(f"학습 주석 없음: {relative}")
        if relative not in linked:
            missing.append(f"파일 안내 없음: {relative}")
if missing:
    raise SystemExit("\n".join(missing))
print(f"학습 주석·파일 안내 확인: {count}개 파일. 내용의 타당성은 코드 리뷰·시나리오 테스트로 확인하세요.")
