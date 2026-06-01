#include "collision.h"
#include "unity.h"

// raylib types provided by raylib.h via snake.h → config.h → raylib.h
#include "snake.h"
#include "food.h"

static Snake make_snake(int len, Vector2 *segments)
{
    Snake s;
    s.length = len;
    for (int i = 0; i < len; i++)
        s.body[i] = segments[i];
    s.direction = (Vector2){1, 0};
    s.dirQueueLen = 0;
    s.dirQueue[0] = (Vector2){0, 0};
    s.dirQueue[1] = (Vector2){0, 0};
    return s;
}

static Food make_food(int x, int y)
{
    Food f;
    f.pos.x = (float)x;
    f.pos.y = (float)y;
    return f;
}

void test_no_self_collision(void)
{
    Vector2 segs[] = {{5,5},{4,5},{3,5}};
    Snake s = make_snake(3, segs);
    TEST_ASSERT_FALSE(CheckSelfCollision(&s));
}

void test_self_collision_head_tail(void)
{
    // Snake loops back onto itself at the tail
    Vector2 segs[] = {{5,5},{6,5},{6,6},{5,6},{5,5}};
    Snake s = make_snake(5, segs);
    TEST_ASSERT_TRUE(CheckSelfCollision(&s));
}

void test_self_collision_mid_body(void)
{
    Vector2 segs[] = {{3,3},{3,4},{3,5},{4,5},{5,5},{5,4},{4,4}};
    Snake s = make_snake(7, segs);
    // Move head to body segment at index 2 (3,5) — self collision
    s.body[0].x = 3; s.body[0].y = 5;
    TEST_ASSERT_TRUE(CheckSelfCollision(&s));
}

void test_wall_collision_left(void)
{
    Vector2 segs[] = {{-1,5}};
    Snake s = make_snake(1, segs);
    TEST_ASSERT_TRUE(CheckWallCollision(&s));
}

void test_wall_collision_right(void)
{
    Vector2 segs[] = {{CELL_COUNT,5}};
    Snake s = make_snake(1, segs);
    TEST_ASSERT_TRUE(CheckWallCollision(&s));
}

void test_wall_collision_top(void)
{
    Vector2 segs[] = {{5,-1}};
    Snake s = make_snake(1, segs);
    TEST_ASSERT_TRUE(CheckWallCollision(&s));
}

void test_wall_collision_bottom(void)
{
    Vector2 segs[] = {{5,CELL_COUNT}};
    Snake s = make_snake(1, segs);
    TEST_ASSERT_TRUE(CheckWallCollision(&s));
}

void test_no_wall_collision(void)
{
    Vector2 segs[] = {{0,0}};
    Snake s = make_snake(1, segs);
    TEST_ASSERT_FALSE(CheckWallCollision(&s));
}

void test_no_wall_collision_max(void)
{
    Vector2 segs[] = {{CELL_COUNT-1, CELL_COUNT-1}};
    Snake s = make_snake(1, segs);
    TEST_ASSERT_FALSE(CheckWallCollision(&s));
}

void test_food_collision_hit(void)
{
    Vector2 segs[] = {{7,8}};
    Snake s = make_snake(1, segs);
    Food f = make_food(7, 8);
    TEST_ASSERT_TRUE(CheckFoodCollision(&s, &f));
}

void test_food_collision_miss(void)
{
    Vector2 segs[] = {{3,4}};
    Snake s = make_snake(1, segs);
    Food f = make_food(7, 8);
    TEST_ASSERT_FALSE(CheckFoodCollision(&s, &f));
}

int main(void)
{
    RUN_TEST(test_no_self_collision);
    RUN_TEST(test_self_collision_head_tail);
    RUN_TEST(test_self_collision_mid_body);
    RUN_TEST(test_wall_collision_left);
    RUN_TEST(test_wall_collision_right);
    RUN_TEST(test_wall_collision_top);
    RUN_TEST(test_wall_collision_bottom);
    RUN_TEST(test_no_wall_collision);
    RUN_TEST(test_no_wall_collision_max);
    RUN_TEST(test_food_collision_hit);
    RUN_TEST(test_food_collision_miss);
    UNITY_END();
    return 0;
}
