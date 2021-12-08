#include "raylib.h"
#include <string>

int main()
{
    const int windowWidth = 512;
    const int windowHeight = 380;
    const int jumpVelocity = -800;
    const int nebulaVelocity = -300;
    const int gravity = 3000;
    const float updateTime = 1.0 / 12.00;
    float runningTime = 0.0;
    int playerVelocity = 0;    
   
    int frame = 0;

    // Open window
    InitWindow(windowWidth, windowHeight, "Samiii");

    Texture2D nebula = LoadTexture("textures/nebula.png");
    Rectangle nebulaRectangle{0.0, 0.0, nebula.width/8, nebula.height/8};
    Vector2 nebulaPosition{windowWidth-40, windowHeight - nebulaRectangle.height};

    Texture2D player = LoadTexture("textures/scarfy.png");
    Rectangle playerRectangle{0.0, 0.0, player.width/6, player.height};
    Vector2 playerPosition{windowWidth/2 - (playerRectangle.width/2), windowHeight - playerRectangle.height};

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

        if(frame > 5)
        {
            nebulaPosition.x = nebulaPosition.x + (nebulaVelocity * timeSinceLastFrame);
        }
        
        playerPosition.y = playerPosition.y + (playerVelocity * timeSinceLastFrame);

        runningTime = runningTime + timeSinceLastFrame;

        if(onTheGround)
        {
            if(runningTime >= updateTime)
            {
                runningTime = 0.0;
                playerRectangle.x = frame * playerRectangle.width;
                frame++;
                if(frame > 5)
                {
                    frame = 0;
                }
            }
        }

        DrawTextureRec(nebula, nebulaRectangle, nebulaPosition, WHITE);
        DrawTextureRec(player, playerRectangle, playerPosition, WHITE);
        EndDrawing();
        
    }
    UnloadTexture(player);
    UnloadTexture(nebula);
    CloseWindow();
}