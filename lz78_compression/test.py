import sys

def lz78_compress(input_string):
    dictionary = {}
    compressed_data = []
    current = ""

    for char in input_string:
        current += char
        if current not in dictionary:
            if current[:-1]:
                compressed_data.append((dictionary[current[:-1]], current[-1]))
            else:
                compressed_data.append((0, current[-1]))
            dictionary[current] = len(dictionary) + 1
            current = ""
    if current:
        try:
            compressed_data.append((dictionary[current[:-1]], current[-1]))
        except:
            compressed_data.append((0, current))
    return compressed_data


def lz78_decompress(compressed_data: list) -> str:
    dictionary = {0: ""}
    result = ""

    for code, char in compressed_data:
        phrase = dictionary[code]
        new_entry = phrase + char
        result += new_entry
        dictionary[len(dictionary)] = new_entry

    return result
# edge case 
# input_string = "234029i034jkeklfsdmlkmojoi2jo"
# input_string = """







#                      INSTITUTE OF ENGINEERING 
#                    TRIBHUVAN UNIVERSITY
#                        PULCHOWK CAMPUS













#                                              A project proposal on
#      Application of Data Structure and Algorithm
                                  
#                                           Submitted To:
#    Department of Electronics and Computer Engineering

#                                       Submitted By:
# Anil Shrestha(078BCT009)
# Ashok Prasad Neupane(078BCT021)
# Jeevan Neupane(078BCT097)
# Bikash Niraula(078BCT031)



# ZIP MAKER

# INTRODUCTION

# We are developing a GUI desktop application for file compression and decompression. By leveraging advanced algorithms Huffman encoding and LZ algorithms, the application aims to optimize storage space and enhance bandwidth efficiency for network file sharing.
# We are planning to use 100% C++ programming so that we can build all the data structures from scratch and achieve great execution time. WxWidgets library is used to develop GUI applications. WxWidgets is a widget toolkit and tools library for creating graphical user interfaces (GUIs) for cross-platform applications.
# The user-friendly interface enables easy file selection, ensuring a seamless experience while saving storage and reducing network bandwidth overhead. 
# We also plan to expand this to compress files other than text files and also to compress the whole directory at once.
# Compression flowchart:
# Read File -> Huffman Encoding -> LZ78 Compression -> Write File
# Decompression flowchart:
# Read File -> LZ78->Decompression -> Huffman Decoding -> Write File


# DATA STRUCTURES AND ALGORITHMS USED
# Huffman Coding
# Huffman Coding is a technique of compressing data to reduce its size without losing any of the details.It is used to compress files efficiently by assigning shorter codes to more frequently occurring symbols.The DSA topics used in this algorithm are:
# Huffman Tree
# A Huffman tree, similar to a binary tree data structure, needs to be created having n leaf nodes and n-1 internal nodes
# Priority Queue
# Priority Queue is used for building the Huffman tree such that nodes with lowest frequency have the highest priority. A Min Heap data structure can be used to implement the functionality of a priority queue.
# Greedy Algorithm
# A greedy algorithm is an approach for solving a problem by selecting the best option available at the moment. It doesn't worry whether the current best result will bring the overall optimal result.
# Recursive Algorithms
# Recursive algorithms are problem-solving techniques where a function calls itself to break down a problem into smaller instances. 
# Tree Traversal (Pre-order)
# Tree traversal refers to the process of visiting and processing each node in a tree data structure. 

# LZ algorithm
# The LZ algorithm has mainly two variants: LZ77 and LZ78.
# LZ algorithm also used for lossless data compression.LZ77 is a dictionary-based algorithm that replaces repeated occurrences of data with references to a single copy, making it a form of compression.The DSA topics used in this algorithm are:
# String Processing:
# String processing involves manipulating and analyzing sequences of characters. 
# Dynamic Programming
# Dynamic programming is an optimization technique used to solve problems by breaking them down into smaller overlapping subproblems.
# Dictionary Data Structures
# Dictionary data structures, often known as associative arrays or maps, store key-value pairs.
# Greedy Algorithms
# Greedy algorithms make locally optimal choices at each stage with the hope of finding a global optimum.
# Data Compression Techniques
# Data compression involves reducing the size of data to save storage space or transmission bandwidth.





# """

input_string = "\n\n\n\n\n\n\n\n\n\n\nmkasdnfkjnknksadfnkjnkjafndsknkfnsadkjsdkkjnfda\n\n\n\n\n\n\n\n\namflksdmad,sksfn sdkf nsadkf sdk\n\n\n\naksnfkdsmnlkmlfksmakldmkldsmlksdmldsmdslfmlmfds\n\n\n";

compressed_data = lz78_compress(input_string)
decompressed_data = lz78_decompress(compressed_data)
# print(decompressed_data)
original_size = sys.getsizeof(input_string)
compressed_size = sys.getsizeof(compressed_data)

# Print the results
print(f"Original Size: {original_size} bytes")
print(f"Compressed Size: {compressed_size} bytes")
assert input_string == decompressed_data, "decompressed data is not same as input data"
