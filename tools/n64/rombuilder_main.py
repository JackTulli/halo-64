"""The ROM builder program's entry (packaged by tools/n64/package_rombuilder.py):
its window, or -- run by the window, "--worker <module> ..." -- one of the
converters (tools/n64/rombuilder.py run_worker)."""

import sys


def main():
    if len(sys.argv) > 1 and sys.argv[1] == "--worker":
        from tools.n64.rombuilder import run_worker
        sys.exit(run_worker(sys.argv[2:]))
    from tools.n64.rombuilder_gui import main as window
    window()


if __name__ == "__main__":
    main()
