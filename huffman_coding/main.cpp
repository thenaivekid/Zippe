#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <bitset>

using namespace std;

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

void traverseHuffmanTree(HuffmanNode *root, string str, unordered_map<char, string> &huffmanCodes)
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

HuffmanNode *buildHuffmanTree(string text)
{
    // count frequency of char and store it in unordered_map
    unordered_map<char, int> freq;
    for (char ch : text)
    {
        freq[ch]++;
    }

    // create pririty_queue to store nodes
    priority_queue<HuffmanNode *, vector<HuffmanNode *>, compareNodes> pq;
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

string compressFile(string text, unordered_map<char, string> &huffmanCodes)
{

    string encodedString = "";
    for (char ch : text)
    {
        encodedString += huffmanCodes[ch];
    }
    // print encoded text
    //  cout << "encoded string is " << encodedString << endl;
    return encodedString;
}

string decompressFile(string encodedText, unordered_map<char, string> huffmanCodes)
{
    string decoded_text = "";
    unordered_map<string, char> reverseCodes;
    for (auto key_value : huffmanCodes)
    {
        reverseCodes[key_value.second] = key_value.first;
    }

    string bit = "";
    for (char ch : encodedText)
    {
        bit += ch;
        if (reverseCodes.find(bit) != reverseCodes.end())
        {
            decoded_text += reverseCodes[bit];
            bit = "";
        }
    }
    // print decoded_text
    // cout << "decoded text is " << decoded_text << endl;
    return decoded_text;
}

string readFile(const string filename)
{
    ifstream inputFile(filename);

    // Check if the file is opened successfully
    if (!inputFile.is_open())
    {
        std::cerr << "Error opening the file: " << filename << std::endl;
        return ""; // Return an empty string to indicate an error
    }

    // Read the contents of the file and store in a string
    stringstream contentStream;
    string line;
    while (getline(inputFile, line))
    {
        contentStream << line << "\n"; // Append each line with a newline character
    }

    // Close the file
    inputFile.close();

    // Return the content as a string
    return contentStream.str();
}

void saveFile(const string filename, const string text)
{
    std::ofstream outputFile(filename);
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

int main()
{
    string text = readFile("sample.txt");
    unordered_map<char, string> huffmanCodes;
    HuffmanNode *rootNode = buildHuffmanTree(text);
    traverseHuffmanTree(rootNode, "", huffmanCodes);
    // compress data
    // string encodedText = compressFile(text, huffmanCodes);
    // saveFile("compressed_data.bin", encodedText);

    // decompress data
    string encodedText = readFile("compressed_data.bin");
    string decodedText = decompressFile(encodedText, huffmanCodes);
    saveFile("decompressed_data.txt", decodedText);

    return 0;
}