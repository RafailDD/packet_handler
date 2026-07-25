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
async def test_mid_packet_reset(dut):
    """Test dropping reset in the middle of driving a packet's payload data, then recovering."""
    cocotb.start_soon(Clock(dut.i_clk, 10, unit="ns").start())
    await reset_dut(dut)

    stream_id = 7
    seq_number = 10
    data_words = [0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555]
    msg_length = 8 + len(data_words) * 4

    while dut.o_ready.value != 1:
        await RisingEdge(dut.i_clk)

    # Word 1: msgLength and streamId
    word1 = ((msg_length & 0xFF) << 24) | (((msg_length >> 8) & 0xFF) << 16) | \
            ((stream_id & 0xFF) << 8) | (((stream_id >> 8) & 0xFF))
    dut.i_data.value = word1
    dut.i_valid.value = 1
    dut.i_last.value = 0
    await RisingEdge(dut.i_clk)

    # Word 2: seqNumber
    word2 = ((seq_number & 0xFF) << 24) | (((seq_number >> 8) & 0xFF) << 16) | \
            (((seq_number >> 16) & 0xFF) << 8) | (((seq_number >> 24) & 0xFF))
    dut.i_data.value = word2
    dut.i_valid.value = 1
    await RisingEdge(dut.i_clk)

    # Send a random number of data words (but not all of them)
    reset_cycle = random.randint(1, len(data_words) - 1)
    for i in range(reset_cycle):
        dut.i_data.value = data_words[i]
        dut.i_valid.value = 1
        dut.i_last.value = 0
        await RisingEdge(dut.i_clk)

    # Drop reset mid-packet
    dut.i_rst_n.value = 0
    dut.i_valid.value = 0
    await Timer(25, unit="ns")
    dut.i_rst_n.value = 1
    await Timer(10, unit="ns")

    while dut.o_ready.value != 1:
        await RisingEdge(dut.i_clk)

    # Send a completely new packet with a random seqNumber
    new_seq_number = random.randint(1, 1000)
    new_data = [0xAAAAAAAA, 0xBBBBBBBB]
    await send_packet(dut, stream_id, new_seq_number, new_data)

    while dut.o_valid.value != 1:
        await RisingEdge(dut.i_clk)

    assert dut.o_packetLost.value == 0, f"packetLost asserted incorrectly after reset with new seqNumber {new_seq_number}"

    dut.i_ready.value = 1
    await RisingEdge(dut.i_clk)
    dut.i_ready.value = 0
