#ifndef SUFFIX_ARRAY
#define SUFFIX_ARRAY

void suffix_array_search(const char *file_name, const char *search_word, void (*handler)(const char *, void *), void *user_data);

#endif
