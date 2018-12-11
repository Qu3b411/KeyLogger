#include "Sanitize_KeyLogBuff.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
using namespace std;
/**
    name: writeUnsanitizedBuffer
    Description: take an XML_PROCCES_BUFFER and appends a string of
    characters to the end of it, if the added buffer combined with the current
    logged buffer data is to exceed the length of the buffer currently allocated
    this function will reallocate the buffer with an additional 0x1000 (4096) bytes.
    this function will further update the buffer offset to reflect changes

    NOTE:internal use only/

    @param buffer: this is the data type defined to track the xml generated throughout
    this process.

        --type: XML_PROCESS_BUFFER

    @param str: this is a pointer to the bytes to be written onto the buffer

        --type: const char*

    @return buffer: this is the completed buffer updated to reflect the new additions.
*/
XML_PROCESS_BUFFER writeUnsanitizedBuffer(XML_PROCESS_BUFFER buffer, const char* str)
{
        /*
            check that the size of the buffer is not going to exceed the total memory
            alloted to the buffer. if it is then allocate 4096 more bytes.
            because this is an internal use function we can rest assured that the strings
            being passed to it are xml tags that are fairly small. or otherwise timestamp data
            PIDS and process handles,
        */
        if(buffer.current_buff_offset+strlen(str) >= buffer.buffer_size)
        {
            /*
                to exceed excessive memory allocation while still having room to
                avoid constant reallocation an arbitrary size of 0x1000(4096) bytes
                was selected as a growth parameter, this program relies on slab allocation
                instead of doubling the memory each time.
            */
            buffer.buffer_size+=DefaultBufferSz;
            buffer.buffered_data = (char*)realloc(buffer.buffered_data,buffer.buffer_size);
        }
        /*
            starting at the current_buff_offset, copy all data from the str char* over to the
            buffer,
        */
        memcpy(buffer.buffered_data+buffer.current_buff_offset,str,strlen(str));
        /*
            adjust our offset's accordingly
        */
        buffer.current_buff_offset+=strlen(str);
    return buffer;
}
/**
    name: writeSanitizedByte

    Description: takes one byte and determines if it is a character that is special to xml
    if it is then it sanitizes the byte by calling writeUnsanitizedBuffer with the proper escape
    sequence, otherwise this function will write one byte to the buffer by calling writeUnsanitizedByte


    @param buffer: this is the data type defined to track the xml generated throughout
    this process.

        --type: XML_PROCESS_BUFFER

    @param ch: this is the bytes to be written onto the buffer

        --type: char

    @return buffer: this is the completed buffer updated to reflect the new additions.
*/

XML_PROCESS_BUFFER writeSanitizedByte(XML_PROCESS_BUFFER buffer, const char ch)
{
    switch(ch)
    {
        case '<':  buffer = writeUnsanitizedBuffer(buffer,"&lt;");break;
        case '>':  buffer = writeUnsanitizedBuffer(buffer,"&gt;"); break;
        case '&':  buffer = writeUnsanitizedBuffer(buffer,"&amp;"); break;
        case '\'': buffer = writeUnsanitizedBuffer(buffer,"&apos;"); break;
        case '"': buffer = writeUnsanitizedBuffer(buffer,"&quot;"); break;

        default:  buffer = writeUnsanitizedByte(buffer,ch);

    }
    return buffer;
}
/**
/**
    name: writeUnsanitizedByte

    Description: calculate the offset to the byte to be written to the buffer and sets the
    byte at that position to the provisioned char, then increments the buffer offset to reflect
    changes, if the offset has exceeded the allocated memory provisioned, this function will reallocate
    memory to adjust.


    @param buffer: this is the data type defined to track the xml generated throughout
    this process.

        --type: XML_PROCESS_BUFFER

    @param ch: this is the bytes to be written onto the buffer

        --type: char

    @return buffer: this is the completed buffer updated to reflect the new additions.
*/

XML_PROCESS_BUFFER writeUnsanitizedByte(XML_PROCESS_BUFFER buffer, const char ch)
{
    /*
            check that the size of the buffer is not going to exceed the total memory
            alloted to the buffer. if it is then allocate 4096 more bytes. this function writes
            one byte at a time, thus adjusting the buffer in this manner will always provide more
            then enough memory to write successive bytes.
        */
    if(buffer.current_buff_offset+1 >= buffer.buffer_size)
        {

            /*
                to exceed excessive memory allocation while still having room to
                avoid constant reallocation an arbitrary size of 0x1000(4096) bytes
                was selected as a growth parameter, this program relies on slab allocation
                instead of doubling the memory each time.
            */
            buffer.buffer_size+=DefaultBufferSz;
            buffer.buffered_data = (char*)realloc(buffer.buffered_data,buffer.buffer_size);
        }
    /*
        at the current offset write the provisioned character ch.
    */
    *(buffer.buffered_data+buffer.current_buff_offset) = ch;
    /*
        increment the offset to lock in the previous character.
    */
    buffer.current_buff_offset++;
    return buffer;
}
/**
    name: writeSanitizedBuffer
    Description: takes a character pointer to the string that must be written to the buffer
    and iterates through it, each character is passed to writeSanitizedByte until the end of
    string has been reached.

    @param buffer: this is the data type defined to track the xml generated throughout
    this process.

        --type: XML_PROCESS_BUFFER

    @param str: this is a pointer to the bytes to be written onto the buffer

        --type: const char*

    @return buffer: this is the completed buffer updated to reflect the new additions.

*/
XML_PROCESS_BUFFER writeSanitizedBuffer(XML_PROCESS_BUFFER buffer, const char* str)
{
    /*
        starting at the first character in the provided str and iterate to the last
    */
    for(int x = 0; x<strlen(str);x++)
    {
     /*
        call writeSanitizedByte with the byte at the offset x
     */
        buffer = writeSanitizedByte(buffer,*(str+x));
    }
   return buffer;
}
