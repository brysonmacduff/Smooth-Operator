## Smooth Operator Protocol

### Header Specification

| HEADER_PREFIX | PAYLOAD_SIZE | PAYLOAD         |
|---------------|--------------|-----------------|
| 8 bytes       | 8 bytes      | [0, 2^32] bytes |

### Header Example

| HEADER_PREFIX | PAYLOAD_SIZE | PAYLOAD           |
|---------------|--------------|-------------------|
| deadbeef      | 00000011     | This is a payload |
