#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define MAX_OBSTACLES 1
#define OBSTACLE_SPEED 2
#define OBSTACLE_DELAY 500 // Delay in milliseconds between obstacles

int groundY = 400; // Y-coordinate of the ground
int playerX = 100; // Initial X-coordinate of the player
int playerY = groundY; // Y-coordinate of the player
int jumpHeight = 115; // Jump height
int jumpFlag = 0; // Flag to indicate whether the player is jumping
int jumpCounter = 0; // Counter to control the jump animation
int score = 0; // Initialize the score
float distance = 0.0; // Initialize the distance traveled with a floating-point number
float maxDistance = 0.0; // Initialize the maximum distance
float distanceIncrement = 0.5; // Adjust this value to control the increase in distance
int gameOver = 0; // Game over state

struct Obstacle {
    int x;
    int y;
};

Obstacle obstacles[MAX_OBSTACLES];
int obstacleWidth = 10;
int obstacleHeight = 35;

void drawDino(int x, int y) {
    // Draw a pixelated dino in white color
    setcolor(WHITE);

    // Head
    rectangle(x + 5, y - 40, x + 15, y - 30);
    floodfill(x + 10, y - 35, WHITE); // Fill head with white color

    // Body
    rectangle(x, y - 30, x + 20, y - 20);
    floodfill(x + 10, y - 25, WHITE); // Fill body with white color

    // Legs
    rectangle(x + 5, y - 20, x + 10, y - 10);
    rectangle(x + 10, y - 20, x + 15, y - 10);
    floodfill(x + 7, y - 15, WHITE); // Fill legs with white color

    // Tail
    rectangle(x - 5, y - 20, x, y - 15);
    floodfill(x - 2, y - 17, WHITE); // Fill tail with white color
}

void drawObstacle(int x, int y) {
    // Draw the obstacles in gray color
    setcolor(GREEN);
    rectangle(x, y, x + obstacleWidth, y + obstacleHeight);
    floodfill(x + 1, y + 1, GREEN);
}

void drawGround() {
    setcolor(GREEN);
    line(0, groundY, getmaxx(), groundY);
}

void jump() {
    if (jumpCounter < jumpHeight) {
        playerY -= 5;
        jumpCounter += 5;
    } else if (jumpCounter >= jumpHeight && jumpCounter < 2 * jumpHeight) {
        // Increase the delay here to slow down the jump animation
        delay(5); // Adjust the delay time as needed
        playerY += 5;
        jumpCounter += 5;
    } else {
        jumpFlag = 0;
        jumpCounter = 0;
    }
}

void updateObstacles() {
    if (gameOver) {
        return;
    }

    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles[i].x < -obstacleWidth) {
            obstacles[i].x = getmaxx() + i * 200 + rand() % 100; // Add random distance between obstacles
            obstacles[i].y = groundY - obstacleHeight;
        } else {
            obstacles[i].x -= OBSTACLE_SPEED;
            if (obstacles[i].x < playerX + 20 && obstacles[i].x + obstacleWidth > playerX &&
                playerY >= groundY - obstacleHeight) {
                // The player has hit an obstacle, set the game over state
                gameOver = 1;
            } else if (obstacles[i].x + obstacleWidth < playerX && !jumpFlag) {
                // The player has successfully jumped over the obstacle, increase the score
                score += 5;
            }
        }
    }
    distanceIncrement = 0.5; // Adjusted increase in distance
    distance += distanceIncrement; // Increase the distance
    if (distance > maxDistance) {
        maxDistance = distance; // Update the maximum distance
    }
}

void drawScore() {
    char scoreText[20];
    sprintf(scoreText, "Score: %d", score);
    setcolor(WHITE);
    outtextxy(getmaxx() - 100, 10, scoreText);

    char distanceText[20];
    sprintf(distanceText, "Distance: %.0f", distance); // Display distance with one decimal place
    setcolor(WHITE);
    outtextxy(10, 10, distanceText);

    if (gameOver && !jumpFlag) {
        setcolor(RED);
        outtextxy(getmaxx() / 2 - 40, getmaxy() / 2 - 10, "Game Over...Press R to Restart");
    }

    if (!gameOver) {
        setcolor(WHITE);
        outtextxy(10, 50, "Press Q to Quit");
    }
}

void resetGame() {
    playerY = groundY;
    jumpFlag = 0;
    jumpCounter = 0;
    score = 0;
    distance = 0.0;
    maxDistance = 0.0; // Reset the maximum distance
    gameOver = 0;

    for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i].x = getmaxx() + i * 200 + rand() % 100;
        obstacles[i].y = groundY - obstacleHeight;
    }
}

void startScreen() {
    setcolor(WHITE);
    outtextxy(getmaxx() / 2 - 100, getmaxy() / 2 - 10, "Press 'S' to Start Game");
    char ch = getch();
    if (ch == 's' || ch == 'S') {
        cleardevice();
    } else {
        startScreen(); // Wait for 'S' key press to start the game
    }
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\Turboc3\\BGI");
    srand(time(NULL));

    startScreen(); // Display the start screen

    resetGame(); // Initialize the game

    while (!gameOver) {
        while (!kbhit()) {
            updateObstacles();
            if (jumpFlag) {
                jump();
            }
            cleardevice();
            drawGround();
            drawDino(playerX, playerY);
            for (int j = 0; j < MAX_OBSTACLES; j++) {
                drawObstacle(obstacles[j].x, obstacles[j].y);
            }
            drawScore();
            delay(10);
        }

        char ch = getch();
        if (ch == ' ') {
            if (!jumpFlag) {
                jumpFlag = 1;
            }
        } else if (ch == 'q' || ch == 'Q') {
            closegraph();
            return 0;
        } else if (ch == 'r' || ch == 'R') {
            resetGame(); // Restart the game
        }
    }

    while (!kbhit()) {
        delay(10);
    }

    char ch = getch();
    if (ch == 'q' || ch == 'Q') {
        closegraph();
        return 0;
    }

    closegraph();
    return 0;
}
