#!/usr/bin/env python3
"""Audit the module surface against png.h (the pinned release's).

- src/libpng.cppm and src/options.cppm are what tools/gen_exports.py writes:
  every type, struct tag and function png.h declares in this configuration is
  exported, and every pnglibconf.h option is a constant.
- Every macro png.h, pngconf.h and pnglibconf.h define in this configuration
  is mapped in src/macros.cppm (or src/options.cppm), stands for an exported
  function of the same name, or is excluded below with a reason; the macros
  they define only in other configurations are listed below too, so a change
  of configuration fails here first.
"""
from pathlib import Path
import re
import subprocess
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
from inventory import fetch, surface  # noqa: E402

# png.h makes these function names macros (PNG_USE_READ_MACROS): the module
# exports the library's functions of the same names, which give the same
# results; the macros' bodies are mapped as PNG_get_uint_32 / _16 / int_32.
FUNCTION_OF_THE_SAME_NAME = {
    'png_get_uint_32': 'the exported function (the macro expands to PNG_get_uint_32, mapped)',
    'png_get_uint_16': 'the exported function (the macro expands to PNG_get_uint_16, mapped)',
    'png_get_int_32': 'the exported function (the macro expands to PNG_get_int_32, mapped)',
}

EXCLUDED = {
    'PNG_H': 'include guard',
    'PNGCONF_H': 'include guard',
    'PNGLCONF_H': 'include guard',
    'PNG_CONST': '`const`, for pre-1.5 code (backward compatibility only)',
    'PNGARG': 'declaration machinery (a parameter list as is)',
    'PNGAPI': 'calling-convention annotation of declarations',
    'PNGCAPI': 'calling-convention annotation of declarations',
    'PNGCBAPI': 'calling-convention annotation of callbacks',
    'PNG_IMPEXP': 'DLL import/export annotation of declarations',
    'PNG_FUNCTION': 'declaration machinery',
    'PNG_EXPORT_TYPE': 'declaration machinery',
    'PNG_EXPORT': 'declaration machinery (the functions it declares are exported)',
    'PNG_EXPORTA': 'declaration machinery (the functions it declares are exported)',
    'PNG_FP_EXPORT': 'declaration machinery (the functions it declares are exported)',
    'PNG_FIXED_EXPORT': 'declaration machinery (the functions it declares are exported)',
    'PNG_REMOVED': 'declaration machinery: declares nothing (png_get_io_chunk_name)',
    'PNG_CALLBACK': 'declaration machinery (the callback types it declares are exported)',
    'PNG_EMPTY': 'declaration machinery (an empty attribute list)',
    'PNG_DEPRECATED': 'attribute of declarations',
    'PNG_USE_RESULT': 'attribute of declarations',
    'PNG_NORETURN': 'attribute of declarations',
    'PNG_ALLOCATED': 'attribute of declarations',
    'PNG_PRIVATE': 'attribute of declarations',
    'PNG_RESTRICT': 'qualifier of declarations (restrict)',
    'PNG_PEDANTIC_WARNINGS_SUPPORTED': "selects pngconf.h's attribute annotations, not a library option",
    'PNG_LINKAGE_API': 'linkage annotation of declarations (pnglibconf.h)',
    'PNG_LINKAGE_CALLBACK': 'linkage annotation of declarations (pnglibconf.h)',
    'PNG_LINKAGE_DATA': 'linkage annotation of declarations (pnglibconf.h)',
    'PNG_LINKAGE_FUNCTION': 'linkage annotation of declarations (pnglibconf.h)',
}

# Defined by the headers only in other configurations or on other targets.
NOT_DEFINED_HERE = {
    'png_error': 'the no-PNG_ERROR_TEXT fallback; the function is exported',
    'png_chunk_error': 'the no-PNG_ERROR_TEXT fallback; the function is exported',
    'png_warning': 'the no-PNG_WARNINGS fallback; the function is exported',
    'png_chunk_warning': 'the no-PNG_WARNINGS fallback; the function is exported',
    'png_benign_error': 'the no-PNG_BENIGN_ERRORS fallback; the function is exported',
    'png_chunk_benign_error': 'the no-PNG_BENIGN_ERRORS fallback; the function is exported',
    'PNG_ARM_NEON': 'PNG_ARM_NEON_API_SUPPORTED is off (compat.libpng builds no SIMD code)',
    'PNG_MIPS_MSA': 'PNG_MIPS_MSA_API_SUPPORTED is off',
    'PNG_MIPS_MMI': 'PNG_MIPS_MMI_API_SUPPORTED is off',
    'PNG_POWERPC_VSX': 'PNG_POWERPC_VSX_API_SUPPORTED is off',
    'PNG_IGNORE_ADLER32': 'PNG_DISABLE_ADLER32_CHECK_SUPPORTED is off',
    'PNG_DLL_EXPORT': 'Windows DLL annotation of declarations',
    'PNG_DLL_IMPORT': 'Windows DLL annotation of declarations',
    'PNG_SMALL_SIZE_T': '16-bit targets only',
}


def mapped_names(*texts: str) -> set[str]:
    names = set()
    for text in texts:
        text = text[text.index('export module'):]
        names |= set(re.findall(r'^inline constexpr (?:[\w:&]+\s+)+?(\w+)\s*=', text, flags=re.M))
        names |= set(re.findall(r'^inline const [\w:]+ (\w+)\s*=', text, flags=re.M))
        names |= set(re.findall(r'^(?:inline\s+|constexpr\s+)+[\w:&\*\s]+?\b(\w+)\s*\(', text, flags=re.M))
    return names


def main():
    root = Path(__file__).resolve().parents[1]
    problems = []
    generated = subprocess.run([sys.executable, str(root / 'tools/gen_exports.py'), '--check'], capture_output=True, text=True)
    if generated.returncode != 0:
        problems.append(generated.stderr.strip() or generated.stdout.strip())
    sf = surface(fetch(root))
    exported = set(re.findall(r'export using ::(\w+);', (root / 'src/libpng.cppm').read_text(encoding='utf-8')))
    declared = set(sf.types) | set(sf.tags) | set(sf.functions)
    if declared - exported:
        problems.append('src/libpng.cppm lacks: ' + ', '.join(sorted(declared - exported)))
    if exported - declared:
        problems.append('src/libpng.cppm exports what png.h does not declare here: ' + ', '.join(sorted(exported - declared)))
    mapped = mapped_names((root / 'src/macros.cppm').read_text(encoding='utf-8'),
                          (root / 'src/options.cppm').read_text(encoding='utf-8'))
    defined = sf.active_macros & set(sf.macros)
    unaccounted = defined - mapped - FUNCTION_OF_THE_SAME_NAME.keys() - EXCLUDED.keys()
    if unaccounted:
        problems.append('macros neither mapped, an exported function nor excluded: ' + ', '.join(sorted(unaccounted)))
    both = (FUNCTION_OF_THE_SAME_NAME.keys() | EXCLUDED.keys() | NOT_DEFINED_HERE.keys()) & mapped
    if both:
        problems.append('listed as unmapped yet mapped: ' + ', '.join(sorted(both)))
    stale = (FUNCTION_OF_THE_SAME_NAME.keys() | EXCLUDED.keys()) - defined
    if stale:
        problems.append('listed but not defined by the headers here: ' + ', '.join(sorted(stale)))
    elsewhere = set(sf.macros) - sf.active_macros
    if elsewhere != NOT_DEFINED_HERE.keys():
        problems.append('macros defined only in other configurations changed: '
                        f'new {sorted(elsewhere - NOT_DEFINED_HERE.keys())}, gone {sorted(NOT_DEFINED_HERE.keys() - elsewhere)}')
    missing = set(sf.options) - mapped
    if missing:
        problems.append('pnglibconf.h options not mapped: ' + ', '.join(sorted(missing)))
    if problems:
        sys.exit('\n'.join(problems))
    print(f'{len(sf.types)} types, {len(sf.tags)} struct tags and {len(sf.functions)} functions exported '
          f'({len(sf.removed)} PNG_REMOVED, not declared); {len(defined)} macros defined here: '
          f'{len(defined & mapped)} mapped ({len(sf.options)} pnglibconf.h options), '
          f'{len(FUNCTION_OF_THE_SAME_NAME)} name an exported function, {len(EXCLUDED)} excluded; '
          f'{len(NOT_DEFINED_HERE)} defined only elsewhere')


if __name__ == '__main__':
    main()
