#ifndef DP_MATCHING
#define DP_MATCHING

#define MOVE_PENALTY 1
#define DIFF_PENALTY 5
#define THRESHOLD 3

void dp_matching_search(const char *file_name, const char *search_word, void (*handler)(const char *, void *), void *user_data);

#endif
