#include "raylib.h"
#include <string>

int main()
{
    const int windowWidth = 800;
    const int windowHeight = 600;
    const int jumpVelocity = -800;
    const int obstacleVelocity = -300;
    const int gravity = 3000;
    const float playerUpdateTime = 1.0 / 12.0;
    const float obstacleUpdateTime = 1.0 / 12.0;
    float playerRunningTime = 0.0;
    float obstacleRunningTime = 0.0;
    int playerVelocity = 0;    
    int playerFrame = 0;
    int obstacleFrame = 0;

    // Open window
    InitWindow(windowWidth, windowHeight, "Samiii");

    Texture2D player = LoadTexture("textures/player.png");
    Rectangle playerRectangle{0.0, 0.0, player.width/6, player.height};
    Vector2 playerPosition{windowWidth/2 - (playerRectangle.width/2), windowHeight - playerRectangle.height};

    Texture2D obstacle = LoadTexture("textures/obstacle.png");
    Rectangle obstacleRectangle{0.0, 0.0, obstacle.width/3, obstacle.height};
    Vector2 obstaclePosition{windowWidth, windowHeight - obstacleRectangle.height - 5};

    Texture2D obstacle2 = LoadTexture("textures/obstacle.png");
    Rectangle obstacleRectangle2{0.0, 0.0, obstacle.width/3, obstacle.height};
    Vector2 obstaclePosition2{windowWidth + 300, windowHeight - obstacleRectangle.height - 5};
    const float obstacleUpdateTime2 = 1.0 / 16.0;
    float obstacleRunningTime2 = 0;
    int obstacleFrame2 = 0;

    // Frames per second
    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        bool jump = IsKeyPressed(KEY_SPACE);
        bool onTheGround = playerPosition.y >= (windowHeight - playerRectangle.height);
        const float timeSinceLastFrame = GetFrameTime();
        
        BeginDrawing();
        ClearBackground(WHITE);  

        // Add gravity if NOT on the ground
        playerVelocity = onTheGround ? 0 : playerVelocity + (gravity * timeSinceLastFrame);

        if(jump && onTheGround)
        {
            playerVelocity = playerVelocity + jumpVelocity;
        }


        obstaclePosition2.x = obstaclePosition2.x + (obstacleVelocity * timeSinceLastFrame);
        obstaclePosition.x = obstaclePosition.x + (obstacleVelocity * timeSinceLastFrame);
        playerPosition.y = playerPosition.y + (playerVelocity * timeSinceLastFrame);

        obstacleRunningTime = obstacleRunningTime + timeSinceLastFrame;
        playerRunningTime = playerRunningTime + timeSinceLastFrame;

        if(onTheGround)
        {
            if(playerRunningTime >= playerUpdateTime)
            {
                playerRunningTime = 0.0;
                // Update animation frame
                playerRectangle.x = playerFrame * playerRectangle.width;
                playerFrame++;
                if(playerFrame > 5)
                {
                    playerFrame = 0;
                }
            }
        }

        if(obstacleRunningTime >= obstacleUpdateTime)
        {
            obstacleRunningTime = 0.0;
            obstacleRectangle.x = obstacleFrame * obstacleRectangle.width;
            obstacleFrame++;
            if(obstacleFrame > 2)
            {
                obstacleFrame = 0;
            }
        }

        if(obstacleRunningTime2 >= obstacleUpdateTime2)
        {
            obstacleRunningTime2 = 0.0;
            obstacleRectangle2.x = obstacleFrame2 * obstacleRectangle2.width;
            obstacleFrame2++;
            if(obstacleFrame2 > 2)
            {
                obstacleFrame2 = 0;
            }
        }


        DrawTextureRec(obstacle2, obstacleRectangle2, obstaclePosition2, WHITE);

        DrawTextureRec(obstacle, obstacleRectangle, obstaclePosition, WHITE);

        DrawTextureRec(player, playerRectangle, playerPosition, WHITE);
        EndDrawing();
        
    }
    UnloadTexture(player);
    UnloadTexture(obstacle);
    CloseWindow();
}