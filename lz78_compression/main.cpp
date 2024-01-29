#include <iostream>
#include <vector>
#include <cassert>
#include <unordered_map>
#include <fstream>
#include <sstream>

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

void saveCompressedDataToFile(const std::string& filename, const std::vector<std::pair<int, char>>& compressed_data) {
    std::ofstream outputFile(filename, std::ios::binary);
    if (outputFile.is_open()) {
        for (const auto& entry : compressed_data) {
            outputFile.write(reinterpret_cast<const char*>(&entry), sizeof(entry));
        }
        outputFile.close();
    } else {
        std::cerr << "Error opening the file for writing: " << filename << std::endl;
    }
}

std::vector<std::pair<int, char>> readCompressedDataFromFile(const std::string& filename) {
    std::ifstream inputFile(filename, std::ios::binary);
    std::vector<std::pair<int, char>> compressed_data;

    if (inputFile.is_open()) {
        std::pair<int, char> entry;
        while (inputFile.read(reinterpret_cast<char*>(&entry), sizeof(entry))) {
            compressed_data.push_back(entry);
        }
        inputFile.close();
    } else {
        std::cerr << "Error opening the file for reading: " << filename << std::endl;
    }

    return compressed_data;
}

int main() {
    std::string input_string = "ashok is the king.";

    // to work with file
    std::ifstream inputFile("sample.txt");

    // Check if the file is opened successfully
    if (!inputFile.is_open()) {
        std::cerr << "Error opening the file: " << std::endl;
        return 1; // Return an error code
    }

    // Read the contents of the file and store in a variable
    std::string line;
    while (std::getline(inputFile, line)) {
        input_string += line + "\n"; // Append each line with a newline character
    }

    // Close the file
    inputFile.close();

    // Compression
    std::vector<std::pair<int, char>> compressed_data = lz78_compress(input_string);

    // Save compressed data to a file
    saveCompressedDataToFile("compressed_data.bin", compressed_data);

    // Read compressed data from the file
    std::vector<std::pair<int, char>> read_compressed_data = readCompressedDataFromFile("compressed_data.bin");

    // Decompression
    std::string decompressed_data = lz78_decompress(read_compressed_data);

    // Print results
    std::cout << decompressed_data << std::endl;
    std::cout << "Original String: " << input_string.substr(0, 100) << std::endl;
    assert(decompressed_data == input_string);
    std::cout << "Decompression successful!" << std::endl;

    std::size_t input_string_memory = sizeof(input_string) + input_string.capacity();
    std::size_t compressed_data_memory = sizeof(compressed_data);
    for (const auto& entry : compressed_data) {
        compressed_data_memory += sizeof(entry);
    }
    compressed_data_memory += compressed_data.capacity() * sizeof(std::pair<int, char>);

    std::cout << "Memory usage of input_string: " << input_string_memory << " bytes" << std::endl;
    std::cout << "Memory usage of compressed_data: " << compressed_data_memory << " bytes" << std::endl;
    std::cout << "memory saved: " << input_string_memory - compressed_data_memory << std::endl;
    return 0;
}



