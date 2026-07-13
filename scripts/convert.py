with open("test-code.txt") as infile, open("test-code.bin", "wb") as outfile:
    for line in infile:
        # Remove comments and whitespace
        clean = line.split("#")[0].strip().replace(" ", "")
        if len(clean) != 16 or any(c not in "01" for c in clean):
            continue  # skip invalid lines
        value = int(clean, 2)
        outfile.write(value.to_bytes(2, byteorder="big"))  # or "little" if needed


