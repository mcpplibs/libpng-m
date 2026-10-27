#!/usr/bin/env python3
"""Run libpng's own pngtest on examples/pngtest, its port to C++ against
import mcpplibs.mwrap.libpng, over the release's test images, and compare
every outcome with upstream's pngtest.c itself.

The release tarball is downloaded once into target/upstream and checked
against upstream/documents.json; its files are used unchanged. Two hosts are
built from it:

- the port, examples/pngtest (the module, no png.h);
- the reference: the release's pngtest.c, byte for byte, compiled as C
  against png.h and the same compat.libpng (target/conformance/reference).

The cases are those of upstream's tests/pngtest-all, plus the image suites:

- `--strict pngtest.png`: the standard test (CMake's and autotools' pngtest);
- `--relaxed` contrib/testpngs/crashers/badcrc.png, then `--xfail` on every
  other crasher (bad Adler-32, bad iCCP, empty and huge chunks, huge IDAT):
  each must be an expected failure (XFAIL) or pass exactly as upstream's;
- contrib/testpngs/badpal/*.png: each must report "Read palette index
  exceeding num_palette" and "Wrote palette index exceeding num_palette";
- `-m` over contrib/pngsuite (with interlaced/) and contrib/testpngs: every
  image must PASS, and the bas*.png zero-sample counts must be the ones
  contrib/pngsuite/README lists;
- `--strict` on each pngsuite image: strict mode also requires the rewritten
  file to be byte-identical, which holds only for images libpng wrote with
  its defaults, so here the outcome is upstream's, whichever it is;
- pngtest with no arguments next to pngtest.png (three runs, the progress
  callbacks on the second).

For every case the port's exit status and output must equal the
reference's, byte for byte.
"""
import argparse
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
from inventory import fetch  # noqa: E402

REFERENCE_MANIFEST = '''[package]
name        = "pngtest-reference"
version     = "0.1.0"
description = "libpng 1.6.43's pngtest.c, unchanged, against png.h: the reference tools/run_conformance.py compares the port with"

[targets.pngtest-reference]
kind = "bin"
main = "src/pngtest.c"

[dependencies]
compat.libpng = "1.6.43"
compat.zlib   = "1.3.2"

# A C program links with the C driver: libpng's gamma code needs libm.
[target.'cfg(os = "linux")'.build]
ldflags = ["-lm"]
'''


def build(project: Path, name: str, mcpp: str, toolchain: str | None) -> Path:
    command = [mcpp, 'build', '--profile', 'release'] + (['--toolchain', toolchain] if toolchain else [])
    print('+', ' '.join(command), f'({project.name})', flush=True)
    subprocess.run(command, cwd=project, check=True)
    exe = name + ('.exe' if os.name == 'nt' else '')
    found = sorted((p for p in (project / 'target').rglob(exe) if p.parent.name == 'bin'), key=lambda p: p.stat().st_mtime)
    if not found:
        sys.exit(f'{project} was not built')
    return found[-1]


def run(exe: Path, args: list[str], cwd: Path) -> tuple[int, str]:
    out = subprocess.run([str(exe), *args], cwd=cwd, capture_output=True, timeout=600)
    return out.returncode, (out.stdout + out.stderr).decode('latin-1')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--mcpp', default='mcpp')
    parser.add_argument('--toolchain')
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[1]
    upstream = fetch(root)

    reference_dir = root / 'target/conformance/reference'
    (reference_dir / 'src').mkdir(parents=True, exist_ok=True)
    (reference_dir / 'mcpp.toml').write_text(REFERENCE_MANIFEST, encoding='utf-8')
    shutil.copyfile(upstream / 'pngtest.c', reference_dir / 'src/pngtest.c')
    port = build(root / 'examples/pngtest', 'pngtest', args.mcpp, args.toolchain)
    reference = build(reference_dir, 'pngtest-reference', args.mcpp, args.toolchain)

    def rel(paths):
        return [p.relative_to(upstream).as_posix() for p in sorted(paths)]

    crashers = upstream / 'contrib/testpngs/crashers'
    suite = rel((upstream / 'contrib/pngsuite').glob('*.png')) + rel((upstream / 'contrib/pngsuite/interlaced').glob('*.png'))
    testpngs = rel((upstream / 'contrib/testpngs').glob('*.png'))
    cases = [('standard', ['--strict', 'pngtest.png'], 'pass'),
             ('crasher', ['--relaxed', 'contrib/testpngs/crashers/badcrc.png'], 'pass')]
    cases += [('crasher', ['--xfail', f], 'xfail') for f in rel(crashers.glob('*.png')) if not f.endswith('/badcrc.png')]
    cases += [('badpal', [f], 'badpal') for f in rel((upstream / 'contrib/testpngs/badpal').glob('*.png'))]
    cases += [('pngsuite -m', ['-m', *suite], 'pass'), ('testpngs -m', ['-m', *testpngs], 'pass')]
    # --strict also fails when the rewritten file differs from the original,
    # which only libpng-written files avoid: the outcome must be upstream's.
    cases += [('pngsuite --strict', ['--strict', f], 'as-upstream') for f in suite]
    cases += [('default', [], 'pass')]

    failures, counted, strict_identical = [], 0, 0
    for kind, case_args, expect in cases:
        label = f'{kind}: pngtest {" ".join(case_args[:2])}{" …" if len(case_args) > 2 else ""}'
        status, output = run(port, case_args, upstream)
        ref_status, ref_output = run(reference, case_args, upstream)
        problems = []
        if (status, output) != (ref_status, ref_output):
            problems.append(f'differs from upstream pngtest.c (exit {status} vs {ref_status})')
            if output != ref_output:
                import difflib
                problems.append(''.join(list(difflib.unified_diff(ref_output.splitlines(True), output.splitlines(True), 'upstream', 'port'))[:40]))
        if expect == 'pass':
            if status != 0 or 'libpng passes test' not in output or ' FAIL' in output:
                problems.append(f'expected to pass (exit {status})')
            counted += output.count(' PASS (')
        elif expect == 'xfail':
            if status != 0 or not re.search(r' (XFAIL|PASS \(\d+ zero samples\))\n', output):
                problems.append(f'expected XFAIL or PASS, exit {status}')
        elif expect == 'badpal':
            for message in ('IDAT: Read palette index exceeding num_palette', 'Wrote palette index exceeding num_palette'):
                if message not in output:
                    problems.append(f'missing report: {message}')
        if expect == 'as-upstream' and not problems:
            strict_identical += status == 0
        verdict = 'XFAIL' if ' XFAIL' in output else ('ok' if not problems else 'FAIL')
        print(f'{verdict:5} {label}', flush=True)
        if problems:
            failures.append(label)
            print('\n'.join(problems) + '\n' + output[-2000:], flush=True)

    # The zero-sample counts contrib/pngsuite/README gives for `pngtest -m bas*.png`.
    readme = (upstream / 'contrib/pngsuite/README').read_text(encoding='utf-8')
    expected = dict(re.findall(r'Testing (bas\w+\.png): PASS \((\d+) zero samples\)', readme))
    _, output = run(port, ['-m', *[f'contrib/pngsuite/{name}' for name in sorted(expected)]], upstream)
    got = dict(re.findall(r'Testing contrib/pngsuite/(bas\w+\.png):\s*PASS \((\d+) zero samples\)', output))
    if got != expected or len(expected) != 15:
        failures.append('contrib/pngsuite/README zero-sample counts')
        print(f'FAIL  README counts: expected {expected}\n got {got}', flush=True)
    else:
        print(f'ok    contrib/pngsuite/README: the zero-sample counts of all {len(expected)} bas*.png', flush=True)

    print(f'{len(cases) - len(failures)} of {len(cases)} cases as upstream, {counted} images passed; '
          f'{len(suite)} pngsuite, {len(testpngs)} testpngs, {len(list(crashers.glob("*.png")))} crashers; '
          f'{strict_identical} pngsuite images rewritten byte-identically under --strict')
    if failures or counted < 167:  # 167 in 1.6.43: a guard against finding no images
        sys.exit(1)


if __name__ == '__main__':
    main()
