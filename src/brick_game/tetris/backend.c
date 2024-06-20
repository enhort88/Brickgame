#include "backend.h"

#include "../../gui/cli/frontend.h"
#include "singleton.h"

void init_piece() {
  Singleton *s = get_instance();
  struct timespec tv;
  clock_gettime(CLOCK_MONOTONIC, &tv);

  unsigned int seed = tv.tv_sec * 1000000 + tv.tv_nsec / 1000;
  srand(seed);
  // Инициализация новой фигуры в верхней части экрана
  s->current_piece.x = WIDTH / 2 - 2;
  s->current_piece.y = 0;

  // Определение различных форм фигур
  int shapes[7][4][4] = SHAPES;

  // Инициализация генератора случайных чисел
  srand(time(NULL));
  int shape_next = rand() % 7;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      s->game.next[i][j] = shapes[shape_next][i][j];
    }
  }
  if (s->shape_curr == -1) {
    s->shape_curr = rand() % 7;
  }
  memcpy(s->current_piece.shape, shapes[s->shape_curr],
         sizeof(shapes[s->shape_curr]));
  s->current_piece.type = s->shape_curr;
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

void move_piece_up() {}

void rotate_piece() {
  Singleton *s = get_instance();
  if (s->state == MOVING && s->current_piece.type != 1) {
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
        s->current_piece.shape[x][y] = temp_shape[x][3 - y];
      }
    }

    // Проверка коллизий
    if (check_collision()) {
      // Попробуем сдвинуть фигуру, чтобы она не застревала в стене
      bool collision = true;
      for (int dx = -1; dx <= 1 && collision; dx++) {
        for (int dy = -1; dy <= 1 && collision; dy++) {
          s->current_piece.x += dx;
          s->current_piece.y += dy;
          if (!check_collision()) {
            collision = false;
            break;
          }
          s->current_piece.x -= dx;
          s->current_piece.y -= dy;
        }
      }

      // Если не удалось избежать коллизий, проверим границы и скорректируем
      if (collision) {
        // Корректируем положение фигуры относительно границ поля
        for (int x = 0; x < 4; x++) {
          for (int y = 0; y < 4; y++) {
            if (s->current_piece.shape[y][x] != 0) {
              if (s->current_piece.x + x < 0) {
                s->current_piece.x++;
              }
              if (s->current_piece.x + x >= WIDTH) {
                s->current_piece.x--;
              }
              if (s->current_piece.y + y >= HEIGHT) {
                s->current_piece.y--;
              }
            }
          }
        }

        // Повторная проверка коллизий после корректировки
        if (check_collision()) {
          // Попробуем переместить фигуру только по горизонтали
          for (int dx = -2; dx <= 2 && collision; dx++) {
            s->current_piece.x += dx;
            if (!check_collision()) {
              collision = false;
              break;
            }
            s->current_piece.x -= dx;
          }

          // Если все еще не удалось избежать коллизий, отменяем поворот
          if (collision) {
            for (int y = 0; y < 4; y++) {
              for (int x = 0; x < 4; x++) {
                s->current_piece.shape[y][x] = temp_shape[y][x];
              }
            }
          }
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
  int lines_cleared = 0;

  for (int y = 0; y < HEIGHT; y++) {
    bool complete = true;
    for (int x = 0; x < WIDTH; x++) {
      if (s->game.field[y][x] == 0) {
        complete = false;
        break;
      }
    }
    if (complete) {
      clear_lines(y);
      lines_cleared++;
      y--;  // Проверяем ту же строку ещё раз, так как она теперь заполнена
            // строкой выше
    }
  }

  // Начисление очков в зависимости от количества очищенных линий
  switch (lines_cleared) {
    case 1:
      s->game.score += 100;
      break;
    case 2:
      s->game.score += 300;
      break;
    case 3:
      s->game.score += 700;
      break;
    case 4:
      s->game.score += 1500;
      break;
    default:
      break;
  }

  s->state = SPAWN;  // Смена состояния после проверки линий
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
