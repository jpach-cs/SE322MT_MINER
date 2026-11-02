'''
Helpers for building throwaway git repos in the coverage gate tests.
'''
import json
import subprocess
from pathlib import Path

GIT = ['git', '-c', 'user.name=test', '-c', 'user.email=test@example.com',
       '-c', 'init.defaultBranch=main', '-c', 'commit.gpgsign=false']

# The base branch's `make coverage-report` just copies a committed summary, so
# each test controls the base coverage without compiling anything.
REPORT_MAKEFILE = 'coverage-report:\n\tmkdir -p coverage && cp summary.json coverage/summary.json\n'


def run_git(repo: Path, *args: str) -> str:
    '''
    Run git in the given repository and return its output.
    '''
    result = subprocess.run([*GIT, *args], cwd=repo, check=True, capture_output=True, text=True)
    return result.stdout.strip()


def write_summary(path: Path, covered: int, total: int) -> Path:
    '''
    Write a gcovr-style JSON summary with the given line counts.
    '''
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps({'line_covered': covered, 'line_total': total}), encoding='utf-8')
    return path


def commit_all(repo: Path, message: str) -> None:
    '''
    Stage everything and commit.
    '''
    run_git(repo, 'add', '-A')
    run_git(repo, 'commit', '-q', '-m', message)
