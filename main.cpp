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
    Texture2D obstacle2 = LoadTexture("textures/obstacle.png");

    // Array of obstacles
    AnimationData obstacles[2]{};

    for (int i = 0; i < 2; i++)
    {
        obstacles[i].rectangle.x = 0.0;
        obstacles[i].rectangle.y = 0.0;
        obstacles[i].rectangle.width = obstacle.width/3;
        obstacles[i].rectangle.height = obstacle.height;
        obstacles[i].position.y = windowHeight - obstacle.height;
        obstacles[i].frame = 0;
        obstacles[i].runningTime = 0.0;
        obstacles[i].updateTime = 0.0;
    }

    obstacles[0].position.x = windowWidth;
    obstacles[1].position.x = windowWidth + 300;

    // Frames per second
    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        bool jump = IsKeyPressed(KEY_SPACE);
        bool onTheGround = playerData.position.y >= (windowHeight - playerData.rectangle.height);
        const float timeSinceLastFrame = GetFrameTime();
        
        BeginDrawing();
        ClearBackground(WHITE);  

        // Add gravity if NOT on the ground
        playerVelocity = onTheGround ? 0 : playerVelocity + (gravity * timeSinceLastFrame);

        if(jump && onTheGround)
        {
            playerVelocity = playerVelocity + jumpVelocity;
        }

        obstacles[1].position.x += (obstacleVelocity * timeSinceLastFrame);
        obstacles[0].position.x += (obstacleVelocity * timeSinceLastFrame);
        playerData.position.y += (playerVelocity * timeSinceLastFrame);

        obstacles[0].runningTime += timeSinceLastFrame;
        playerData.runningTime += timeSinceLastFrame;

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

        if(obstacles[0].runningTime >= obstacles[0].updateTime)
        {
            obstacles[0].runningTime = 0.0;
            // Update animation frame
            obstacles[0].rectangle.x = obstacles[0].frame * obstacles[0].rectangle.width;
            obstacles[0].frame++;
            if(obstacles[0].frame > 2)
            {
                obstacles[0].frame = 0;
            }
        }

        if(obstacles[1].runningTime >= obstacles[1].updateTime)
        {
            obstacles[1].runningTime = 0.0;
            // Update animation frame
            obstacles[1].rectangle.x = obstacles[1].frame * obstacles[1].rectangle.width;
            obstacles[1].frame++;
           if(obstacles[1].frame > 2)
            {
                obstacles[1].frame = 0;
            }
        }
        DrawTextureRec(obstacle2, obstacles[1].rectangle, obstacles[1].position, WHITE);
        DrawTextureRec(obstacle, obstacles[0].rectangle, obstacles[0].position, WHITE);
        DrawTextureRec(player, playerData.rectangle, playerData.position, WHITE);
        EndDrawing();
        
    }
    UnloadTexture(player);
    UnloadTexture(obstacle);
    UnloadTexture(obstacle2);
    CloseWindow();
}