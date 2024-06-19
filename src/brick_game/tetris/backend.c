#include "backend.h"

#include "../../gui/cli/frontend.h"
#include "singleton.h"

void init_piece() {
  Singleton* s = get_instance();
  // Инициализация новой фигуры в верхней части экрана
  s->current_piece.x = WIDTH / 2 - 2;
  s->current_piece.y = 0;
  // Определение формы фигуры (например, квадрат 2x2)
  int new_shape[4][4] = {
      {1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  memcpy(s->current_piece.shape, new_shape, sizeof(new_shape));
}

// void move_piece_down() {
//   Singleton* s = get_instance();
//   s->current_piece.y++;
//   if (check_collision()) {
//     s->current_piece.y--;
//     s->state = ATTACHING;
//     // Присоединение фигуры к полю и проверка на завершенные линии
//     attach_piece_to_field();
//     check_for_complete_lines();
//     s->state = SPAWN;
//   }
// }
void move_piece_down() {
  Singleton* s = get_instance();
  s->current_piece.y++;
  if (check_collision()) {
    s->current_piece.y--;
    s->state = ATTACHING;
  }
}
void move_piece_left() {
  Singleton* s = get_instance();
  if (s->state == MOVING) {
    s->current_piece.x--;
    if (check_collision()) {
      s->current_piece.x++;
    }
  }
}

void move_piece_right() {
  Singleton* s = get_instance();
  if (s->state == MOVING) {
    s->current_piece.x++;
    if (check_collision()) {
      s->current_piece.x--;
    }
  }
}

void move_piece_up() {
  //
}

void rotate_piece() {
  Singleton* s = get_instance();
  if (s->state == MOVING) {
    int temp_shape[4][4];
    for (int y = 0; y < 4; y++) {
      for (int x = 0; x < 4; x++) {
        temp_shape[x][y] = s->current_piece.shape[3 - y][x];
      }
    }
    memcpy(s->current_piece.shape, temp_shape, sizeof(temp_shape));
    if (check_collision()) {
      // Откат вращения при столкновении
      for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
          s->current_piece.shape[y][x] = temp_shape[3 - x][y];
        }
      }
    }
  }
}

bool check_collision() {
  Singleton* s = get_instance();
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (s->current_piece.shape[y][x] != 0) {
        int new_y = s->current_piece.y + y;
        int new_x = s->current_piece.x + x;
        if (new_y >= HEIGHT || new_x < 0 || new_x >= WIDTH ||
            s->game.field[new_y][new_x] != 0) {
          return true;
        }
      }
    }
  }
  return false;
}
bool check_collision_end_game() {
  Singleton* s = get_instance();
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (s->current_piece.shape[y][x] != 0) {
        int new_y = s->current_piece.y + y;
        int new_x = s->current_piece.x + x;
        // Проверка на выход за границы поля
        if (new_y >= HEIGHT || new_x < 0 || new_x >= WIDTH) {
          continue;
        }
        // Проверка на столкновение с уже существующими фигурами
        if (s->game.field[new_y][new_x] != 0) {
          return true;
        }
      }
    }
  }
  return false;
}
void attach_piece_to_field() {
  Singleton* s = get_instance();
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (s->current_piece.shape[y][x] != 0) {
        int field_y = s->current_piece.y + y;
        int field_x = s->current_piece.x + x;
        if (field_y >= 0 && field_y < HEIGHT && field_x >= 0 &&
            field_x < WIDTH) {
          s->game.field[field_y][field_x] = s->current_piece.shape[y][x];
        }
      }
    }
  }
  
}

void check_for_complete_lines() {
  Singleton* s = get_instance();
  for (int y = 0; y < HEIGHT; y++) {
    bool complete = true;
    for (int x = 0; x < WIDTH; x++) {
      if (s->game.field[y][x] == 0) {
        complete = false;
        break;
      }
    }
    if (complete) {
      // Удаление линии и обновление счета
      clear_lines(y);
      s->game.score += 100;  // Пример обновления счета
    }
  }
  s->state = SPAWN;  // Добавлено для смены состояния после проверки линий
}
void clear_lines(int line) {
  Singleton* s = get_instance();
  for (int y = line; y > 0; y--) {
    for (int x = 0; x < WIDTH; x++) {
      s->game.field[y][x] = s->game.field[y - 1][x];
    }
  }
   for (int x = 0; x < WIDTH; x++) {
     s->game.field[0][x] = 0;
   }
}
