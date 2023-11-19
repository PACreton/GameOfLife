#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <raylib.h>

#define nrows 40
#define ncols 40
#define UPDATE_FREQUENCY 15 // Mettez à jour la grille tous les 10 frames

typedef unsigned int uint;

const int screenWidth = 1000;
const int screenHeight = 1000;

const int cellWidth = screenWidth / ncols;
const int cellHeight = screenHeight / nrows;

struct cell_t
{
    unsigned int i;
    unsigned int j;
    unsigned int neighbors;
    bool alive;
};

typedef struct cell_t cell_t;

void checkNeighbors(cell_t **cell, unsigned int i, unsigned int j);
void drawGrid(void); // Different from DrawGrid(), which is a function from raylib
void drawCell(cell_t cell);
cell_t **updateGrid(cell_t **cell);
bool indexIsValid(unsigned int i, unsigned int j);

int framesCounter = 0;

int main(int argc, char **argv)
{

    bool start_simulation = false;

    cell_t **cellGrid = malloc(nrows * sizeof(cell_t *));
    for (unsigned int i = 0; i < nrows; i++)
    {
        cellGrid[i] = malloc(ncols * sizeof(cell_t));
        for (unsigned int j = 0; j < ncols; j++)
        {
            cellGrid[i][j].i = i;
            cellGrid[i][j].j = j;
            cellGrid[i][j].neighbors = 0;
            cellGrid[i][j].alive = false;
        }
    }

    InitWindow(screenWidth, screenHeight, "Conway's Game of Life");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {

        framesCounter++;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mPos = GetMousePosition();
            unsigned int indexI = mPos.x / cellWidth;
            unsigned int indexJ = mPos.y / cellHeight;

            if (indexIsValid(indexI, indexJ))
            {
                cellGrid[indexI][indexJ].alive = true;
            }
        }

        if (IsKeyPressed(KEY_SPACE))
        {
            start_simulation = !start_simulation;
            printf("Simulation has %s\n", (start_simulation) ? "started" : "stopped");
        }

        if (framesCounter >= UPDATE_FREQUENCY)
        {
            framesCounter = 0;
        }

        if (start_simulation && framesCounter % UPDATE_FREQUENCY == 0)
        {
            for (unsigned int i = 0; i < nrows; i++)
            {
                for (unsigned int j = 0; j < ncols; j++)
                {
                    checkNeighbors(cellGrid, i, j);
                }
            }

            cellGrid = updateGrid(cellGrid);
        }

        BeginDrawing();

        ClearBackground(BLACK);
        drawGrid();

        for (unsigned int i = 0; i < nrows; i++)
        {
            for (unsigned int j = 0; j < ncols; j++)
            {
                drawCell(cellGrid[i][j]);
            }
        }

        EndDrawing();
    }

    CloseWindow();

    for (unsigned int i = 0; i < nrows; i++)
    {
        free(cellGrid[i]);
    }

    free(cellGrid);
    return EXIT_SUCCESS;
}

bool indexIsValid(unsigned int i, unsigned int j)
{
    return (i >= 0 && i < nrows) && (j >= 0 && j < ncols);
}

void drawGrid(void)
{
    Vector2 startPos;
    startPos.x = 0;
    startPos.y = 0;

    Vector2 endPos;
    endPos.x = screenWidth;
    endPos.y = screenHeight;

    // Draw rows line
    for (unsigned int x = cellWidth; x < screenWidth; x = x + cellWidth)
    {
        startPos.y = x;
        endPos.y = x;
        DrawLineEx(startPos, endPos, 5.0, RAYWHITE);
    }

    startPos.y = 0;
    endPos.y = screenHeight;

    // Draw columns line
    for (unsigned int y = cellHeight; y < screenHeight; y = y + cellHeight)
    {
        startPos.x = y;
        endPos.x = y;
        DrawLineEx(startPos, endPos, 5.0, RAYWHITE);
    }
}

void drawCell(cell_t cell)
{
    if (cell.alive)
    {
        DrawRectangle(cell.i * cellWidth, cell.j * cellHeight, cellWidth, cellHeight, WHITE);
    }
    else if (!cell.alive)
    {
        DrawRectangle(cell.i * cellWidth, cell.j * cellHeight, cellWidth - 3, cellHeight - 3, BLACK);
    }
}

void checkNeighbors(cell_t **cell, unsigned int i, unsigned int j)
{
    int coordinates[8][2] = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};

    cell[i][j].neighbors = 0;
    for (unsigned int indexI = 0; indexI < 8; indexI++)
    {
        int checkPosI = i + coordinates[indexI][0];
        int checkPosJ = j + coordinates[indexI][1];
        if (indexIsValid(checkPosI, checkPosJ))
        {
            if (cell[checkPosI][checkPosJ].alive)
            {
                cell[i][j].neighbors++;
            }
        }
    }
}

cell_t **updateGrid(cell_t **cell)
{
    cell_t **newGrid = malloc(nrows * sizeof(cell_t *));
    for (unsigned int i = 0; i < nrows; i++)
    {
        newGrid[i] = malloc(ncols * sizeof(cell_t));
        for (unsigned int j = 0; j < ncols; j++)
        {
            newGrid[i][j].i = cell[i][j].i;
            newGrid[i][j].j = cell[i][j].j;
            newGrid[i][j].neighbors = cell[i][j].neighbors;
            newGrid[i][j].alive = cell[i][j].alive;
        }
    }

    for (unsigned int i = 0; i < nrows; i++)
    {
        for (unsigned int j = 0; j < ncols; j++)
        {
            if (cell[i][j].alive == false && cell[i][j].neighbors == 3)
            {
                newGrid[i][j].alive = true;
            }
            else if (cell[i][j].alive && (cell[i][j].neighbors < 2 || cell[i][j].neighbors > 3))
            {
                newGrid[i][j].alive = false;
            }
        }
    }

    for (unsigned int i = 0; i < nrows; i++)
    {
        for (unsigned int j = 0; j < ncols; j++)
        {
            cell[i][j].i = newGrid[i][j].i;
            cell[i][j].j = newGrid[i][j].j;
            cell[i][j].neighbors = newGrid[i][j].neighbors;
            cell[i][j].alive = newGrid[i][j].alive;
        }
    }

    for (unsigned int i = 0; i < nrows; i++)
    {
        free(newGrid[i]);
    }
    free(newGrid);

    return cell;
}