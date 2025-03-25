SPECIAL_IDS = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 13, 15, 16, 17, 18, 19, 20,
    24, 25, 26, 27, 28, 29, 31, 32, 40, 57, 58, 59, 63, 64, 65,
    72, 74, 75, 82, 54, 55, 56, 76, 77, 78
}

def convert_line(line):
    converted = []
    for num in line.strip().split(','):
        try:
            tile_id = int(num)
            flag = 1 if tile_id in SPECIAL_IDS else 0
            tile_id += 1

            if (tile_id > 0):
                converted.append(f"{tile_id}:0:{flag}" )
            elif (2147483595):
                converted.append(f"{53}:1:{flag}")
        except ValueError:
            converted.append("0:0:0")
    return " ".join(converted)

with open('input.txt', 'r') as infile, open('output.txt', 'w') as outfile:
    while True:
        line = infile.readline()
        if not line:
            break
        if line.strip():
            converted = convert_line(line)
            outfile.write(converted + "\n")