#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "model.h"
#include "view.h"

static HomePanel *new_home_panel() {
  HomePanel *home_panel = (HomePanel *) malloc(sizeof(HomePanel));

  home_panel->file_name_entry = gtk_entry_new();
  home_panel->file_chooser_button = gtk_button_new_with_label("選択");
  home_panel->file_name_group = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
  home_panel->partial_match_toggle_button = gtk_toggle_button_new_with_label("部分一致");
  home_panel->spell_check_toggle_button = gtk_toggle_button_new_with_label("スペルチェック");
  home_panel->toggle_group = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
  home_panel->search_word_entry = gtk_search_entry_new();
  home_panel->search_button = gtk_button_new_with_label("検索");
  home_panel->search_group = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
  home_panel->vertical_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
  home_panel->horizontal_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);

  gtk_widget_set_margin_top(home_panel->file_name_group, 160);
  gtk_widget_set_margin_top(home_panel->toggle_group, 10);
  gtk_widget_set_margin_top(home_panel->search_group, 10);
  gtk_widget_set_size_request(home_panel->file_name_entry, 240, 0);
  gtk_widget_set_size_request(home_panel->search_word_entry, 240, 0);
  gtk_box_pack_start(GTK_BOX(home_panel->file_name_group), home_panel->file_name_entry, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(home_panel->file_name_group), home_panel->file_chooser_button, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(home_panel->toggle_group), home_panel->partial_match_toggle_button, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(home_panel->toggle_group), home_panel->spell_check_toggle_button, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(home_panel->search_group), home_panel->search_word_entry, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(home_panel->search_group), home_panel->search_button, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(home_panel->vertical_box), home_panel->file_name_group, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(home_panel->vertical_box), home_panel->toggle_group, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(home_panel->vertical_box), home_panel->search_group, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(home_panel->horizontal_box), home_panel->vertical_box, TRUE, FALSE, 0);
  gtk_box_set_spacing(GTK_BOX(home_panel->file_name_group), 0);
  gtk_box_set_spacing(GTK_BOX(home_panel->toggle_group), 0);
  gtk_box_set_spacing(GTK_BOX(home_panel->search_group), 0);
  gtk_style_context_add_class(gtk_widget_get_style_context(home_panel->file_name_group), GTK_STYLE_CLASS_LINKED);
  gtk_style_context_add_class(gtk_widget_get_style_context(home_panel->toggle_group), GTK_STYLE_CLASS_LINKED);
  gtk_style_context_add_class(gtk_widget_get_style_context(home_panel->search_group), GTK_STYLE_CLASS_LINKED);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(home_panel->partial_match_toggle_button), TRUE);

  return home_panel;
}

static void set_tree_view_model(GtkWidget *tree_view) {
  GtkListStore *store = gtk_list_store_new(1, G_TYPE_STRING);
  gtk_tree_view_set_model(GTK_TREE_VIEW(tree_view), GTK_TREE_MODEL(store));
  g_object_unref(store);
}

static void set_tree_view_view(GtkWidget *tree_view) {
  GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
  GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes("単語名", renderer, "text", 0, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
}

static ResultPanel *new_result_panel() {
  ResultPanel *result_panel = (ResultPanel *) malloc(sizeof(ResultPanel));

  result_panel->for_search_word_label = gtk_label_new("検索文字列: ");
  result_panel->search_word_label = gtk_label_new("");
  result_panel->search_word_group = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
  result_panel->tree_view = gtk_tree_view_new();
  result_panel->scroll_panel = gtk_scrolled_window_new(NULL, NULL);
  result_panel->back_button = gtk_button_new_with_label("戻る");
  result_panel->button_group = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
  result_panel->vertical_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
  result_panel->horizontal_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);

  gtk_widget_set_size_request(result_panel->scroll_panel, 480, 0);
  gtk_widget_set_size_request(result_panel->back_button, 80, 0);
  gtk_container_add(GTK_CONTAINER(result_panel->scroll_panel), result_panel->tree_view);
  gtk_box_pack_start(GTK_BOX(result_panel->search_word_group), result_panel->for_search_word_label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(result_panel->search_word_group), result_panel->search_word_label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(result_panel->button_group), result_panel->back_button, TRUE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(result_panel->vertical_box), result_panel->search_word_group, FALSE, FALSE, 10);
  gtk_box_pack_start(GTK_BOX(result_panel->vertical_box), result_panel->scroll_panel, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(result_panel->vertical_box), result_panel->button_group, FALSE, FALSE, 10);
  gtk_box_pack_start(GTK_BOX(result_panel->horizontal_box), result_panel->vertical_box, TRUE, FALSE, 0);
  set_tree_view_model(result_panel->tree_view);
  set_tree_view_view(result_panel->tree_view);

  return result_panel;
}

static void open_file_chooser(View *view) {
  GtkWidget *dialog = gtk_file_chooser_dialog_new(
    "ファイルを選択", GTK_WINDOW(view->window), GTK_FILE_CHOOSER_ACTION_OPEN,
    "キャンセル", GTK_RESPONSE_CANCEL, "開く", GTK_RESPONSE_ACCEPT, NULL);

  gint res = gtk_dialog_run(GTK_DIALOG(dialog));
  if (res == GTK_RESPONSE_ACCEPT) {
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    const char *file_name = gtk_file_chooser_get_filename(chooser);
    gtk_entry_set_text(GTK_ENTRY(view->home_panel->file_name_entry), file_name);
  }

  gtk_widget_destroy(dialog);
}

static void file_chooser_button_clicked_handler(GtkWidget *file_chooser_button, gpointer user_data) {
  fprintf(stderr, "called file_chooser_button_clicked_handler!\n");
  View *view = (View *) user_data;
  open_file_chooser(view);
}

static void partial_match_toggle_button_toggled_handler(GtkWidget *partial_match_toggle_button, gpointer user_data) {
  fprintf(stderr, "called partial_match_toggle_button_toggled_handler!\n");
  View *view = (View *) user_data;
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(view->home_panel->partial_match_toggle_button))) {
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(view->home_panel->spell_check_toggle_button), FALSE);
  }
}

static void spell_check_toggle_button_toggled_handler(GtkWidget *spell_check_toggle_button, gpointer user_data) {
  fprintf(stderr, "called spell_check_toggle_button_toggled_handler!\n");
  View *view = (View *) user_data;
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(view->home_panel->spell_check_toggle_button))) {
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(view->home_panel->partial_match_toggle_button), FALSE);
  }
}

static void container_remove(gpointer data, gpointer user_data) {
  GtkWidget *widget = (GtkWidget *)data;
  GtkWidget *parent = (GtkWidget *)user_data;
  g_object_ref(widget);
  gtk_container_remove(GTK_CONTAINER(parent), widget);
}

static void switch_to_result_panel(View *view) {
  GList *window_chidren = gtk_container_get_children(GTK_CONTAINER(view->window));
  g_list_foreach(window_chidren, container_remove, view->window);
  gtk_container_add(GTK_CONTAINER(view->window), view->result_panel->horizontal_box);
  gtk_widget_show_all(view->window);
}

static void append_to_list(View *view, const char *word) {
  GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(view->result_panel->tree_view)));
  GtkTreeIter iter;
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, 0, word, -1);
}

static void remove_all_from_list(View *view) {
  GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(view->result_panel->tree_view)));
  gtk_list_store_clear(store);
}

static void searched_handler(const char *found_word, void *user_data) {
  View *view = (View *) user_data;
  append_to_list(view, found_word);
}

static void search_button_clicked_handler(GtkWidget *search_button, gpointer user_data) {
  fprintf(stderr, "called search_button_clicked_handler!\n");
  View *view = (View *) user_data;

  if (strlen(gtk_entry_get_text(GTK_ENTRY(view->home_panel->search_word_entry))) <= 0) return;

  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(view->home_panel->partial_match_toggle_button))) {
    char file_name[1024];
    char search_word[1024];
    strncpy(file_name, gtk_entry_get_text(GTK_ENTRY(view->home_panel->file_name_entry)), 1024);
    file_name[1024 - 1] = '\0';
    strncpy(search_word, gtk_entry_get_text(GTK_ENTRY(view->home_panel->search_word_entry)), 1024);
    search_word[1024 - 1] = '\0';
    gtk_label_set_text(GTK_LABEL(view->result_panel->search_word_label), search_word);
    remove_all_from_list(view);
    switch_to_result_panel(view);
    search(SEARCH_TYPE_PARTIAL_MATCH, file_name, search_word, searched_handler, view);
  } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(view->home_panel->spell_check_toggle_button))) {
    char file_name[1024];
    char search_word[1024];
    strncpy(file_name, gtk_entry_get_text(GTK_ENTRY(view->home_panel->file_name_entry)), 1024);
    file_name[1024 - 1] = '\0';
    strncpy(search_word, gtk_entry_get_text(GTK_ENTRY(view->home_panel->search_word_entry)), 1024);
    search_word[1024 - 1] = '\0';
    gtk_label_set_text(GTK_LABEL(view->result_panel->search_word_label), search_word);
    remove_all_from_list(view);
    switch_to_result_panel(view);
    search(SEARCH_TYPE_SPELL_CHECK, file_name, search_word, searched_handler, view);
  }
}

static void switch_to_home_panel(View *view) {
  GList *window_chidren = gtk_container_get_children(GTK_CONTAINER(view->window));
  g_list_foreach(window_chidren, container_remove, view->window);
  gtk_container_add(GTK_CONTAINER(view->window), view->home_panel->horizontal_box);
  gtk_widget_show_all(view->window);
}

static void back_button_clicked_handler(GtkWidget *back_button, gpointer user_data) {
  fprintf(stderr, "called back_button_clicked_handler!\n");
  View *view = (View *) user_data;
  switch_to_home_panel(view);
}

View *new_view() {
  View *view = (View *) malloc(sizeof(View));

  view->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  view->home_panel = new_home_panel();
  view->result_panel = new_result_panel();

  gtk_window_set_title(GTK_WINDOW(view->window), "辞書");
  gtk_widget_set_size_request(view->window, 640, 480);
  gtk_window_set_resizable(GTK_WINDOW(view->window), FALSE);
  gtk_entry_set_text(GTK_ENTRY(view->home_panel->file_name_entry), "./data/default.txt");
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(view->home_panel->partial_match_toggle_button), TRUE);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(view->home_panel->spell_check_toggle_button), FALSE);
  gtk_entry_set_text(GTK_ENTRY(view->home_panel->search_word_entry), "");
  gtk_label_set_text(GTK_LABEL(view->result_panel->search_word_label), "");

  g_signal_connect(view->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(view->home_panel->file_chooser_button, "clicked", G_CALLBACK(file_chooser_button_clicked_handler), view);
  g_signal_connect(view->home_panel->partial_match_toggle_button, "toggled", G_CALLBACK(partial_match_toggle_button_toggled_handler), view);
  g_signal_connect(view->home_panel->spell_check_toggle_button, "toggled", G_CALLBACK(spell_check_toggle_button_toggled_handler), view);
  g_signal_connect(view->home_panel->search_button, "clicked", G_CALLBACK(search_button_clicked_handler), view);
  g_signal_connect(view->result_panel->back_button, "clicked", G_CALLBACK(back_button_clicked_handler), view);

  gtk_container_add(GTK_CONTAINER(view->window), view->home_panel->horizontal_box);

  return view;
}

void run(View *view) {
  gtk_widget_grab_focus(view->home_panel->search_word_entry);
  gtk_widget_show_all(view->window);
}
