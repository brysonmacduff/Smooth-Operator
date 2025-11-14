## Smooth Operator Protocol

### Header Specification

| HEADER_PREAMBLE | PAYLOAD_SIZE | VERSION | CHECKSUM | PAYLOAD               |
|-----------------|--------------|---------|----------|-----------------------|
| 4 bytes         | 4 bytes      | 1 byte  | 2 bytes  | [0, (2^32) - 1] bytes |
