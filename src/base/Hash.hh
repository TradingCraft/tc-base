#pragma once

#include <string>
#include <string_view>

namespace TC {

// CRC-32/ISO-HDLC (same polynomial as zlib) of data,
// returned as 8 lowercase hex digits.
std::string crc32Hex(std::string_view data);

// SHA-256 of data, returned as 64 lowercase hex digits.
std::string sha256Hex(std::string_view data);

} // namespace TC
