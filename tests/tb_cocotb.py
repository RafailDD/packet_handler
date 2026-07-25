import cocotb
from cocotb.clock import Clock
from cocotb.triggers import RisingEdge, Timer

async def reset_dut(dut):
    dut.i_clk.value = 0
    dut.i_rst_n.value = 0
    dut.i_data.value = 0
    dut.i_valid.value = 0
    dut.i_last.value = 0
    dut.i_ready.value = 0
    await Timer(10, unit="ns")
    dut.i_rst_n.value = 1
    await Timer(10, unit="ns")

async def send_word_bp(dut, word, is_last=False):
    """Sends a single word, respecting o_ready backpressure."""
    dut.i_data.value = word
    dut.i_valid.value = 1
    dut.i_last.value = 1 if is_last else 0

    # Wait untilDUT is ready to accept
    while dut.o_ready.value != 1:
        await RisingEdge(dut.i_clk)

    await RisingEdge(dut.i_clk)

async def send_packet_bp(dut, stream_id, seq_number, data_words):
    """Sends a packet, respecting o_ready backpressure on every word."""
    msg_length = 8 + len(data_words) * 4

    word1 = ((msg_length & 0xFF) << 24) | (((msg_length >> 8) & 0xFF) << 16) | \
            ((stream_id & 0xFF) << 8) | (((stream_id >> 8) & 0xFF))

    word2 = ((seq_number & 0xFF) << 24) | (((seq_number >> 8) & 0xFF) << 16) | \
            (((seq_number >> 16) & 0xFF) << 8) | (((seq_number >> 24) & 0xFF))

    await send_word_bp(dut, word1)
    await send_word_bp(dut, word2)

    for i, data_word in enumerate(data_words):
        is_last = (i == len(data_words) - 1)
        await send_word_bp(dut, data_word, is_last=is_last)

    dut.i_valid.value = 0
    dut.i_last.value = 0

def send_bytes_little_endian(dut, val, bytes_count):
    # Mask to ensure val fits within bytes_count before conversion to avoid OverflowError
    val_masked = val & ((1 << (8 * bytes_count)) - 1)
    return int.from_bytes(val_masked.to_bytes(bytes_count, 'little'), 'big')

async def send_packet(dut, stream_id, seq_number, data_words):
    # Calculate msgLength: 8 bytes header + data_words * 4 bytes
    msg_length = 8 + len(data_words) * 4

    # Wait until DUT is ready
    while dut.o_ready.value != 1:
        await RisingEdge(dut.i_clk)

    # Word 1: msgLength (2 bytes), streamId (2 bytes)
    word1 = ((msg_length & 0xFF) << 24) | (((msg_length >> 8) & 0xFF) << 16) | \
            ((stream_id & 0xFF) << 8) | (((stream_id >> 8) & 0xFF))

    dut.i_data.value = word1
    dut.i_valid.value = 1
    dut.i_last.value = 0
    await RisingEdge(dut.i_clk)

    # Word 2: seqNumber (4 bytes)
    word2 = ((seq_number & 0xFF) << 24) | (((seq_number >> 8) & 0xFF) << 16) | \
            (((seq_number >> 16) & 0xFF) << 8) | (((seq_number >> 24) & 0xFF))

    dut.i_data.value = word2
    dut.i_valid.value = 1
    await RisingEdge(dut.i_clk)

    # Data Words
    for i, data_word in enumerate(data_words):
        dut.i_data.value = data_word
        dut.i_valid.value = 1
        if i == len(data_words) - 1:
            dut.i_last.value = 1
        else:
            dut.i_last.value = 0
        await RisingEdge(dut.i_clk)

    dut.i_valid.value = 0
    dut.i_last.value = 0

@cocotb.test()
async def test_reset(dut):
    """Test initial state after reset."""
    cocotb.start_soon(Clock(dut.i_clk, 10, unit="ns").start())
    await reset_dut(dut)

    assert dut.o_ready.value == 1, "DUT should be ready after reset"
    assert dut.o_valid.value == 0, "DUT should not be valid after reset"
    assert dut.o_packetLost.value == 0, "packetLost should be 0 after reset"

@cocotb.test()
async def test_basic_packet(dut):
    """Test sending a basic packet."""
    cocotb.start_soon(Clock(dut.i_clk, 10, unit="ns").start())
    await reset_dut(dut)

    data = [0x5A22B2D4, 0x5A22B49E, 0x42E3C71E, 0x43995B7E]
    await send_packet(dut, stream_id=15, seq_number=1, data_words=data)

    # Wait for o_valid
    for _ in range(5):
        if dut.o_valid.value == 1:
            break
        await RisingEdge(dut.i_clk)

    assert dut.o_valid.value == 1, "DUT should assert o_valid after receiving packet"
    assert dut.o_packetLost.value == 0, "packetLost should not be asserted"

    dut.i_ready.value = 1
    await RisingEdge(dut.i_clk)
    await RisingEdge(dut.i_clk)

    expected_data = (data[0] << 96) | (data[1] << 64) | (data[2] << 32) | data[3]
    try:
        out_val = dut.o_data.value.to_unsigned() & 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
    except Exception:
        out_val = 0

    assert out_val == expected_data, f"Output data mismatch. Expected {hex(expected_data)}, got {hex(out_val)}"

    dut.i_ready.value = 0

@cocotb.test()
async def test_multiple_valid_streams(dut):
    """Test multiple streams with correct seqNumbers."""
    cocotb.start_soon(Clock(dut.i_clk, 10, unit="ns").start())
    await reset_dut(dut)

    # Stream 1, seq 1
    data1 = [0x11111111, 0x22222222]
    await send_packet(dut, stream_id=1, seq_number=1, data_words=data1)

    while dut.o_valid.value != 1:
        await RisingEdge(dut.i_clk)
    assert dut.o_packetLost.value == 0
    dut.i_ready.value = 1
    await RisingEdge(dut.i_clk)
    dut.i_ready.value = 0

    # Stream 2, seq 1
    data2 = [0x33333333, 0x44444444]
    await send_packet(dut, stream_id=2, seq_number=1, data_words=data2)

    while dut.o_valid.value != 1:
        await RisingEdge(dut.i_clk)
    assert dut.o_packetLost.value == 0
    dut.i_ready.value = 1
    await RisingEdge(dut.i_clk)
    dut.i_ready.value = 0

    # Stream 1, seq 2
    data3 = [0x55555555, 0x66666666]
    await send_packet(dut, stream_id=1, seq_number=2, data_words=data3)

    while dut.o_valid.value != 1:
        await RisingEdge(dut.i_clk)
    assert dut.o_packetLost.value == 0
    dut.i_ready.value = 1
    await RisingEdge(dut.i_clk)
    dut.i_ready.value = 0

import random

@cocotb.test()
async def test_dropped_lost_packet(dut):
    """Test sending a packet with skipped seqNumber to trigger packetLost."""
    cocotb.start_soon(Clock(dut.i_clk, 10, unit="ns").start())
    await reset_dut(dut)

    # Send packet 1, stream 5, seq 1
    data1 = [0xAAAAAAAA, 0xBBBBBBBB]
    await send_packet(dut, stream_id=5, seq_number=1, data_words=data1)

    while dut.o_valid.value != 1:
        await RisingEdge(dut.i_clk)
    assert dut.o_packetLost.value == 0, "Packet lost should not be 1 for first packet"

    dut.i_ready.value = 1
    await RisingEdge(dut.i_clk)
    dut.i_ready.value = 0

    # Now send packet 2, stream 5, seq 3 (skipped 2)
    packet_lost_detected = False

    async def monitor_packet_lost():
        nonlocal packet_lost_detected
        while True:
            await RisingEdge(dut.i_clk)
            if dut.o_packetLost.value == 1:
                packet_lost_detected = True

    monitor_task = cocotb.start_soon(monitor_packet_lost())

    data2 = [0xCCCCCCCC, 0xDDDDDDDD]
    await send_packet(dut, stream_id=5, seq_number=3, data_words=data2)

    while dut.o_valid.value != 1:
        await RisingEdge(dut.i_clk)

    dut.i_ready.value = 1
    await RisingEdge(dut.i_clk)
    dut.i_ready.value = 0

    monitor_task.cancel()

    assert packet_lost_detected, "Packet lost signal was not asserted for skipped seqNumber"

@cocotb.test()
async def test_min_length_packet(dut):
    """Test sending the smallest possible packet (9 bytes).
    This means header (8 bytes) + 1 byte data.
    """
    cocotb.start_soon(Clock(dut.i_clk, 10, unit="ns").start())
    await reset_dut(dut)

    data = [0x000000FF]

    msg_length = 9
    stream_id = 10
    seq_number = 1

    while dut.o_ready.value != 1:
        await RisingEdge(dut.i_clk)

    word1 = ((msg_length & 0xFF) << 24) | (((msg_length >> 8) & 0xFF) << 16) | \
            ((stream_id & 0xFF) << 8) | (((stream_id >> 8) & 0xFF))
    dut.i_data.value = word1
    dut.i_valid.value = 1
    dut.i_last.value = 0
    await RisingEdge(dut.i_clk)

    word2 = ((seq_number & 0xFF) << 24) | (((seq_number >> 8) & 0xFF) << 16) | \
            (((seq_number >> 16) & 0xFF) << 8) | (((seq_number >> 24) & 0xFF))
    dut.i_data.value = word2
    dut.i_valid.value = 1
    await RisingEdge(dut.i_clk)

    dut.i_data.value = data[0]
    dut.i_valid.value = 1
    dut.i_last.value = 1
    await RisingEdge(dut.i_clk)

    dut.i_valid.value = 0
    dut.i_last.value = 0

    while dut.o_valid.value != 1:
        await RisingEdge(dut.i_clk)

    assert dut.o_packetLost.value == 0

    dut.i_ready.value = 1
    await RisingEdge(dut.i_clk)
    await RisingEdge(dut.i_clk)

    try:
        out_val = dut.o_data.value.to_unsigned() & 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
    except Exception:
        out_val = 0
    expected_data = data[0]
    assert out_val == expected_data, f"Output data mismatch. Expected {hex(expected_data)}, got {hex(out_val)}"

    dut.i_ready.value = 0

@cocotb.test()
async def test_randomized_packets(dut):
    """Test with randomized packet lengths, streams, and data payloads."""
    cocotb.start_soon(Clock(dut.i_clk, 10, unit="ns").start())
    await reset_dut(dut)

    streams_seq = {i: 1 for i in range(1, 33)}

    for _ in range(50):
        stream_id = random.randint(1, 32)
        seq_num = streams_seq[stream_id]
        streams_seq[stream_id] += 1

        num_words = random.randint(1, 8)
        data = [random.getrandbits(32) for _ in range(num_words)]

        await send_packet(dut, stream_id, seq_num, data)

        while dut.o_valid.value != 1:
            await RisingEdge(dut.i_clk)

        assert dut.o_packetLost.value == 0, f"packetLost asserted incorrectly for stream {stream_id}"

        dut.i_ready.value = 1
        await RisingEdge(dut.i_clk)
        await RisingEdge(dut.i_clk)

        try:
            out_val = dut.o_data.value.to_unsigned()
        except Exception:
            out_val = 0

        expected_data = 0
        for word in data:
            expected_data = (expected_data << 32) | word

        mask = (1 << (num_words * 32)) - 1
        out_val_masked = out_val & mask

        assert out_val_masked == expected_data, f"Output data mismatch. Expected {hex(expected_data)}, got {hex(out_val_masked)}"

        dut.i_ready.value = 0

        for _ in range(random.randint(0, 3)):
            await RisingEdge(dut.i_clk)

@cocotb.test()
async def test_fifo_full_backpressure(dut):
    """Test FIFO full conditions and backpressure (o_ready)."""
    cocotb.start_soon(Clock(dut.i_clk, 10, unit="ns").start())
    await reset_dut(dut)

    # We will send 65 packets, each having 2 header words + 2 data words = 4 words.
    # Total words = 65 * 4 = 260 words.
    # The FIFO depth is 256. This means the FIFO will fill up and assert o_ready=0.

    num_packets = 65
    data_words_per_packet = 2

    # Do not assert i_ready initially so the FIFO fills up
    dut.i_ready.value = 0

    async def sender_task():
        for i in range(num_packets):
            data = [i, i+1]
            # using the backpressure-aware packet sender
            await send_packet_bp(dut, stream_id=1, seq_number=i+1, data_words=data)

    sender = cocotb.start_soon(sender_task())

    # Wait for the DUT to assert backpressure (o_ready == 0)
    # The FIFO should fill up and then o_ready should drop
    while dut.o_ready.value == 1:
        await RisingEdge(dut.i_clk)

    # Confirm it stays 0 for a few cycles
    for _ in range(5):
        assert dut.o_ready.value == 0, "o_ready should remain 0 when FIFO is full and i_ready is 0"
        await RisingEdge(dut.i_clk)

    # Now begin asserting i_ready to drain the FIFO
    dut.i_ready.value = 1

    # Receive all the packets
    for i in range(num_packets):
        while dut.o_valid.value == 0:
            await RisingEdge(dut.i_clk)

        try:
            out_val = dut.o_data.value.to_unsigned()
        except Exception:
            out_val = 0

        expected_data = (i << 32) | (i + 1)
        mask = (1 << (data_words_per_packet * 32)) - 1
        out_val_masked = out_val & mask

        assert out_val_masked == expected_data, f"Output data mismatch for packet {i}. Expected {hex(expected_data)}, got {hex(out_val_masked)}"

        # Wait for o_valid to go low before waiting for next packet
        while dut.o_valid.value == 1:
            await RisingEdge(dut.i_clk)

    # Wait for sender task to finish
    await sender

    assert dut.o_packetLost.value == 0, "packetLost should not be asserted"
async def test_max_length_packet(dut):
    """Test sending the maximum possible packet length (45 bytes).
    This means header (8 bytes) + 37 bytes data.
    Since data is 32 bits (4 bytes) wide, we send 10 words,
    and exactly 296 bits (37 bytes) are retained in o_data.
    """
    cocotb.start_soon(Clock(dut.i_clk, 10, unit="ns").start())
    await reset_dut(dut)

    # 10 data words
    data = [
        0x11223344, 0x55667788, 0x99AABBCC, 0xDDEEFF00,
        0x12345678, 0x9ABCDEF0, 0x0FEDCBA9, 0x87654321,
        0xCAFEBABE, 0xDEADBEEF
    ]

    msg_length = 45
    stream_id = 15
    seq_number = 1

    while dut.o_ready.value != 1:
        await RisingEdge(dut.i_clk)

    # Header Word 1
    word1 = ((msg_length & 0xFF) << 24) | (((msg_length >> 8) & 0xFF) << 16) | \
            ((stream_id & 0xFF) << 8) | (((stream_id >> 8) & 0xFF))
    dut.i_data.value = word1
    dut.i_valid.value = 1
    dut.i_last.value = 0
    await RisingEdge(dut.i_clk)

    # Header Word 2
    word2 = ((seq_number & 0xFF) << 24) | (((seq_number >> 8) & 0xFF) << 16) | \
            (((seq_number >> 16) & 0xFF) << 8) | (((seq_number >> 24) & 0xFF))
    dut.i_data.value = word2
    dut.i_valid.value = 1
    await RisingEdge(dut.i_clk)

    # Data Words
    for i, data_word in enumerate(data):
        dut.i_data.value = data_word
        dut.i_valid.value = 1
        if i == len(data) - 1:
            dut.i_last.value = 1
        else:
            dut.i_last.value = 0
        await RisingEdge(dut.i_clk)

    dut.i_valid.value = 0
    dut.i_last.value = 0

    while dut.o_valid.value != 1:
        await RisingEdge(dut.i_clk)

    assert dut.o_packetLost.value == 0

    dut.i_ready.value = 1
    await RisingEdge(dut.i_clk)
    await RisingEdge(dut.i_clk)

    try:
        out_val = dut.o_data.value.to_unsigned()
    except Exception:
        out_val = 0

    # Calculate expected data:
    # 37 bytes = 296 bits.
    # Data is shifted in word by word.
    # 10 words * 32 bits = 320 bits.
    # The first word's top 24 bits are shifted out and lost.
    # So the expected data is the lower 8 bits of the first word,
    # followed by the remaining 9 words.
    expected_data = (data[0] & 0xFF)
    for word in data[1:]:
        expected_data = (expected_data << 32) | word

    # The RTL shift register logic:
    # shiftReg <= {shiftReg[263:0], fifo_data};
    # After 10 words, the first 3 bytes are shifted out of the top.

    assert out_val == expected_data, f"Output data mismatch. Expected {hex(expected_data)}, got {hex(out_val)}"

    dut.i_ready.value = 0
