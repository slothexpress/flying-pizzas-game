#include "raylib.h"
#include <string>

struct AnimationData
{
    Rectangle rectangle;
    Vector2 position;
    int frame;
    float updateInterval;
    float runningTime;
};

bool isOnGround(AnimationData data, int windowHeight)
{
    return data.position.y >= (windowHeight - data.rectangle.height);
}

AnimationData updateAnimationData(AnimationData data, float deltaTime, int maxFrame)
{
    // Update running time
    data.runningTime += deltaTime;
    // Update animation frame
    if (data.runningTime >= data.updateInterval)
    {
        data.runningTime = 0.0;
        data.rectangle.x = data.frame * data.rectangle.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

void drawScrollingBackground(Texture2D background, float xPosition, float scale, float timeSinceLastFrame)
{
    if (xPosition <= (-background.width * scale))
    {
        xPosition = 0.0;
    }
    Vector2 bgPosition{xPosition, 0.0};
    DrawTextureEx(background, bgPosition, 0.0, scale, WHITE);
    Vector2 bg2Position{xPosition + background.width * scale, 0.0};
    DrawTextureEx(background, bg2Position, 0.0, scale, WHITE);
}

int main()
{
    const int numberOfObstacles = 3;
    const int windowWidth = 800;
    const int windowHeight = 600;
    const int jumpVelocity = -1300;
    const int obstacleVelocity = -300;
    const int gravity = 3000;
    const int maxPlayerFrames = 5;
    const int maxObstaclesFrames = 2;
    const float padding = 50.0;
    int playerVelocity = 0;
    bool collision = false;
    bool gameOver = false;

    // Open window
    InitWindow(windowWidth, windowHeight, "Samiii");

    Texture2D player = LoadTexture("textures/player.png");
    AnimationData playerData;
    playerData.rectangle.width = player.width / 6;
    playerData.rectangle.height = player.height;
    playerData.rectangle.x = 0;
    playerData.rectangle.y = 0;
    playerData.position.x = windowWidth / 2 - (playerData.rectangle.width / 2);
    playerData.position.y = windowHeight - playerData.rectangle.height;
    playerData.frame = 0;
    playerData.updateInterval = 1.0 / 12.0;
    playerData.runningTime = 0.0;

    Texture2D obstacle = LoadTexture("textures/obstacle.png");
    Texture2D backgrounds[] = {
        LoadTexture("textures/background.png"),
        LoadTexture("textures/midground.png"),
        LoadTexture("textures/foreground.png")};
    float backgroundX[] = {0.0, 0.0, 0.0};

    // Array of obstacles
    AnimationData obstacles[numberOfObstacles]{};

    for (int i = 0; i < numberOfObstacles; i++)
    {
        obstacles[i].rectangle.x = 0.0;
        obstacles[i].rectangle.y = 0.0;
        obstacles[i].rectangle.width = obstacle.width / 3;
        obstacles[i].rectangle.height = obstacle.height;
        obstacles[i].position.x = windowWidth + 700 + i * 500;
        obstacles[i].position.y = windowHeight - obstacle.height;
        obstacles[i].frame = 0;
        obstacles[i].updateInterval = 1 / 9.0;
        obstacles[i].runningTime = 0.0;
    }

    float finishLine = obstacles[numberOfObstacles - 1].position.x;

    // Frames per second
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        const float timeSinceLastFrame = GetFrameTime();
        const float scale = 3.3;
        bool jump = IsKeyPressed(KEY_SPACE);
        bool onTheGround = isOnGround(playerData, windowHeight);

        // Start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        for (unsigned int i = 0; i < 3; i++)
        {
            backgroundX[i] -= i * 30 * timeSinceLastFrame;
            drawScrollingBackground(backgrounds[i], backgroundX[i], scale, timeSinceLastFrame);
        }

        // Perform ground check
        playerVelocity = onTheGround ? 0 : playerVelocity + (gravity * timeSinceLastFrame);

        // Check for jump (and avoid air jump/double jump)
        if (jump && onTheGround)
        {
            playerVelocity = playerVelocity + jumpVelocity;
        }

        // Update player position (must be after jump check)
        playerData.position.y += (playerVelocity * timeSinceLastFrame);

        // Update player animation frame
        if (onTheGround)
        {
            playerData = updateAnimationData(playerData, timeSinceLastFrame, maxPlayerFrames);
        }

        // Check collision
        if (!gameOver)
        {
            for (AnimationData obs : obstacles)
            {
                Rectangle obsRec{
                    obs.position.x - padding,
                    obs.position.y - padding,
                    obs.rectangle.width - padding,
                    obs.rectangle.height - padding};

                Rectangle playRec{
                    playerData.position.x - padding,
                    playerData.position.y - padding,
                    playerData.rectangle.width - padding,
                    playerData.rectangle.height - padding};

                if (CheckCollisionRecs(obsRec, playRec))
                {
                    collision = true;
                }
            }
        }

        // Check if game is over
        if (collision)
        {
            DrawText("Game Over :(", windowWidth / 4, windowHeight / 2, 70, WHITE);
            gameOver = true;
        }
        else if (playerData.position.x >= finishLine)
        {
            DrawText("YOU WIN :)", windowWidth / 3, windowHeight / 2, 50, WHITE);
            gameOver = true;
        }
        else
        {
            // Draw player
            DrawTextureRec(player, playerData.rectangle, playerData.position, WHITE);

            // Draw obstacles
            for (int i = 0; i < numberOfObstacles; i++)
            {
                // Update obstacle position (moving left on X-axis)
                obstacles[i].position.x += (obstacleVelocity * timeSinceLastFrame);
                // Update obstacle animation frame
                obstacles[i] = updateAnimationData(obstacles[i], timeSinceLastFrame, maxObstaclesFrames);
                // Draw obstacle
                DrawTextureRec(obstacle, obstacles[i].rectangle, obstacles[i].position, WHITE);
            }
        }

        // Update finish line
        finishLine += obstacleVelocity * timeSinceLastFrame;

        EndDrawing();
    }
    UnloadTexture(player);
    UnloadTexture(obstacle);
    for (auto bg : backgrounds)
    {
        UnloadTexture(bg);
    }

    CloseWindow();
}