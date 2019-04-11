#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sort.h"

static void copy_to_ws(int *result_index, int *work_result_index, int a, int b) {
  work_result_index[b] = result_index[a];
}

static void copy_from_ws(int *result_index, int *work_result_index, int a, int b) {
  result_index[a] = work_result_index[b];
}

static int compare_ws(const char *line_table, int *work_result_index, int word_length, int i, int j) {
  return strncmp(line_table + work_result_index[i] * word_length,
    line_table + work_result_index[j] * word_length,
    word_length);
}

static void merge_sort_merge(const char *line_table, int word_length, int *result_index, int *work_result_index, int low, int high) {
  int mid = (low + high) / 2;

  for (int i = low; i <= mid; i++) {
    copy_to_ws(result_index, work_result_index, i, i);
  }

  for (int i = mid + 1, j = high; i <= high; i++, j--) {
    copy_to_ws(result_index, work_result_index, j, i);
  }

  for (int i = low, j = high, k = low; k <= high; k++) {
    if (compare_ws(line_table, work_result_index, word_length, i, j) <= 0) {
      copy_from_ws(result_index, work_result_index, k, i++);
    } else {
      copy_from_ws(result_index, work_result_index, k, j--);
    }
  }
}

static void merge_sort(const char *line_table, int word_length, int *result_index, int *work_result_index, int low, int high) {
  if (low < high) {
    int mid = (low + high) / 2;

    merge_sort(line_table, word_length, result_index, work_result_index, low, mid);
    merge_sort(line_table, word_length, result_index, work_result_index, mid + 1, high);

    merge_sort_merge(line_table, word_length, result_index, work_result_index, low, high);
  }
}


void sort(const char *line_table, int word_length, int *result_index, int max_line_table_index) {
  for (int i = 0; i < max_line_table_index; i++) {
    result_index[i] = i;
  }

  int *work_result_index = (int *) malloc(sizeof(int) * max_line_table_index);
  merge_sort(line_table, word_length, result_index, work_result_index, 0, max_line_table_index - 1);
  free(work_result_index);
}
