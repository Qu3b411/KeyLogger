#ifndef SANITIZE_KEYLOGBUFF_H_INCLUDED
#define SANITIZE_KEYLOGBUFF_H_INCLUDED


#define DefaultBufferSz     0x1000


typedef struct XML_PROCESS_BUFFER
{
    unsigned int buffer_size;
    unsigned int current_buff_offset;
    char* buffered_data;

}XML_PROCESS_BUFFER;

XML_PROCESS_BUFFER writeUnsanitizedBuffer(XML_PROCESS_BUFFER buf, const char* str);
XML_PROCESS_BUFFER writeSanitizedBuffer(XML_PROCESS_BUFFER buf, const char* str);
XML_PROCESS_BUFFER writeUnsanitizedByte(XML_PROCESS_BUFFER buff, const char ch);
XML_PROCESS_BUFFER writeSanitizedByte(XML_PROCESS_BUFFER buff, const char ch);
#endif // SANITIZE_KEYLOGBUFF_H_INCLUDED
