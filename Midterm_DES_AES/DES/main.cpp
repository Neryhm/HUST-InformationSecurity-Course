#include "DES.hpp"

int main() {
    // Example key (8 bytes)
    unsigned char key[8] = {0x13, 0x34, 0x57, 0x79, 0x9B, 0xBC, 0xDF, 0xF1};
    // 133457799BCDF1

    // encryption
    process_file("input.txt", "encrypted.txt", "debug_logs/encrypt_log.txt", key, encrypt_block);
    // decryption
    process_file("encrypted.txt", "decrypted.txt", "debug_logs/decrypt_log.txt", key, decrypt_block);

    return 0;
}