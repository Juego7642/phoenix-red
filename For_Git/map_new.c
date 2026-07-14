// #include "raylib.h"

// #define TILE_SIZE 40
// #define MAP_WIDTH 20
// #define MAP_HEIGHT 15
// #define SCREEN_WIDTH (MAP_WIDTH * TILE_SIZE)
// #define SCREEN_HEIGHT (MAP_HEIGHT * TILE_SIZE)

// // Tile Types
// typedef enum {
//     TILE_WALKABLE = 0,
//     TILE_OBSTACLE,
//     TILE_BOUNDARY
// } TileType;

// // Define a simple 20x15 map grid
// int map[MAP_HEIGHT][MAP_WIDTH] = {
//     {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
//     {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
//     {2,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,2},
//     {2,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,2},
//     {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
//     {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
//     {2,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,2},
//     {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
//     {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
//     {2,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,2},
//     {2,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,2},
//     {2,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,2},
//     {2,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,2},
//     {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
//     {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}
// };

// int main(void) {
//     // Initialize Window
//     InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Grid Map");
//     SetTargetFPS(60);

//     // Load Player Texture
//     Texture2D playerSprite = LoadTexture("player.png");            
    
//     // Player State (Grid units)
//     Vector2 playerGridPos = { 9.0f, 9.0f }; 
    
//     // Movement delay to control grid step speed
//     float moveDelay = 0.05f;
//     float timeSinceLastMove = 0.0f;

//     // Main Game Loop
//     while (!WindowShouldClose()) {
//         float deltaTime = GetFrameTime();
//         timeSinceLastMove += deltaTime;

//         // Handle Grid Movement Input
//         if (timeSinceLastMove >= moveDelay) {
//             Vector2 targetPos = playerGridPos;

//             if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))    targetPos.y -= 1;
//             if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))  targetPos.y += 1;
//             if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))  targetPos.x -= 1;
//             if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) targetPos.x += 1;

//             // Collision Check: Only move if target tile is walkable (0)
            
                
//                 if (map[(int)targetPos.y][(int)targetPos.x] == TILE_WALKABLE) {
//                     playerGridPos = targetPos;
//                     timeSinceLastMove = 0.0f; // Reset movement timer
//                 }
            
//         }

//         // Render Graphics
//         BeginDrawing();
//         ClearBackground(RAYWHITE);

//         // Draw Map Grid Layout
//         for (int y = 0; y < MAP_HEIGHT; y++) {
//             for (int x = 0; x < MAP_WIDTH; x++) {
//                 Color tileColor;
                
//                 switch (map[y][x]) {
//                     case TILE_OBSTACLE:
//                         tileColor = RED;       // Obstacles
//                         break;
//                     case TILE_BOUNDARY:
//                         tileColor = DARKGRAY;  // Outer Wall boundaries
//                         break;
//                     case TILE_WALKABLE:
//                     default:
//                         tileColor = LIME;      // Open floor walkable path
//                         break;
//                 }

//                 DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, tileColor);
//                 // Subtle borders to visualize grid tiles
//                 DrawRectangleLines(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, Fade(BLACK, 0.15f));
//             }
//         }

//         // Draw Player PNG (Resized directly to the tile container sizing)
//         Vector2 screenPos = { playerGridPos.x * TILE_SIZE, playerGridPos.y * TILE_SIZE };
//         Rectangle sourceRec = { 0.0f, 0.0f, (float)playerSprite.width, (float)playerSprite.height };
//         Rectangle destRec = { screenPos.x, screenPos.y, TILE_SIZE, TILE_SIZE };
//         Vector2 origin = { 0.0f, 0.0f };

//         DrawTexturePro(playerSprite, sourceRec, destRec, origin, 0.0f, WHITE);

//         EndDrawing();
//     }

//     // Cleanup resources
//     UnloadTexture(playerSprite);
//     CloseWindow();

//     return 0;
// }
#include "raylib.h"
#include <math.h>

#define TILE_SIZE 40
#define MAP_WIDTH 20
#define MAP_HEIGHT 15
#define SCREEN_WIDTH (MAP_WIDTH * TILE_SIZE)
#define SCREEN_HEIGHT (MAP_HEIGHT * TILE_SIZE)

// Sprite sheet layout: 4 walk-cycle frames per row, 4 direction rows
#define SPRITE_COLUMNS 4
#define SPRITE_ROWS    4

// Tile Types
typedef enum {
    TILE_WALKABLE = 0,
    TILE_OBSTACLE,
    TILE_BOUNDARY
} TileType;

// Define a simple 20x15 map grid
int map[MAP_HEIGHT][MAP_WIDTH] = {
    {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,2},
    {2,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}
};

// ---------------------------------------------------------
// Check whether a given pixel-space rectangle (the player's
// hitbox) overlaps any non-walkable tile.
// ---------------------------------------------------------
bool IsRectWalkable(float px, float py, float size) {
    // Find which tile columns/rows the rectangle's four corners fall into
    int leftTile   = (int)(px / TILE_SIZE);
    int rightTile  = (int)((px + size - 1) / TILE_SIZE);
    int topTile    = (int)(py / TILE_SIZE);
    int bottomTile = (int)((py + size - 1) / TILE_SIZE);

    // Out-of-map safety check
    if (leftTile < 0 || topTile < 0 || rightTile >= MAP_WIDTH || bottomTile >= MAP_HEIGHT) {
        return false;
    }

    // Check every tile the rectangle overlaps (usually 1, up to 4 near tile edges)
    for (int ty = topTile; ty <= bottomTile; ty++) {
        for (int tx = leftTile; tx <= rightTile; tx++) {
            if (map[ty][tx] != TILE_WALKABLE) {
                return false;
            }
        }
    }
    return true;
}

int main(void) {
    // Initialize Window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Grid Map - Pixel Movement + Animated Sprite");
    SetTargetFPS(60);

    // ---------------------------------------------------------
    // Load Player Sprite Sheet
    // ---------------------------------------------------------
    Texture2D playerSprite = LoadTexture("player_sprites.png");

    // Frame size is the full sheet divided into a 4x4 grid
    const float frameWidth  = playerSprite.width  / (float)SPRITE_COLUMNS;
    const float frameHeight = playerSprite.height / (float)SPRITE_ROWS;

    // Which row = facing direction (sheet order: 0=down, 1=left, 2=right, 3=up)
    int facingRow = 0;

    // Which column = current walk-cycle frame, and animation timing
    int animFrame = 0;
    float animTimer = 0.0f;
    const float animSpeed = 0.12f; // seconds per frame - lower = faster animation

    // ---------------------------------------------------------
    // Player state (PIXEL space, not grid space)
    // ---------------------------------------------------------
    const float playerSize = (float)TILE_SIZE; // hitbox matches one tile
    Vector2 playerPos = { 9.0f * TILE_SIZE, 9.0f * TILE_SIZE }; // start tile (9,9) in pixels

    const float playerSpeed = 200.0f; // pixels per second

    // Main Game Loop
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        

        // --- Gather input direction ---
        Vector2 moveDir = { 0 };
        if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) moveDir.y -= 1.0f;
        if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) moveDir.y += 1.0f;
        if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) moveDir.x -= 1.0f;
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) moveDir.x += 1.0f;

        // Update facing direction based on currently pressed key
        // Priority order: down > left > right > up
        if (moveDir.y > 0)      facingRow = 0; // down
        else if (moveDir.x < 0) facingRow = 1; // left
        else if (moveDir.x > 0) facingRow = 2; // right
        else if (moveDir.y < 0) facingRow = 3; // up

        // Advance walk animation only while actually moving
        bool isMoving = (moveDir.x != 0.0f || moveDir.y != 0.0f);
        if (isMoving) {
            animTimer += deltaTime;
            if (animTimer >= animSpeed) {
                animTimer = 0.0f;
                animFrame = (animFrame + 1) % SPRITE_COLUMNS;
            }
        } else {
            animFrame = 0;  // reset to standing pose when idle
            animTimer = 0.0f;
        }

        // Normalize diagonal movement so it isn't faster than straight movement
        float len = sqrtf(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
        if (len > 0.0f) {
            moveDir.x /= len;
            moveDir.y /= len;
        }

        // --- Move + collide per axis (allows sliding along walls) ---
        float step = playerSpeed * deltaTime;

        float newX = playerPos.x + moveDir.x * step;
        if (IsRectWalkable(newX, playerPos.y, playerSize)) {
            playerPos.x = newX;
        }

        float newY = playerPos.y + moveDir.y * step;
        if (IsRectWalkable(playerPos.x, newY, playerSize)) {
            playerPos.y = newY;
        }

        // ---------------------------------------------------------
        // Render Graphics
        // ---------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw Map Grid Layout
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                Color tileColor;

                switch (map[y][x]) {
                    case TILE_OBSTACLE:
                        tileColor = RED;       // Obstacles
                        break;
                    case TILE_BOUNDARY:
                        tileColor = DARKGRAY;  // Outer Wall boundaries
                        break;
                    case TILE_WALKABLE:
                    default:
                        tileColor = LIME;      // Open floor walkable path
                        break;
                }

                DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, tileColor);
                DrawRectangleLines(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, Fade(BLACK, 0.15f));
            }
        }

        // Draw Player: pick the correct frame out of the sheet based on
        // direction (row) and walk cycle (column), then draw at pixel position
        Rectangle sourceRec = {
            animFrame * frameWidth,
            facingRow * frameHeight,
            frameWidth,
            frameHeight
        };
        Rectangle destRec = { playerPos.x, playerPos.y, playerSize, playerSize };
        Vector2 origin = { 0.0f, 0.0f };

        DrawTexturePro(playerSprite, sourceRec, destRec, origin, 0.0f, WHITE);

        EndDrawing();
    }

    // Cleanup resources
    UnloadTexture(playerSprite);
    CloseWindow();

    return 0;
}
