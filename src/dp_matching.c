#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"
#include "dp_matching.h"

static void construct_match_table(int *match_table, const char *pattern1, const char *pattern2) {
  int len1 = strlen(pattern1);
  int len2 = strlen(pattern2);
  for (int i = 0; i < len1; i++) {
    for (int j = 0; j < len2; j++) {
      if (pattern1[i] == pattern2[j]) {
        match_table[MAX_WORD_LENGTH * i + j] = 0;
      } else {
        match_table[MAX_WORD_LENGTH * i + j] = 1;
      }
    }
  }
}

static void construct_cost_table(int *cost_table, int *match_table, int len1, int len2) {
  /* base */
  cost_table[MAX_WORD_LENGTH * 0 + 0] = match_table[MAX_WORD_LENGTH * 0 + 0] * DIFF_PENALTY;

  /* vertical */
  for (int i = 1; i < len1; i++) {
    cost_table[MAX_WORD_LENGTH * i + 0] = cost_table[MAX_WORD_LENGTH * (i - 1) + 0] + MOVE_PENALTY + match_table[MAX_WORD_LENGTH * i + 0] * DIFF_PENALTY;
  }

  /* horizontal */
  for (int i = 1; i < len2; i++) {
    cost_table[MAX_WORD_LENGTH * 0 + i] = cost_table[MAX_WORD_LENGTH * 0 + (i - 1)] + MOVE_PENALTY + match_table[MAX_WORD_LENGTH * 0 + i] * DIFF_PENALTY;
  }

  /* middle */
  for (int i = 1; i < len1; i++) {
    for (int j = 1; j < len2; j++) {
      int vert = cost_table[MAX_WORD_LENGTH * (i - 1) + j] + MOVE_PENALTY + match_table[MAX_WORD_LENGTH * i + j] * DIFF_PENALTY;
      int hori = cost_table[MAX_WORD_LENGTH * i + (j - 1)] + MOVE_PENALTY + match_table[MAX_WORD_LENGTH * i + j] * DIFF_PENALTY;
      int diag = cost_table[MAX_WORD_LENGTH * (i - 1) + (j - 1)] + match_table[MAX_WORD_LENGTH * i + j] * DIFF_PENALTY;

      /* select minimum */
      if (diag <= vert && diag <= hori) {
        cost_table[MAX_WORD_LENGTH * i + j] = diag;
      } else if (vert <= hori) {
        cost_table[MAX_WORD_LENGTH * i + j] = vert;
      } else {
        cost_table[MAX_WORD_LENGTH * i + j] = hori;
      }
    }
  }
}

void dp_matching_search(const char *file_name, const char *search_word, void (*handler)(const char *, void *), void *user_data) {
  FILE *fp = fopen(file_name, "r");

  int *match_table = (int *) malloc(sizeof(int) * MAX_WORD_LENGTH * MAX_WORD_LENGTH);
  int *cost_table = (int *) malloc(sizeof(int) * MAX_WORD_LENGTH * MAX_WORD_LENGTH);

  int search_word_length = strlen(search_word);

  char *p;
  char line[MAX_WORD_LENGTH];
  while ((p = fgets(line, MAX_WORD_LENGTH, fp)) != NULL) {
    char *np;
    np = strchr(line, '\n');
    if (np != NULL) *np = '\0';
    np = strchr(line, '\r');
    if (np != NULL) *np = '\0';
    line[MAX_WORD_LENGTH - 1] = '\0';

    int line_length = strlen(line);

    construct_match_table(match_table, line, search_word);
    construct_cost_table(cost_table, match_table, line_length, search_word_length);

    if (cost_table[MAX_WORD_LENGTH * (line_length - 1) + (search_word_length - 1)] < THRESHOLD * (line_length + search_word_length) / 2) {
      if (handler != NULL) {
        handler(line, user_data);
      }
    }
  }

  fclose(fp);

  free(match_table);
  free(cost_table);
}
