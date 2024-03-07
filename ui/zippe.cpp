#include <wx/wx.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <bitset>
#include <iomanip>
#include <string.h>
#include <wx/filename.h>
#include < wx/button.h >
#include <wx/progdlg.h>



using namespace std;

struct HuffmanNode
{
    char key;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;
    HuffmanNode(char data, int freq, HuffmanNode* l = nullptr, HuffmanNode* r = nullptr) : key(data), freq(freq), left(l), right(r) {}
};

struct compareNodes
{
    bool operator()(HuffmanNode* l, HuffmanNode* r)
    {
        return l->freq > r->freq;
    }
};

void traverseHuffmanTree(HuffmanNode* root, std::string str, std::unordered_map<char, std::string>& huffmanCodes)
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

HuffmanNode* buildHuffmanTree(std::string text)
{
    // count frequency of char and store it in unordered_map
    std::unordered_map<char, int> freq;
    for (char ch : text)
    {
        freq[ch]++;
    }

    // create priority_queue to store nodes
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, compareNodes> pq;
    for (const auto& key : freq)
    {
        pq.push(new HuffmanNode(key.first, key.second));
    }
    while (pq.size() != 1)
    {
        HuffmanNode* left = pq.top();
        pq.pop();
        HuffmanNode* right = pq.top();
        pq.pop();
        int sum = left->freq + right->freq;
        HuffmanNode* newNode = new HuffmanNode('\0', sum);
        newNode->left = left;
        newNode->right = right;
        pq.push(newNode);
    }

    return pq.top();
}

std::string compressFile(std::string text, std::unordered_map<char, std::string>& huffmanCodes)
{
    std::string encodedText = "";
    for (char ch : text)
    {
        encodedText += huffmanCodes[ch];
    }
    // cout << "text endcoded " << encodedText << endl;
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
    return decodedText;
}

std::string readFile(const std::string filename)
{
    std::ifstream inputFile(filename);

    // Check if the file is opened successfully
    if (!inputFile.is_open())
    {
        std::cerr << "Error opening the file: " << filename << std::endl;
        wxMessageBox("Error opening the file");
        return ""; // Return an empty std::string to indicate an error
    }

    // Read the contents of the file and store in a std::string
    std::stringstream contentStream;
    char ch;
    while (inputFile.get(ch)) // Read characters one by one
    {
        contentStream << ch; // Append each character to the stringstream
    }

    // Close the file
    inputFile.close();

    // Return the content as a std::string
    return contentStream.str();
}

void saveFile(const std::string filename, const std::string text)
{
    std::ofstream outputFile(filename);
    if (outputFile.is_open())
    {
        outputFile << text;
        outputFile.close();
    }
    else
    {
        std::cerr << "Error opening the file for writing: " << filename << std::endl;
        wxMessageBox("Error opening the file for writing");
    }
}

std::string serializeHuffmanTree(HuffmanNode* root)
{
    if (!root)
        return "";

    if (!root->left && !root->right)
        return "1" + std::string(1, root->key);

    return "0" + serializeHuffmanTree(root->left) + serializeHuffmanTree(root->right);
}

std::string compressAndSerialize(const std::string& text, std::unordered_map<char, std::string>& huffmanCodes, HuffmanNode* root)
{
    std::string serializedTree = serializeHuffmanTree(root);    // Serialize the Huffman tree
    std::string encodedText = compressFile(text, huffmanCodes); // Compress the text

    return serializedTree + "$" + encodedText; // Combine serialized tree and compressed data with delimiter
}

void saveCompressedDataToFile(const std::string& filename, const std::string& combinedData)
{
    std::ofstream outputFile(filename, std::ios::binary);
    if (outputFile.is_open())
    {
        outputFile << combinedData; // Write combined data to the file
        outputFile.close();
    }
    else
    {
        std::cerr << "Error opening the file for writing: " << filename << std::endl;
        wxMessageBox("Error opening the file for writing");
    }
}

HuffmanNode* deserializeHelper(std::istringstream& stream)
{
    char bit;
    stream.get(bit);

    if (bit == '1')
    {
        char data;
        stream.get(data);
        return new HuffmanNode(data, 0);
    }

    HuffmanNode* left = deserializeHelper(stream);
    HuffmanNode* right = deserializeHelper(stream);
    return new HuffmanNode('\0', 0, left, right);
}
HuffmanNode* deserializeHuffmanTree(const std::string& serializedTree)
{
    if (serializedTree.empty())
        return nullptr;

    std::istringstream stream(serializedTree);
    return deserializeHelper(stream);
}

std::string readStringUntilDelimiter(const std::string& input, char delimiter)
{
    std::stringstream contentStream(input);

    // Read the contents of the string until the delimiter and store in a std::string
    std::stringstream resultStream;
    char ch;
    while (contentStream.get(ch) && ch != delimiter)
    {
        resultStream << ch; // Append each character until delimiter is encountered
    }

    // Return the content as a std::string
    return resultStream.str();
}

void printHuffmanTree(HuffmanNode* root, int indent = 0)
{
    if (root == nullptr)
        return;

    // Print right subtree
    printHuffmanTree(root->right, indent + 4);

    // Print current node
    std::cout << setw(indent) << ' ';
    if (root->key == '\0')
        std::cout << "Freq: " << root->freq << std::endl;
    else
        std::cout << root->key << ": " << root->freq << std::endl;

    // Print left subtree
    printHuffmanTree(root->left, indent + 4);
}
void printHuffmanTreeToFile(HuffmanNode* root, std::ofstream& outputFile, int indent = 0)
{
    if (root == nullptr)
        return;

    // Print right subtree
    printHuffmanTreeToFile(root->right, outputFile, indent + 4);

    // Print current node
    outputFile << std::setw(indent) << ' ';
    if (root->key == '\0')
        outputFile << "Freq: " << root->freq << std::endl;
    else
        outputFile << root->key << ": " << root->freq << std::endl;

    // Print left subtree
    printHuffmanTreeToFile(root->left, outputFile, indent + 4);
}

bool generateHuffmanCodeHelper(HuffmanNode* root, char character, std::string path, std::string& code)
{
    if (root == nullptr)
        return false;

    // If the character is found, return the path as the Huffman code
    if (root->key == character)
    {
        code = path;
        return true;
    }

    // Recursively search the left and right subtrees
    if (generateHuffmanCodeHelper(root->left, character, path + "0", code))
        return true;
    if (generateHuffmanCodeHelper(root->right, character, path + "1", code))
        return true;

    return false;
}

std::string generateHuffmanCode(HuffmanNode* root, char character)
{
    std::string code;
    generateHuffmanCodeHelper(root, character, "", code);
    return code;
}

std::string readCompressedDataFromFile(const std::string& compressedData)
{
    std::string encodedText;

    // Using istringstream to treat the input string as a stream
    std::istringstream inputFile(compressedData);

    // Skip text up to '$' sign
    char ch;
    while (inputFile.get(ch) && ch != '$')
    {
    }

    // Read remaining content until end of string
    while (inputFile.get(ch))
    {
        encodedText += ch;
    }

    return encodedText;
}

std::string decompressWithHuffmanTree(const std::string& compressedData, const std::string& serializedTree)
{
    // Reconstruct Huffman tree
    HuffmanNode* rootNode = deserializeHuffmanTree(serializedTree);

    if (rootNode == nullptr)
    {
        std::cerr << "Error: Failed to reconstruct Huffman tree." << std::endl;
        wxMessageBox("Error: failed to reconstruct Huffman tree");
        return "";
    }

    // Decode compressed text using Huffman tree
    std::string decompressedText;
    HuffmanNode* current = rootNode;
    for (char bit : compressedData)
    {
        if (bit == '0')
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }

        if (current->left == nullptr && current->right == nullptr)
        {
            decompressedText += current->key;
            current = rootNode;
        }
    }

    return decompressedText;
}

void decompressAndSave(const std::string& decompressedString, const std::string& decompressedFilename, HuffmanNode* rootNode)
{
    // Read the remaining compressed data after '$'
    std::string compressedData = readCompressedDataFromFile(decompressedString);

    // Decompress the data using the Huffman tree
    std::string decompressedText = decompressWithHuffmanTree(compressedData, serializeHuffmanTree(rootNode));

    // Save the decompressed text to a new file
    saveFile(decompressedFilename, decompressedText);

    cout << "Saved Successfully " << endl;
    //wxMessageBox("Saved Successfully");
}

std::vector<std::pair<int, char>> lz78_compress(const std::string& input)
{
    std::unordered_map<std::string, int> dictionary;
    std::vector<std::pair<int, char>> compressed;
    std::string current_match;

    int index = 0;
    for (char c : input)
    {
        current_match += c;
        if (dictionary.find(current_match) == dictionary.end())
        {
            // Add the current match to the dictionary
            dictionary[current_match] = ++index;
            // Add the previous match to the compressed output
            std::string previous_match = current_match.substr(0, current_match.size() - 1);
            int prev_index = dictionary[previous_match];
            char prev_char = current_match.back();
            compressed.push_back({ prev_index, prev_char });
            // Reset the current match
            current_match = "";
        }
    }

    // Check for any remaining characters
    if (!current_match.empty())
    {
        compressed.push_back({ dictionary[current_match], '\0' });
    }

    return compressed;
}

std::string lz78_decompress(const std::vector<std::pair<int, char>>& compressed)
{
    std::unordered_map<int, std::string> dictionary;
    std::string decompressed;

    int index = 0;
    for (const auto& p : compressed)
    {
        if (p.first == 0)
        {
            // If index is 0, append the character to decompressed string
            decompressed += p.second;
            dictionary[++index] = std::string(1, p.second);
        }
        else
        {
            // Retrieve the string from dictionary and append the new character
            std::string entry = dictionary[p.first];
            entry += p.second;
            decompressed += entry;
            // Add the new entry to the dictionary
            dictionary[++index] = entry;
        }
    }

    return decompressed;
}

void writeCompressedDataToFile(const std::vector<std::pair<int, char>>& compressed_data, const std::string& filename)
{
    std::ofstream outFile(filename, std::ios::binary);
    if (outFile.is_open())
    {
        for (const auto& p : compressed_data)
        {
            outFile.write(reinterpret_cast<const char*>(&p.first), sizeof(p.first));
            outFile.write(reinterpret_cast<const char*>(&p.second), sizeof(p.second));
        }
        outFile.close();
        std::cout << "Compressed data saved to " << filename << std::endl;
        //wxMessageBox("Compressed data saved to " + filename);

    }
    else
    {
        std::cerr << "Unable to open file " << filename << " for writing." << std::endl;
        wxMessageBox("Unable to open file");
    }
}

std::vector<std::pair<int, char>> readLZ_78_CompressedDataFromFile(const std::string& filename)
{
    std::vector<std::pair<int, char>> compressed_data;

    std::ifstream inFile(filename, std::ios::binary);
    if (inFile.is_open())
    {
        while (!inFile.eof()) // Loop until end of file
        {
            int index;
            char character;
            // Read index
            inFile.read(reinterpret_cast<char*>(&index), sizeof(index));
            // Read character
            inFile.read(reinterpret_cast<char*>(&character), sizeof(character));
            // Check if both index and character were successfully read
            if (inFile)
            {
                compressed_data.push_back({ index, character });
            }
        }
        inFile.close();
    }
    else
    {
        std::cerr << "Unable to open file " << filename << " for reading." << std::endl;
        wxMessageBox("Unable to open file" + filename + "for reading");

    }

    return compressed_data;
}
//


// Define your compression and decompression functions here
void compress(const string& inputFile,const string& outputfile) {
    std::string text = readFile(inputFile);
    std::unordered_map<char, std::string> huffmanCodes;
    HuffmanNode* rootNode = buildHuffmanTree(text);
    traverseHuffmanTree(rootNode, "", huffmanCodes);
    std::string combinedData = compressAndSerialize(text, huffmanCodes, rootNode); // Compress and serialize the data

    // // //*lz78 compression
    std::vector<std::pair<int, char>> compressedCombinedData = lz78_compress(combinedData);
    writeCompressedDataToFile(compressedCombinedData, outputfile);
}

void decompress(const string& inputFile,const string& outputfile) {
    //! This is for decompression
    //*for lz decompression;

    std::vector<std::pair<int, char>> finalCompressedData = readLZ_78_CompressedDataFromFile(inputFile);

    std::string decompressed = lz78_decompress(finalCompressedData);

    //*for deocmpreesion
    std::string serializedTree = readStringUntilDelimiter(decompressed, '$');
    HuffmanNode* rootNode = deserializeHuffmanTree(serializedTree);
    decompressAndSave(decompressed, outputfile, rootNode);

}




class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
    {// Material Design colors
        wxColour backgroundColor(48, 48, 48); // Dark Grey for background
        wxColour buttonColor(33, 150, 243);   // Blue for buttons

        // Set background color
        SetBackgroundColour(backgroundColor);

        wxPanel* panel = new wxPanel(this, wxID_ANY);
        panel->SetBackgroundColour(backgroundColor);

        wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

        // Add spacer
        vbox->AddSpacer(100);

        // Add compress button
        compressButton = new wxButton(panel, wxID_ANY, "Compress");
        compressButton->SetBackgroundColour(buttonColor);
        compressButton->SetForegroundColour(*wxWHITE);
        compressButton->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)); // Set button font size

        vbox->Add(compressButton, 0, wxALIGN_CENTER | wxALL, 30);
        compressButton->Bind(wxEVT_BUTTON, &MyFrame::OnCompressButtonClicked, this);

        // Add decompress button
        decompressButton = new wxButton(panel, wxID_ANY, "Decompress");
        decompressButton->SetBackgroundColour(buttonColor);
        decompressButton->SetForegroundColour(*wxWHITE);
        
        decompressButton->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)); // Set button font size
        vbox->Add(decompressButton, 0, wxALIGN_CENTER | wxALL, 30);
        decompressButton->Bind(wxEVT_BUTTON, &MyFrame::OnDecompressButtonClicked, this);

        panel->SetSizer(vbox);
    }

    void OnCompressButtonClicked(wxCommandEvent& event)
    {
        // Create and show file dialog for input file selection
        wxFileDialog openFileDialog(nullptr, _("Open File"), "", "", "All files (.)|*.txt*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
        openFileDialog.SetSize(wxSize(1500, 600)); // Set dialog size
        openFileDialog.SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)); // Set font size
        if (openFileDialog.ShowModal() == wxID_CANCEL)
            return;

        wxString inputFile = openFileDialog.GetPath();


        if (!inputFile.empty())
        {
            // Create and show file dialog for output file selection
                wxFileDialog saveFileDialog(nullptr, _("Save File"), "", "", "All files (.)|*.txt*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
            saveFileDialog.SetSize(wxSize(1500, 600)); // Set dialog size
            saveFileDialog.SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)); // Set font size
            if (saveFileDialog.ShowModal() == wxID_CANCEL)
                return ;

            wxString outputFile = saveFileDialog.GetPath();

            if (!outputFile.empty())
            {
                wxProgressDialog progressDialog("Compression Progress", "Compressing file...", 100, this, wxPD_AUTO_HIDE | wxPD_APP_MODAL);

                //Simulate compression process
                for (int i = 0; i < 100; ++i)
                {
                    progressDialog.Update(i, wxString::Format("Compressing %d%%", i));
                    wxMilliSleep(50); // Simulate work being done
                }
                    
                // Logic for compression
                compress(inputFile.ToStdString(), outputFile.ToStdString());
                wxMessageBox("Compression completed.", "Compression", wxOK | wxICON_INFORMATION);
            }
        }
    }

    void OnDecompressButtonClicked(wxCommandEvent& event)
    {
        wxFileDialog openFileDialog(nullptr, _("Open File"), "", "", "All files (.)|*.txt*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
        openFileDialog.SetSize(wxSize(1500, 600)); // Set dialog size
        openFileDialog.SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)); // Set font size
        if (openFileDialog.ShowModal() == wxID_CANCEL)
            return;

        wxString inputFile = openFileDialog.GetPath();

        if (!inputFile.empty())
        {
            wxFileDialog saveFileDialog(nullptr, _("Save File"), "", "", "All files (.)|*.txt*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
            saveFileDialog.SetSize(wxSize(1500, 600)); // Set dialog size
            saveFileDialog.SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)); // Set font size
            if (saveFileDialog.ShowModal() == wxID_CANCEL)
                return;
            wxString outputFile = saveFileDialog.GetPath();

            if (!outputFile.empty())
            {
                wxProgressDialog progressDialog("Decompression Progress", "Decompressing file...", 100, this, wxPD_AUTO_HIDE | wxPD_APP_MODAL);

                //Simulate compression process
                for (int i = 0; i < 100; ++i)
                {
                    progressDialog.Update(i, wxString::Format("Decompressing %d%%", i));
                    wxMilliSleep(50); // Simulate work being done
                }
                decompress(inputFile.ToStdString(), outputFile.ToStdString());
                wxMessageBox("Decompression completed.", "Decompression", wxOK | wxICON_INFORMATION);
            }
        }
    }

private:
    wxButton* compressButton;
    wxButton* decompressButton;
};

class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        MyFrame* frame = new MyFrame("Compression Tool", wxPoint(50, 50), wxSize(600, 500));
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
