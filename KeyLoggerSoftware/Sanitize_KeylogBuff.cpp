#include "Sanitize_KeyLogBuff.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
using namespace std;
XML_PROCESS_BUFFER writeUnsanitizedBuffer(XML_PROCESS_BUFFER buffer, const char* str)
{
    //    cout << strlen(str);
        if(buffer.current_buff_offset+strlen(str) >= buffer.buffer_size)
        {
            buffer.buffer_size+=0x1000;
            buffer.buffered_data = (char*)realloc(buffer.buffered_data,buffer.buffer_size);
        }
        memcpy(buffer.buffered_data+buffer.current_buff_offset,str,strlen(str));
        buffer.current_buff_offset+=strlen(str);
  //      cout<< (char)*(buffer.buffered_data+strlen(str)-2);

    return buffer;
}

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
XML_PROCESS_BUFFER writeUnsanitizedByte(XML_PROCESS_BUFFER buffer, const char ch)
{
    if(buffer.current_buff_offset+1 >= buffer.buffer_size)
        {
            buffer.buffer_size+=0x1000;
            buffer.buffered_data = (char*)realloc(buffer.buffered_data,buffer.buffer_size);
        }
    *(buffer.buffered_data+buffer.current_buff_offset) = ch;
    buffer.current_buff_offset++;
    return buffer;
}
XML_PROCESS_BUFFER writeSanitizedBuffer(XML_PROCESS_BUFFER buffer, const char* str)
{
    for(int x = 0; x<strlen(str);x++)
        buffer = writeSanitizedByte(buffer,*(str+x));
   return buffer;
}
