import re

with open("tests/tb_cocotb.py", "r") as f:
    content = f.read()

conflict_marker_start = "<<<<<<< test-mid-packet-reset-15716333768439793566"
conflict_marker_mid = "======="
conflict_marker_end = ">>>>>>> jules-experimenting"

if conflict_marker_start in content:
    # Just clean it up by regex or replacing blocks
    print("Found conflict markers. Need more robust replacement.")
