#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "model.h"
#include "sort.h"
#include "suffix_array.h"

static bool load_file(const char *file_name, char *word_table, int *max_word_table_index) {
  FILE *fp = fopen(file_name, "r");
  if (fp == NULL) return false;

  int i = 0;
  char *p;
  char line[MAX_WORD_LENGTH];
  while ((p = fgets(line, MAX_WORD_LENGTH, fp)) != NULL) {
    char *np;
    np = strchr(line, '\n');
    if (np != NULL) *np = '\0';
    np = strchr(line, '\r');
    if (np != NULL) *np = '\0';
    line[MAX_WORD_LENGTH - 1] = '\0';
    strncpy(word_table + MAX_WORD_LENGTH * i, line, MAX_WORD_LENGTH);
    word_table[MAX_WORD_LENGTH * i + MAX_WORD_LENGTH - 1] = '\0';
    i++;
  }
  *max_word_table_index = i;

  fclose(fp);

  return true;
}

static bool create_suffix_file(const char *file_name, const char *hc_file_name) {
  FILE *fp_in = fopen(file_name,"r");
  if (fp_in == NULL) return false;

  FILE *fp_out = fopen(hc_file_name, "w");
  if (fp_out == NULL) return false;

  int i = 0;
  char *p;
  char line[MAX_WORD_LENGTH];
  while ((p = fgets(line, MAX_WORD_LENGTH, fp_in)) != NULL) {
    char *np;
    np = strchr(line, '\n');
    if (np != NULL) *np = '\0';
    np = strchr(line, '\r');
    if (np != NULL) *np = '\0';
    int line_length = strlen(line);
    for (int j = 0; j < line_length; j++) {
      fprintf(fp_out, "%s,%d\n", line + j, i);
    }
    i++;
  }

  fclose(fp_in);
  fclose(fp_out);

  return true;
}

static bool load_suffix_file(const char *hc_file_name, char *line_table, int *max_line_table_index) {
  FILE *fp = fopen(hc_file_name, "r");
  if (fp == NULL) return false;

  int i = 0;
  char *p;
  char line[MAX_WORD_LENGTH];
  while((p = fgets(line, MAX_WORD_LENGTH, fp)) != NULL) {
    char *np;
    np = strchr(line, '\n');
    if (np != NULL) *np = '\0';
    np = strchr(line, '\r');
    if (np != NULL) *np = '\0';
    strncpy(line_table + MAX_WORD_LENGTH * i, line, MAX_WORD_LENGTH);
    line_table[MAX_WORD_LENGTH * i + MAX_WORD_LENGTH - 1] = '\0';
    i++;
  }
  *max_line_table_index = i;

  fclose(fp);

  return true;
}

static bool create_sorted_suffix_file(const char *hc_file_name, const char *shc_file_name) {
  char *line_table = (char *) malloc(sizeof(char) * MAX_WORDS_SIZE * MAX_WORD_LENGTH);
  int max_line_table_index = 0;
  bool success = load_suffix_file(hc_file_name, line_table, &max_line_table_index);
  if (!success) {
    free(line_table);

    return false;
  }

  int *result_index = (int *) malloc(sizeof(int) * max_line_table_index);
  sort(line_table, MAX_WORD_LENGTH, result_index, max_line_table_index);

  FILE *fp = fopen(shc_file_name, "w");
  if (fp == NULL) {
    free(line_table);
    free(result_index);

    return false;
  }

  for(int i = 0; i < max_line_table_index; i++) {
    fprintf(fp, "%d\n", result_index[i]);
  }

  fclose(fp);

  free(line_table);
  free(result_index);

  return true;
}

static bool load_sorted_suffix_file(const char *shc_file_name, int *result_index) {
  FILE *fp = fopen(shc_file_name, "r");
  if (fp == NULL) return false;

  int i = 0;
  char *p;
  char index[16];
  while((p = fgets(index, 16, fp)) != NULL) {
    result_index[i] = atoi(index);
    i++;
  }

  fclose(fp);

  return true;
}

void suffix_array_search(const char *file_name, const char *search_word, void (*handler)(const char *, void *), void *user_data) {
  struct stat stat_file;
  if (stat(file_name, &stat_file) == -1) return;

  char hc_file_name[1024];
  strncpy(hc_file_name, file_name, 1024);
  hc_file_name[1024 - 1] = '\0';
  strcat(hc_file_name, ".hc");
  hc_file_name[1024 - 1] = '\0';

  struct stat stat_hc_file;
  if (stat(hc_file_name, &stat_hc_file) == -1 || stat_hc_file.st_mtime < stat_file.st_mtime) {
    bool success = create_suffix_file(file_name, hc_file_name);
    if (!success) return;
  }

  char shc_file_name[1024];
  strncpy(shc_file_name, file_name, 1024);
  shc_file_name[1024 - 1] = '\0';
  strcat(shc_file_name, ".shc");
  shc_file_name[1024 - 1] = '\0';

  struct stat stat_shc_file;
  stat(hc_file_name, &stat_hc_file);
  if (stat(shc_file_name, &stat_shc_file) == -1 || stat_shc_file.st_mtime < stat_hc_file.st_mtime) {
    bool success = create_sorted_suffix_file(hc_file_name, shc_file_name);
    if (!success) return;
  }

  char *word_table = (char *) malloc(sizeof(char) * MAX_WORDS_SIZE * MAX_WORD_LENGTH);
  int max_word_table_index;
  load_suffix_file(file_name, word_table, &max_word_table_index);

  char *line_table = (char *) malloc(sizeof(char) * MAX_WORDS_SIZE * MAX_WORD_LENGTH);
  int max_line_table_index;
  load_suffix_file(hc_file_name, line_table, &max_line_table_index);

  int *result_index = (int *) malloc(sizeof(int) * max_line_table_index);
  load_sorted_suffix_file(shc_file_name, result_index);

  int search_word_length = strlen(search_word);
  int low = 0;
  int mid = 0;
  int high = max_line_table_index - 1;

  while (low <= high) {
    mid = (low + high) / 2;
    if(strncmp(search_word, line_table + MAX_WORD_LENGTH * result_index[mid], search_word_length) > 0) {
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }

  int ip;
  ip = mid;
  while (0 <= ip && strncmp(search_word, line_table + MAX_WORD_LENGTH * result_index[ip], search_word_length) == 0) {
    char *p = strchr(line_table + MAX_WORD_LENGTH * result_index[ip], ',');
    if (handler != NULL) {
      handler(word_table + MAX_WORD_LENGTH * atoi(p + 1), user_data);
    }
    ip--;
  }

  ip = mid + 1;
  while (ip < max_line_table_index && strncmp(search_word, line_table + MAX_WORD_LENGTH * result_index[ip], search_word_length) == 0) {
    char *p = strchr(line_table + MAX_WORD_LENGTH * result_index[ip], ',');
    if (handler != NULL) {
      handler(word_table + MAX_WORD_LENGTH * atoi(p + 1), user_data);
    }
    ip++;
  }

  free(word_table);
  free(line_table);
  free(result_index);
}
