#!/usr/bin/env python3
"""Copy the waveforms of a SimCrux run into a stable folder for WaveCrux.

SimCrux writes each test's waveform into a temporary work directory
(/tmp/runs/<run_id>/<test>/...) and records it as `waveform_path` in
results.ndjson. This copies each one to <out_dir>/<suite>_<test>.<ext>, so
`make wavecrux` and packet_handler.crux-project can point at fixed paths.

Usage: collect_waves.py <results.ndjson> <out_dir>
"""
import json
import pathlib
import re
import shutil
import sys


def main() -> int:
    if len(sys.argv) != 3:
        print(__doc__.strip().splitlines()[-1], file=sys.stderr)
        return 2
    results, out_dir = pathlib.Path(sys.argv[1]), pathlib.Path(sys.argv[2])
    if not results.is_file():
        print(f"collect_waves: {results} not found, nothing to copy")
        return 0
    out_dir.mkdir(parents=True, exist_ok=True)
    copied = 0
    for line in results.read_text().splitlines():
        try:
            rec = json.loads(line)
        except json.JSONDecodeError:
            continue
        src = rec.get("waveform_path") if rec.get("type") == "result" else None
        if not src:
            continue
        src = pathlib.Path(src)
        if not src.is_file():
            print(f"collect_waves: {rec['id']}: {src} no longer exists")
            continue
        # Test ids look like "suite/name" or "suite/name+PARAM=value".
        name = rec["id"].split("+", 1)[0]
        dest = out_dir / (re.sub(r"[^A-Za-z0-9_.-]", "_", name.replace("/", "_")) + src.suffix)
        shutil.copyfile(src, dest)
        copied += 1
        print(f"collect_waves: {rec['id']} ({rec.get('status')}) -> {dest}")
    if copied == 0:
        print("collect_waves: no waveforms recorded in this run")
    return 0


if __name__ == "__main__":
    sys.exit(main())
