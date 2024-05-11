A small program to trim the 512 byte "USBC" (55 53 42 43) header, likely from some sort of corruption, from a JPEG to somewhat restore its contents

This depends on std::filesystem, compile with C++17

Use at your own risk! This will overwrite the file/directory of the input prefixed with "fixed_"