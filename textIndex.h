#ifndef TEXT_INDEX_H

typedef struct textIndex textIndex;

textIndex *createTI(const char *fileName);
void deleteTI(textIndex *ti);

char *getTextTI(textIndex *ti, int index);
int getWordCountTI(textIndex *ti);
int getTextCountTI(textIndex *ti);
int textWordCountTI(textIndex *ti, int index);

#define TEXT_INDEX_H
#endif
