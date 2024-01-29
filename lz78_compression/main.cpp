#include <iostream>
#include <vector>
#include <cassert>
#include <unordered_map>
#include <fstream>
#include <sstream>

std::vector<std::pair<int, char>> lz78_compress(const std::string& input_string) {
    // todo use more efficient data strucutes
     
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

int main() {
    // std::string input_string = "\n\n\n\n\n\n\n\n\n\n\nmkasdnfkjnknksadfnkjnkjafndsknkfnsadkjsdkkjnfda\n\n\n\n\n\n\n\n\namflksdmad,sksfn sdkf nsadkf sdk\n\n\n\naksnfkdsmnlkmlfksmakldmkldsmlksdmldsmdslfmlmfds\n\n\n";
    std::string input_string = "ashok is the king.";
    // std::string input_string = "234029i034jkeklfsdmlkmojoi2jo";

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

    // Decompression
    std::string decompressed_data = lz78_decompress(compressed_data);

    // // Print results
    std::cout << "Original String: " << input_string.substr(0, 100) << std::endl;

    // std::cout << "Decompressed Data: " << decompressed_data << std::endl;

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
