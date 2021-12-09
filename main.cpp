#include "raylib.h"
#include <string>

struct AnimationData
{
    Rectangle rectangle;
    Vector2 position;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimationData data, int windowHeight)
{
    return data.position.y >= (windowHeight - data.rectangle.height);
}

int main()
{
    const int windowWidth = 800;
    const int windowHeight = 600;
    const int jumpVelocity = -800;
    const int obstacleVelocity = -300;
    const int gravity = 3000;
    int playerVelocity = 0;    

    // Open window
    InitWindow(windowWidth, windowHeight, "Samiii");

    Texture2D player = LoadTexture("textures/player.png");
    AnimationData playerData;
    playerData.rectangle.width = player.width/6;
    playerData.rectangle.height = player.height;
    playerData.rectangle.x = 0;
    playerData.rectangle.y = 0;
    playerData.position.x = windowWidth/2 - (playerData.rectangle.width/2);
    playerData.position.y = windowHeight - playerData.rectangle.height;
    playerData.frame = 0;
    playerData.updateTime = 1.0/12.0;
    playerData.runningTime = 0.0;

    Texture2D obstacle = LoadTexture("textures/obstacle.png");

    const int numberOfObstacles = 6;

    // Array of obstacles
    AnimationData obstacles[numberOfObstacles]{};

    for (int i = 0; i < numberOfObstacles; i++)
    {
        obstacles[i].rectangle.x = 0.0;
        obstacles[i].rectangle.y = 0.0;
        obstacles[i].rectangle.width = obstacle.width/3;
        obstacles[i].rectangle.height = obstacle.height;
        obstacles[i].position.x = windowWidth + i * 400;
        obstacles[i].position.y = windowHeight - obstacle.height;
        obstacles[i].frame = 0;
        obstacles[i].updateTime = 1/9.0; // Bug: only first obstacle is moving, but if removed, all of them move too fast!
        obstacles[i].runningTime = 0.0;
    }

    // Frames per second
    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        bool jump = IsKeyPressed(KEY_SPACE);
        bool onTheGround = isOnGround(playerData, windowHeight);
        const float timeSinceLastFrame = GetFrameTime();
        
        BeginDrawing();
        ClearBackground(WHITE);  

        // Perform ground check
        playerVelocity = onTheGround ? 0 : playerVelocity + (gravity * timeSinceLastFrame);

        // Avoid air jump/double jump
        if(jump && onTheGround)
        {
            playerVelocity = playerVelocity + jumpVelocity;
        }

        // Update obstacle position
        for(int i = 0; i < numberOfObstacles; i++)
        {
            obstacles[i].position.x += (obstacleVelocity * timeSinceLastFrame);
        }

        // Update player position
        playerData.position.y += (playerVelocity * timeSinceLastFrame);

        for(int i = 0; i < numberOfObstacles; i++)
        {
            obstacles[i].runningTime += timeSinceLastFrame;
        }
        playerData.runningTime += timeSinceLastFrame;

        // Update player animation frame
        if(onTheGround)
        {
            if(playerData.runningTime >= playerData.updateTime)
            {
                playerData.runningTime = 0.0;
                // Update animation frame
                playerData.rectangle.x = playerData.frame * playerData.rectangle.width;
                playerData.frame++;
                if(playerData.frame > 5)
                {
                    playerData.frame = 0;
                }
            }
        }

        // Update obstacle animation frame
        for(int i = 0; i < numberOfObstacles; i++)
        {
            if(obstacles[i].runningTime >= obstacles[i].updateTime)
            {
                obstacles[i].runningTime = 0.0;
                // Update animation frame
                obstacles[i].rectangle.x = obstacles[i].frame * obstacles[i].rectangle.width;
                obstacles[i].frame++;

            if(obstacles[i].frame > 2)
                {
                    obstacles[i].frame = 0;
                }
            }
        }

        for(int i = 0; i < numberOfObstacles; i++)
        {
            DrawTextureRec(obstacle, obstacles[i].rectangle, obstacles[i].position, WHITE);
        }

        DrawTextureRec(player, playerData.rectangle, playerData.position, WHITE);
        EndDrawing();
    }
    UnloadTexture(player);
    UnloadTexture(obstacle);
    CloseWindow();
}