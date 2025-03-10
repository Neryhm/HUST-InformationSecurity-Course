#include <iostream>

std::string rc4(const std::string &key, const std::string &data) {
    int S[256];
    
    for (int i = 0; i < 256; i++) {
        S[i] = i;
    }
    
    int j = 0;
    int keyLength = key.size();
    for (int i = 0; i < 256; i++) {
        j = (j + S[i] + static_cast<unsigned char>(key[i % keyLength])) % 256;
        int temp = S[i];
        S[i] = S[j];
        S[j] = temp;
    }
    
    //Pseudo-Random Generation Algorithm (PRGA)
    std::string output = data;
    int i = 0;
    j = 0;
    for (std::size_t k = 0; k < data.size(); k++) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        int temp = S[i];
        S[i] = S[j];
        S[j] = temp;
        int t = (S[i] + S[j]) % 256;
        int keyStreamValue = S[t];
        output[k] = data[k] ^ keyStreamValue;
    }
    
    return output;
}

int main() {
    std::string key, plaintext;
    
    // Get key input from the user.
    std::cout << "Enter key: ";
    std::getline(std::cin, key);
    
    // Get plaintext input from the user.
    std::cout << "Enter plaintext: ";
    std::getline(std::cin, plaintext);
    
    // Encrypt the plaintext using RC4.
    std::string ciphertext = rc4(key, plaintext);
    
    // Print the original and encrypted text.
    std::cout << "\nOriginal Text: " << plaintext << "\n";
    
    // Print ciphertext as hexadecimal values for clarity.
    std::cout << "Encrypted Text (in hex): ";
    for (std::size_t i = 0; i < ciphertext.size(); i++) {
        // Convert each character to unsigned char for correct hex conversion.
        unsigned char uc = static_cast<unsigned char>(ciphertext[i]);
        std::cout << std::hex;
        if (uc < 16) std::cout << "0"; // For single-digit hex numbers, add a leading zero.
        std::cout << static_cast<int>(uc) << " ";
    }
    std::cout << std::dec << "\n"; // Reset to decimal output.
    
    // Decrypt the ciphertext (RC4 is symmetric, so reapplying with the same key decrypts).
    std::string decrypted = rc4(key, ciphertext);
    std::cout << "Decrypted Text: " << decrypted << "\n";
    
    return 0;
}
