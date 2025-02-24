#include <iostream>

// Key: KEYWORD
// Input: HELLO
// Output: GYIZSC
class PlayfairCipher {
private:
    char keyMatrix[5][5];

    // Generate the 5x5 key matrix
    void generateKeyMatrix(const std::string &key) {
        bool present[26] = {false};
        int x = 0, y = 0;
        
        for (char ch : key) {
            if (ch == 'J') ch = 'I';
            if (!present[ch - 'A']) {
                keyMatrix[x][y++] = ch;
                present[ch - 'A'] = true;
                if (y == 5) { x++; y = 0; }
            }
        }
        
        for (char ch = 'A'; ch <= 'Z'; ch++) {
            if (ch == 'J') continue;
            if (!present[ch - 'A']) {
                keyMatrix[x][y++] = ch;
                present[ch - 'A'] = true;
                if (y == 5) { x++; y = 0; }
            }
        }
    }

    // Find the position of a character in the key matrix
    void findPosition(char ch, int &row, int &col) {
        if (ch == 'J') ch = 'I';
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (keyMatrix[i][j] == ch) {
                    row = i;
                    col = j;
                    return;
                }
            }
        }
    }

    // Prepare the text by handling duplicate letters and adding 'X' where needed
    std::string prepareText(std::string text) {
        std::string result;
        for (char ch : text) {
            if (ch == 'J') ch = 'I';
            if (isalpha(ch)) result += ch;
        }
        for (size_t i = 0; i < result.length(); i += 2) {
            if (i + 1 < result.length() && result[i] == result[i + 1]) {
                result.insert(i + 1, "X");
            }
        }
        if (result.length() % 2 != 0) result += 'X';
        return result;
    }

public:
    // Constructor to generate the key matrix
    PlayfairCipher(const std::string &key) {
        generateKeyMatrix(key);
    }

    // Encrypt the plaintext using Playfair rules
    std::string encrypt(std::string text) {
        text = prepareText(text);
        std::string cipher;
        
        for (size_t i = 0; i < text.length(); i += 2) {
            int r1, c1, r2, c2;
            findPosition(text[i], r1, c1);
            findPosition(text[i + 1], r2, c2);
            
            if (r1 == r2) {
                cipher += keyMatrix[r1][(c1 + 1) % 5];
                cipher += keyMatrix[r2][(c2 + 1) % 5];
            } else if (c1 == c2) {
                cipher += keyMatrix[(r1 + 1) % 5][c1];
                cipher += keyMatrix[(r2 + 1) % 5][c2];
            } else {
                cipher += keyMatrix[r1][c2];
                cipher += keyMatrix[r2][c1];
            }
        }
        return cipher;
    }

    // Print the key matrix
    void printKeyMatrix() {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                std::cout << keyMatrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    std::string key, plaintext;
    // std::cout << "Enter key: ";
    // std::cin >> key;
    // std::cout << "Enter plaintext: ";
    // std::cin >> plaintext;

    key = "KEYWORD";
    plaintext = "HELLO";
    
    PlayfairCipher cipher(key);
    
    std::cout << "\nKey Matrix:\n";
    cipher.printKeyMatrix();
    
    std::string encryptedText = cipher.encrypt(plaintext);
    std::cout << "\nEncrypted Text: " << encryptedText << std::endl;
    
    return 0;
}
