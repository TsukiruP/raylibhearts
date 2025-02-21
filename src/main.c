/* Core, Shapes, Textures, Text, Audio */

#include <stdlib.h>
#include <string.h>
#include "raylib.h" 
#include "raymath.h"
#include "resource_dir.h"

#define COMMAND_HEIGHT 16
#define COMMAND_MARGIN 12
#define COMMAND_BOTTOM 218
#define COMMAND_INDENT 10
#define COMMAND_BASE_MAX 4
#define COMMAND_MAGIC_MAX 7
#define COMMAND_ICON_WIDTH 14
#define NAME_MAX 16
#define HEALTH_POINTS_MAX 20
#define MAGIC_POINTS_MAX 100

#define FONT_1(text, x, y, color) DrawTextEx(font1, text, (Vector2){ x, y }, (float)font1.baseSize, -2, color)
#define FONT_2(text, x, y, color) DrawTextEx(font2, text, (Vector2){ x, y }, (float)font1.baseSize, -2, color)

int healthPoints;
int magicPoints;

Texture2D texCommandBase1;
Texture2D texCommandBase2;
Texture2D texCommandBase3;
Texture2D texCommandMagic;
Texture2D texCommandIcon;
Texture2D texGaugeHP;
Texture2D texBarHP;
Texture2D texGaugeMP;
Texture2D texBarMP;
Texture2D texGaugeSora;

Font font1;
Font font2;

typedef struct Command
{
    enum
    {
        BASE,
        MAGIC,
        ITEM,
        LIMIT
    } type;

    char name[NAME_MAX];

    union
    {
        struct Base
        {
            bool unlock;
            void (*action)();
        } Base;

        struct Magic
        {
            int level;
            int cost;
            char namera[NAME_MAX];
            char namega[NAME_MAX];
        } Magic;
    };

} Command;

typedef struct CommandMenu
{
    int cursor;
    int count;
    Command *arr;
} CommandMenu;

Command commandBase[COMMAND_BASE_MAX];
Command commandMagic[COMMAND_MAGIC_MAX];
CommandMenu menuBase;
CommandMenu menuMagic;
CommandMenu *menuSub;

Command SetCommandBase(char name[], void (*action)());
CommandMenu SetCommandMenu(int count, Command mnu[]);
void InitCommands();
void InitMenuMagic();
bool CheckCommand(Command *cmd);
char *GetCommandName(Command *cmd);
void DrawCommandMenu(CommandMenu *mnu, int indent);
void DrawGaugePlayer(void);

int main(void)
{
    // Window initialization:
    const int screenWidth = 420;
    const int screenHeight = 240;
    
    InitWindow(screenWidth, screenHeight, "Command Menu");
    SearchAndSetResourceDir("resources");
    
    // Texture initialization:
    texCommandBase1 = LoadTexture("command/CommandBase1.png");
    texCommandBase2 = LoadTexture("command/CommandBase2.png");
    texCommandBase3 = LoadTexture("command/CommandBase3.png");
    texCommandMagic = LoadTexture("command/CommandMagic.png");
    texCommandIcon = LoadTexture("command/CommandIcon.png");
    texGaugeHP = LoadTexture("gauge/GaugeHP.png");
    texBarHP = LoadTexture("gauge/BarHP.png");
    texGaugeMP = LoadTexture("gauge/GaugeMP.png");
    texBarMP = LoadTexture("gauge/BarMP.png");
    texGaugeSora = LoadTexture("gauge/GaugeSora.png");
    
    // Font initialization:
    font1 = LoadFont("font/Font1.png");
    font2 = LoadFont("font/Font2.png");
    
    // Command initialization:
    InitCommands();
    InitMenuMagic();
    healthPoints = 10;
    magicPoints = 25;
    menuSub = NULL;
    
    // Target FPS:
    SetTargetFPS(60);
    
    // Game loop:
    while (!WindowShouldClose())
    {
            CommandMenu *commandMenuPointer;

            if (menuSub == NULL) commandMenuPointer = &menuBase;
            else commandMenuPointer = menuSub;

            if (IsKeyPressed(KEY_DOWN)) commandMenuPointer->cursor += 1;
            if (IsKeyPressed(KEY_UP)) commandMenuPointer->cursor -= 1;

            commandMenuPointer->cursor = Wrap(commandMenuPointer->cursor, 0, commandMenuPointer->count);
            if (IsKeyPressed(KEY_ENTER)) CheckCommand(&commandMenuPointer->arr[commandMenuPointer->cursor]);

            if (menuSub != NULL && IsKeyPressed(KEY_BACKSPACE))
            {
                free(menuSub->arr);
                menuBase.cursor = 0;
                menuSub->cursor = 0;
                menuSub->arr = NULL;
                menuSub = NULL;
            }

            // Draw:
            BeginDrawing();
                ClearBackground(VIOLET);
                DrawCommandMenu(&menuBase, 0);
                if (menuSub != NULL) DrawCommandMenu(menuSub, 1);
                DrawGaugePlayer();
            EndDrawing();
    }

    UnloadTexture(texCommandBase1);
    UnloadTexture(texCommandBase2);
    UnloadTexture(texCommandBase3);
    UnloadTexture(texCommandMagic);
    UnloadTexture(texCommandIcon);
    UnloadTexture(texGaugeHP);
    UnloadTexture(texBarHP);
    UnloadTexture(texGaugeMP);
    UnloadTexture(texBarMP);
    UnloadTexture(texGaugeSora);
    UnloadFont(font1);
    UnloadFont(font2);
    CloseWindow();
    return 0;
}

Command SetCommandBase(char name[], void (*action)())
{
    Command commandTemp;

    commandTemp.type = BASE;
    commandTemp.Base.unlock = true;
    commandTemp.Base.action = action;
    strcpy(commandTemp.name, name);
    return commandTemp;
}

Command SetCommandMagic(char name[], char namera[], char namega[], int cost)
{
    Command commandTemp;

    commandTemp.type = MAGIC;
    commandTemp.Magic.level = 0;
    commandTemp.Magic.cost = cost;
    strcpy(commandTemp.name, name);
    strcpy(commandTemp.Magic.namera, namera);
    strcpy(commandTemp.Magic.namega, namega);
    return commandTemp;
}

CommandMenu SetCommandMenu(int count, Command mnu[])
{
    CommandMenu menuTemp;

    menuTemp.cursor = 0;
    menuTemp.count = count;
    menuTemp.arr = mnu;
    return menuTemp;
}

void InitCommands()
{
    commandBase[0] = SetCommandBase("Attack", NULL);
    commandBase[1] = SetCommandBase("Magic", &InitMenuMagic);
    commandBase[2] = SetCommandBase("Items", NULL);
    commandBase[3] = SetCommandBase("Limit", NULL);
    menuBase = SetCommandMenu(COMMAND_BASE_MAX, commandBase);

    commandMagic[0] = SetCommandMagic("Fire", "Fira", "Firaga", 10);
    commandMagic[1] = SetCommandMagic("Blizzard", "Blizzara", "Blizzaga", 12);
    commandMagic[2] = SetCommandMagic("Thunder", "Thundara", "Thundaga", 14);
    commandMagic[3] = SetCommandMagic("Cure", "Cura", "Curaga", -1);
    commandMagic[4] = SetCommandMagic("Aero", "Aerora", "Aeroga", 13);
    commandMagic[5] = SetCommandMagic("Gravity", "Gravira", "Graviga", 15);
    commandMagic[6] = SetCommandMagic("Stop", "Stopra", "Stopga", 20);
    menuMagic = SetCommandMenu(0, NULL);

    commandMagic[0].Magic.level = 3;
    commandMagic[1].Magic.level = 1;
    commandMagic[2].Magic.level = 1;
    commandMagic[3].Magic.level = 2;
    commandMagic[5].Magic.level = 1;
}

void InitMenuMagic()
{
    int magicCount = 0;

    for (int i = 0; i < COMMAND_MAGIC_MAX; i++)
    {
        if (commandMagic[i].Magic.level > 0)
        {
            menuMagic.arr = (Command *)realloc(menuMagic.arr, (magicCount + 1) * sizeof(Command));
            menuMagic.arr[magicCount] = commandMagic[i];
            magicCount++;
        }
    }

    if (magicCount)
    {
        menuMagic.count = magicCount;
        menuSub = &menuMagic;
    }
}

bool CheckCommand(Command *cmd)
{
    switch (cmd->type)
    {
        case BASE:
            if (cmd->Base.unlock && cmd->Base.action != NULL)
            {
                cmd->Base.action();
            }
            break;

        case MAGIC:
            break;
    }
}

char *GetCommandName(Command *cmd)
{
    if (cmd->type == MAGIC)
    {
        switch (cmd->Magic.level)
        {
            case 2:
                return cmd->Magic.namera;

            case 3:
                return cmd->Magic.namega;
        }
    }

    return cmd->name;
}

void DrawCommandMenu(CommandMenu *mnu, int indent)
{
    int i;
    int cursor = mnu->cursor;
    int count = mnu->count;
    Command *arr = mnu->arr;
    Vector2 commandPosition = { COMMAND_MARGIN + COMMAND_INDENT * indent, COMMAND_BOTTOM};
    Texture2D tex = texCommandBase1;

    if (mnu == &menuMagic) tex = texCommandMagic;

    // Commands:
    for (i = count - 1; i > -1; i--)
    {
        int commandSource = (cursor == i) ? 1 : ((i == count - 1) ? 3 : 2);
        int cursorOffset = (cursor == i) ? 6 : 0;

        DrawTextureRec(tex, (Rectangle){ 0, COMMAND_HEIGHT * commandSource, tex.width, COMMAND_HEIGHT }, (Vector2){ commandPosition.x + cursorOffset, commandPosition.y }, WHITE);
        FONT_1(GetCommandName(&arr[i]), commandPosition.x + COMMAND_INDENT + cursorOffset, commandPosition.y + 4, WHITE);
        commandPosition.y -= COMMAND_HEIGHT;
    }

    // Header:
    DrawTextureRec(tex, (Rectangle){ 0, 0, tex.width, COMMAND_HEIGHT }, (Vector2){ commandPosition.x, commandPosition.y }, WHITE);

    // Icon:
    Vector2 iconPosition = { commandPosition.x + tex.width - 11 - ((mnu == &menuBase) ? 12 : 0), commandPosition.y + COMMAND_HEIGHT + 1 };
    int iconSource = COMMAND_ICON_WIDTH * ((arr[cursor].type == BASE) ? cursor : arr[cursor].type);

    DrawTextureRec(texCommandIcon, (Rectangle){ iconSource, 0, COMMAND_ICON_WIDTH, texCommandIcon.height }, (Vector2){ iconPosition.x, iconPosition.y + 16 * cursor }, WHITE);
}

void DrawGaugePlayer(void)
{
    const Vector2 position = { 420 - COMMAND_MARGIN, COMMAND_BOTTOM };
    const int healthWidth = 97 * ((float)healthPoints / HEALTH_POINTS_MAX);
    const int healthHeight = 5;
    const int magicWidth = 50 * ((float)magicPoints / MAGIC_POINTS_MAX);

    DrawTexture(texGaugeSora, position.x - texGaugeSora.width - 2, position.y - texGaugeSora.height, WHITE);

    DrawTexture(texGaugeHP, position.x - texGaugeHP.width, position.y, WHITE);
    DrawTexturePro(texBarHP, (Rectangle){ 0, 0, 1, texBarHP.height }, (Rectangle){ position.x - 15, position.y + 1, healthWidth, healthHeight }, (Vector2){ healthWidth, 0 }, 0, WHITE);

    DrawTextureRec(texGaugeMP, (Rectangle){ 0, 0, 67, texGaugeMP.height }, (Vector2){ position.x - 114, position.y - texGaugeMP.height - 1 }, WHITE);
    DrawTexturePro(texBarMP, (Rectangle){ 0, 0, 1, texBarMP.height }, (Rectangle){ position.x - 62, position.y - texGaugeHP.height, magicWidth, healthHeight }, (Vector2){ magicWidth, 0 }, 0, WHITE);
}
