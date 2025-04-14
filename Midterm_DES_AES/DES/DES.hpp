#include <iostream>
#include <fstream>
#include <iomanip>

// DES Lookup Tables
const unsigned char IP[64] = {
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};

const unsigned char FP[64] = {
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25
};

const unsigned char E[48] = {
    32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
};

const unsigned char P[32] = {
    16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26,
    5, 18, 31, 10, 2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25
};

const unsigned char SBOX[8][4][16] = {
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
    },
    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
    },
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
    },
    {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
    },
    {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
    },
    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
    },
    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
    },
    {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
    }
};

const unsigned char PC1[56] = {
    57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4
};

const unsigned char PC2[48] = {
    14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

const unsigned char SHIFTS[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

// Debug log
std::ofstream log_file;

void log_bits(const unsigned char* data, int len, const char* step) {
    log_file << step << ":\n";
    for (int i = 0; i < len; ++i) {
        log_file << std::hex << std::setw(2) << std::setfill('0') << (int)data[i] << " ";
    }
    log_file << "\n\n";
}

// ==== BIT MANIPULATION ===
void permute(const unsigned char* input, unsigned char* output, const unsigned char* table, int in_len, int out_len) {
    for (int i = 0; i < out_len / 8; ++i) {
        output[i] = 0; // Initialize output byte
    }
    for (int i = 0; i < out_len; ++i) {
        int pos = table[i] - 1; // Table is 1-based
        int byte_pos = pos / 8;
        int bit_pos = 7 - (pos % 8); // MSB first
        int bit = (input[byte_pos] >> bit_pos) & 1;
        output[i / 8] |= (bit << (7 - (i % 8)));
    }
}

void split(const unsigned char* input, unsigned char* left, unsigned char* right, int len) {
    for (int i = 0; i < len / 2 / 8; ++i) {
        left[i] = input[i];
        right[i] = input[len / 2 / 8 + i];
    }
}

void combine(const unsigned char* left, const unsigned char* right, unsigned char* output, int len) {
    for (int i = 0; i < len / 2 / 8; ++i) {
        output[i] = left[i];
        output[len / 2 / 8 + i] = right[i];
    }
}

void left_shift(unsigned char* key, int shifts, int len) {
    unsigned char temp[7];
    for (int i = 0; i < len / 8; ++i) {
        temp[i] = key[i];
    }
    for (int i = 0; i < len / 8; ++i) {
        key[i] = temp[(i + shifts) % (len / 8)];
    }
}

// ==== KEY SCHEDULE ===
void generate_subkeys(const unsigned char key[8], unsigned char subkeys[16][6]) {
    unsigned char key_bits[8], pc1_out[7] = {0}, c[4] = {0}, d[4] = {0};
    for (int i = 0; i < 8; ++i) {
        key_bits[i] = key[i];
    }
    permute(key_bits, pc1_out, PC1, 64, 56);
    split(pc1_out, c, d, 56);
    for (int i = 0; i < 16; ++i) {
        left_shift(c, SHIFTS[i], 28);
        left_shift(d, SHIFTS[i], 28);
        unsigned char cd[7] = {0};
        combine(c, d, cd, 56);
        permute(cd, subkeys[i], PC2, 56, 48);
        log_bits(subkeys[i], 6, "Subkey");
    }
}

// ==== EXPANSION ===
void expansion(const unsigned char* right, unsigned char* expanded) {
    permute(right, expanded, E, 32, 48);
    log_bits(expanded, 6, "Expansion");
}

// ==== S-BOX ===
void sbox(const unsigned char* input, unsigned char* output) {
    for (int i = 0; i < 4; ++i) {
        output[i] = 0; // Clear output
    }
    for (int i = 0; i < 8; ++i) {
        int byte_idx = i * 6 / 8;
        int bit_offset = (i * 6) % 8;
        unsigned char block = (input[byte_idx] << bit_offset);
        if (bit_offset > 2 && byte_idx + 1 < 6) {
            block |= (input[byte_idx + 1] >> (8 - bit_offset));
        }
        block &= 0x3F; // Keep 6 bits
        int row = ((block >> 5) & 0x01) | ((block & 0x01) << 1);
        int col = (block >> 1) & 0x0F;
        unsigned char val = SBOX[i][row][col];
        output[i / 2] |= (i % 2 == 0) ? (val << 4) : val;
    }
    log_bits(output, 4, "S-Box");
}

// ==== PERMUTATION ===
void permutation(const unsigned char* input, unsigned char* output) {
    permute(input, output, P, 32, 32);
    log_bits(output, 4, "Permutation");
}

// ==== XOR ===
void xor_bits(unsigned char* a, const unsigned char* b, int len) {
    for (int i = 0; i < len / 8; ++i) {
        a[i] ^= b[i];
    }
}

// ==== DES ROUNDS ===
void des_round(unsigned char* left, unsigned char* right, const unsigned char* subkey) {
    unsigned char expanded[6] = {0}, sbox_out[4] = {0}, p_out[4] = {0};
    expansion(right, expanded);
    xor_bits(expanded, subkey, 48);
    sbox(expanded, sbox_out);
    permutation(sbox_out, p_out);
    xor_bits(left, p_out, 32);
    unsigned char temp[4];
    for (int i = 0; i < 4; ++i) {
        temp[i] = left[i];
        left[i] = right[i];
        right[i] = temp[i];
    }
}

// ==== ENCRYPTION ===
void encrypt_block(unsigned char block[8], const unsigned char subkeys[16][6]) {
    unsigned char ip_out[8] = {0}, left[4], right[4];
    permute(block, ip_out, IP, 64, 64);
    log_bits(ip_out, 8, "Initial Permutation");
    split(ip_out, left, right, 64);
    for (int i = 0; i < 16; ++i) {
        des_round(left, right, subkeys[i]);
        log_bits(left, 4, "Left Half");
        log_bits(right, 4, "Right Half");
    }
    combine(right, left, ip_out, 64);
    permute(ip_out, block, FP, 64, 64);
    log_bits(block, 8, "Final Permutation");
}

// ==== DECRYPTION ===
void decrypt_block(unsigned char block[8], const unsigned char subkeys[16][6]) {
    unsigned char ip_out[8] = {0}, left[4], right[4];
    permute(block, ip_out, IP, 64, 64);
    log_bits(ip_out, 8, "Initial Permutation");
    split(ip_out, left, right, 64);
    for (int i = 15; i >= 0; --i) {
        des_round(left, right, subkeys[i]);
        log_bits(left, 4, "Left Half");
        log_bits(right, 4, "Right Half");
    }
    combine(right, left, ip_out, 64);
    permute(ip_out, block, FP, 64, 64);
    log_bits(block, 8, "Final Permutation");
}

// ==== FILE PROCESSING ===
void process_file(const char* input_file, const char* output_file, const char* log_file_path,
                 const unsigned char key[8], void (*process_block)(unsigned char[8], const unsigned char[16][6])) {
    std::ifstream in_file(input_file, std::ios::binary);
    std::ofstream out_file(output_file, std::ios::binary);
    log_file.open(log_file_path, std::ios::binary);

    if (!in_file || !out_file || !log_file) {
        std::cerr << "File error: " << input_file << " or " << output_file << " or " << log_file_path << "\n";
        return;
    }

    unsigned char subkeys[16][6];
    generate_subkeys(key, subkeys);

    unsigned char block[8] = {0};
    std::streamsize bytes_read;
    while ((bytes_read = in_file.read((char*)block, 8).gcount()) > 0) {
        if (bytes_read < 8) {
            for (std::streamsize i = bytes_read; i < 8; ++i) {
                block[i] = 0;
            }
        }
        log_bits(block, 8, "Input Block");
        process_block(block, subkeys);
        out_file.write((char*)block, 8);
        std::fill(block, block + 8, 0);
    }

    in_file.close();
    out_file.close();
    log_file.close();
}