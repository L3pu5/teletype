#ifndef _MESSAGE
#define _MESSAGE

#include <stdio.h>

#define uint unsigned int
#define TEXT_WIND_WINDOW_DEFALT 4800

// Defines the contents of a singular message.
typedef struct TT_Message {
    uint    szMessage;
    uint    nWords;
    char*   pText;
    // Has this been processed already by a TextWindow?
    union {
        bool    transformed;
        int     lines;
    } has;
} Msg;

void Msg_init(Msg* msg, uint szMessage, char* pText);
void Msg_print(Msg* msg);


// Defines a linked list or array of messages
typedef struct TT_Feed {
    uint    nMessages; // Number of active messages
    uint    cMessages; //capacityMessages
    Msg*    messages;
    int     cursor;
} Feed;

void Feed_advance(Feed* f, int n);


// Defines a volume being read. 
// For now assume this is a file
typedef struct TT_Text {
    FILE*   fSource;
    size_t  szSource;
    size_t  iRead;
    // The Window (pBuffer) holds the read characters currently buffered within the text
    size_t  szWindow;
    char*   pBuffer;
    Feed*   feed;
} Text;

void Text_init(Text* t, size_t szWindow);
void Text_open_file(Text* t, char* file);
void Text_next(Text* t);
void Text_rewind(Text* t, size_t nChars);
void Text_advance(Text* t, int n);

// Feed req Text
void Feed_init(Feed* feed, Text* t);
void Feed_construct(Feed* feed, Text* t);
void Feed_print(Feed* feed);

// Feed req Msg

#endif