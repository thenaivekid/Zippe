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
    # Output the last remaining phrase in the dictionary
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

# Example usage
input_string = "234029i034jkeklfsdmlkmojoi2jo"
compressed_data = lz78_compress(input_string)
# print("Compressed Data:")
# for code, char in compressed_data:
#     print(f"({code}, '{char}')")
assert input_string == lz78_decompress(compressed_data)