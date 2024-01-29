#include <iostream>
#include <string>

int main() {
    std::string input_string = "h";

    if (!input_string.empty()) {
        // Get all letters except the last one
        std::string letters_except_last = input_string.substr(0, input_string.size() - 1);
        std::cout << "length of substring: " << letters_except_last.length() << std::endl;
        // Print the result
        std::cout << "Original String: " << input_string << std::endl;
        std::cout << "Letters except the last one: " << letters_except_last << std::endl;
    } else {
        std::cout << "The string is empty." << std::endl;
    }

    return 0;
}
