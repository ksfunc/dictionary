#ifndef MODEL
#define MODEL

#define MAX_WORDS_SIZE 65536
#define MAX_WORD_LENGTH 256

#define SEARCH_TYPE_PARTIAL_MATCH 0
#define SEARCH_TYPE_SPELL_CHECK 1

void search(int search_type, const char *file_name, const char *search_word, void (*handler)(const char *, void *), void *user_data);

#endif
