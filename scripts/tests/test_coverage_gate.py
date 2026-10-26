'''
Tests for scripts/coverage_gate.py, run against real throwaway git repos.
'''
import os
import sys
from pathlib import Path

import pytest

import coverage_gate
from repo_helpers import commit_all, run_git, write_summary

# `make test-python` passes its own make here, so the tests use the same one.
MAKE = os.environ.get('MAKE', 'make')


def gate(monkeypatch: pytest.MonkeyPatch, summary: Path, *extra: str) -> int:
    '''
    Run the gate's main() with the given summary and arguments.
    '''
    argv = ['coverage_gate.py', '--summary', str(summary), '--base', 'main', '--make', MAKE, *extra]
    monkeypatch.setattr(sys, 'argv', argv)
    return coverage_gate.main()


def test_line_coverage_is_covered_over_total(tmp_path: Path) -> None:
    '''
    The percentage comes from the line counts, not a rounded field.
    '''
    summary = write_summary(tmp_path / 's.json', 1, 3)
    assert coverage_gate.line_coverage(summary) == pytest.approx(100.0 / 3)


def test_line_coverage_with_no_lines_is_full(tmp_path: Path) -> None:
    '''
    No measurable lines means nothing is uncovered.
    '''
    summary = write_summary(tmp_path / 's.json', 0, 0)
    assert coverage_gate.line_coverage(summary) == pytest.approx(100.0)


def test_passes_at_the_minimum(repo: Path, monkeypatch: pytest.MonkeyPatch) -> None:
    '''
    90% passes outright, without measuring the base.
    '''
    assert gate(monkeypatch, write_summary(repo / 'cur.json', 90, 100)) == 0


def test_skips_on_the_base_branch(repo: Path, monkeypatch: pytest.MonkeyPatch,
                                  capsys: pytest.CaptureFixture[str]) -> None:
    '''
    The base branch itself only gets the report.
    '''
    run_git(repo, 'switch', '-q', 'main')
    assert gate(monkeypatch, write_summary(repo / 'cur.json', 10, 100)) == 0
    assert 'SKIP' in capsys.readouterr().out


def test_skips_when_no_watched_path_changed(repo: Path, monkeypatch: pytest.MonkeyPatch,
                                            capsys: pytest.CaptureFixture[str]) -> None:
    '''
    A docs-only branch is not gated, even with low coverage.
    '''
    (repo / 'README.md').write_text('docs\n', encoding='utf-8')
    commit_all(repo, 'docs only')
    assert gate(monkeypatch, write_summary(repo / 'cur.json', 10, 100)) == 0
    assert 'no changes' in capsys.readouterr().out


def test_passes_when_coverage_beats_the_base(repo: Path, monkeypatch: pytest.MonkeyPatch) -> None:
    '''
    41% beats the base's 40%.
    '''
    (repo / 'src' / 'game.c').write_text('int x;\nint y;\n', encoding='utf-8')
    commit_all(repo, 'code')
    assert gate(monkeypatch, write_summary(repo / 'cur.json', 41, 100)) == 0


def test_fails_when_coverage_equals_the_base(repo: Path, monkeypatch: pytest.MonkeyPatch,
                                             capsys: pytest.CaptureFixture[str]) -> None:
    '''
    Equal is not an increase.
    '''
    (repo / 'tests' / 'test_game.cpp').write_text('// more tests\n', encoding='utf-8')
    commit_all(repo, 'tests')
    assert gate(monkeypatch, write_summary(repo / 'cur.json', 40, 100)) == 1
    assert 'FAIL' in capsys.readouterr().out


def test_fails_when_coverage_drops(repo: Path, monkeypatch: pytest.MonkeyPatch) -> None:
    '''
    Lower than the base fails.
    '''
    (repo / 'src' / 'game.c').write_text('int x;\nint y;\n', encoding='utf-8')
    commit_all(repo, 'code')
    assert gate(monkeypatch, write_summary(repo / 'cur.json', 30, 100)) == 1


def test_uncommitted_changes_turn_the_gate_on(repo: Path, monkeypatch: pytest.MonkeyPatch) -> None:
    '''
    Local runs see working-tree edits, not just commits.
    '''
    (repo / 'src' / 'game.c').write_text('int changed;\n', encoding='utf-8')
    run_git(repo, 'commit', '-q', '--allow-empty', '-m', 'empty')
    assert gate(monkeypatch, write_summary(repo / 'cur.json', 30, 100)) == 1


def test_base_without_a_report_counts_as_zero(repo: Path, monkeypatch: pytest.MonkeyPatch,
                                              capsys: pytest.CaptureFixture[str]) -> None:
    '''
    A base that cannot produce a report (like main before this PR) is 0%.
    '''
    run_git(repo, 'switch', '-q', 'main')
    (repo / 'Makefile').write_text('all:\n\ttrue\n', encoding='utf-8')
    commit_all(repo, 'no report target')
    run_git(repo, 'switch', '-q', 'feature')
    run_git(repo, 'rebase', '-q', 'main')
    (repo / 'src' / 'game.c').write_text('int x;\nint y;\n', encoding='utf-8')
    commit_all(repo, 'code')
    assert gate(monkeypatch, write_summary(repo / 'cur.json', 5, 100)) == 0
    assert 'treating its coverage as 0%' in capsys.readouterr().out


def test_base_worktree_is_cleaned_up(repo: Path) -> None:
    '''
    Measuring the base leaves no worktree behind.
    '''
    assert coverage_gate.base_coverage('main', MAKE, []) == pytest.approx(40.0)
    worktrees = run_git(repo, 'worktree', 'list', '--porcelain').count('worktree ')
    assert worktrees == 1


def test_make_vars_reach_the_base_build(repo: Path) -> None:
    '''
    VAR=value arguments are passed to make when measuring the base.
    '''
    run_git(repo, 'switch', '-q', 'main')
    (repo / 'Makefile').write_text(
        'coverage-report:\n\tmkdir -p coverage && cp $(SUMMARY) coverage/summary.json\n',
        encoding='utf-8')
    write_summary(repo / 'other.json', 70, 100)
    commit_all(repo, 'parameterized report')
    run_git(repo, 'switch', '-q', 'feature')
    assert coverage_gate.base_coverage('main', MAKE, ['SUMMARY=other.json']) == pytest.approx(70.0)
