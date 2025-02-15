/* Core, Shapes, Textures, Text, Audio */

#include <string.h>
#include "raylib.h" 
#include "raymath.h"
#include "resource_dir.h"

#define ACTION_HEIGHT 16
#define ACTION_MARGIN 12
#define ACTION_BOTTOM 234
#define ACTION_COMMANDS 4

#define FONT_1(text, x, y, color) DrawTextEx(font1, text, (Vector2){ x, y }, (float)font1.baseSize, -2, color)

Texture2D texCommandMenu1;
Texture2D texCommandMenu2;
Texture2D texCommandMenu3;
Texture2D texMagicMenu;

Font font1;

typedef struct Action
{
    char name[16];
    int type;
} Action;

typedef struct ActionMenu
{
    int active;
    Action *ptr;
    int actionCount;
    bool submenu;
    
} ActionMenu;

enum COMMANDS
{
    ATTACK,
    MAGIC,
    ITEMS,
    LIMIT,
};

Action actAttack;
Action actMagic;
Action actItems;
Action actLimit;
Action arrCommands[LIMIT + 1];
ActionMenu mnuCommand;

enum MAGIC
{
    FIRE,
    BLIZZARD,
    THUNDER,
    CURE,
    GRAVITY,
    STOP,
    AERO,
};

Action actFire;
Action actBlizzard;
Action actThunder;
Action actCure;
Action actGravity;
Action actStop;
Action actAero;
Action arrMagic[AERO + 1];
ActionMenu mnuMagic;

Action InitCommand();
void InitActions();


void DrawCommandMenu(Texture2D tex);
void DrawMagicMenu();

int main(void)
{
    // Window initialization:
    const int screenWidth = 420;
    const int screenHeight = 240;
    
    InitWindow(screenWidth, screenHeight, "Command Menu");
    SearchAndSetResourceDir("resources");
    
    // Image initialization:
    Image imCommandMenu1 = LoadImage("command/CommandMenu1.png");
    Image imCommandMenu2 = LoadImage("command/CommandMenu2.png");
    Image imCommandMenu3 = LoadImage("command/CommandMenu3.png");
    Image imMagicMenu = LoadImage("command/MagicMenu.png");
    
    // Texture initialization:
    texCommandMenu1 = LoadTextureFromImage(imCommandMenu1);
    texCommandMenu2 = LoadTextureFromImage(imCommandMenu2);
    texCommandMenu3 = LoadTextureFromImage(imCommandMenu3);
    texMagicMenu = LoadTextureFromImage(imMagicMenu);
    
    // Font initialization:
    font1 = LoadFont("font/Font1.png");
    
    // Command menu:
    InitActions();
    
    // Target FPS:
    SetTargetFPS(60);
    
    // Game loop:
    while (!WindowShouldClose())
    {
            if (IsKeyPressed(KEY_DOWN))
            {
                mnuCommand.active += 1;
            }
            
            if (IsKeyPressed(KEY_UP))
            {
                mnuCommand.active -= 1;
            }
            
            mnuCommand.active = Wrap(mnuCommand.active, 0, 4);
            
            // Draw:
            BeginDrawing();
                ClearBackground(VIOLET);
                
                DrawCommandMenu(texCommandMenu1);
                DrawMagicMenu();
            EndDrawing();
    }
    
    CloseWindow();
    
    return 0;
}

Action InitCommand(Action *arr, int id, char name[])
{
    Action temp;
    
    strcpy(temp.name, name);
    arr[id] = temp;
    
    return temp;
}

ActionMenu InitActionMenu(Action arr[], int actionCount, bool submenu)
{
    ActionMenu temp;
    
    temp.active = 0;
    temp.ptr = arr;
    temp.actionCount = actionCount;
    temp.submenu = submenu;
    
    return temp;
}

void InitActions()
{
    // Commands:
    actAttack = InitCommand(arrCommands, ATTACK, "Attack");
    actMagic = InitCommand(arrCommands, MAGIC, "Magic");
    actItems = InitCommand(arrCommands, ITEMS, "Items");
    actLimit = InitCommand(arrCommands, LIMIT, "Limits");
    mnuCommand = InitActionMenu(arrCommands, LIMIT + 1, false);
    
    // Magic:
    actFire = InitCommand(arrMagic, FIRE, "Fire");
    actBlizzard = InitCommand(arrMagic, BLIZZARD, "Blizzard");
    actThunder = InitCommand(arrMagic, THUNDER, "-");
    actCure = InitCommand(arrMagic, CURE, "-");
    actGravity = InitCommand(arrMagic, GRAVITY, "-");
    actStop = InitCommand(arrMagic, STOP, "-");
    actAero = InitCommand(arrMagic, AERO, "-");
    mnuMagic = InitActionMenu(arrMagic, AERO + 1, true);
}

void DrawActionMenu(Texture2D tex, ActionMenu menu)
{
    int i;
    Action *arr = menu.ptr;
    int actionCount = menu.actionCount;
    int active = menu.active;
    int indent = (menu.submenu == true) ? 10 : 0;
    
    Vector2 posAction = { ACTION_MARGIN + indent, ACTION_BOTTOM - (actionCount * ACTION_HEIGHT) };
    
    // Header:
    DrawTextureRec(tex, (Rectangle){ 0, 0, tex.width, ACTION_HEIGHT }, (Vector2){ posAction.x, posAction.y - ACTION_HEIGHT }, WHITE);
    
    // Actions:
    for (i = 0; i < actionCount; i++)
    {
        int source_y, active_x;
        
        source_y = (active == i) ? 1 : ((i == actionCount - 1) ? 3 : 2);
        active_x = (active == i) ? 6 : 0;
        
        DrawTextureRec(tex, (Rectangle){ 0, ACTION_HEIGHT * source_y, tex.width, ACTION_HEIGHT }, (Vector2){ posAction.x + active_x, posAction.y }, WHITE);
        
        // Action name:
        FONT_1(arr[i].name, posAction.x + 10 + active_x, posAction.y + 4, WHITE);
        
        posAction.y += ACTION_HEIGHT;
    }
}

void DrawCommandMenu(Texture2D tex)
{
    DrawActionMenu(tex, mnuCommand);
}

void DrawMagicMenu()
{
    DrawActionMenu(texMagicMenu, mnuMagic);
}
