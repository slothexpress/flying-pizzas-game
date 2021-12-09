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

AnimationData updateAnimationData(AnimationData data, float deltaTime, int maxFrame)
{
    // Update running time
    data.runningTime += deltaTime;
    // Update animation frame
    if (data.runningTime >= data.updateTime)
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
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX;

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
        float bgScale = 3.3;
        
        // Start drawing
        BeginDrawing();
        ClearBackground(WHITE); 
         
        bgX -= 20 * timeSinceLastFrame;

        // Draw background
        Vector2 bgPosition{bgX, 0.0};
        DrawTextureEx(background, bgPosition, 0.0, bgScale, WHITE);
        Vector2 bg2Position{bgX + background.width * bgScale, 0.0};
        DrawTextureEx(background, bg2Position, 0.0, bgScale, WHITE);

        // Perform ground check
        playerVelocity = onTheGround ? 0 : playerVelocity + (gravity * timeSinceLastFrame);

        // Avoid air jump/double jump
        if(jump && onTheGround)
        {
            playerVelocity = playerVelocity + jumpVelocity;
        }

        // Update player position (must be after condition to check for jump)
        playerData.position.y += (playerVelocity * timeSinceLastFrame);

        // Update player animation frame
        if(onTheGround)
        {
            playerData = updateAnimationData(playerData, timeSinceLastFrame, 5);
        }

        for(int i = 0; i < numberOfObstacles; i++)
        {
            // Update obstacle position (moving left on X-axis)
            obstacles[i].position.x += (obstacleVelocity * timeSinceLastFrame);
            // Update obstacle animation frame
            obstacles[i] = updateAnimationData(obstacles[i], timeSinceLastFrame, 2);
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
    UnloadTexture(background);
    CloseWindow();
}