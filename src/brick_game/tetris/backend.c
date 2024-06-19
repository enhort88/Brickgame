#include "backend.h"

#include "../../gui/cli/frontend.h"
#include "singleton.h"

void init_piece() {
  // static int count = 1;
  Singleton *s = get_instance();

  // Инициализация новой фигуры в верхней части экрана
  s->current_piece.x = WIDTH / 2 - 2;
  s->current_piece.y = 0;

  // Определение различных форм фигур
  int shapes[7][4][4] = {
      {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // I
      {{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // O
      {{1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // T
      {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // Z
      {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // S
      {{1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // L
      {{1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}  // J
  };

  // Инициализация генератора случайных чисел
  srand(time(NULL));
  int shape_next = rand() % 7;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      s->game.next[i][j] = shapes[shape_next][i][j];
    }
  }
  if (s->shape_curr == -1)
    s->shape_curr = rand() % 7;
  memcpy(s->current_piece.shape, shapes[s->shape_curr],
         sizeof(shapes[s->shape_curr]));
  s->shape_curr = shape_next;
}

void move_piece_down() {
  Singleton *s = get_instance();
  s->current_piece.y++;
  if (check_collision()) {
    s->current_piece.y--;
    s->state = ATTACHING;
  }
}
void move_piece_left() {
  Singleton *s = get_instance();
  if (s->state == MOVING) {
    s->current_piece.x--;
    if (check_collision()) {
      s->current_piece.x++;
    }
  }
}

void move_piece_right() {
  Singleton *s = get_instance();
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

#include "../../gui/cli/frontend.h"
#include "backend.h"
#include "singleton.h"
#include <time.h>

void rotate_piece() {
  Singleton *s = get_instance();
  if (s->state == MOVING) {
    // Создаем временную матрицу для хранения новой формы после поворота
    int temp_shape[4][4] = {0};

    // Транспонируем матрицу
    for (int y = 0; y < 4; y++) {
      for (int x = 0; x < 4; x++) {
        temp_shape[x][y] = s->current_piece.shape[y][x];
      }
    }

    // Отражаем матрицу по вертикали
    for (int y = 0; y < 4; y++) {
      for (int x = 0; x < 4; x++) {
        s->current_piece.shape[y][x] = temp_shape[y][3 - x];
      }
    }

    // Проверка коллизий
    if (check_collision()) {
      // Попробуем сдвинуть фигуру, чтобы она не застревала в стене
      for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
          s->current_piece.x += dx;
          s->current_piece.y += dy;
          if (!check_collision()) {
            return; // Если удалось найти положение без коллизий, выходим
          }
          s->current_piece.x -= dx;
          s->current_piece.y -= dy;
        }
      }

      // Если не удалось избежать коллизий, отменяем поворот
      for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
          s->current_piece.shape[y][x] = temp_shape[y][x];
        }
      }
    }
  }
}

bool check_collision() {
  Singleton *s = get_instance();
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
  Singleton *s = get_instance();
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
  Singleton *s = get_instance();
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
  Singleton *s = get_instance();
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
      s->game.score += 100; // Пример обновления счета
    }
  }
  s->state = SPAWN; // Добавлено для смены состояния после проверки линий
}
void clear_lines(int line) {
  Singleton *s = get_instance();
  for (int y = line; y > 0; y--) {
    for (int x = 0; x < WIDTH; x++) {
      s->game.field[y][x] = s->game.field[y - 1][x];
    }
  }
  for (int x = 0; x < WIDTH; x++) {
    s->game.field[0][x] = 0;
  }
}
