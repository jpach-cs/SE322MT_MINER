'''
pytest fixtures for the coverage gate tests.
'''
import subprocess
from pathlib import Path

import pytest

from repo_helpers import GIT, REPORT_MAKEFILE, commit_all, run_git, write_summary


@pytest.fixture(name='repo')
def fixture_repo(tmp_path: Path, monkeypatch: pytest.MonkeyPatch) -> Path:
    '''
    A git repo on main with src/, tests/, and a base coverage of 40 of 100 lines,
    checked out on a feature branch and set as the working directory.
    '''
    repo = tmp_path / 'repo'
    (repo / 'src').mkdir(parents=True)
    (repo / 'tests').mkdir()
    (repo / 'src' / 'game.c').write_text('int x;\n', encoding='utf-8')
    (repo / 'tests' / 'test_game.cpp').write_text('// tests\n', encoding='utf-8')
    (repo / 'Makefile').write_text(REPORT_MAKEFILE, encoding='utf-8')
    write_summary(repo / 'summary.json', 40, 100)
    subprocess.run([*GIT, 'init', '-q'], cwd=repo, check=True)
    commit_all(repo, 'base')
    run_git(repo, 'switch', '-q', '-c', 'feature')
    for key, value in (('GIT_AUTHOR_NAME', 'test'), ('GIT_AUTHOR_EMAIL', 'test@example.com'),
                       ('GIT_COMMITTER_NAME', 'test'), ('GIT_COMMITTER_EMAIL', 'test@example.com')):
        monkeypatch.setenv(key, value)
    monkeypatch.chdir(repo)
    return repo
