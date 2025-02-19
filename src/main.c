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

/*
enum
{
    ATTACK,
    MAGIC,
    ITEMS,
    LIMIT,
};

typedef struct Command
{
    int type;
    int index;
} Command;

typedef struct Base
{
    char name[NAME_MAX];
} Base;

typedef struct CommandMenu
{
    Command *arr;
    int commandCount;
    int cursor;
    bool submenu;
} CommandMenu;

Base commandBase[4];
Command optionBase[4];
CommandMenu menuBase;
CommandMenu menuMagic;

Command SetCommand(int type, int index);
void SetCommandBase(int index, char name[]);
void InitCommandBase();
CommandMenu InitCommandMenu(Command arr[], int commandCount, bool submenu);
void InitMenuBase();
void DrawCommand();
void DrawCommandMenu();

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
*/

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
    
    // Texture initialization:
    texCommandBase1 = LoadTextureFromImage(imCommandBase1);
    texCommandBase2 = LoadTextureFromImage(imCommandBase2);
    texCommandBase3 = LoadTextureFromImage(imCommandBase3);
    texCommandMagic = LoadTextureFromImage(imCommandMagic);
    
    // Font initialization:
    font1 = LoadFont("font/Font1.png");
    
    // Command initialization:
    //InitCommandBase();
    //InitMenuBase();
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

/*
Command SetCommand(int type, int index)
{
    Command commandTemp;

    commandTemp.type = type;
    commandTemp.index = index;

    return commandTemp;
}

void SetCommandBase(int index, char name[])
{
    strcpy(commandBase[index].name, name);
}

void InitCommandBase()
{
    SetCommandBase(ATTACK, "Attack");
    SetCommandBase(MAGIC, "Magic");
    SetCommandBase(ITEMS, "Items");
    SetCommandBase(LIMIT, "Limit");
}

CommandMenu InitCommandMenu(Command arr[], int commandCount, bool submenu)
{
    CommandMenu menuTemp;

    menuTemp.arr = arr;
    menuTemp.commandCount = commandCount;
    menuTemp.cursor = 0;
    menuTemp.submenu = submenu;

    return menuTemp;
}

void InitMenuBase()
{
    optionBase[0] = SetCommand(0, ATTACK);
    optionBase[1] = SetCommand(0, MAGIC);
    optionBase[2] = SetCommand(0, ITEMS);
    optionBase[3] = SetCommand(0, LIMIT);
    menuBase = InitCommandMenu(optionBase, LIMIT + 1, false);
}

void DrawCommand(Command command, Vector2 position)
{
    char nameTemp[NAME_MAX] = "FAKE";
    int typeTemp = command.type;
    int indexTemp = command.index;

    switch (typeTemp)
    {
        default:
            strcpy(nameTemp, commandBase[indexTemp].name);
    }

    FONT_1(TextFormat("%d", indexTemp), position.x, position.y, WHITE);
}

void DrawCommandMenu(CommandMenu menu)
{
    int i;
    Command *arr = menu.arr;
    int commandCount = menu.commandCount;
    int cursor = menu.cursor;
    Vector2 commandPosition = { COMMAND_MARGIN, COMMAND_BOTTOM - (commandCount * COMMAND_HEIGHT) };

    // Header:


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
    
    Vector2 posAction = { COMMAND_MARGIN + indent, COMMAND_BOTTOM - (actionCount * COMMAND_HEIGHT) };
    
    // Header:
    DrawTextureRec(tex, (Rectangle){ 0, 0, tex.width, COMMAND_HEIGHT }, (Vector2){ posAction.x, posAction.y - COMMAND_HEIGHT }, WHITE);
    
    // Actions:
    for (i = 0; i < actionCount; i++)
    {
        int source_y, active_x;
        
        source_y = (active == i) ? 1 : ((i == actionCount - 1) ? 3 : 2);
        active_x = (active == i) ? 6 : 0;
        
        DrawTextureRec(tex, (Rectangle){ 0, COMMAND_HEIGHT * source_y, tex.width, COMMAND_HEIGHT }, (Vector2){ posAction.x + active_x, posAction.y }, WHITE);
        
        // Action name:
        FONT_1(arr[i].name, posAction.x + 10 + active_x, posAction.y + 4, WHITE);
        
        posAction.y += COMMAND_HEIGHT;
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
*/
