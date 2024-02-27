#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <bitset>

struct HuffmanNode
{
    char key;
    int freq;
    HuffmanNode *left;
    HuffmanNode *right;
    HuffmanNode(char data, int freq) : key(data), freq(freq), left(nullptr), right(nullptr) {}
};

struct compareNodes
{
    bool operator()(HuffmanNode *l, HuffmanNode *r)
    {
        return l->freq > r->freq;
    }
};

void traverseHuffmanTree(HuffmanNode *root, std::string str, std::unordered_map<char, std::string> &huffmanCodes)
{
    if (!root)
    {
        return;
    }
    if (!root->left && !root->right)
    {
        huffmanCodes[root->key] = str;
    }
    traverseHuffmanTree(root->left, str + '0', huffmanCodes);
    traverseHuffmanTree(root->right, str + '1', huffmanCodes);
}

HuffmanNode *buildHuffmanTree(std::string text)
{
    // count frequency of char and store it in unordered_map
    std::unordered_map<char, int> freq;
    for (char ch : text)
    {
        freq[ch]++;
    }

    // create pririty_queue to store nodes
    std::priority_queue<HuffmanNode *, std::vector<HuffmanNode *>, compareNodes> pq;
    for (const auto &key : freq)
    {
        pq.push(new HuffmanNode(key.first, key.second));
    }
    while (pq.size() != 1)
    {
        HuffmanNode *left = pq.top();
        pq.pop();
        HuffmanNode *right = pq.top();
        pq.pop();
        int sum = left->freq + right->freq;
        HuffmanNode *newNode = new HuffmanNode('\0', sum);
        newNode->left = left;
        newNode->right = right;
        pq.push(newNode);
    }

    return pq.top();
}

std::string compressFile(std::string text, std::unordered_map<char, std::string> &huffmanCodes)
{
    HuffmanNode *rootNode = buildHuffmanTree(text);
    traverseHuffmanTree(rootNode, "", huffmanCodes);

    std::string encodedText = "";
    for (char ch : text)
    {
        encodedText += huffmanCodes[ch];
    }
    // print encoded text
    //  cout << "encoded text is " << encodedText << endl;
    return encodedText;
}

std::string decompressFile(std::string encodedText, std::unordered_map<char, std::string> huffmanCodes)
{
    std::string decodedText = "";
    std::unordered_map<std::string, char> reverseCodes;
    for (auto key_value : huffmanCodes)
    {
        reverseCodes[key_value.second] = key_value.first;
    }

    std::string bit = "";
    for (char ch : encodedText)
    {
        bit += ch;
        if (reverseCodes.find(bit) != reverseCodes.end())
        {
            decodedText += reverseCodes[bit];
            bit = "";
        }
    }
    // print decoded text
    // cout << "decoded text is " << decodedText << endl;
    return decodedText;
}

std::string readFile(const std::string filename)
{
    std::ifstream inputFile(filename);

    // Check if the file is opened successfully
    if (!inputFile.is_open())
    {
        std::cerr << "Error opening the file: " << filename << std::endl;
        return ""; // Return an empty std::string to indicate an error
    }

    // Read the contents of the file and store in a std::string
    std::stringstream contentStream;
    std::string line;
    while (std::getline(inputFile, line))
    {
        contentStream << line << "\n"; // Append each line with a newline character
    }

    // Close the file
    inputFile.close();

    // Return the content as a std::string
    return contentStream.str();
}

void saveFile(const std::string filename, const std::string text)
{
    std::ofstream outputFile(filename, std::ios::binary);
    if (outputFile.is_open())
    {
        for (const auto &entry : text)
        {
            outputFile.write(reinterpret_cast<const char *>(&entry), sizeof(entry));
        }
        outputFile.close();
    }
    else
    {
        std::cerr << "Error opening the file for writing: " << filename << std::endl;
    }
}

void saveCompressedDataToFile(const std::string filename, const std::string encodedText, std::unordered_map<char, std::string> &huffmanCodes)
{
    std::ofstream outputFile(filename, std::ios::binary);
    if (outputFile.is_open())
    {
        size_t codesSize = huffmanCodes.size();
        outputFile.write(reinterpret_cast<const char *>(&codesSize), sizeof(size_t));

        // Write Huffman codes to file as header
        for (const auto &pair : huffmanCodes)
        {
            // Write character and code length
            char ch = pair.first;
            size_t codeLength = pair.second.size();
            outputFile.write(&ch, sizeof(char));
            outputFile.write(reinterpret_cast<const char *>(&codeLength), sizeof(size_t));

            // Write code
            const std::string &code = pair.second;
            outputFile.write(code.c_str(), codeLength);
        }
        // Write encoded data to file
        std::string paddedEncodedText = encodedText;
        size_t paddingSize = 8 - (paddedEncodedText.length() % 8); // Calculate padding size
        if (paddingSize != 8)
        {
            // Pad the encoded text to ensure byte alignment
            paddedEncodedText.append(paddingSize, '0');
        }

        for (size_t i = 0; i < paddedEncodedText.size(); i += 8)
        {
            char byte = 0;
            for (size_t j = 0; j < 8; ++j)
            {
                byte |= (paddedEncodedText[i + j] - '0') << (7 - j); // Convert '0'/'1' to bit and shift
            }
            outputFile.write(&byte, 1);
        }

        outputFile.close();
    }
    else
    {
        std::cerr << "Error opening the file for writing: " << filename << std::endl;
    }
}

std::string readCompressedDataFromFile(const std::string &filename, std::unordered_map<char, std::string> &huffmanCodes)
{
    std::ifstream inputFile(filename, std::ios::binary);
    if (!inputFile.is_open())
    {
        std::cerr << "Error: Unable to open file for reading." << std::endl;
        std::exit(1);
    }

    // Read size of Huffman codes section from header
    size_t codesSize;
    inputFile.read(reinterpret_cast<char *>(&codesSize), sizeof(size_t));

    // Read Huffman codes from file
    for (size_t i = 0; i < codesSize; ++i)
    {
        char ch;
        size_t codeLength;
        inputFile.read(&ch, sizeof(char));
        inputFile.read(reinterpret_cast<char *>(&codeLength), sizeof(size_t));

        char *codeBuffer = new char[codeLength + 1];
        inputFile.read(codeBuffer, codeLength);
        codeBuffer[codeLength] = '\0'; // Null-terminate the string
        std::string code(codeBuffer);
        delete[] codeBuffer;

        huffmanCodes[ch] = code;
    }

    // Read encoded data from file
    std::string encodedText;
    char byte;
    while (inputFile.read(&byte, 1))
    {
        encodedText += std::bitset<8>(byte).to_string(); // Convert byte to binary string
    }

    inputFile.close();
    return encodedText;
}

int main()
{
    std::string text = readFile("sample.txt");
    std::unordered_map<char, std::string> huffmanCodes;

    // compress data
    std::string encodedText = compressFile(text, huffmanCodes);
    saveCompressedDataToFile("compressed_data.bin", encodedText, huffmanCodes);

    // decompress data
    // std::string encodedText = readCompressedDataFromFile("compressed_data.bin", huffmanCodes);
    // std::string decodedText = decompressFile(encodedText, huffmanCodes);
    // saveFile("decompressed_data.txt", decodedText);

    return 0;
}