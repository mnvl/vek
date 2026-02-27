from __future__ import annotations

import importlib
import importlib.machinery
import os
import re
import sys
from pathlib import Path
from types import ModuleType

__version__ = "1.0.0"


def _binding_dirs() -> list[Path]:
    explicit = os.environ.get("ROVE_PYTHON_BINDINGS_DIR")
    if explicit:
        return [Path(explicit).expanduser()]

    rove_root = Path(__file__).resolve().parents[1]
    edge_root = rove_root.parent
    return [
        rove_root / "build" / "lib",
        edge_root / "build" / "lib",
        rove_root / "cmake-build-debug" / "lib",
        rove_root / "cmake-build-release" / "lib",
        edge_root / "cmake-build-debug" / "lib",
        edge_root / "cmake-build-release" / "lib",
    ]


def _module_files(bindings_dir: Path, module_name: str) -> list[Path]:
    patterns = (
        f"{module_name}*.so",
        f"{module_name}*.pyd",
        f"{module_name}*.dylib",
        f"{module_name}*.dll",
    )
    result: list[Path] = []
    for pattern in patterns:
        result.extend(sorted(bindings_dir.glob(pattern)))
    return result


def _guess_python_tags(paths: list[Path]) -> list[str]:
    tags = set()
    for path in paths:
        match = re.search(r"(cpython-\d+)", path.name)
        if match:
            tags.add(match.group(1))
    return sorted(tags)


def _import_required_binding(module_name: str) -> ModuleType:
    # Prefer local build artifacts (e.g. edge/build/lib/pyrove_bind*.so).
    for candidate in _binding_dirs():
        if candidate.exists() and str(candidate) not in sys.path:
            sys.path.insert(0, str(candidate))

    try:
        module = importlib.import_module(module_name)
        # Keep compatibility with code expecting pyrove.pyrove_bind.
        sys.modules.setdefault(f"{__name__}.{module_name}", module)
        return module
    except (ModuleNotFoundError, ImportError):
        pass

    try:
        # Installed-package mode: pyrove/pyrove_bind.so exists next to this file.
        return importlib.import_module(f".{module_name}", __name__)
    except (ModuleNotFoundError, ImportError) as exc:
        extension_suffixes = ", ".join(importlib.machinery.EXTENSION_SUFFIXES)
        built_files: list[Path] = []
        for candidate in _binding_dirs():
            if candidate.exists():
                built_files.extend(_module_files(candidate, module_name))

        running_tag = sys.implementation.cache_tag
        built_tags = _guess_python_tags(built_files)
        mismatch_note = ""
        if built_tags and running_tag not in built_tags:
            mismatch_note = (
                "\nDetected ABI mismatch:\n"
                f"- active interpreter tag: {running_tag}\n"
                f"- built module tags: {', '.join(built_tags)}\n"
            )

        raise ImportError(
            f"Could not import required binding module '{module_name}'.\n"
            "Build native bindings first with the same interpreter:\n"
            "  cmake -S . -B build -DPython_EXECUTABLE=$(which python)\n"
            "  cmake --build build --target pyrove_bind\n"
            "Or install package in editable mode:\n"
            "  pip install -e .\n"
            f"extension_suffixes={extension_suffixes}\n"
            f"module_candidates={[path.name for path in built_files]}"
            f"{mismatch_note}"
        ) from exc


_bindings: ModuleType = _import_required_binding("pyrove_bind")
pyrove_bind = _bindings

for _name in dir(_bindings):
    if _name.startswith("_"):
        continue
    globals()[_name] = getattr(_bindings, _name)

__all__ = [name for name in dir(_bindings) if not name.startswith("_")]
