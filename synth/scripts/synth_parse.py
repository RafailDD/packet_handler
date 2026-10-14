import sys
import re
import os

def parse_yosys_log(filepath):
    if not os.path.exists(filepath):
        print(f"Error: Log file '{filepath}' not found.")
        sys.exit(1)

    warnings = []
    errors = []

    # Statistics extraction flags
    in_stats = False
    stats_lines = []
    area = None

    with open(filepath, 'r') as f:
        for line in f:
            stripped = line.strip()

            # Extract warnings and errors
            if stripped.startswith("Warning:"):
                warnings.append(stripped)
            elif stripped.startswith("ERROR:"):
                errors.append(stripped)

            # Extract module statistics section
            if "Printing statistics." in line:
                in_stats = True
                continue

            if in_stats:
                if stripped.startswith("Chip area for module"):
                    match = re.search(r"Chip area for module '.*?': ([\d\.]+)", stripped)
                    if match:
                        area = match.group(1)
                    in_stats = False # End of stats we care about mostly
                elif stripped and not stripped.startswith("==="):
                    stats_lines.append(stripped)

    # Print summary
    print("=" * 60)
    print(" SYNTHESIS REPORT SUMMARY ")
    print("=" * 60)

    print(f"\n[ERRORS]: {len(errors)}")
    for err in errors:
        print(f"  - {err}")

    print(f"\n[WARNINGS]: {len(warnings)}")
    # Deduplicate common warnings slightly for readability, or just print them all
    unique_warnings = list(set(warnings))
    for warn in unique_warnings[:10]: # Print top 10 unique warnings
        print(f"  - {warn}")
    if len(unique_warnings) > 10:
        print(f"  ... and {len(unique_warnings) - 10} more unique warnings.")

    print("\n[STATISTICS]")
    if area:
        print(f"  Total Estimated Area: {area} sq um (Sky130 HD)")

    cell_count = 0
    print("  Cell Breakdown:")
    for stat in stats_lines:
        if "Number of cells:" in stat:
            match = re.search(r"Number of cells:\s+(\d+)", stat)
            if match:
                cell_count = int(match.group(1))
            print(f"    {stat}")
        elif "sky130" in stat:
            print(f"    {stat}")
        elif stat.startswith("Number of"):
             print(f"    {stat}")

    print("=" * 60)


def parse_sta_report(filepath):
    """Print WNS / TNS / Fmax from the OpenSTA report written by `make sta`."""
    if not os.path.exists(filepath):
        print(f"\n[TIMING]: '{filepath}' not found (run 'make sta').")
        return

    patterns = {
        "WNS (ns)": r"^wns max\s+(\S+)",
        "TNS (ns)": r"^tns max\s+(\S+)",
        "Min period (ns)": r"period_min = (\S+)",
        "Fmax (MHz)": r"fmax = (\S+)",
    }
    with open(filepath, 'r') as f:
        text = f.read()

    print("\n[TIMING] (OpenSTA, no wire parasitics)")
    for label, pattern in patterns.items():
        match = re.search(pattern, text, re.MULTILINE)
        print(f"  {label:16}: {match.group(1) if match else 'n/a'}")
    print("=" * 60)


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 synth_parse.py <path_to_synth_log> [<path_to_sta_rpt>]")
        sys.exit(1)

    log_file = sys.argv[1]
    parse_yosys_log(log_file)
    if len(sys.argv) > 2:
        parse_sta_report(sys.argv[2])
