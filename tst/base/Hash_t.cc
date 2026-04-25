#include "Hash.hh"
#include <gtest/gtest.h>
#include <algorithm>
#include <string>

using namespace TC;

// Returns true if every character in s is a lowercase hex digit.
static bool isLowerHex(const std::string& s)
{
    return std::all_of(s.begin(), s.end(), [](unsigned char c) {
        return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
    });
}

// ─── crc32Hex ─────────────────────────────────────────────────────────────────

TEST(Crc32Hex, EmptyString)
{
    EXPECT_EQ(crc32Hex(""), "00000000");
}

TEST(Crc32Hex, CheckValue)
{
    // Standard CRC-32/ISO-HDLC check value (ISO/IEC 3309 / zlib)
    EXPECT_EQ(crc32Hex("123456789"), "cbf43926");
}

TEST(Crc32Hex, OutputLength)
{
    EXPECT_EQ(crc32Hex("").size(),                    8u);
    EXPECT_EQ(crc32Hex("hello world").size(),         8u);
    EXPECT_EQ(crc32Hex(std::string(1000, 'x')).size(), 8u);
}

TEST(Crc32Hex, OutputIsLowercaseHex)
{
    EXPECT_TRUE(isLowerHex(crc32Hex("")));
    EXPECT_TRUE(isLowerHex(crc32Hex("abc")));
    EXPECT_TRUE(isLowerHex(crc32Hex("123456789")));
}

TEST(Crc32Hex, Idempotent)
{
    const std::string in = "hello world";
    EXPECT_EQ(crc32Hex(in), crc32Hex(in));
}

TEST(Crc32Hex, DifferentInputsDifferentOutputs)
{
    EXPECT_NE(crc32Hex(""),      crc32Hex("a"));
    EXPECT_NE(crc32Hex("abc"),   crc32Hex("cba"));
    EXPECT_NE(crc32Hex("hello"), crc32Hex("Hello"));
}

TEST(Crc32Hex, NullBytesHandled)
{
    // Null bytes must not be treated as string terminators
    const std::string one(1, '\0');
    const std::string two(2, '\0');
    EXPECT_EQ(crc32Hex(one).size(), 8u);
    EXPECT_TRUE(isLowerHex(crc32Hex(one)));
    EXPECT_NE(crc32Hex(one), crc32Hex(two));
}

TEST(Crc32Hex, HighByteValues)
{
    const std::string all_ff(4, '\xff');
    EXPECT_EQ(crc32Hex(all_ff).size(), 8u);
    EXPECT_TRUE(isLowerHex(crc32Hex(all_ff)));
}

// ─── sha256Hex ────────────────────────────────────────────────────────────────

TEST(Sha256Hex, EmptyString)
{
    // NIST FIPS 180-4
    EXPECT_EQ(sha256Hex(""),
        "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
}

TEST(Sha256Hex, Abc)
{
    // NIST FIPS 180-4 example 1 (single block)
    EXPECT_EQ(sha256Hex("abc"),
        "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
}

TEST(Sha256Hex, TwoBlockMessage)
{
    // NIST FIPS 180-4 example 2 (56-byte input forces two 512-bit blocks)
    EXPECT_EQ(sha256Hex("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"),
        "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1");
}

TEST(Sha256Hex, OutputLength)
{
    EXPECT_EQ(sha256Hex("").size(),                    64u);
    EXPECT_EQ(sha256Hex("hello world").size(),         64u);
    EXPECT_EQ(sha256Hex(std::string(1000, 'x')).size(), 64u);
}

TEST(Sha256Hex, OutputIsLowercaseHex)
{
    EXPECT_TRUE(isLowerHex(sha256Hex("")));
    EXPECT_TRUE(isLowerHex(sha256Hex("abc")));
    EXPECT_TRUE(isLowerHex(sha256Hex("The quick brown fox jumps over the lazy dog")));
}

TEST(Sha256Hex, Idempotent)
{
    const std::string in = "hello world";
    EXPECT_EQ(sha256Hex(in), sha256Hex(in));
}

TEST(Sha256Hex, DifferentInputsDifferentOutputs)
{
    EXPECT_NE(sha256Hex(""),      sha256Hex("a"));
    EXPECT_NE(sha256Hex("abc"),   sha256Hex("cba"));
    EXPECT_NE(sha256Hex("hello"), sha256Hex("Hello"));
}

TEST(Sha256Hex, NullBytesHandled)
{
    const std::string one(1, '\0');
    const std::string two(2, '\0');
    EXPECT_EQ(sha256Hex(one).size(), 64u);
    EXPECT_TRUE(isLowerHex(sha256Hex(one)));
    EXPECT_NE(sha256Hex(one), sha256Hex(two));
}

// Padding boundary: 55 bytes of input fills one block exactly after padding
// (55 data + 1 pad byte 0x80 + 8 length bytes = 64 bytes = one block)
TEST(Sha256Hex, PaddingBoundary55Bytes)
{
    const std::string in(55, 'a');
    EXPECT_EQ(sha256Hex(in).size(), 64u);
    EXPECT_TRUE(isLowerHex(sha256Hex(in)));
}

// Padding boundary: 56 bytes forces a second block
// (56 data + 1 pad byte = 57; must pad to 120 + 8 length = 128 bytes = two blocks)
TEST(Sha256Hex, PaddingBoundary56Bytes)
{
    const std::string in(56, 'a');
    EXPECT_EQ(sha256Hex(in).size(), 64u);
    EXPECT_TRUE(isLowerHex(sha256Hex(in)));
    EXPECT_NE(sha256Hex(in), sha256Hex(std::string(55, 'a')));
}

// Padding boundary: 64 bytes of input (exactly one block of raw data, two after padding)
TEST(Sha256Hex, PaddingBoundary64Bytes)
{
    const std::string in(64, 'a');
    EXPECT_EQ(sha256Hex(in).size(), 64u);
    EXPECT_TRUE(isLowerHex(sha256Hex(in)));
    EXPECT_NE(sha256Hex(in), sha256Hex(std::string(56, 'a')));
}
