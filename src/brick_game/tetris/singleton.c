#include "singleton.h"

#include <stdlib.h>

static Singleton *instance = NULL;

Singleton *get_instance() {
  if (instance == NULL) {
    instance = (Singleton *)malloc(sizeof(Singleton));
    instance->game = (GameInfo_t){0};
    instance->state = START;
    instance->current_piece = (Piece){0};
    instance->action = 7;
    instance->shape_curr = -1;
    instance->test = 0;
  }
  return instance;
}
void free_singleton() {
  if (instance != NULL) {
    free(instance);
    instance = NULL;
  }
}