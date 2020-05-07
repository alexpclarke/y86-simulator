//this is the loader fisle

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Y86.h"
#include "Y86Loader.h"

static uint64_t lastByteWritten = 0;

// Attempts to load the file.
bool Y86::load(char *fname){
    Y86::reset();
    std::string fileName(fname);
    if (!isValidFileName(fileName)) {
        return false;
    } else {
        std::ifstream inFile (fileName);
        if (inFile.fail()) {
            std::cout << "Failed to load file: " << fileName << '\n';
            return false;
        } else {
            return readFile(inFile);
        }
    }
}

// Takes as input a string representing the name of a file and returns TRUE if
// the file name is valid (ends with a .yo extension)
bool isValidFileName(std::string fileName) {
    std::string extension = fileName.substr(fileName.length() - 3);
    if (extension == ".yo") {
        return true;
    } else {
        std::cout << "Invalid file name: " << fileName << '\n';
        return false;
    }
}

// Given a valid file, reads and processes file line by line (this is a "top
// level" function that calls others to process the file)
bool Y86::readFile(std::ifstream& inFile) {
    int lineCount = 0;
    std::string line;
    while (std::getline(inFile, line)) {
        lineCount++;
        if (!checkAlignment(line)) {
            std::cout << "Error on line " << lineCount << '\n';
            std::cout << line << '\n';
            return false;
        } else {
            try {
                if (line[line.find_first_not_of(' ')] != '|') {
                    uint64_t address = getAddress(line);
                    uint8_t dataBuffer[10];
                    int dataLength = getData(line, dataBuffer);
                    storeData(address, dataBuffer, dataLength);
                }
            } catch (const bool fail) {
                if (fail) {
                    std::cout << "Error on line " << lineCount << '\n';
                    std::cout << line << '\n';
                    return false;
                }
            }
        }
    }
    return true;
}

// Given a line, getAddress checks to see if there is an address, and if there
// is, it confirms that it is a valid hex value and then conerts it to a
// uint64_t.
uint64_t getAddress(std::string line) {
    std::string beforePipe = line.substr(0, line.find('|'));
    size_t colonPos = beforePipe.find(':');
    if (colonPos == std::string::npos) {
        throw true;
    } else {
        if (line.substr(0, 2) != "0x") {
            // Invalid address prefix.
            // std::cout << "invalid address prefix\n";
            throw true;
        } else {
            std::string address = line.substr(2, colonPos - 2);
            if (!checkHex(address)) {
                //std::cout << "invalid address value \n";
                // Invalic address value.
                throw true;
            } else {
                return std::stoi(address, nullptr, 16);
            }
        }
    }
}

int getData(std::string line, uint8_t dataBuffer[10]) {
    // Clears the buffer.
    for (int i = 0; i < 11; i ++) {
        dataBuffer[i] = 0;
    }
    
    // Creates a string of all the non-blank characters after the colon and
    // before the pipe.
    std::string beforePipe = line.substr(0, line.find('|'));
    int colonPos = beforePipe.find(':');
    std::string dataString = "";
    for (unsigned int i = colonPos + 1; i < beforePipe.length(); i++) {
        char c = beforePipe[i];
        if (c != ' ') {
            dataString += c;
        }
    }
    
    // Determines the number of characters in the data.
    int len = dataString.length();

    if (len % 2 != 0) {
        // The data string is uneven, stop loading file.
        throw true;
    } else {
        // Check that the characters are valid hex values.
        if (checkHex(dataString)) {
            for (int i = 0; i < len / 2; i++) {
                // Take the characters in Pairs.
                std::string byteString = dataString.substr(i * 2, 2);
                
                // Convert them into a uint8_t.
                uint8_t byteVal = hexCharToInt(byteString[0]);
                byteVal = (byteVal << 4) | hexCharToInt(byteString[1]);
                
                // Store them in the buffer.
                dataBuffer[i] = byteVal;
            }

            // Returns the length of the data.
            return len / 2;
        } else {
            // Invalid hex value in data.
            throw true;
        }
    }
}

bool checkHex(std::string hexStr) {
    for (unsigned int i = 0; i < hexStr.length(); i++) {
        if (!checkHexChar(hexStr[i])) {
            //std::cout << hexStr[i] << '\n';
            return false;
        }
    }
    return true;
}

bool checkHexChar(char hexChar) {
    bool isNum = (hexChar >= '0' && hexChar <= '9');
    bool isUpper = (hexChar >= 'A' && hexChar <= 'F');
    bool isLower = (hexChar >= 'a' && hexChar <= 'f');

    return (isNum || isUpper || isLower);
}

void Y86::storeData(uint64_t address, uint8_t dataBuffer[10], int dataLength) {
    if (lastByteWritten == 0 || address > lastByteWritten) {
        for (int i = 0; i < dataLength; i++) {
            memory.putByte(address + i, dataBuffer[i]);
            lastByteWritten = address + i;
        }
    } else {
        // Memory is being overwritten.
        throw true;
    } 
}

//looks for specific placement of spaces, colons, and pipes
bool checkAlignment(std::string line) {
    size_t colonPosition = line.find(':');
    size_t pipePosition = line.find('|');
    //bool aligned = false;
    
    //check for blank line alignment
    if(colonPosition == std::string::npos && (pipePosition == 28 || pipePosition == 29)){
        return true;   
    }
    //if colon is present
    else if (colonPosition != std::string::npos) {
        std::string dataStr = line.substr(colonPosition + 1,
        line.length() - colonPosition);
        dataStr = dataStr.substr(0, dataStr.find('|'));
        bool noData = true;
        for(unsigned i = 0; i < dataStr.length(); i++){
            if(dataStr[i] != ' '){
                noData = false;
            }
        }
        //address 3 chars no data alignment check
        if(colonPosition == 5 && pipePosition == 28 && noData)
        {
            return true;
        }
        //address 4 chars no data alignment check
        else if(colonPosition == 6 && pipePosition == 29 && noData)
        {
            return true;
        }
        //address 3 chars alignment check
        else if(colonPosition == 5 && pipePosition == 28 && line[6] == ' ' && line[27] == ' '
        && dataStr.find_first_of(' ') == 0){
            return true;    
        }
        //address 4 chars alignment check
        else if(colonPosition == 6 && pipePosition == 29 && line[27] == ' ' && line[28] == ' '
        && dataStr.find_first_not_of(' ') == 0){
            return true;
        }
    }
    return false;;
}

uint8_t hexCharToInt(char c) {
    unsigned int8 = (unsigned) c;
    if (int8 >= 48 && int8 <= 57) {
        int8 -= 48;
    } else if (int8 >= 97 && int8 <= 102) {
        int8 -= 87;
    } else {
        int8 = -1;
    }
    return int8;
}
