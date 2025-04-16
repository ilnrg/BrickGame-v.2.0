#define _GNU_SOURSE
#include <check.h>

#include "../brick_game/common/common_back.h"
#include "../brick_game/tetris/tetris_backend.h"

START_TEST(getTetrisInfo_t_test) {
  TetrisInfo_t *game_state = getTetrisInfo_t();
  ck_assert_ptr_nonnull(game_state);
  ck_assert_ptr_nonnull(game_state->game_info.field);
  removeGameInfo_t();
}
END_TEST

START_TEST(createInfo_t_test) {
  TetrisInfo_t *game_state = getTetrisInfo_t();
  ck_assert_ptr_nonnull(game_state->game_info.field);
  ck_assert_ptr_nonnull(game_state->game_info.next);
  ck_assert_int_eq(game_state->action, Start);
  ck_assert_int_eq(game_state->game_status, kStart);
  ck_assert_int_eq(game_state->game_info.score, 0);
  ck_assert_int_eq(game_state->game_info.high_score, getHighScore());
  ck_assert_int_eq(game_state->game_info.level, 1);
  ck_assert_int_eq(game_state->game_info.speed, START_SPEED);
  ck_assert_int_eq(game_state->game_info.pause, 0);
  removeGameInfo_t();
}
END_TEST

START_TEST(createMatrix_test) {
  int height = 3;
  int width = 4;
  int **matrix;
  int status = createMatrix(height, width, &matrix);
  ck_assert_int_eq(status, START);
  ck_assert_ptr_nonnull(matrix);
  for (int i = 0; i < height; i++) {
    ck_assert_ptr_nonnull(matrix[i]);
    for (int j = 0; j < width; j++) {
      ck_assert_int_eq(matrix[i][j], 0);
    }
  }
  for (int i = 0; i < height; i++) {
    free(matrix[i]);
  }
  free(matrix);
}
END_TEST

START_TEST(removeGameInfo_t_test) {
  {
    TetrisInfo_t *game_state = getTetrisInfo_t();
    removeGameInfo_t();
    ck_assert_ptr_eq(game_state->game_info.field, NULL);
    ck_assert_ptr_eq(game_state->game_info.next, NULL);
    ck_assert_int_eq(game_state->game_info.high_score, 0);
    ck_assert_int_eq(game_state->game_info.level, 1);
    ck_assert_int_eq(game_state->game_info.pause, 0);
    ck_assert_int_eq(game_state->game_info.score, 0);
    ck_assert_int_eq(game_state->game_info.speed, START_SPEED);
  }
  {
    TetrisInfo_t *game_state = malloc(sizeof(TetrisInfo_t));
    game_state->game_info.field = NULL;
    game_state->game_info.next = NULL;
    game_state->game_info.high_score = 0;
    game_state->game_info.level = 1;
    game_state->game_info.pause = 0;
    game_state->game_info.score = 0;
    game_state->game_info.speed = 1;
    removeGameInfo_t();
    ck_assert_ptr_eq(game_state->game_info.field, NULL);
    ck_assert_ptr_eq(game_state->game_info.next, NULL);
    ck_assert_int_eq(game_state->game_info.high_score, 0);
    ck_assert_int_eq(game_state->game_info.level, 1);
    ck_assert_int_eq(game_state->game_info.pause, 0);
    ck_assert_int_eq(game_state->game_info.score, 0);
    ck_assert_int_eq(game_state->game_info.speed, 1);
    free(game_state);
  }
}
END_TEST

START_TEST(removeMatrix_test) {
  {
    int **matrix = (int **)malloc(3 * sizeof(int *));
    for (int i = 0; i < 3; i++) {
      matrix[i] = (int *)malloc(3 * sizeof(int));
    }
    removeMatrix(matrix, 3);
    matrix = NULL;
    ck_assert_ptr_eq(matrix, NULL);
  }
  {
    int **matrix = NULL;
    removeMatrix(matrix, 3);
    ck_assert_ptr_eq(matrix, NULL);
  }
}
END_TEST

START_TEST(figureOrdering_test) {
  figureOrdering();
  TetrisInfo_t *game_state = getTetrisInfo_t();
  for (int i = 0; i < FIGURES_COUNT; i++) {
    ck_assert_int_eq(game_state->figures[i], i);
  }
  removeGameInfo_t();
}
END_TEST

START_TEST(corrSpawn_test) {
  {
    int figures[] = {1, 2, 3, 4, 5};
    int count = 5;
    corrSpawn(figures, count);
    int is_sorted = 1;
    for (int i = 0; i < count - 1 && !is_sorted; i++) {
      if (figures[i] > figures[i + 1]) {
        is_sorted = 0;
      }
    }
    ck_assert_int_eq(is_sorted, 1);
  }
  {
    int figures_single[] = {1};
    int count_single = 1;
    corrSpawn(figures_single, count_single);
    ck_assert_int_eq(figures_single[0], 1);
  }
}
END_TEST

START_TEST(spawnFigure_test) {
  TetrisInfo_t *game_state = getTetrisInfo_t();
  game_state->game_info.next[0][0] = 1;
  game_state->game_info.next[0][1] = 1;
  game_state->game_info.next[0][2] = 1;
  game_state->game_info.next[1][0] = 1;
  game_state->game_info.next[1][1] = 1;
  game_state->game_info.next[1][2] = 1;
  game_state->next_figure.width = 4;
  game_state->next_figure.height = 2;
  spawnFigure(game_state);
  ck_assert_int_eq(game_state->figure.f[0][0], MOVING_CELL);
  ck_assert_int_eq(game_state->figure.f[0][1], MOVING_CELL);
  ck_assert_int_eq(game_state->figure.f[0][2], 1);
  ck_assert_int_eq(game_state->figure.f[1][0], MOVING_CELL);
  ck_assert_int_eq(game_state->figure.f[1][1], MOVING_CELL);
  ck_assert_int_eq(game_state->figure.f[1][2], 1);
  ck_assert_int_eq(game_state->figure.width, 4);
  ck_assert_int_eq(game_state->figure.height, 2);
  ck_assert_int_eq(game_state->figure.x,
                   (WIDTH - game_state->figure.width) / 2 + 1);
  ck_assert_int_eq(game_state->figure.y, 1);
  removeGameInfo_t();
}
END_TEST

START_TEST(initNextFigure_test) {
  GameInfo_t stats;
  TetrisInfo_t *game_state = getTetrisInfo_t();
  game_state->curr_figure = FIGURES_COUNT;
  game_state->figures[0] = 0;
  game_state->next_figure.width = 4;
  game_state->next_figure.height = 2;
  stats.next = malloc(4 * sizeof(int *));
  for (int i = 0; i < 4; i++) {
    stats.next[i] = malloc(4 * sizeof(int));
  }
  initNextFigure(&stats);
  ck_assert_int_eq(stats.next[0][0], game_state->next_figure.f[0][0]);
  ck_assert_int_eq(stats.next[0][1], game_state->next_figure.f[0][1]);
  ck_assert_int_eq(stats.next[0][2], game_state->next_figure.f[0][2]);
  ck_assert_int_eq(stats.next[0][3], game_state->next_figure.f[0][3]);
  ck_assert_int_eq(stats.next[1][0], game_state->next_figure.f[1][0]);
  ck_assert_int_eq(stats.next[1][1], game_state->next_figure.f[1][1]);
  ck_assert_int_eq(stats.next[1][2], game_state->next_figure.f[1][2]);
  ck_assert_int_eq(stats.next[1][3], game_state->next_figure.f[1][3]);
  ck_assert_int_eq(stats.next[2][0], game_state->next_figure.f[2][0]);
  ck_assert_int_eq(stats.next[2][1], game_state->next_figure.f[2][1]);
  ck_assert_int_eq(stats.next[2][2], game_state->next_figure.f[2][2]);
  ck_assert_int_eq(stats.next[2][3], game_state->next_figure.f[2][3]);
  ck_assert_int_eq(stats.next[3][0], game_state->next_figure.f[3][0]);
  ck_assert_int_eq(stats.next[3][1], game_state->next_figure.f[3][1]);
  ck_assert_int_eq(stats.next[3][2], game_state->next_figure.f[3][2]);
  ck_assert_int_eq(stats.next[3][3], game_state->next_figure.f[3][3]);
  ck_assert_int_eq(game_state->curr_figure, 1);
  removeGameInfo_t();
  removeMatrix(stats.next, 4);
}
END_TEST

START_TEST(initFigure_test) {
  {
    Figure_t figure;
    for (int i = 0; i <= 6; i++) {
      initFigure(&figure, i);
      ck_assert_int_eq(i, i);
    }
  }
  {
    Figure_t figure;
    initFigure(&figure, 7);
    ck_assert_int_eq(7, 7);
  }
}
END_TEST

START_TEST(copyFigure_test) {
  {
    int src[4][4] = {
        {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
    int dst[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
    copyFigure(src, dst, 4, 4);
    ck_assert_int_eq(dst[0][0], 1);
    ck_assert_int_eq(dst[0][1], 2);
    ck_assert_int_eq(dst[0][2], 3);
    ck_assert_int_eq(dst[0][3], 4);
    ck_assert_int_eq(dst[1][0], 5);
    ck_assert_int_eq(dst[1][1], 6);
    ck_assert_int_eq(dst[1][2], 7);
    ck_assert_int_eq(dst[1][3], 8);
    ck_assert_int_eq(dst[2][0], 9);
    ck_assert_int_eq(dst[2][1], 10);
    ck_assert_int_eq(dst[2][2], 11);
    ck_assert_int_eq(dst[2][3], 12);
    ck_assert_int_eq(dst[3][0], 13);
    ck_assert_int_eq(dst[3][1], 14);
    ck_assert_int_eq(dst[3][2], 15);
    ck_assert_int_eq(dst[3][3], 16);
  }
  {
    int src[4][4] = {
        {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
    int dst[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
    copyFigure(src, dst, 2, 2);
    ck_assert_int_eq(dst[0][0], 1);
    ck_assert_int_eq(dst[0][1], 2);
    ck_assert_int_eq(dst[1][0], 5);
    ck_assert_int_eq(dst[1][1], 6);
  }
  {
    int src[4][4] = {
        {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
    int dst[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
    copyFigure(src, dst, 1, 1);
    ck_assert_int_eq(dst[0][0], 1);
  }
}
END_TEST

START_TEST(lineFigure_test) {
  Figure_t figure;
  initFigure(&figure, 0);
  ck_assert_int_eq(figure.width, 4);
  ck_assert_int_eq(figure.height, 2);
  int expected_f[4][4] = {
      {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  for (int x = 0; x < 4; x++) {
    ck_assert_int_eq(figure.f[0][x], expected_f[0][x]);
  }
}
END_TEST

START_TEST(squareFigure_test) {
  Figure_t figure;
  initFigure(&figure, 1);
  ck_assert_int_eq(figure.width, 4);
  ck_assert_int_eq(figure.height, 2);
  int expected_f[4][4] = {
      {1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  for (int x = 0; x < 2; x++) {
    ck_assert_int_eq(figure.f[0][x], expected_f[0][x]);
  }
  for (int x = 0; x < 2; x++) {
    ck_assert_int_eq(figure.f[1][x], expected_f[1][x]);
  }
}
END_TEST

START_TEST(zFigure_test) {
  Figure_t figure;
  initFigure(&figure, 2);
  ck_assert_int_eq(figure.width, 4);
  ck_assert_int_eq(figure.height, 2);
  int expected_f[4][4] = {
      {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  for (int x = 0; x < 3; x++) {
    ck_assert_int_eq(figure.f[0][x], expected_f[0][x]);
  }
  for (int x = 0; x < 3; x++) {
    ck_assert_int_eq(figure.f[1][x], expected_f[1][x]);
  }
}
END_TEST

START_TEST(sFigure_test) {
  Figure_t figure;
  initFigure(&figure, 3);
  ck_assert_int_eq(figure.width, 4);
  ck_assert_int_eq(figure.height, 2);
  int expected_f[4][4] = {
      {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  for (int x = 0; x < 3; x++) {
    ck_assert_int_eq(figure.f[0][x], expected_f[0][x]);
  }
  for (int x = 0; x < 3; x++) {
    ck_assert_int_eq(figure.f[1][x], expected_f[1][x]);
  }
}
END_TEST

START_TEST(tFigure_test) {
  Figure_t figure;
  initFigure(&figure, 4);
  ck_assert_int_eq(figure.width, 4);
  ck_assert_int_eq(figure.height, 2);
  int expected_f[4][4] = {
      {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  for (int x = 0; x < 3; x++) {
    ck_assert_int_eq(figure.f[0][x], expected_f[0][x]);
  }
  for (int x = 0; x < 3; x++) {
    ck_assert_int_eq(figure.f[1][x], expected_f[1][x]);
  }
}
END_TEST

START_TEST(lFigure_test) {
  {
    Figure_t figure;
    initFigure(&figure, 5);
    ck_assert_int_eq(figure.width, 4);
    ck_assert_int_eq(figure.height, 2);
    int expected_f[4][4] = {
        {1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
    for (int x = 0; x < 3; x++) {
      ck_assert_int_eq(figure.f[0][x], expected_f[0][x]);
    }
    for (int x = 0; x < 3; x++) {
      ck_assert_int_eq(figure.f[1][x], expected_f[1][x]);
    }
  }
}
END_TEST

START_TEST(jFigure_test) {
  Figure_t figure;
  initFigure(&figure, 6);
  ck_assert_int_eq(figure.width, 4);
  ck_assert_int_eq(figure.height, 2);
  int expected_f[4][4] = {
      {0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  for (int x = 0; x < 3; x++) {
    ck_assert_int_eq(figure.f[0][x], expected_f[0][x]);
  }
  for (int x = 0; x < 3; x++) {
    ck_assert_int_eq(figure.f[1][x], expected_f[1][x]);
  }
}
END_TEST

START_TEST(updateCurrentState_test) {
  TetrisInfo_t *game_state = getTetrisInfo_t();
  ck_assert_ptr_nonnull(game_state);
  GameInfo_t stats = updateCurrentState();
  ck_assert_ptr_nonnull(&stats);
  ck_assert_int_eq(stats.high_score, game_state->game_info.high_score);
  ck_assert_int_eq(stats.level, game_state->game_info.level);
  ck_assert_int_eq(stats.pause, game_state->game_info.pause);
  ck_assert_int_eq(stats.score, game_state->game_info.score);
  ck_assert_int_eq(stats.speed, game_state->game_info.speed);
  removeGameInfo_t();
}
END_TEST

START_TEST(tetrisMechanics_test) {
  {
    TetrisInfo_t *game_state = getTetrisInfo_t();
    GameInfo_t stats = updateCurrentState();
    stats.score = 10000;
    game_state->game_info = stats;
    tetrisMechanics(game_state);
    ck_assert_int_eq(game_state->game_status, kWin);
    removeGameInfo_t();
  }
  {
    TetrisInfo_t *game_state = getTetrisInfo_t();
    GameInfo_t stats = updateCurrentState();
    stats.score = 0;
    game_state->game_info = stats;
    tetrisMechanics(game_state);
    ck_assert_int_ne(game_state->game_status, kWin);
    removeGameInfo_t();
  }
}
END_TEST

START_TEST(continueOrNot_test) {
  {
    TetrisInfo_t *game_state = getTetrisInfo_t();
    UserAction_t state = Terminate;
    continueOrNot(state, game_state);
    ck_assert_int_eq(game_state->game_status, kGameOver);
    removeGameInfo_t();
  }
  {
    TetrisInfo_t *game_state = getTetrisInfo_t();
    UserAction_t state = Start;
    game_state->set_time = setTime() + SECOND;
    continueOrNot(state, game_state);
    ck_assert_int_ne(game_state->game_status, kGameOver);
    removeGameInfo_t();
  }
  {
    TetrisInfo_t *game_state = getTetrisInfo_t();
    UserAction_t state = Start;
    game_state->set_time = setTime() - SECOND;
    game_state->game_info.speed = SECOND;
    for (int i = 0; i < HEIGHT; i++) {
      for (int j = 0; j < WIDTH; j++) {
        game_state->game_info.field[i][j] = STATIC_CELL;
      }
    }
    game_state->figure.y = 10;
    continueOrNot(state, game_state);
    ck_assert_int_eq(game_state->game_status, kGameOver);
    removeGameInfo_t();
  }
}
END_TEST

START_TEST(updateField_test) {
  TetrisInfo_t *game_state = getTetrisInfo_t();
  game_state->figure.x = 1;
  game_state->figure.y = 1;
  game_state->figure.width = 4;
  game_state->figure.height = 2;
  game_state->figure.f[0][0] = MOVING_CELL;
  game_state->figure.f[0][1] = MOVING_CELL;
  game_state->figure.f[1][0] = MOVING_CELL;
  game_state->figure.f[1][1] = MOVING_CELL;
  updateField(&game_state->game_info, &game_state->figure, STATIC_CELL);
  ck_assert_int_eq(game_state->game_info.field[1][1], STATIC_CELL);
  ck_assert_int_eq(game_state->game_info.field[1][2], STATIC_CELL);
  ck_assert_int_eq(game_state->game_info.field[2][1], STATIC_CELL);
  ck_assert_int_eq(game_state->game_info.field[2][2], STATIC_CELL);
  removeGameInfo_t();
}
END_TEST

START_TEST(checkCollision_test) {
  {
    TetrisInfo_t *game_state = getTetrisInfo_t();
    game_state->figure.x = 1;
    game_state->figure.y = 1;
    game_state->figure.width = 4;
    game_state->figure.height = 2;
    game_state->figure.f[0][0] = MOVING_CELL;
    game_state->figure.f[0][1] = MOVING_CELL;
    game_state->figure.f[0][2] = MOVING_CELL;
    game_state->figure.f[0][3] = MOVING_CELL;
    int dx = 0;
    int dy = HEIGHT + 1;
    ck_assert_int_eq(
        checkCollision(&game_state->game_info, &game_state->figure, dx, dy),
        STOP);
    removeGameInfo_t();
  }
  {
    TetrisInfo_t *game_state = getTetrisInfo_t();
    for (int i = 0; i < HEIGHT; i++) {
      for (int j = 0; j < WIDTH; j++) {
        game_state->game_info.field[i][j] = STATIC_CELL;
      }
    }
    game_state->figure.x = 1;
    game_state->figure.y = 1;
    game_state->figure.width = 4;
    game_state->figure.height = 2;
    game_state->figure.f[0][0] = MOVING_CELL;
    game_state->figure.f[0][1] = MOVING_CELL;
    game_state->figure.f[0][2] = MOVING_CELL;
    game_state->figure.f[0][3] = MOVING_CELL;
    int dx = 0;
    int dy = 0;
    ck_assert_int_eq(
        checkCollision(&game_state->game_info, &game_state->figure, dx, dy),
        STOP);
    removeGameInfo_t();
  }
  {
    TetrisInfo_t *game_state = getTetrisInfo_t();
    game_state->figure.x = 1;
    game_state->figure.y = 1;
    game_state->figure.width = 4;
    game_state->figure.height = 2;
    game_state->figure.f[0][0] = MOVING_CELL;
    game_state->figure.f[0][1] = MOVING_CELL;
    game_state->figure.f[0][2] = MOVING_CELL;
    game_state->figure.f[0][3] = MOVING_CELL;
    int dx = 0;
    int dy = 0;
    ck_assert_int_eq(
        checkCollision(&game_state->game_info, &game_state->figure, dx, dy),
        START);
    removeGameInfo_t();
  }
  {
    TetrisInfo_t *game_state = getTetrisInfo_t();
    game_state->figure.x = 1;
    game_state->figure.y = 1;
    game_state->figure.width = 4;
    game_state->figure.height = 2;
    game_state->figure.f[0][0] = MOVING_CELL;
    game_state->figure.f[0][1] = MOVING_CELL;
    game_state->figure.f[0][2] = MOVING_CELL;
    game_state->figure.f[0][3] = MOVING_CELL;
    int dx = 1;
    int dy = 1;
    ck_assert_int_eq(
        checkCollision(&game_state->game_info, &game_state->figure, dx, dy),
        START);
    removeGameInfo_t();
  }
}
END_TEST

START_TEST(getUserInput_test) {
  TetrisInfo_t *game_state = getTetrisInfo_t();
  getUserInput();
  ck_assert_int_eq(game_state->action, Up);
  int key = getch();
  ck_assert_int_ne(key, 0);
  bool hold = false;
  setUserAction(key, &game_state->action);
  ck_assert_int_eq(game_state->action, Up);
  userInput(game_state->action, hold);
  removeGameInfo_t();
}
END_TEST

START_TEST(setUserAction_test) {
  {
    UserAction_t state;
    setUserAction(ENTER_KEY, &state);
    ck_assert_int_eq(state, Start);
  }
  {
    UserAction_t state;
    setUserAction(SPACE_KEY, &state);
    ck_assert_int_eq(state, Action);
  }
  {
    UserAction_t state;
    setUserAction(KEY_UP, &state);
    ck_assert_int_eq(state, Action);
  }
  {
    UserAction_t state;
    setUserAction(KEY_LEFT, &state);
    ck_assert_int_eq(state, Left);
  }
  {
    UserAction_t state;
    setUserAction(KEY_RIGHT, &state);
    ck_assert_int_eq(state, Right);
  }
  {
    UserAction_t state;
    setUserAction(KEY_DOWN, &state);
    ck_assert_int_eq(state, Down);
  }
  {
    UserAction_t state;
    setUserAction('p', &state);
    ck_assert_int_eq(state, Pause);
  }
  {
    UserAction_t state;
    setUserAction('P', &state);
    ck_assert_int_eq(state, Pause);
  }
  {
    UserAction_t state;
    setUserAction('q', &state);
    ck_assert_int_eq(state, Terminate);
  }
  {
    UserAction_t state;
    setUserAction('Q', &state);
    ck_assert_int_eq(state, Terminate);
  }
  {
    UserAction_t state;
    setUserAction(ESCAPE, &state);
    ck_assert_int_eq(state, Terminate);
  }
  {
    UserAction_t state;
    setUserAction(0, &state);
    ck_assert_int_eq(state, Up);
  }
}
END_TEST

START_TEST(userInput_test) {
  {
    bool hold = false;
    userInput(Action, true);
    ck_assert_int_eq(hold, false);
  }
  {
    TetrisInfo_t *game_state = getTetrisInfo_t();
    game_state->game_info.pause = 0;
    userInput(Pause, false);
    ck_assert_int_eq(game_state->game_info.pause, 1);
    userInput(Action, false);
    userInput(Left, false);
    userInput(Right, false);
    userInput(Down, false);
    userInput(Start, false);
    userInput(Terminate, false);
    userInput(Up, false);
    userInput(Pause, false);
    ck_assert_int_eq(game_state->game_info.pause, 0);
    userInput(Left, false);
    userInput(Right, false);
    userInput(Down, false);
    removeGameInfo_t();
  }
}
END_TEST

START_TEST(rotateFigure_test) {
  TetrisInfo_t *game_state = getTetrisInfo_t();
  game_state->figure.width = 4;
  game_state->figure.height = 2;
  game_state->figure.f[0][0] = MOVING_CELL;
  game_state->figure.f[0][1] = MOVING_CELL;
  game_state->figure.f[1][0] = MOVING_CELL;
  game_state->figure.f[1][1] = MOVING_CELL;
  rotateFigure(game_state);
  ck_assert_int_eq(game_state->figure.width, 2);
  ck_assert_int_eq(game_state->figure.height, 4);
  ck_assert_int_eq(game_state->figure.f[0][0], MOVING_CELL);
  ck_assert_int_eq(game_state->figure.f[0][1], MOVING_CELL);
  ck_assert_int_eq(game_state->figure.f[1][0], MOVING_CELL);
  ck_assert_int_eq(game_state->figure.f[1][1], MOVING_CELL);
  removeGameInfo_t();
}
END_TEST

START_TEST(moveLeft_test) {
  TetrisInfo_t *game_state = getTetrisInfo_t();
  game_state->figure.x = 2;
  moveLeft(game_state);
  ck_assert_int_eq(game_state->figure.x, 1);
  removeGameInfo_t();
}
END_TEST

START_TEST(moveRight_test) {
  TetrisInfo_t *game_state = getTetrisInfo_t();
  game_state->figure.x = 0;
  moveRight(game_state);
  ck_assert_int_eq(game_state->figure.x, 1);
  removeGameInfo_t();
}
END_TEST

START_TEST(moveDown_test) {
  TetrisInfo_t *game_state = getTetrisInfo_t();
  game_state->figure.y = 0;
  moveDown(game_state);
  ck_assert_int_eq(game_state->figure.y, 19);
  removeGameInfo_t();
}
END_TEST

START_TEST(removeLine_test) {
  {
    TetrisInfo_t *game_state = getTetrisInfo_t();
    for (int y = 1; y <= HEIGHT; y++) {
      for (int x = 1; x <= WIDTH; x++) {
        game_state->game_info.field[y][x] = EMPTY_CELL;
      }
    }
    removeLine(game_state);
    for (int y = 1; y <= HEIGHT; y++) {
      for (int x = 1; x <= WIDTH; x++) {
        ck_assert_int_eq(game_state->game_info.field[y][x], EMPTY_CELL);
      }
    }
    removeGameInfo_t();
  }
  {
    TetrisInfo_t *game_state = getTetrisInfo_t();
    for (int x = 1; x <= WIDTH; x++) {
      game_state->game_info.field[HEIGHT][x] = STATIC_CELL;
    }
    removeLine(game_state);
    for (int x = 1; x <= WIDTH; x++) {
      ck_assert_int_eq(game_state->game_info.field[HEIGHT][x], EMPTY_CELL);
    }
    ck_assert_int_eq(game_state->game_info.score, 100);
    removeGameInfo_t();
  }
}
END_TEST

START_TEST(checkLine_test) {
  TetrisInfo_t *game_state = getTetrisInfo_t();
  for (int x = 1; x <= WIDTH; x++) {
    game_state->game_info.field[1][x] = STATIC_CELL;
  }
  ck_assert_int_eq(checkLine(&game_state->game_info, 1), 1);
  for (int x = 1; x <= WIDTH; x++) {
    game_state->game_info.field[1][x] = EMPTY_CELL;
  }
  ck_assert_int_eq(checkLine(&game_state->game_info, 1), 0);
  game_state->game_info.field[1][1] = STATIC_CELL;
  game_state->game_info.field[1][2] = EMPTY_CELL;
  game_state->game_info.field[1][3] = STATIC_CELL;
  ck_assert_int_eq(checkLine(&game_state->game_info, 1), 0);
  for (int x = 1; x <= WIDTH; x++) {
    game_state->game_info.field[1][x] = EMPTY_CELL;
  }
  game_state->game_info.field[1][1] = STATIC_CELL;
  ck_assert_int_eq(checkLine(&game_state->game_info, 1), 0);
  for (int x = 1; x <= WIDTH; x++) {
    game_state->game_info.field[1][x] = STATIC_CELL;
  }
  game_state->game_info.field[1][1] = EMPTY_CELL;
  ck_assert_int_eq(checkLine(&game_state->game_info, 1), 0);
  removeGameInfo_t();
}
END_TEST

START_TEST(updateScore_test) {
  GameInfo_t stats;
  stats.score = 0;
  stats.high_score = 0;
  stats.level = 1;
  stats.speed = START_SPEED;
  updateScore(&stats, 1);
  ck_assert_int_eq(stats.score, 100);
  updateScore(&stats, 2);
  ck_assert_int_eq(stats.score, 400);
  updateScore(&stats, 3);
  ck_assert_int_eq(stats.score, 1100);
  updateScore(&stats, 4);
  ck_assert_int_eq(stats.score, 2600);
  stats.score = 500;
  stats.high_score = 400;
  updateScore(&stats, 1);
  ck_assert_int_eq(stats.high_score, 600);
  stats.score = 1200;
  updateScore(&stats, 0);
  ck_assert_int_eq(stats.level, 3);
  ck_assert_double_eq(stats.speed, START_SPEED * (1 - (stats.level - 1) * 0.1));
  stats.score = 6000;
  updateScore(&stats, 0);
  ck_assert_int_eq(stats.level, 10);
  ck_assert_double_eq(stats.speed, START_SPEED * 0.05);
}
END_TEST

START_TEST(getHighScore_test) {
  int high_score = getHighScore();
  ck_assert_int_eq(high_score, 100);
}
END_TEST

START_TEST(setHighScore_test) {
  setHighScore(100);
  int high_score = getHighScore();
  ck_assert_int_eq(high_score, 100);
}
END_TEST

Suite *test_suite() {
  Suite *s = suite_create("tetris_tests");
  TCase *test = tcase_create("tetris_tests");

  tcase_add_test(test, getTetrisInfo_t_test);
  tcase_add_test(test, createInfo_t_test);
  tcase_add_test(test, figureOrdering_test);
  tcase_add_test(test, corrSpawn_test);
  tcase_add_test(test, createMatrix_test);
  tcase_add_test(test, removeGameInfo_t_test);
  tcase_add_test(test, removeMatrix_test);
  tcase_add_test(test, spawnFigure_test);
  tcase_add_test(test, initNextFigure_test);
  tcase_add_test(test, initFigure_test);
  tcase_add_test(test, copyFigure_test);
  tcase_add_test(test, lineFigure_test);
  tcase_add_test(test, squareFigure_test);
  tcase_add_test(test, zFigure_test);
  tcase_add_test(test, sFigure_test);
  tcase_add_test(test, tFigure_test);
  tcase_add_test(test, lFigure_test);
  tcase_add_test(test, jFigure_test);
  tcase_add_test(test, updateCurrentState_test);
  tcase_add_test(test, tetrisMechanics_test);
  tcase_add_test(test, continueOrNot_test);
  tcase_add_test(test, checkCollision_test);
  tcase_add_test(test, updateField_test);
  tcase_add_test(test, getUserInput_test);
  tcase_add_test(test, setUserAction_test);
  tcase_add_test(test, userInput_test);
  tcase_add_test(test, rotateFigure_test);
  tcase_add_test(test, moveLeft_test);
  tcase_add_test(test, moveRight_test);
  tcase_add_test(test, moveDown_test);
  tcase_add_test(test, removeLine_test);
  tcase_add_test(test, checkLine_test);
  tcase_add_test(test, updateScore_test);
  tcase_add_test(test, getHighScore_test);
  tcase_add_test(test, setHighScore_test);

  suite_add_tcase(s, test);
  return s;
}

int main() {
  printf("\nSTARTING TETRIS TESTS\n");
  Suite *s;
  s = test_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  int nf = srunner_ntests_failed(sr);
  srunner_free(sr);
  printf("TETRIS TESTS ARE OVER\n\n");
  return nf == 0 ? 0 : 1;
}
