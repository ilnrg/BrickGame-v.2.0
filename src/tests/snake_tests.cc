#include "../brick_game/snake/snake_controller.h"
#include "gtest/gtest.h"

TEST(ClassSnake, ConstructorAndGetters) {
  s21::Snake snake(0, 0);
  EXPECT_EQ(snake.getSnakeBody().size(), 4);
  EXPECT_EQ(snake.getDirection(), s21::Snake::kDown);
}

TEST(ClassSnake, Setter) {
  s21::Snake snake(0, 0);
  EXPECT_EQ(snake.getDirection(), s21::Snake::kDown);
  snake.setDirection(s21::Snake::kUp);
  EXPECT_NE(snake.getDirection(), s21::Snake::kUp);
  snake.setDirection(s21::Snake::kLeft);
  EXPECT_EQ(snake.getDirection(), s21::Snake::kLeft);
}

TEST(ClassSnake, Moving) {
  s21::Snake snake(0, 0);
  EXPECT_EQ(snake.getSnakeBody().size(), 4);
  snake.move();
  EXPECT_EQ(snake.getSnakeBody().size(), 4);
  snake.grow();
  EXPECT_EQ(snake.getSnakeBody().size(), 5);
  snake.setDirection(s21::Snake::kRight);
  auto head = snake.nextStep();
  EXPECT_EQ(head.first, 2);
  snake.setDirection(s21::Snake::kDown);
  head = snake.nextStep();
  EXPECT_EQ(head.second, 6);
  snake.setDirection(s21::Snake::kLeft);
  head = snake.nextStep();
  EXPECT_EQ(head.first, -2);
  snake.setDirection(s21::Snake::kUp);
  head = snake.nextStep();
  EXPECT_EQ(head.second, 4);
}

TEST(ClassApple, ConstructorAndGetters) {
  s21::Apple apple;
  EXPECT_EQ(apple.getAppleX(), 0);
  EXPECT_EQ(apple.getAppleY(), 0);
}

TEST(ClassApple, AppleSpawning) {
  s21::Apple apple;
  std::vector<std::pair<int, int>> snake_body;
  snake_body.push_back(std::make_pair(0, 0));
  snake_body.push_back(std::make_pair(2, 0));
  snake_body.push_back(std::make_pair(4, 0));
  snake_body.push_back(std::make_pair(6, 0));
  snake_body.push_back(std::make_pair(8, 0));
  apple.spawnApple(snake_body);
  EXPECT_NE(apple.getAppleX(), 0);
  EXPECT_NE(apple.getAppleY(), 0);
}

TEST(ClassModel, ConstructorAndGetters) {
  s21::SnakeModel model;
  s21::SnakeModel::SnakeInfo_t *game_state = model.getSnakeInfo_t();
  GameInfo_t *game_info = model.getGameInfo_t();
  EXPECT_NE(game_state->game_info.field, nullptr);
  EXPECT_EQ(game_state->game_info.field, game_info->field);
  EXPECT_EQ(game_state->action, Start);
  EXPECT_EQ(game_state->game_status, kStart);
  EXPECT_EQ(game_state->game_info.score, 0);
  EXPECT_EQ(game_state->game_info.score, game_info->score);
  EXPECT_EQ(game_state->game_info.high_score, model.getHighScore());
  EXPECT_EQ(game_state->game_info.high_score, game_info->high_score);
  EXPECT_EQ(game_state->game_info.level, 1);
  EXPECT_EQ(game_state->game_info.level, game_info->level);
  EXPECT_EQ(game_state->game_info.speed, START_SPEED);
  EXPECT_EQ(game_state->game_info.speed, game_info->speed);
  EXPECT_EQ(game_state->game_info.speed, game_state->current_speed);
  EXPECT_EQ(game_state->game_info.pause, 0);
  EXPECT_EQ(game_state->game_info.pause, game_info->pause);
  EXPECT_EQ(game_state->set_time, 0);
  EXPECT_EQ(model.getSnake().getDirection(), s21::Snake::kDown);
}

TEST(ClassModel, MainLogic) {
  {
    s21::SnakeModel model;
    s21::SnakeModel::SnakeInfo_t *game_state = model.getSnakeInfo_t();
    EXPECT_EQ(game_state->game_status, kStart);
    model.snakeMechanics(game_state->game_status);
    EXPECT_EQ(game_state->game_status, kStart);
  }
  {
    s21::SnakeModel model;
    s21::SnakeModel::SnakeInfo_t *game_state = model.getSnakeInfo_t();
    GameInfo_t *game_info = model.getGameInfo_t();
    EXPECT_EQ(game_state->game_status, kStart);
    game_info->score = SNAKE_MAX_SCORE;
    model.snakeMechanics(game_state->game_status);
    EXPECT_EQ(game_state->game_status, kWin);
  }
  {
    s21::SnakeModel model;
    s21::SnakeModel::SnakeInfo_t *game_state = model.getSnakeInfo_t();
    game_state->action = Terminate;
    model.snakeMechanics(game_state->game_status);
    EXPECT_EQ(game_state->game_status, kGameOver);
  }
  {
    s21::SnakeModel model;
    GameInfo_t *game_info = model.getGameInfo_t();
    model.updateField(*game_info);
    EXPECT_NE(game_info->field, nullptr);
    EXPECT_EQ(game_info->field[1][1], 0);
  }
}

TEST(ClassModel, SpeedBoost) {
  {
    s21::SnakeModel model;
    bool hold = false;
    int key = KEY_DOWN;
    model.boostOrNot(key, &hold);
    EXPECT_EQ(hold, true);
    key = KEY_UP;
    model.boostOrNot(key, &hold);
    EXPECT_EQ(hold, false);
    key = KEY_LEFT;
    model.boostOrNot(key, &hold);
    EXPECT_EQ(hold, false);
    key = KEY_RIGHT;
    model.boostOrNot(key, &hold);
    EXPECT_EQ(hold, false);
  }
  {
    s21::SnakeModel model;
    GameInfo_t *game_info = model.getGameInfo_t();
    EXPECT_EQ(game_info->speed, START_SPEED);
    model.speedBoost(true);
    EXPECT_EQ(game_info->speed, START_SPEED / 5);
    model.speedBoost(false);
    EXPECT_EQ(game_info->speed, START_SPEED);
  }
  {
    s21::SnakeModel model;
    GameInfo_t *game_info = model.getGameInfo_t();
    game_info->level = 7;
    EXPECT_EQ(game_info->speed, START_SPEED);
    model.speedBoost(true);
    EXPECT_EQ(game_info->speed, START_SPEED / 3);
    model.speedBoost(false);
    EXPECT_EQ(game_info->speed, START_SPEED);
  }
}

TEST(ClassModel, HighScoreGetterAndSetter) {
  {
    s21::SnakeModel model;
    int high_score = model.getHighScore();
    EXPECT_EQ(high_score, 0);
  }
  {
    s21::SnakeModel model;
    model.setHighScore(100);
    int high_score = model.getHighScore();
    EXPECT_EQ(high_score, 100);
  }
}

TEST(ClassController, InputProcessing) {
  {
    s21::SnakeModel model;
    s21::SnakeController controller(&model);
    controller.getUserInput();
    EXPECT_EQ(model.getSnake().getDirection(), s21::Snake::kDown);
  }
  {
    s21::SnakeModel model;
    s21::SnakeController controller(&model);
    s21::SnakeModel::SnakeInfo_t *game_state =
        controller.getModel()->getSnakeInfo_t();
    bool hold = false;
    controller.setUserAction(SPACE_KEY, &game_state->action, &hold);
    EXPECT_EQ(game_state->action, Action);
    controller.setUserAction(KEY_UP, &game_state->action, &hold);
    EXPECT_EQ(game_state->action, Up);
    controller.setUserAction(KEY_LEFT, &game_state->action, &hold);
    EXPECT_EQ(game_state->action, Left);
    controller.setUserAction(KEY_RIGHT, &game_state->action, &hold);
    EXPECT_EQ(game_state->action, Right);
    controller.setUserAction(KEY_DOWN, &game_state->action, &hold);
    EXPECT_EQ(game_state->action, Down);
    controller.setUserAction('p', &game_state->action, &hold);
    EXPECT_EQ(game_state->action, Pause);
    controller.setUserAction('P', &game_state->action, &hold);
    EXPECT_EQ(game_state->action, Pause);
    controller.setUserAction('q', &game_state->action, &hold);
    EXPECT_EQ(game_state->action, Terminate);
    controller.setUserAction('Q', &game_state->action, &hold);
    EXPECT_EQ(game_state->action, Terminate);
    controller.setUserAction(ESCAPE, &game_state->action, &hold);
    EXPECT_EQ(game_state->action, Terminate);
    controller.setUserAction(ENTER_KEY, &game_state->action, &hold);
    EXPECT_EQ(game_state->action, Start);
  }
  {
    s21::SnakeModel model;
    s21::SnakeController controller(&model);
    s21::SnakeModel::SnakeInfo_t *game_state =
        controller.getModel()->getSnakeInfo_t();
    GameInfo_t *game_info = model.getGameInfo_t();
    controller.getUserInput();
    EXPECT_EQ(game_state->action, Start);
    game_info->pause = 0;
    controller.userInput(Pause, false);
    EXPECT_EQ(game_info->pause, 1);
    controller.userInput(Left, false);
    EXPECT_NE(model.getSnake().getDirection(), s21::Snake::kLeft);
    controller.userInput(Action, false);
    EXPECT_EQ(game_info->pause, 0);
    controller.userInput(Left, false);
    EXPECT_EQ(model.getSnake().getDirection(), s21::Snake::kLeft);
    controller.userInput(Up, false);
    EXPECT_EQ(model.getSnake().getDirection(), s21::Snake::kUp);
    controller.userInput(Right, false);
    EXPECT_EQ(model.getSnake().getDirection(), s21::Snake::kRight);
    controller.userInput(Down, false);
    EXPECT_EQ(model.getSnake().getDirection(), s21::Snake::kDown);
  }
}

TEST(ClassController, Getters) {
  {
    s21::SnakeModel model;
    s21::SnakeController controller(&model);
    EXPECT_EQ(controller.getModel(), &model);
  }
  {
    s21::SnakeModel model;
    s21::SnakeController controller(&model);
    GameInfo_t *game_info_ptr = model.getGameInfo_t();
    GameInfo_t game_info_copy = controller.updateCurrentState();
    EXPECT_EQ(game_info_ptr->score, game_info_copy.score);
    EXPECT_EQ(game_info_ptr->high_score, game_info_copy.high_score);
    EXPECT_EQ(game_info_ptr->level, game_info_copy.level);
    EXPECT_EQ(game_info_ptr->speed, game_info_copy.speed);
    EXPECT_EQ(game_info_ptr->pause, game_info_copy.pause);
  }
}

int main(int argc, char **argv) {
  std::cout << std::endl << "STARTING SNAKE TESTS" << std::endl;
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}