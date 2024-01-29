#include <iostream>
#include <vector>
#include <cassert>
#include <unordered_map>

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

int main() {
    std::string input_string = "\n\n\n\n\n\n\n\n                     INSTITUTE OF ENGINEERING \n                   TRIBHUVAN UNIVERSITY\n                       PULCHOWK CAMPUS\n\n\n\n\n\n\n\n\n\n\n\n\n\n                                             A project proposal on\n     Application of Data Structure and Algorithm\n                                  \n                                          Submitted To:\n   Department of Electronics and Computer Engineering\n\n                                      Submitted By:\nAnil Shrestha(078BCT009)\nAshok Prasad Neupane(078BCT021)\nJeevan Neupane(078BCT097)\nBikash Niraula(078BCT031)\n\n\n\nZIP MAKER\n\nINTRODUCTION\n\nWe are developing a GUI desktop application for file compression and decompression. By leveraging advanced algorithms Huffman encoding and LZ algorithms, the application aims to optimize storage space and enhance bandwidth efficiency for network file sharing.\nWe are planning to use 100% C++ programming so that we can build all the data structures from scratch and achieve great execution time. WxWidgets library is used to develop GUI applications. WxWidgets is a widget toolkit and tools library for creating graphical user interfaces (GUIs) for cross-platform applications.\nThe user-friendly interface enables easy file selection, ensuring a seamless experience while saving storage and reducing network bandwidth overhead. \nWe also plan to expand this to compress files other than text files and also to compress the whole directory at once.\nCompression flowchart:\nRead File -> Huffman Encoding -> LZ78 Compression -> Write File\nDecompression flowchart:\nRead File -> LZ78->Decompression -> Huffman Decoding -> Write File\n\n\nDATA STRUCTURES AND ALGORITHMS USED\nHuffman Coding\nHuffman Coding is a technique of compressing data to reduce its size without losing any of the details.It is used to compress files efficiently by assigning shorter codes to more frequently occurring symbols.The DSA topics used in this algorithm are:\nHuffman Tree\nA Huffman tree, similar to a binary tree data structure, needs to be created having n leaf nodes and n-1 internal nodes\nPriority Queue\nPriority Queue is used for building the Huffman tree such that nodes with lowest frequency have the highest priority. A Min Heap data structure can be used to implement the functionality of a priority queue.\nGreedy Algorithm\nA greedy algorithm is an approach for solving a problem by selecting the best option available at the moment. It doesn't worry whether the current best result will bring the overall optimal result.\nRecursive Algorithms\nRecursive algorithms are problem-solving techniques where a function calls itself to break down a problem into smaller instances. \nTree Traversal (Pre-order)\nTree traversal refers to the process of visiting and processing each node in a tree data structure. \n\nLZ algorithm\nThe LZ algorithm has mainly two variants: LZ77 and LZ78.\nLZ algorithm also used for lossless data compression.LZ77 is a dictionary-based algorithm that replaces repeated occurrences of data with references to a single copy, making it a form of compression.The DSA topics used in this algorithm are:\nString Processing:\nString processing involves manipulating and analyzing sequences of characters. \nDynamic Programming\nDynamic programming is an optimization technique used to solve problems by breaking them down into smaller overlapping subproblems.\nDictionary Data Structures\nDictionary data structures, often known as associative arrays or maps, store key-value pairs.\nGreedy Algorithms\nGreedy algorithms make locally optimal choices at each stage with the hope of finding a global optimum.\nData Compression Techniques\nData compression involves reducing the size of data to save storage space or transmission bandwidth.\n\n\n\n\n\n";
    // std::string input_string = "\n\n\n\n\n\n\n\n\n\n\nmkasdnfkjnknksadfnkjnkjafndsknkfnsadkjsdkkjnfda\n\n\n\n\n\n\n\n\namflksdmad,sksfn sdkf nsadkf sdk\n\n\n\naksnfkdsmnlkmlfksmakldmkldsmlksdmldsmdslfmlmfds\n\n\n";
    // std::string input_string = "ashok is the king.";
    // std::string input_string = "234029i034jkeklfsdmlkmojoi2jo";

    // Compression
    std::vector<std::pair<int, char>> compressed_data = lz78_compress(input_string);

    // Decompression
    std::string decompressed_data = lz78_decompress(compressed_data);

    // Print results
    std::cout << "Original String: " << input_string << std::endl;
    // std::cout << "Compressed Data: ";
    // for (const auto& entry : compressed_data) {
    //     std::cout << "(" << entry.first << ", " << entry.second << ") ";
    // }
    // std::cout << std::endl;
    std::cout << "Decompressed Data: " << decompressed_data << std::endl;

    assert(decompressed_data == input_string);

    // If the program reaches here, decompression was successful
    std::cout << "Decompression successful!" << std::endl;

    std::size_t input_string_memory = sizeof(input_string) + input_string.capacity();

    // Calculate memory usage of std::vector and its elements
    std::size_t compressed_data_memory = sizeof(compressed_data);
    for (const auto& entry : compressed_data) {
        compressed_data_memory += sizeof(entry);
    }
    compressed_data_memory += compressed_data.capacity() * sizeof(std::pair<int, char>);

    std::cout << "Memory usage of input_string: " << input_string_memory << " bytes" << std::endl;
    std::cout << "Memory usage of compressed_data: " << compressed_data_memory << " bytes" << std::endl;


    return 0;
}
