#include <iostream>
#include <fstream>
#include <iomanip>

// AES-128 Lookup tables
const unsigned char SBOX[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

const unsigned char INV_SBOX[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

const unsigned char RCON[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

// debug log để giám sát quá trình encrypt decrypt
std::ofstream log_file;

void log_state(const unsigned char state[4][4], const char* step) {
    log_file << step << ":\n";
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            log_file << std::hex << std::setw(2) << std::setfill('0') << (int)state[i][j] << " ";
        }
        log_file << "\n";
    }
    log_file << "\n";
}

// Nhân trường GF(2^8)
unsigned char gmul(unsigned char a, unsigned char b) {
    unsigned char p = 0;
    for (int i = 0; i < 8; ++i) {
        if (b & 1) p ^= a;
        bool hi_bit_set = a & 0x80;
        a <<= 1;
        if (hi_bit_set) a ^= 0x1b; // AES polynomial
        b >>= 1;
    }
    return p;
}

// ======= KEY EXPANSION =======
void rot_word(unsigned char word[4]) {
    unsigned char temp = word[0];
    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = temp;
}

void sub_word(unsigned char word[4]) {
    for (int i = 0; i < 4; ++i) {
        word[i] = SBOX[word[i]];
    }
}

void key_expansion(const unsigned char key[16], unsigned char expanded_key[176]) {
    for (int i = 0; i < 16; ++i) {
        expanded_key[i] = key[i];
    }

    unsigned char temp[4];
    int bytes_generated = 16;
    int rcon_index = 0;

    while (bytes_generated < 176) {
        for (int i = 0; i < 4; ++i) {
            temp[i] = expanded_key[bytes_generated - 4 + i];
        }

        if (bytes_generated % 16 == 0) {
            rot_word(temp);
            sub_word(temp);
            temp[0] ^= RCON[rcon_index++];
        }

        for (int i = 0; i < 4; ++i) {
            expanded_key[bytes_generated] = expanded_key[bytes_generated - 16] ^ temp[i];
            ++bytes_generated;
        }
    }
}

// ======= SUB BYTES =======
void sub_bytes(unsigned char state[4][4]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] = SBOX[state[i][j]];
        }
    }
    log_state(state, "SubBytes");
}

// ======= SHIFT ROWS =======
void shift_rows(unsigned char state[4][4]) {
    unsigned char temp;

    // Row 1: left shift 1
    temp = state[1][0];
    state[1][0] = state[1][1];
    state[1][1] = state[1][2];
    state[1][2] = state[1][3];
    state[1][3] = temp;

    // Row 2: left shift 2
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;

    // Row 3: left shift 3
    temp = state[3][0];
    state[3][0] = state[3][3];
    state[3][3] = state[3][2];
    state[3][2] = state[3][1];
    state[3][1] = temp;

    log_state(state, "ShiftRows");
}

// ======= MIX COLUMNS =======
void mix_columns(unsigned char state[4][4]) {
    unsigned char temp[4];
    for (int i = 0; i < 4; ++i) {
        temp[0] = gmul(0x02, state[0][i]) ^ gmul(0x03, state[1][i]) ^ state[2][i] ^ state[3][i];
        temp[1] = state[0][i] ^ gmul(0x02, state[1][i]) ^ gmul(0x03, state[2][i]) ^ state[3][i];
        temp[2] = state[0][i] ^ state[1][i] ^ gmul(0x02, state[2][i]) ^ gmul(0x03, state[3][i]);
        temp[3] = gmul(0x03, state[0][i]) ^ state[1][i] ^ state[2][i] ^ gmul(0x02, state[3][i]);
        for (int j = 0; j < 4; ++j) {
            state[j][i] = temp[j];
        }
    }
    log_state(state, "MixColumns");
}

// ======= ADD ROUND KEY =======
void add_round_key(unsigned char state[4][4], const unsigned char round_key[16]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] ^= round_key[i * 4 + j];
        }
    }
    log_state(state, "AddRoundKey");
}

// ===== INVERSE SUB BYTES =======
void inv_sub_bytes(unsigned char state[4][4]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] = INV_SBOX[state[i][j]];
        }
    }
    log_state(state, "InvSubBytes");
}

// ======= INVERSE SHIFT ROWS =======
void inv_shift_rows(unsigned char state[4][4]) {
    unsigned char temp;

    // Row 1: right shift 1
    temp = state[1][3];
    state[1][3] = state[1][2];
    state[1][2] = state[1][1];
    state[1][1] = state[1][0];
    state[1][0] = temp;

    // Row 2: right shift 2
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;

    // Row 3: right shift 3
    temp = state[3][0];
    state[3][0] = state[3][1];
    state[3][1] = state[3][2];
    state[3][2] = state[3][3];
    state[3][3] = temp;

    log_state(state, "InvShiftRows");
}

// ======= INVERSE MIX COLUMNS =======
void inv_mix_columns(unsigned char state[4][4]) {
    unsigned char temp[4];
    for (int i = 0; i < 4; ++i) {
        temp[0] = gmul(0x0e, state[0][i]) ^ gmul(0x0b, state[1][i]) ^ gmul(0x0d, state[2][i]) ^ gmul(0x09, state[3][i]);
        temp[1] = gmul(0x09, state[0][i]) ^ gmul(0x0e, state[1][i]) ^ gmul(0x0b, state[2][i]) ^ gmul(0x0d, state[3][i]);
        temp[2] = gmul(0x0d, state[0][i]) ^ gmul(0x09, state[1][i]) ^ gmul(0x0e, state[2][i]) ^ gmul(0x0b, state[3][i]);
        temp[3] = gmul(0x0b, state[0][i]) ^ gmul(0x0d, state[1][i]) ^ gmul(0x09, state[2][i]) ^ gmul(0x0e, state[3][i]);
        for (int j = 0; j < 4; ++j) {
            state[j][i] = temp[j];
        }
    }
    log_state(state, "InvMixColumns");
}

// ======= ENCRYPTION =======
void encrypt_block(unsigned char state[4][4], const unsigned char expanded_key[176]) {
    unsigned char round_key[16];

    for (int i = 0; i < 16; ++i) {
        round_key[i] = expanded_key[i];
    }
    add_round_key(state, round_key);

    for (int round = 1; round < 10; ++round) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        for (int i = 0; i < 16; ++i) {
            round_key[i] = expanded_key[round * 16 + i];
        }
        add_round_key(state, round_key);
    }

    sub_bytes(state);
    shift_rows(state);
    for (int i = 0; i < 16; ++i) {
        round_key[i] = expanded_key[160 + i];
    }
    add_round_key(state, round_key);
}

// ======= DECRYPTION =======
void decrypt_block(unsigned char state[4][4], const unsigned char expanded_key[176]) {
    unsigned char round_key[16];

    for (int i = 0; i < 16; ++i) {
        round_key[i] = expanded_key[160 + i];
    }
    add_round_key(state, round_key);
    inv_shift_rows(state);
    inv_sub_bytes(state);

    for (int round = 9; round > 0; --round) {
        for (int i = 0; i < 16; ++i) {
            round_key[i] = expanded_key[round * 16 + i];
        }
        add_round_key(state, round_key);
        inv_mix_columns(state);
        inv_shift_rows(state);
        inv_sub_bytes(state);
    }

    for (int i = 0; i < 16; ++i) {
        round_key[i] = expanded_key[i];
    }
    add_round_key(state, round_key);
}

// ======= FILE PROCESSING =======
void process_file(const char* input_file, const char* output_file, const char* log_file_path,
                 const unsigned char key[16], void (*process_block)(unsigned char[4][4], const unsigned char[176])) {
    std::ifstream in_file(input_file, std::ios::binary);
    std::ofstream out_file(output_file, std::ios::binary);
    log_file.open(log_file_path, std::ios::binary);

    if (!in_file || !out_file || !log_file) {
        std::cerr << "File error: " << input_file << " or " << output_file << " or " << log_file_path << "\n";
        return;
    }

    unsigned char expanded_key[176];
    key_expansion(key, expanded_key);

    unsigned char block[16];
    unsigned char state[4][4];

    while (in_file.read((char*)block, 16)) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                state[i][j] = block[i * 4 + j];
            }
        }

        log_state(state, "Initial State");
        process_block(state, expanded_key);

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                block[i * 4 + j] = state[i][j];
            }
        }

        out_file.write((char*)block, 16);
    }

    in_file.close();
    out_file.close();
    log_file.close();
}