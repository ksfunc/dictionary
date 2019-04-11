#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "model.h"
#include "suffix_array.h"
#include "dp_matching.h"

typedef struct search_context_tag {
  char file_name[1024];
  char search_word[1024];
  void (*handler)(const char *, void *);
  void *handler_user_data;
} SearchContext;

static void *suffix_array_search_task(void *user_data) {
  pthread_detach(pthread_self());

  SearchContext *search_context = (SearchContext *) user_data;
  suffix_array_search(search_context->file_name, search_context->search_word, search_context->handler, search_context->handler_user_data);
  free(search_context);

  return NULL;
}

static void *dp_matching_search_task(void *user_data) {
  pthread_detach(pthread_self());

  SearchContext *search_context = (SearchContext *) user_data;
  dp_matching_search(search_context->file_name, search_context->search_word, search_context->handler, search_context->handler_user_data);
  free(search_context);

  return NULL;
}

static void partial_match_search(const char *file_name, const char *search_word, void (*handler)(const char *, void *), void *user_data) {
  SearchContext *search_context = (SearchContext *) malloc(sizeof(SearchContext));

  strncpy(search_context->file_name, file_name, 1024);
  search_context->file_name[1024 - 1] = '\0';
  strncpy(search_context->search_word, search_word, 1024);
  search_context->search_word[1024 - 1] = '\0';
  search_context->handler = handler;
  search_context->handler_user_data = user_data;

  pthread_t pt;
  pthread_create(&pt, NULL, suffix_array_search_task, search_context);
}

static void spell_check_search(const char *file_name, const char *search_word, void (*handler)(const char *, void *), void *user_data) {
  SearchContext *search_context = (SearchContext *) malloc(sizeof(SearchContext));

  strncpy(search_context->file_name, file_name, 1024);
  search_context->file_name[1024 - 1] = '\0';
  strncpy(search_context->search_word, search_word, 1024);
  search_context->search_word[1024 - 1] = '\0';
  search_context->handler = handler;
  search_context->handler_user_data = user_data;

  pthread_t pt;
  pthread_create(&pt, NULL, dp_matching_search_task, search_context);
}

void search(int search_type, const char *file_name, const char *search_word, void (*handler)(const char *, void *), void *user_data) {
  if (search_type == SEARCH_TYPE_PARTIAL_MATCH) {
    partial_match_search(file_name, search_word, handler, user_data);
  } else if (search_type == SEARCH_TYPE_SPELL_CHECK) {
    spell_check_search(file_name, search_word, handler, user_data);
  }
}
