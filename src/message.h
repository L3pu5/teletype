#ifndef _MESSAGE
#define _MESSAGE

#define uint unsigned int

// Defines the contents of a singular message.
typedef struct TT_Message {
    uint    szMessage;
    uint    nWords;
    char*   pText;
} Msg;

void Msg_init(Msg* msg, uint szMessage, char* pText);
void Msg_print(Msg* msg);


// Defines a linked list or array of messages
typedef struct TT_Feed {
    uint    nMessages;
    Msg*    messages;
} Feed;

// Defines a volume being read. 
// For now assume this is a file
typedef struct TT_Text {
    FILE*   fSource;
    size_t  iRead;
    size_t  szWindow;
    char*   pBuffer;
    Feed*   feed;
} Text;

void Text_init(Text* t, size_t szWindow);
void Text_open_file(Text* t, char* file);
void Text_next(Text* t);
void Text_rewind(Text* t, size_t nChars);

// Feed req Text
void Feed_init(Feed* feed, Text* t);
void Feed_construct(Feed* feed, Text* t);

// Feed req Msg

#endif