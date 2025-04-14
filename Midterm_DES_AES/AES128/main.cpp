#include "AES128.hpp"

int main() {
    // Example key (16 bytes for AES-128)
    unsigned char key[16] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    };// 2b7e151628aed2a6abf7158809cf4f3c


    // encryption
    process_file("input.txt", "encrypted.txt", "debug_logs/encrypt_log.txt", key, encrypt_block);
    // decryption
    process_file("encrypted.txt", "decrypted.txt", "debug_logs/decrypt_log.txt", key, decrypt_block);

    return 0;
// }