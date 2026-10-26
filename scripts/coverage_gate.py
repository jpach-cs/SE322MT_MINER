'''
Coverage ratchet for `make coverage`.

Passes when line coverage of src/ is at or above the minimum (90%). Below the
minimum, the branch must have strictly higher coverage than the base branch
(main), so every PR that changes code moves coverage up until the target is
reached. Branches that change nothing under the watched paths (src/, tests/)
and the base branch itself only get the report, not the gate.

The base branch is measured by checking it out into a temporary git worktree
and running its own `make coverage-report`. If the base has no such target yet,
or its report fails, its coverage counts as 0%.
'''
import argparse
import json
import shlex
import subprocess
import sys
import tempfile
from pathlib import Path


def git(*args: str) -> str:
    '''
    Run a git command and return its trimmed standard output.
    '''
    result = subprocess.run(['git', *args], check=True, capture_output=True, text=True)
    return result.stdout.strip()


def line_coverage(summary: Path) -> float:
    '''
    Line coverage percentage from a gcovr --json-summary file.
    '''
    data = json.loads(summary.read_text(encoding='utf-8'))
    total = data['line_total']
    return 100.0 * data['line_covered'] / total if total else 100.0


def changes_watched_paths(base: str, paths: list[str]) -> bool:
    '''
    True if the working tree differs from the merge base under any watched path.
    '''
    merge_base = git('merge-base', base, 'HEAD')
    result = subprocess.run(['git', 'diff', '--quiet', merge_base, '--', *paths], check=False)
    return result.returncode == 1


def base_coverage(base: str, make: str, make_vars: list[str]) -> float:
    '''
    Measure line coverage of the base branch in a temporary worktree.
    '''
    with tempfile.TemporaryDirectory() as tmp:
        worktree = Path(tmp) / 'base'
        git('worktree', 'add', '--detach', str(worktree), base)
        try:
            result = subprocess.run([*shlex.split(make), 'coverage-report', *make_vars],
                                    cwd=worktree, capture_output=True, text=True, check=False)
            summary = worktree / 'coverage' / 'summary.json'
            if result.returncode != 0 or not summary.exists():
                print(f'coverage gate: could not measure {base} (no coverage-report target, '
                      'or it failed); treating its coverage as 0%')
                return 0.0
            return line_coverage(summary)
        finally:
            git('worktree', 'remove', '--force', str(worktree))


def main() -> int:
    '''
    Apply the gate and return the process exit code.
    '''
    parser = argparse.ArgumentParser(description='Coverage ratchet for make coverage.')
    add = parser.add_argument
    add('--summary', type=Path, required=True, help='gcovr --json-summary of this branch')
    add('--minimum', type=float, default=90.0, help='coverage that always passes')
    add('--base', default='origin/main', help='branch to beat while below the minimum')
    add('--make', default='make', help='make command used to measure the base')
    add('--watch', nargs='+', default=['src', 'tests'], help='paths that turn the gate on')
    add('--make-var', action='append', default=[], help='VAR=value passed to make for the base')
    args = parser.parse_args()

    current = line_coverage(args.summary)
    print(f'coverage gate: this branch covers {current:.1f}% of lines '
          f'(minimum {args.minimum:.0f}%)')
    if current >= args.minimum:
        print('coverage gate: PASS, at or above the minimum')
        return 0
    if git('rev-parse', 'HEAD') == git('rev-parse', args.base):
        print(f'coverage gate: SKIP, this is {args.base} itself')
        return 0
    if not changes_watched_paths(args.base, args.watch):
        print(f'coverage gate: SKIP, no changes under {" ".join(args.watch)}')
        return 0

    baseline = base_coverage(args.base, args.make, args.make_var)
    print(f'coverage gate: {args.base} covers {baseline:.1f}% of lines')
    if current > baseline:
        print(f'coverage gate: PASS, up {current - baseline:.1f} points from {args.base}')
        return 0
    print(f'coverage gate: FAIL, below {args.minimum:.0f}% and not above {args.base} '
          f'({baseline:.1f}%). Add tests so this branch raises coverage.')
    return 1


if __name__ == '__main__':
    sys.exit(main())
