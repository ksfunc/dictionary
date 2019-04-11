#include <stdio.h>
#include <stdlib.h>
#include "model.h"
#include "view.h"

int main(int argc, char *argv[]) {
  gtk_init(&argc, &argv);
  View *view = new_view();
  run(view);
  gtk_main();

  return 0;
}
