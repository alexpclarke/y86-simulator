/*
    Utility functions (e.g., for bit/byte manipulations) used in Y86 simulator.
    
    Author:    
    version:    
*/
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <cstdint>
#include "Tools.h"

namespace Tools {
    
/*-----------------------------------------------------------------------------------------------
    getBits
	Extracts bits 'low' to 'high' (inclusive) from 'source' and returns the result as
	an 64-bit integer. 
	Example:
	
	getBits(12,15,0x0123456789abcdef);
	
	would return the value:
	
	0x00000000000000c;
	
	(Bits 12-15 of the 64-bit word, counting from the right correspond to hex digit 'c' in the
	source word. Note that any number of bits from 1 to 64 could be specified depending on the values
	of 'low' and 'high'.
	The value of high must be greater than or equal to low and high must be in the range 0 to 63. 
------------------------------------------------------------------------------------------------*/
uint64_t getBits(unsigned low, unsigned high, uint64_t source)
{
    assert(high < 64 && (low <= high));
    if ((high == 63) && (low == 0))
    {
        return source;
    }
    unsigned int n = high - low + 1;

    uint64_t mask = ~0; 
    mask = mask << n;
    mask = ~mask;

    return (source >> low) & mask;
}
/*-----------------------------------------------------------------------------------------------
    setBits
	Sets (to 1) the bits from bit numbers 'low' to 'high' (inclusive) in 
	the source word and returns the resulting word.
	
	The value of high must be greater than or equal to low and high must be in the range 0 to 63. 
------------------------------------------------------------------------------------------------*/

uint64_t setBits(unsigned low, unsigned high, uint64_t source)
{
    assert(high < 64 && (low <=high));
    if ((high == 63) && (low == 0))
    {
        return ~0;
    }

    unsigned int n = high - low + 1;
    uint64_t mask = ~0;
    mask = mask << n;
    mask = ~mask;
    mask = mask << low;
    return source | mask;
}
/*-----------------------------------------------------------------------------------------------
    clearBits
	
	Clears (to 0) the bits from bit numbers 'low' to 'high' (inclusive) in 
	the source word and returns the resulting word.
	
	The value of high must be greater than or equal to low and high must be in the range 0 to 63. 
------------------------------------------------------------------------------------------------*/
uint64_t clearBits(unsigned low, unsigned high, uint64_t source)
{
    assert(high < 64 && (low <= high));
    
    if((high == 63) && (low == 0))
    {
        return 0;
    }
    unsigned int n = high - low + 1;

    uint64_t mask = ~0;
    mask = mask << n;
    mask = ~mask;
    mask = mask << low;
    mask = ~mask;

    return source & mask;
}

/*-----------------------------------------------------------------------------------------------
    assignOneBit
	Sets the bit number 'bitNum' to the binary value (0 or 1) specified by 'bitVal' in
	the source word and returns the resulting word.
	
	'bitNum' must be in the range 0 to 63 (inclusive) and 'bitVal' must be 0 or 1.
------------------------------------------------------------------------------------------------*/
uint64_t assignOneBit(unsigned bitNum, unsigned bitVal, uint64_t source)
{
    assert(bitNum < 64 && (bitVal == 0 || bitVal == 1));

    uint64_t mask = ~0;
    mask = mask << 1;
    mask = ~mask;
    mask = mask << bitNum; 

    if(bitVal == 0)
    {
        mask = ~mask;
        return source & mask;
    }
    else
    {
        return source | mask;
    }
}

/*-----------------------------------------------------------------------------------------------
    getByteNumber
	Returns the specified byte number from the source word.
	
	Example:
	getByteNumber(3,0x0011223344556677);
	
	will return the single byte value:
	
	0x44
	
	(Note that the '3' refers to byte 3 of a 8-byte unsigned integer (bytes are numbered from
	the right starting at 0 when looking at a word value in hex). 
------------------------------------------------------------------------------------------------*/
uint8_t getByteNumber(unsigned byteNum, uint64_t source)
{
    assert(byteNum < (64/8));
    uint64_t mask = ~0;
    mask = mask << 8;
    mask = ~mask;
    mask = mask << (byteNum * 8);
     
    return ((source & mask) >> (byteNum * 8));
}
/*-----------------------------------------------------------------------------------------------
    putByteNumber
	Returns the source word after replacing the specified byte (byteNum) with a given 
	byte value (byteVal).
	
	Example:
	putByteNumber(3,0x00,0x0011223344556677);
	
	will return the value:
	
	0x0011223300556677
	
	(Note that the '3' refers to byte 3 of a 8-byte unsigned integer (bytes are numbered from
	the right starting at 0 when looking at a word value in hex). 
------------------------------------------------------------------------------------------------*/
uint64_t putByteNumber(unsigned byteNum, uint8_t byteVal, uint64_t source)
{
    assert(byteNum < (64/8));
    unsigned int low = byteNum * 8;
    unsigned int high = low + 7;
    uint64_t newSource;
    newSource = clearBits(low, high, source);
    uint64_t  byteVal64 = (uint64_t) byteVal;
    byteVal64 = byteVal64 << byteNum * 8;
    
    return newSource | byteVal64;
}
/*-----------------------------------------------------------------------------------------------
    buildWord
	
	Returns a 64 bit word consisting of the 8 parameter bytes, where b0 is the least significant
	and b7 is the most significant byte.
------------------------------------------------------------------------------------------------*/
uint64_t buildWord(unsigned char b0, unsigned char b1,unsigned char b2, unsigned char b3,
                          unsigned char b4, unsigned char b5,unsigned char b6, unsigned char b7)
{
    uint64_t word = 0;
    uint64_t b_0 = (uint64_t)b0;
    uint64_t b_1 = (uint64_t)b1;     
    uint64_t b_2 = (uint64_t)b2;
    uint64_t b_3 = (uint64_t)b3;
    uint64_t b_4 = (uint64_t)b4;
    uint64_t b_5 = (uint64_t)b5;
    uint64_t b_6 = (uint64_t)b6;
    uint64_t b_7 = (uint64_t)b7;

    word |= b_0;
    word |= (b_1<<8);
    word |= (b_2<<16);
    word |= (b_3<<24);
    word |= (b_4<<32);
    word |= (b_5<<40);
    word |= (b_6<<48);
    word |= (b_7<<56);

    return word;            
}
/*-----------------------------------------------------------------------------------------------
    isNegative

	Returns true if the input parameter is negative when interpreted as a signed value.
------------------------------------------------------------------------------------------------*/
bool isNegative(uint64_t source)
{
    source = source >> 63;
    if (source == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*-----------------------------------------------------------------------------------------------
    expandBits
	
	Reads the source and converts it to a character string consisting of 72 bytes--71 characters
	followed by a '\0' character (string terminator).
	Example usage: 
		char buf[72];
		expandBits(0xAABBCCDDEE001122UL,buf);

		Will cause the following string to be written to the buf[] array (not including quotes):
	
	"10101010 10111011 11001100 11011101 11101110 00000000 00010001 00100010"
	
	Note the space after every group of 8 digits except the last. The last byte of
	the string is the '\0' byte, that is not printable but used to indicate the end
	of a C string.
------------------------------------------------------------------------------------------------*/
void expandBits(uint64_t source, char *bits)
{
    /*
    int bit = 0;
    int index = 0;
    for(int i = 63; i >= 0; i--, bits++, index++)
    {
        bit = getBits(i,i,source);
        if ((index%8 == 0) && (index != 0))
        {
            *bits = ' ';
            bits++;
        }
        if (bit == 1)
        {
            *bits = '1';
        }
        else
        {
            *bits = '0';
        }
    } 
    *bits = '\0';
    return;*/

    assert(bits != NULL);

    uint64_t spaceMask = 0x0080808080808080;

    char * bitPos = bits;

    for(uint64_t bit = 0x8000000000000000; bit > 0; bit >>= 1)
    {
        if (bit & spaceMask)
        {
             *bitPos ++= ' ';
        }
        *bitPos ++= bit & source?'1':'0';
    }
    *bitPos = '\0';
}
/*-----------------------------------------------------------------------------------------------
    clearBuffer
	Treats pbuf as a pointer to an array of 'size' bytes and clears them all to zero.
------------------------------------------------------------------------------------------------*/
void clearBuffer(char * pbuf, int size)
{
    assert(pbuf != NULL && size >= 0 && size <= 2147483647);

    for (int i = 0; i < size; i++)
    {
       *pbuf = 0;
       pbuf++;
    }
    return;
}

} // end namespace Tools
