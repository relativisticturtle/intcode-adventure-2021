// "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64"
// cl /EHsc /O2 intcode_vm.cpp

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>


#define MAX_SIZE 100000000
int64_t code[MAX_SIZE];


int64_t eval(int64_t p, int mode, int64_t rb) {
    if(mode == 0) {
        if(p >= 0 && p <= MAX_SIZE)
            return code[p];
        std::cout << "Illegal access: code[" << p << "]" << std::endl;
        exit(EXIT_FAILURE);
    }
    else if(mode == 1) {
        return p;
    }
    else if(mode == 2) {
        if(rb + p >= 0 && rb + p <= MAX_SIZE)
            return code[rb + p];
        std::cout << "Illegal access: code[" << rb << "+" << p << "]" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Illegal mode for eval: " << mode << std::endl;
    exit(EXIT_FAILURE);
}

void assign(int64_t val, int64_t p, int mode, int64_t rb) {
    if(mode == 0) {
        if(p >= 0 && p <= MAX_SIZE) {
            code[p] = val;
            return;
        }
        std::cout << "Illegal access: code[" << p << "]" << std::endl;
        exit(EXIT_FAILURE);
    }
    else if(mode == 1) {
        std::cout << "Immediate mode forbidden for assignment" << std::endl;
        exit(EXIT_FAILURE);
    }
    else if(mode == 2) {
        if(rb + p >= 0 && rb + p <= MAX_SIZE) {
            code[rb + p] = val;
            return;
        }
        std::cout << "Illegal access: code[" << rb << "+" << p << "]" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Illegal mode for assign: " << mode << std::endl;
    exit(EXIT_FAILURE);
}


int64_t run(int io_format) {
    int64_t n = 0;
    int64_t i = 0;
    int64_t rb = 0;
    bool stop = false;

    while(!stop) {
        if(i<0 || i>=MAX_SIZE) {
            std::cout << "Illegal instruction-address" << i << std::endl;
            exit(EXIT_FAILURE);
        }

        int OP = code[i] % 100;
        int A = (code[i] / 10000) % 10;
        int B = (code[i] / 1000) % 10;
        int C = (code[i] / 100) % 10;

        switch(OP) {
        case 1:
            assign(eval(code[i+1], C, rb) + eval(code[i+2], B, rb), code[i+3], A, rb);
            i += 4;
            break;
        case 2:
            assign(eval(code[i+1], C, rb) * eval(code[i+2], B, rb), code[i+3], A, rb);
            i += 4;
            break;
        case 3:
            int64_t x;
            if(io_format == 0)
                std::cin >> x;
            else
                x = std::cin.get();
            if(!std::cin.eof() && std::cin.good())
                assign(x, code[i+1], C, rb);
            else
                assign(-999999, code[i+1], C, rb);
            i += 2;
            break;
        case 4:
            if(io_format == 0)
                std::cout << eval(code[i+1], C, rb) << std::endl;
            else
                std::cout.put(eval(code[i+1], C, rb));
            i += 2;
            break;
        case 5:
            if(eval(code[i+1], C, rb) != 0)
                i = eval(code[i+2], B, rb);
            else
                i += 3;
            break;
        case 6:
            if(eval(code[i+1], C, rb) == 0)
                i = eval(code[i+2], B, rb);
            else
                i += 3;
            break;
        case 7:
            assign(eval(code[i+1], C, rb) < eval(code[i+2], B, rb) ? 1 : 0, code[i+3], A, rb);
            i += 4;
            break;
        case 8:
            assign(eval(code[i+1], C, rb) == eval(code[i+2], B, rb) ? 1 : 0, code[i+3], A, rb);
            i += 4;
            break;
        case 9:
            rb += eval(code[i+1], C, rb);
            i += 2;
            break;
        case 99:
            stop = true;
            break;
        default:
            std::cout << "Invalid OP: " << OP << std::endl;
            exit(EXIT_FAILURE);
        }
        n++;
    }
    return n;
}

void print_usage_and_exit(const char* program_name) {
    std::cout << "Usage: " << program_name << " [-d]  [-v]  intcode_file.txt" << std::endl;
    exit(EXIT_FAILURE);
}

int main(int argc, const char* argv[]) {
    //
    // Parse arguments
    //
    int io_format = 1;  // I/O format. 0: Integers, 1: ASCII
    int code_p = 0;     // Index of IntCode-program file
    int verbose = 0;    // Verbose mode?
    for(int j=1; j<argc; j++) {
        if(strcmp(argv[j], "-d") == 0)
            io_format = 0;
        else if(strcmp(argv[j], "-v") == 0)
            verbose = 1;
        else if(code_p == 0)
            code_p = j;
        else {
            print_usage_and_exit(argv[0]);
        }
    }

    // Zero-set program-memory
    std::memset(code, 0, sizeof code);

    // IntCode from file or stdin?
    std::ifstream infile;
    std::istringstream firstline;
    if(0 < code_p) {
        infile.open(argv[code_p]);
        if(!infile) {
            std::cout << "Failed to read file " << argv[code_p] << "." << std::endl;
            print_usage_and_exit(argv[0]);
        }
    }
    else {
        std::string line;
        std::getline(std::cin, line);
        firstline.str(line);
    }
    std::istream& program_stream = infile.is_open() ? static_cast<std::istream&>(infile) : firstline;

    //
    // Read the IntCode
    //
    int i;
    for(i=0; program_stream && i<MAX_SIZE; i++) {
        // Discard whitespaces, read up to comma, attempt parse as integer
        char buffer[20];
        program_stream >> std::skipws;
        program_stream.getline(buffer, 20, ',');
        if(!(std::stringstream(buffer) >> code[i])) {
            // Failed to parse as integer.
            // Break, but first check if non-ASCII flag:
            if(buffer[0] == 'D') {
                io_format = 0;
            }
            break;
        }
    }
    if(verbose) {
        std::cout << i << " integers read" << std::endl;
        std::cout << "-------------------------" << std::endl;
    }

    //
    // Run!
    //
    int64_t n = run(io_format);

    if(verbose) {
        std::cout << "-------------------------" << std::endl;
        std::cout << "done! " << n << " instructions executed." << std::endl;
    }
}