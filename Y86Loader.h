//File Name: Y86Loader.h
//Description: header file for the y86loader
//Authors: Alex Clarke & Nicholas Noonan

#ifndef Y86Loader_H
#define Y86Loader_H

//bool readFile (std::ifstream&);
//void storeData (uint64_t, uint8_t [10], int);
//int writeMemory (std::string, uint64_t);
//void getLine (uint64_t *, uint64_t);
bool isValidFileName(std::string);
uint64_t getAddress(std::string);
int getData(std::string, uint8_t [10]);
bool checkHex(std::string);
bool checkHexChar(char);
bool checkAlignment(std::string);
uint8_t hexCharToInt(char);

#endif
