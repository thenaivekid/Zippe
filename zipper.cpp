#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>

struct HuffmanNode {
    char data;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char data, int frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
};

struct CompareNodes {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->frequency > b->frequency;
    }
};

HuffmanNode* buildHuffmanTree(const std::unordered_map<char, int>& frequencies) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareNodes> minHeap;

    for (const auto& entry : frequencies) {
        minHeap.push(new HuffmanNode(entry.first, entry.second));
    }

    while (minHeap.size() > 1) {
        HuffmanNode* left = minHeap.top();
        minHeap.pop();

        HuffmanNode* right = minHeap.top();
        minHeap.pop();

        HuffmanNode* newNode = new HuffmanNode('\0', left->frequency + right->frequency);
        newNode->left = left;
        newNode->right = right;

        minHeap.push(newNode);
    }

    return minHeap.top();
}

void generateHuffmanCodes(HuffmanNode* root, const std::string& code, std::unordered_map<char, std::string>& huffmanCodes) {
    if (root == nullptr) {
        return;
    }

    if (root->data != '\0') {
        huffmanCodes[root->data] = code;
    }

    generateHuffmanCodes(root->left, code + "0", huffmanCodes);
    generateHuffmanCodes(root->right, code + "1", huffmanCodes);
}

std::unordered_map<char, std::string> buildHuffmanCodes(const std::unordered_map<char, int>& frequencies) {
    HuffmanNode* root = buildHuffmanTree(frequencies);
    std::unordered_map<char, std::string> huffmanCodes;
    generateHuffmanCodes(root, "", huffmanCodes);
    return huffmanCodes;
}

std::vector<std::pair<int, char>> lz78_compress(const std::string& input_string) {
    std::unordered_map<std::string, int> dictionary;
    std::vector<std::pair<int, char>> compressed_data;
    std::string current = "";

    for (char ch : input_string) {
        current += ch;
        if (dictionary.find(current) == dictionary.end()) {
            if (current.substr(0, current.size() - 1).length() > 0) {
                compressed_data.push_back({dictionary[current.substr(0, current.size() - 1)], current.back()});
            } else {
                compressed_data.push_back({0, current.back()});
            }
            dictionary[current] = static_cast<int>(dictionary.size()) + 1;
            current = "";
        }
    }

    if (!current.empty()) {
        compressed_data.push_back({dictionary[current.substr(0, current.size() - 1)], current.back()});
    }

    return compressed_data;
}

std::string lz78_decompress(const std::vector<std::pair<int, char>>& compressed_data) {
    std::unordered_map<int, std::string> dictionary;
    std::string result = "";

    for (const auto& entry : compressed_data) {
        int code = entry.first;
        char ch = entry.second;

        std::string phrase = dictionary[code];
        std::string new_entry = phrase + ch;
        result += new_entry;
        dictionary[static_cast<int>(dictionary.size())] = new_entry;
    }

    return result;
}

std::string huffman_decode(const std::string& encoded_text, const std::unordered_map<char, std::string>& huffmanCodes) {
    std::string decoded_text;
    std::unordered_map<std::string, char> reverseCodes;

    for (const auto& entry : huffmanCodes) {
        reverseCodes[entry.second] = entry.first;
    }

    std::string current_code;
    for (char bit : encoded_text) {
        current_code += bit;
        if (reverseCodes.find(current_code) != reverseCodes.end()) {
            decoded_text += reverseCodes[current_code];
            current_code = "";
        }
    }

    return decoded_text;
}

int main() {
    std::string input_string = "hello world";
    std::unordered_map<char, int> frequencies;

    for (char ch : input_string) {
        frequencies[ch]++;
    }

    std::unordered_map<char, std::string> huffmanCodes = buildHuffmanCodes(frequencies);

    // Perform Huffman Encoding
    std::string huffman_encoded;
    for (char ch : input_string) {
        huffman_encoded += huffmanCodes[ch];
    }

    // Perform LZ78 Compression on Huffman Encoded Data
    std::vector<std::pair<int, char>> compressed_data = lz78_compress(huffman_encoded);

    // Perform LZ78 Decompression on Compressed Data
    std::string decompressed_data = lz78_decompress(compressed_data);

    // Decode Huffman Encoded Data
    std::string huffman_decoded = huffman_decode(huffman_encoded, huffmanCodes);

    // Display Original String, Huffman Encoded Data, and Decompressed Data
    std::cout << "Original String: " << input_string << std::endl;
    std::cout << "Huffman Encoded Data: " << huffman_encoded << std::endl;
    std::cout << "Decompressed Data: " << decompressed_data << std::endl;
    std::cout << "Huffman Decoded Data: " << huffman_decoded << std::endl;

    return 0;
}
