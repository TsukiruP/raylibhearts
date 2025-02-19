/* Core, Shapes, Textures, Text, Audio */

#include <string.h>
#include "raylib.h" 
#include "raymath.h"
#include "resource_dir.h"

#define COMMAND_HEIGHT 16
#define COMMAND_MARGIN 12
#define COMMAND_BOTTOM 218
#define NAME_MAX 16

#define FONT_1(text, x, y, color) DrawTextEx(font1, text, (Vector2){ x, y }, (float)font1.baseSize, -2, color)

Texture2D texCommandBase1;
Texture2D texCommandBase2;
Texture2D texCommandBase3;
Texture2D texCommandMagic;
Texture2D texGaugeHP;
Texture2D texBarHP;
Texture2D texGaugeMP;
Texture2D texBarMP;
Texture2D texGaugeSora;

Font font1;

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
        } Base;

        struct Magic
        {
            int level;
            char name_ra[NAME_MAX];
            char name_ga[NAME_MAX];
        } Magic;
    };

} Command;

typedef struct CommandMenu
{
    int cursor;
    int count;
    Command *arr;
} CommandMenu;

Command commandBase[4];
CommandMenu menuBase;

Command commandMagic[7];
CommandMenu menuMagic;

Command SetCommandBase(char name[]);
CommandMenu SetCommandMenu(int count, Command mnu[]);
void InitCommands();
char *GetCommandName(Command *cmd);
void DrawCommandMenu(CommandMenu *mnu);
void DrawGaugePlayer(void);

int main(void)
{
    // Window initialization:
    const int screenWidth = 420;
    const int screenHeight = 240;
    
    InitWindow(screenWidth, screenHeight, "Command Menu");
    SearchAndSetResourceDir("resources");
    
    // Image initialization:
    Image imCommandBase1 = LoadImage("command/CommandBase1.png");
    Image imCommandBase2 = LoadImage("command/CommandBase2.png");
    Image imCommandBase3 = LoadImage("command/CommandBase3.png");
    Image imCommandMagic = LoadImage("command/CommandMagic.png");
    Image imGaugeHP = LoadImage("gauge/GaugeHP.png");
    Image imBarHP = LoadImage("gauge/BarHP.png");
    Image imGaugeMP = LoadImage("gauge/GaugeMP.png");
    Image imBarMP = LoadImage("gauge/BarMP.png");
    Image imGaugeSora = LoadImage("gauge/GaugeSora.png");
    
    // Texture initialization:
    texCommandBase1 = LoadTextureFromImage(imCommandBase1);
    texCommandBase2 = LoadTextureFromImage(imCommandBase2);
    texCommandBase3 = LoadTextureFromImage(imCommandBase3);
    texCommandMagic = LoadTextureFromImage(imCommandMagic);
    texGaugeHP = LoadTextureFromImage(imGaugeHP);
    texBarHP = LoadTextureFromImage(imBarHP);
    texGaugeMP = LoadTextureFromImage(imGaugeMP);
    texBarMP = LoadTextureFromImage(imBarMP);
    texGaugeSora = LoadTextureFromImage(imGaugeSora);
    
    // Font initialization:
    font1 = LoadFont("font/Font1.png");
    
    // Command initialization:
    InitCommands();
    
    // Target FPS:
    SetTargetFPS(60);
    
    // Game loop:
    while (!WindowShouldClose())
    {
            /*
            if (IsKeyPressed(KEY_DOWN))
            {
                mnuCommand.active += 1;
            }

            if (IsKeyPressed(KEY_UP))
            {
                mnuCommand.active -= 1;
            }

            mnuCommand.active = Wrap(mnuCommand.active, 0, 4);
            */

            // Draw:
            BeginDrawing();
                ClearBackground(VIOLET);
                DrawCommandMenu(&menuBase);
                DrawGaugePlayer();
            EndDrawing();
    }
    
    CloseWindow();
    
    return 0;
}

Command SetCommandBase(char name[])
{
    Command commandTemp;

    commandTemp.type = BASE;
    strcpy(commandTemp.name, name);

    return commandTemp;
}

Command SetCommandMagic(char name[], char name_ra[], char name_ga[])
{
    Command commandTemp;

    commandTemp.type = MAGIC;
    commandTemp.Magic.level = 0;
    strcpy(commandTemp.name, name);
    strcpy(commandTemp.Magic.name_ra, name_ra);
    strcpy(commandTemp.Magic.name_ga, name_ga);

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
    commandBase[0] = SetCommandBase("Attack");
    commandBase[1] = SetCommandBase("Magic");
    commandBase[2] = SetCommandBase("Items");
    commandBase[3] = SetCommandBase("Limit");
    menuBase = SetCommandMenu(4, commandBase);

    commandMagic[0] = SetCommandMagic("Fire", "Fira", "Firaga");
    commandMagic[1] = SetCommandMagic("Fire", "Fira", "Firaga");
    commandMagic[2] = SetCommandMagic("Fire", "Fira", "Firaga");
    commandMagic[3] = SetCommandMagic("Fire", "Fira", "Firaga");
    commandMagic[4] = SetCommandMagic("Fire", "Fira", "Firaga");
    commandMagic[5] = SetCommandMagic("Fire", "Fira", "Firaga");
    commandMagic[6] = SetCommandMagic("Fire", "Fira", "Firaga");
    menuMagic = SetCommandMenu(7, commandMagic);
}

char *GetCommandName(Command *cmd)
{
    return cmd->name;
}

void DrawCommandMenu(CommandMenu *mnu)
{
    int i;
    int cursor = mnu->cursor;
    int count = mnu->count;
    int indent = 0;

    Command *arr = mnu->arr;
    Vector2 position = { COMMAND_MARGIN + indent, COMMAND_BOTTOM};
    Texture2D tex = texCommandBase1;

    // Commands:
    for (i = count - 1; i > -1; i--)
    {
        if (arr[i].type == MAGIC && arr[i].Magic.level == 0) continue;

        int texture_y = (cursor == i) ? 1 : ((i == count - 1) ? 3 : 2);
        int cursor_x = (cursor == i) ? 6 : 0;

        DrawTextureRec(tex, (Rectangle){ 0, COMMAND_HEIGHT * texture_y, tex.width, COMMAND_HEIGHT }, (Vector2){ position.x + cursor_x, position.y }, WHITE);
        FONT_1(GetCommandName(&arr[i]), position.x + 10 + cursor_x, position.y + 4, WHITE);
        position.y -= COMMAND_HEIGHT;
    }

    // Header:
    DrawTextureRec(tex, (Rectangle){ 0, 0, tex.width, COMMAND_HEIGHT }, (Vector2){ position.x, position.y }, WHITE);
}

void DrawGaugePlayer(void)
{
    const Vector2 position = { 420 - COMMAND_MARGIN, COMMAND_BOTTOM };
    const int healthWidth = 62;
    const int healthHeight = 5;
    const int magicWidth = 75;

    DrawTexture(texGaugeHP, position.x - texGaugeHP.width, position.y, WHITE);
    DrawTexturePro(texBarHP, (Rectangle){ 0, 0, 1, texBarHP.height }, (Rectangle){ position.x - 15, position.y + 1, healthWidth, healthHeight }, (Vector2){ healthWidth, 0 }, 0, WHITE);
    DrawTexture(texGaugeSora, position.x - texGaugeSora.width - 2, position.y - texGaugeSora.height, WHITE);
    DrawTexture(texGaugeMP, position.x - texGaugeMP.width - 47, position.y - texGaugeMP.height - 1, WHITE);
    DrawTexturePro(texBarMP, (Rectangle){ 0, 0, 1, texBarMP.height }, (Rectangle){ position.x - 63, position.y - texGaugeHP.height, magicWidth, healthHeight }, (Vector2){ magicWidth, 0 }, 0, WHITE);
}
