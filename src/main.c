/* Core, Shapes, Textures, Text, Audio */

#include <string.h>
#include "raylib.h" 
#include "raymath.h"
#include "resource_dir.h"

#define COMMAND_HEIGHT 16
#define COMMAND_MARGIN 12
#define COMMAND_BOTTOM 218
#define COMMAND_INDENT 10
#define NAME_MAX 16

#define FONT_1(text, x, y, color) DrawTextEx(font1, text, (Vector2){ x, y }, (float)font1.baseSize, -2, color)

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

Command commandBase[4];
CommandMenu menuBase;

Command commandMagic[7];
CommandMenu menuMagic;

Command SetCommandBase(char name[]);
CommandMenu SetCommandMenu(int count, Command mnu[]);
void InitCommands();
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
    
    // Image initialization:
    Image imCommandBase1 = LoadImage("command/CommandBase1.png");
    Image imCommandBase2 = LoadImage("command/CommandBase2.png");
    Image imCommandBase3 = LoadImage("command/CommandBase3.png");
    Image imCommandMagic = LoadImage("command/CommandMagic.png");
    Image imCommandIcon = LoadImage("command/CommandIcon.png");
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
    texCommandIcon = LoadTextureFromImage(imCommandIcon);
    texGaugeHP = LoadTextureFromImage(imGaugeHP);
    texBarHP = LoadTextureFromImage(imBarHP);
    texGaugeMP = LoadTextureFromImage(imGaugeMP);
    texBarMP = LoadTextureFromImage(imBarMP);
    texGaugeSora = LoadTextureFromImage(imGaugeSora);
    
    // Font initialization:
    font1 = LoadFont("font/Font1.png");
    
    // Command initialization:
    InitCommands();
    CommandMenu *menuSub = &menuMagic;
    
    // Target FPS:
    SetTargetFPS(60);
    
    // Game loop:
    while (!WindowShouldClose())
    {
            if (IsKeyPressed(KEY_DOWN))
            {
                menuBase.cursor += 1;
            }

            if (IsKeyPressed(KEY_UP))
            {
                menuBase.cursor -= 1;
            }

            menuBase.cursor = Wrap(menuBase.cursor, 0, 4);

            // Draw:
            BeginDrawing();
                ClearBackground(VIOLET);
                DrawCommandMenu(&menuBase, 0);
                if (menuSub != NULL) DrawCommandMenu(menuSub, 1);
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

Command SetCommandMagic(char name[], char namera[], char namega[])
{
    Command commandTemp;

    commandTemp.type = MAGIC;
    commandTemp.Magic.level = 3;
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
    commandBase[0] = SetCommandBase("Attack");
    commandBase[1] = SetCommandBase("Magic");
    commandBase[2] = SetCommandBase("Items");
    commandBase[3] = SetCommandBase("Limit");
    menuBase = SetCommandMenu(4, commandBase);

    commandMagic[0] = SetCommandMagic("Fire", "Fira", "Firaga");
    commandMagic[1] = SetCommandMagic("Blizzard", "Blizzara", "Blizzaga");
    commandMagic[2] = SetCommandMagic("Thunder", "Thundara", "Thundaga");
    commandMagic[3] = SetCommandMagic("Cure", "Cura", "Curaga");
    commandMagic[4] = SetCommandMagic("Aero", "Aerora", "Aeroga");
    commandMagic[5] = SetCommandMagic("Gravity", "Gravira", "Graviga");
    commandMagic[6] = SetCommandMagic("Stop", "Stopra", "Stopga");
    menuMagic = SetCommandMenu(7, commandMagic);
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
        if (arr[i].type == MAGIC && arr[i].Magic.level == 0) continue;

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
    int iconSource = 14 * ((arr[cursor].type == BASE) ? cursor : arr[cursor].type);

    DrawTextureRec(texCommandIcon, (Rectangle){ iconSource, 0, 14, texCommandIcon.height }, (Vector2){ iconPosition.x, iconPosition.y + 16 * cursor }, WHITE);
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
