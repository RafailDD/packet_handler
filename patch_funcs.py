with open("tests/tb_cocotb.py", "r") as f:
    content = f.read()

funcs = """
async def send_word_bp(dut, word, is_last=False):
    \"\"\"Sends a single word, respecting o_ready backpressure.\"\"\"
    dut.i_data.value = word
    dut.i_valid.value = 1
    dut.i_last.value = 1 if is_last else 0

    # Wait untilDUT is ready to accept
    while dut.o_ready.value != 1:
        await RisingEdge(dut.i_clk)

    await RisingEdge(dut.i_clk)

async def send_packet_bp(dut, stream_id, seq_number, data_words):
    \"\"\"Sends a packet, respecting o_ready backpressure on every word.\"\"\"
    msg_length = 8 + len(data_words) * 4

    word1 = ((msg_length & 0xFF) << 24) | (((msg_length >> 8) & 0xFF) << 16) | \\
            ((stream_id & 0xFF) << 8) | (((stream_id >> 8) & 0xFF))

    word2 = ((seq_number & 0xFF) << 24) | (((seq_number >> 8) & 0xFF) << 16) | \\
            (((seq_number >> 16) & 0xFF) << 8) | (((seq_number >> 24) & 0xFF))

    await send_word_bp(dut, word1)
    await send_word_bp(dut, word2)

    for i, data_word in enumerate(data_words):
        is_last = (i == len(data_words) - 1)
        await send_word_bp(dut, data_word, is_last=is_last)

    dut.i_valid.value = 0
    dut.i_last.value = 0
"""

import sys
if "async def send_word_bp" not in content:
    idx = content.find("def send_bytes_little_endian")
    content = content[:idx] + funcs + "\n" + content[idx:]
    with open("tests/tb_cocotb.py", "w") as f:
        f.write(content)
