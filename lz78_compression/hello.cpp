#include <iostream>
#include <unordered_map>
#include <vector>

std::vector<std::pair<int, char>> lz78CompressAshok(const std::string &input) {
    std::unordered_map<std::string, int> dictionary;
    std::vector<std::pair<int, char>> compressedData;

    int nextCode = 1;
    std::string current = "";

    for (char c : input) {
        current += c;
        if (dictionary.find(current) == dictionary.end()) {
            // Current phrase not in dictionary, add it
            dictionary[current] = nextCode++;
            // Output the code for the previous phrase and the current character
            compressedData.push_back({dictionary[current.substr(0, current.size() - 1)], c});
            // Reset the current phrase
            current = "";
        }
    }

    // Output the last remaining phrase in the dictionary
    if (!current.empty()) {
        compressedData.push_back({dictionary[current], '\0'});
    }

    return compressedData;
}

int main(){
    printf("hello world");
    std::string input;
    std::cout << "Enter the input string: ";
    std::cin >> input;
    std::cout << input;
    std::vector<std::pair<int, char>> compressedData = lz78CompressAshok(input);

    std::cout << "Compressed Data:\n";
    for (const auto &pair : compressedData) {
        std::cout << "(" << pair.first << ", '" << pair.second << "') ";
    }
    return 0;
}