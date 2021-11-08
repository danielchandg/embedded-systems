#include "code.h"
#include "defines.h"
#include "io.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

uint64_t bytes_read = 0;
uint64_t bytes_written = 0;

static uint8_t buffer[BLOCK];
static uint32_t buffer_index;
static uint8_t buffer2[BLOCK];
static uint32_t buffer2_index;

// Function to read bytes using read(). Updates bytes_read.
int read_bytes(int infile, uint8_t *buf, int nbytes) {
    int cur_read = 0;
    int count = read(infile, buf, nbytes);
    while (count > 0) {
        cur_read += count;
        if (cur_read >= nbytes)
            break;
        count = read(infile, buf + cur_read, nbytes - cur_read);
    }
    bytes_read += (uint64_t) cur_read;
    return cur_read;
}

// Function to write bytes using write(). Updates bytes_written.
int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    int count;
    int cur_written = 0;
    while ((count = write(outfile, buf + cur_written, nbytes - cur_written)) > 0) {
        cur_written += count;
        if (cur_written >= nbytes)
            break;
    }
    bytes_written += (uint64_t) cur_written;
    return count;
}

// Function to read one bit from infile using a buffer.
bool read_bit(int infile, uint8_t *bit) {

    if (buffer_index == 0 || buffer_index >= 8 * BLOCK) {
        buffer_index = 0;
        read_bytes(infile, buffer, BLOCK);
    }
    int index = buffer_index / 8, shift = buffer_index % 8;
    *bit = (buffer[index] >> shift) & 1;
    buffer_index++;
    return (buffer_index < 8 * BLOCK);
}

// Function to write Codes to outfile. Saves Codes in outfile until buffer2
// is full. When buffer2 is full, the codes get printed to outfile.
void write_code(int outfile, Code *c) {
    for (uint32_t i = 0; i < code_size(c); i++) {
        if (code_get_bit(c, i)) {
            buffer2[buffer2_index / 8] |= (1 << buffer2_index % 8);
        } else {
            buffer2[buffer2_index / 8] &= ~(1 << buffer2_index % 8);
        }
        buffer2_index++;
        if (buffer2_index == 8 * BLOCK) {
            int written = write_bytes(outfile, buffer2, BLOCK);
            if (written != BLOCK) {
                printf("write_code only wrote %d bytes out of %d\n", written, BLOCK);
            }
            buffer2_index = 0;
        }
    }
}

// Function to write remaining Codes to outfile, when buffer2 is not full.
void flush_codes(int outfile) {
    for (; buffer2_index % 8 > 0; buffer2_index++) {
        buffer2[buffer2_index / 8] &= ~(1 << buffer2_index % 8);
    }
    if (buffer2_index % 8)
        printf("Flush codes error\n");
    int written = write_bytes(outfile, buffer2, buffer2_index / 8);
    if (written != buffer2_index / 8) {
        printf("flush_codes only wrote %d bytes out of %d\n", written, (int) buffer2_index / 8);
    }
}
