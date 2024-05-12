#include <iostream>
#include <fstream>
#include <filesystem>

void fix(std::filesystem::path path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << path << std::endl;
        return;
    }

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    char *buffer = new char[size];

    file.read(buffer, size);
    file.close();

    if (size < 512) {
        std::cerr << "File is too small: " << path << std::endl;
        delete[] buffer;
        return;
    }

    if (!std::equal(buffer + 512, buffer + 515, "\xFF\xD8\xFF")) {
        std::cout << "Invalid file: " << path << std::endl;
        delete[] buffer;
        return;
    }

    std::filesystem::create_directories("fixed_" + path.parent_path().string());
    std::ofstream out("fixed_" + path.string(), std::ios::binary | std::ios::out);
    if (!out.is_open()) {
        std::cerr << "Unable to write output for file: " << path << std::endl;
        out.close();
        delete[] buffer;
        return;
    }

    out.write(buffer + 512, size - 512);
    out.close();
    delete[] buffer;

    std::cout << "Fixed file: " << path << std::endl;
}

void recursive_fix(std::filesystem::path path) {
    if (std::filesystem::is_regular_file(path)) {
        fix(path);
        return;
    }
    for (std::filesystem::directory_entry file : std::filesystem::directory_iterator(path)) {
        if (std::filesystem::is_regular_file(file.path())) fix(file.path());
        else recursive_fix(file.path());
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename/directory>" << std::endl;
        return 1;
    }

    recursive_fix(argv[1]);

    return 0;
}