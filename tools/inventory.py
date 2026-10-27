#!/usr/bin/env python3
"""The public surface of libpng's png.h, from the pinned release.

Shared by tools/gen_exports.py (which writes the export list and the option
constants) and tools/check_exports.py (which audits the module against it).
The headers come from the upstream tarball, downloaded once into
target/upstream and checked against upstream/documents.json: the same bytes
compat.libpng builds. compat.libpng's pnglibconf.h is upstream's
scripts/pnglibconf.h.prebuilt (the same options and settings; only the
comment header differs), so that file is the configuration read here.

png.h declares its API through macros (PNG_EXPORT, PNG_EXPORTA,
PNG_FP_EXPORT, PNG_FIXED_EXPORT, PNG_CALLBACK, PNG_FUNCTION) and inside
`#ifdef PNG_..._SUPPORTED` blocks, so a plain scan of the text would both
miss declarations and find ones this configuration does not declare. The
three headers are therefore first preprocessed here, conditionals only:
`#if` / `#ifdef` / `#elif` are evaluated with the configuration's macros (and
the <limits.h> values of every supported target: 8-bit char, 32-bit int),
other identifiers are undefined. The declarations are then read from the
active text with the export macros normalised.
"""
from dataclasses import dataclass, field
import hashlib
import json
from pathlib import Path
import re
import sys
import tarfile
import urllib.request

HEADERS = ('pnglibconf.h', 'pngconf.h', 'png.h')

# <limits.h> on every supported target; pngconf.h picks its integer types with them.
LIMITS = {
    'CHAR_BIT': '8', 'UCHAR_MAX': '255', 'SHRT_MIN': '(-32768)', 'SHRT_MAX': '32767', 'USHRT_MAX': '65535',
    'INT_MIN': '(-2147483647-1)', 'INT_MAX': '2147483647', 'UINT_MAX': '4294967295',
    'LONG_MIN': '(-2147483647-1)', 'LONG_MAX': '2147483647', 'ULONG_MAX': '4294967295',
}


def fetch(root: Path) -> Path:
    pins = json.loads((root / 'upstream/documents.json').read_text(encoding='utf-8'))
    cache = root / 'target/upstream'
    cache.mkdir(parents=True, exist_ok=True)
    archive = cache / 'libpng-release.tar.gz'
    if not archive.exists() or hashlib.sha256(archive.read_bytes()).hexdigest() != pins['upstream_sha256']:
        print('+ fetch', pins['upstream'], flush=True)
        with urllib.request.urlopen(pins['upstream'], timeout=120) as response:
            archive.write_bytes(response.read())
    digest = hashlib.sha256(archive.read_bytes()).hexdigest()
    if digest != pins['upstream_sha256']:
        sys.exit(f'upstream archive checksum {digest} != {pins["upstream_sha256"]}')
    with tarfile.open(archive) as tar:
        top = tar.getmembers()[0].name.split('/')[0]
        if not (cache / top).exists():
            tar.extractall(cache, filter='data')
    return cache / top


def header_path(upstream: Path, name: str) -> Path:
    return upstream / ('scripts/pnglibconf.h.prebuilt' if name == 'pnglibconf.h' else name)


def strip_comments(text: str) -> str:
    text = re.sub(r'/\*.*?\*/', lambda m: ' ' + '\n' * m.group(0).count('\n'), text, flags=re.S)
    return re.sub(r'//[^\n]*', '', text)


def logical_lines(text: str) -> list[str]:
    return text.replace('\\\n', ' ').split('\n')


class Preprocessor:
    """Conditionals and #define / #undef only; nothing is expanded in the text."""

    def __init__(self, predefined: dict[str, str]):
        self.defines = dict(predefined)
        self.active_text: list[str] = []
        self.defined_active: list[str] = []      # names #define'd in an active branch, in order
        self.defined_anywhere: set[str] = set()  # every #define of the headers, active or not

    def value(self, expr: str, depth: int = 0) -> int:
        expr = re.sub(r'\bdefined\s*\(\s*(\w+)\s*\)|\bdefined\s+(\w+)',
                      lambda m: '1' if (m.group(1) or m.group(2)) in self.defines else '0', expr)
        expr = re.sub(r'\b\w+\s*\([^()]*\)', '0', expr)  # __has_attribute(...) and the like
        expr = expr.replace('~0U', '4294967295')

        def name(m):
            word = m.group(0)
            if word in self.defines and depth < 8:
                body = self.defines[word].strip()
                return f'({self.value(body, depth + 1)})' if body else '1'
            return '0'
        expr = re.sub(r'\b[A-Za-z_]\w*\b', name, expr)
        expr = re.sub(r'\b(0x[0-9A-Fa-f]+|\d+)[uUlL]+\b', r'\1', expr)
        expr = expr.replace('&&', ' and ').replace('||', ' or ')
        expr = re.sub(r'!(?!=)', ' not ', expr)
        expr = expr.replace('/', '//')
        try:
            return int(eval(expr, {'__builtins__': {}}))  # noqa: S307 - integer constant expressions of three headers
        except Exception as error:  # pragma: no cover
            raise SystemExit(f'cannot evaluate #if {expr!r}: {error}')

    def run(self, text: str):
        stack: list[list[bool]] = []  # [taking now, taken already, parent active]
        active = True
        for line in logical_lines(strip_comments(text)):
            m = re.match(r'\s*#\s*(\w+)\s*(.*)', line)
            if not m:
                if active:
                    self.active_text.append(line)
                continue
            directive, rest = m.group(1), m.group(2).strip()
            if directive in ('if', 'ifdef', 'ifndef'):
                if directive == 'if':
                    cond = active and bool(self.value(rest))
                elif directive == 'ifdef':
                    cond = active and rest.split()[0] in self.defines
                else:
                    cond = active and rest.split()[0] not in self.defines
                stack.append([cond, cond, active])
                active = cond
            elif directive == 'elif':
                frame = stack[-1]
                cond = frame[2] and not frame[1] and bool(self.value(rest))
                frame[0] = cond
                frame[1] = frame[1] or cond
                active = cond
            elif directive == 'else':
                frame = stack[-1]
                cond = frame[2] and not frame[1]
                frame[0] = cond
                frame[1] = True
                active = cond
            elif directive == 'endif':
                frame = stack.pop()
                active = frame[2]
            elif directive == 'define':
                d = re.match(r'(\w+)(\([^)]*\))?\s*(.*)', rest)
                self.defined_anywhere.add(d.group(1))
                if active:
                    self.defines[d.group(1)] = '' if d.group(2) else d.group(3)
                    self.defined_active.append(d.group(1))
            elif directive == 'undef':
                if active:
                    self.defines.pop(rest.split()[0], None)


@dataclass
class Surface:
    types: list[str] = field(default_factory=list)       # typedef names
    tags: list[str] = field(default_factory=list)        # struct tags (struct png_color_struct …)
    functions: list[str] = field(default_factory=list)   # PNG_EXPORT* declarations of this configuration
    deprecated: list[str] = field(default_factory=list)  # … of which PNG_DEPRECATED
    removed: list[str] = field(default_factory=list)     # PNG_REMOVED: not declared at all
    options: list[str] = field(default_factory=list)     # pnglibconf.h's PNG_*_SUPPORTED, on or off
    macros: dict[str, str] = field(default_factory=dict)  # every #define of the headers -> header
    active_macros: set[str] = field(default_factory=set)  # defined by this configuration


def surface(upstream: Path) -> Surface:
    pp = Preprocessor(LIMITS)
    s = Surface()
    for name in HEADERS:
        text = header_path(upstream, name).read_text(encoding='utf-8')
        before = set(pp.defined_anywhere)
        pp.run(text)
        for macro in pp.defined_anywhere - before:
            s.macros.setdefault(macro, name)
        if name == 'pnglibconf.h':
            s.options = sorted(set(re.findall(r'^\s*(?:#\s*define\s+|/\*\s*#\s*undef\s+)(PNG_\w+_SUPPORTED)\b',
                                              text, flags=re.M)))
    s.active_macros = set(pp.defined_active)
    code = '\n'.join(pp.active_text)
    # PNG_EXPORT(ordinal, type, name, args) and its variants: the third argument
    # is the declared name, whatever attributes follow (PNG_DEPRECATED …).
    for m in re.finditer(r'\bPNG_(EXPORTA|EXPORT|FP_EXPORT|FIXED_EXPORT)\s*\(\s*\d+\s*,[^,]*,\s*(\w+)\s*,', code):
        s.functions.append(m.group(2))
        if m.group(1) == 'EXPORTA':
            attributes = code[m.end():code.index(';', m.end())]
            if 'PNG_DEPRECATED' in attributes:
                s.deprecated.append(m.group(2))
    s.removed = re.findall(r'\bPNG_REMOVED\s*\(\s*\d+\s*,[^,]*,\s*(\w+)\s*,', code)
    types, tags = set(), set()
    # typedef PNG_CALLBACK(ret, *name, args); and PNG_FUNCTION(ret, (PNGCAPI *name), args, typedef);
    types |= set(re.findall(r'typedef\s+PNG_CALLBACK\s*\([^,]*,\s*\*\s*(\w+)\s*,', code))
    types |= set(re.findall(r'PNG_FUNCTION\s*\([^,]*,\s*\(\s*\w*\s*\*\s*(\w+)\s*\)[^;]*\btypedef\s*\)', code))
    plain = re.sub(r'typedef\s+PNG_CALLBACK\s*\([^;]*;|\bPNG_FUNCTION\s*\([^;]*\btypedef\s*\)\s*;', ';', code)
    # typedef … name;  (a struct body is reduced to a marker first)
    flat = re.sub(r'\{[^{}]*\}', ' @BODY@ ', plain)
    for m in re.finditer(r'\btypedef\s+([^;]*?);', flat, flags=re.S):
        decl = ' '.join(m.group(1).split())
        record = re.match(r'struct\s+(\w+)?\s*(@BODY@)?\s*(.*)', decl)
        if record:
            tag, body, rest = record.groups()
            if tag and not body and not re.search(r'\w', rest):  # `struct name;` is not a typedef
                raise SystemExit(f'unexpected typedef {decl!r}')
            if tag:
                tags.add(tag)
            declarators = rest
        else:
            declarators = re.match(r'(?:const\s+)?(?:unsigned\s+\w+(?:\s+int)?|long\s+int|\w+)\s*(.*)', decl).group(1)
        for declarator in declarators.split(','):
            names = [n for n in re.findall(r'\w+', declarator) if n not in ('const', 'PNG_RESTRICT')]
            if names:
                types.add(names[-1])
    s.types = sorted(types)
    s.tags = sorted(tags - types)
    s.functions = sorted(set(s.functions))
    return s


if __name__ == '__main__':
    root = Path(__file__).resolve().parents[1]
    sf = surface(fetch(root))
    print(f'{len(sf.types)} types, {len(sf.tags)} struct tags, {len(sf.functions)} functions '
          f'({len(sf.deprecated)} deprecated: {" ".join(sf.deprecated)}), {len(sf.removed)} removed, '
          f'{len(sf.options)} options, {len(sf.macros)} macros ({len(sf.active_macros & sf.macros.keys())} defined here)')
    print('types:', ' '.join(sf.types))
    print('tags:', ' '.join(sf.tags))
    print('inactive macros:', ' '.join(sorted(set(sf.macros) - sf.active_macros)))
