#!/usr/bin/env python3
# 학습: 검증 환경을 코드로 재현
# Maven 검증 리포트의 클래스패스를 재사용해 실제 라이브러리 버전으로 테스트 서버를 실행한다.
# 먼저 verify가 필요하다는 의존성을 명시하고 shell 문자열 대신 인자 배열로 JVM을 실행한다.
"""Launch the isolated browser backend using the classpath from ./mvnw verify."""
import os
from pathlib import Path
import xml.etree.ElementTree as ET

root = Path(__file__).resolve().parent.parent
report = root / "integration-tests/target/surefire-reports/TEST-dev.study.SagaIntegrationTest.xml"
if not report.exists():
    raise SystemExit("Run ./mvnw verify from the repository root before npm test.")
properties = ET.parse(report).findall(".//property")
classpath = next(p.attrib["value"] for p in properties if p.attrib["name"] == "java.class.path")
os.chdir(root)
os.execvp("java", ["java", "-cp", classpath, "dev.study.BrowserLab"])
