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
    AnimationData obstacleData{
        {0.0, 0.0, obstacle.width/3, obstacle.height}, // Rectangle rectangle
        {windowWidth, windowHeight - obstacle.height}, // Vector2 position
        0, // int frame
        1.0/12.0, // float updateTime
        0.0 // float runningTime
    };

    Texture2D obstacle2 = LoadTexture("textures/obstacle.png");
    AnimationData obstacle2Data{
        {0.0, 0.0, obstacle2.width/3, obstacle2.height}, // Rectangle rectangle
        {windowWidth + 300, windowHeight - obstacle2.height}, // Vector2 position
        0, // int frame
        1.0/16.0, // float updateTime
        0.0 // float runningTime
    };

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

        obstacle2Data.position.x += (obstacleVelocity * timeSinceLastFrame);
        obstacleData.position.x += (obstacleVelocity * timeSinceLastFrame);
        playerData.position.y += (playerVelocity * timeSinceLastFrame);

        obstacleData.runningTime += timeSinceLastFrame;
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

        if(obstacleData.runningTime >= obstacleData.updateTime)
        {
            obstacleData.runningTime = 0.0;
            obstacleData.rectangle.x = obstacleData.frame * obstacleData.rectangle.width;
            obstacleData.frame++;
            if(obstacleData.frame > 2)
            {
                obstacleData.frame = 0;
            }
        }

        if(obstacle2Data.runningTime >= obstacle2Data.updateTime)
        {
            obstacle2Data.runningTime = 0.0;
            obstacle2Data.rectangle.x = obstacle2Data.frame * obstacle2Data.rectangle.width;
            obstacle2Data.frame++;
           if(obstacle2Data.frame > 2)
            {
                obstacle2Data.frame = 0;
            }
        }
        DrawTextureRec(obstacle2, obstacle2Data.rectangle, obstacle2Data.position, WHITE);
        DrawTextureRec(obstacle, obstacleData.rectangle, obstacleData.position, WHITE);
        DrawTextureRec(player, playerData.rectangle, playerData.position, WHITE);
        EndDrawing();
        
    }
    UnloadTexture(player);
    UnloadTexture(obstacle);
    CloseWindow();
}