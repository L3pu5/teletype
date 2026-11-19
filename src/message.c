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
    msg->has.transformed = false;
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

    // Seek to get the length of the file
    fseek(f, SEEK_END, 0);
    t->szSource = ftell(f);
    fseek(f, SEEK_SET, 0);

    Text_next(t);
}

void Text_next(Text* t){
    memset(t->pBuffer, 0, t->szWindow);
    // Include one byte of null termination to prevent overflowing
    fread(t->pBuffer, t->szWindow-1, 1, t->fSource);
    t->iRead += t->szWindow-1;
}

// Rewind nChars
void Text_rewind(Text* t, size_t nChars){
    t->iRead -= nChars;
    fseek(t->fSource, -nChars, SEEK_CUR);
}

// Advance a text
void Text_advance(Text* t, int n){
    Feed* f = t->feed;
    // Advance the feed, if it did not move, page next pages
    int previousCursor = f->cursor;
    if (n > 0) {
        Feed_advance(f, n);
        if (f->cursor != 0 && f->cursor == previousCursor){
            Text_next(t);
            Feed_construct(f, t);
            f->cursor = 0;
        } 
    }
    if (n < 0) {
        if (previousCursor == 0) {
            // TODO
        } else {
            Feed_advance(f, n);
        }
    }
}


// FEED
// A feed should include enough text records for the entire 
void _Feed__clear(Feed* feed) {
    memset(feed->messages, 0, sizeof(Msg) * feed->cMessages);
}

void Feed_init(Feed* feed, Text* t) {
    // For now statically assign 50 messages;
    feed->cMessages = 50;
    feed->messages = calloc(feed->cMessages, sizeof(Msg));
    feed->nMessages = 0;
    feed->cursor;
}

// Advances the cursor
void Feed_advance(Feed* f, int n){
    f->cursor += n;
    if (f->cursor < 0) 
        f->cursor = 0;
    
    if (f->cursor >= f->nMessages)
        f->cursor = f->nMessages-1;
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
        // Increment previousCandidate if it's a ' '
        if (*previousCanddate == ' ') {
            previousCanddate++;
        }

        // Step over newlines. 
        if (*previousCanddate == '\n') {
            previousCanddate++;
            szMessage--;
        }

        Msg_init(&feed->messages[nMessage], szMessage, previousCanddate);
        nMessage++;
        if (nMessage > feed->cMessages) {
            fprintf(stderr, "Insufficient messages for buffer in Feed_construct");
            exit(2);
        }
    }

    feed->nMessages = nMessage;
    // We will rewind any remaning characters
    Text_rewind(t, t->szWindow - (previousCanddate - t->pBuffer));
}

void Feed_print(Feed* feed) {
    for (size_t i = 0; i < feed->nMessages; i++)
    {
        Msg_print(&feed->messages[i]);
    }
    
}