#ifndef VIEW
#define VIEW

#include <gtk/gtk.h>

typedef struct home_panel_tag {
  GtkWidget *file_name_entry;
  GtkWidget *file_chooser_button;
  GtkWidget *file_name_group;
  GtkWidget *partial_match_toggle_button;
  GtkWidget *spell_check_toggle_button;
  GtkWidget *toggle_group;
  GtkWidget *search_word_entry;
  GtkWidget *search_button;
  GtkWidget *search_group;
  GtkWidget *vertical_box;
  GtkWidget *horizontal_box;
} HomePanel;

typedef struct result_panel_tag {
  GtkWidget *for_search_word_label;
  GtkWidget *search_word_label;
  GtkWidget *search_word_group;
  GtkWidget *tree_view;
  GtkWidget *scroll_panel;
  GtkWidget *back_button;
  GtkWidget *button_group;
  GtkWidget *vertical_box;
  GtkWidget *horizontal_box;
} ResultPanel;

typedef struct view_tag {
  GtkWidget *window;
  HomePanel *home_panel;
  ResultPanel *result_panel;
} View;

View *new_view();

void run(View *view);

#endif
