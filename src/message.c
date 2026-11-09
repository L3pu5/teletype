#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "message.h"

// MESSAGE
void Msg_init(Msg* msg, uint szMessage, char* pText){
    char* index = pText;
    uint nWords = 0;
    bool continueReading = true;
    do {
        index = strstr(index, " ");
        if (index==NULL || (index-pText) >= szMessage) 
            continueReading = false; 
        
        nWords++;
        // Step over the character
        index++;
    } while (continueReading);

    msg->szMessage = szMessage;
    msg->pText = pText;
    msg->nWords = nWords;
}

void Msg_print(Msg* msg){
    printf("%d|%d|%.*s\n", msg->szMessage, msg->nWords, msg->szMessage, msg->pText);
}

// TEXT
void Text_init(Text* t, size_t szWindow){
    t->szWindow = szWindow;
    t->pBuffer = calloc(1, szWindow);
}

void Text_open_file(Text* t, char* file){
    FILE* f = fopen(file, "r");
    t->fSource = f;
    Text_next(t);
}

void Text_next(Text* t){
    memset(t->pBuffer, 0, t->szWindow);
    fread(t->pBuffer, t->szWindow, 1, t->fSource);
}

// Rewind nChars
void Text_rewind(Text* t, size_t nChars){
    t->iRead -= nChars;
    fseek(t->fSource, -nChars, SEEK_CUR);
}


// FEED
// A feed should include enough text records for the entire 
void _Feed__clear(Feed* feed) {
    memset(feed->messages, 0, sizeof(Msg) * feed->nMessages);
}

void Feed_init(Feed* feed, Text* t) {
    // For now statically assign 50 messages;
    feed->nMessages = 50;
    feed->messages = calloc(feed->nMessages, sizeof(Msg));
}

// Populates a feed with messages based upon the text. The text may be one more more lines.
void Feed_construct(Feed* feed, Text* t) {
    char* messageCandidate = t->pBuffer;
    char* previousCanddate = NULL;
    uint nMessage = 0;
    while (true) {
        previousCanddate = messageCandidate;
        messageCandidate = strstr(previousCanddate, ".");
        if (messageCandidate == NULL) 
            break;

        messageCandidate++;
        uint szMessage = messageCandidate - previousCanddate;
        // Off-by-one for '.'
        Msg_init(&feed->messages[nMessage], szMessage-1, previousCanddate+1);
    }

    // We will rewind any remaning characters
    Text_rewind(t, t->szWindow - (previousCanddate - t->pBuffer));
}