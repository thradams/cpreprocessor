
#pragma once

#include "..\Base\Array.h"
#include "BasicScanner.h"
#include "..\Base\Map.h"

typedef struct
{
    String Name;
    StrArray Args;
    StrBuilder Content;
    int FileIndex;
} TMacro;
#define TMACRO_INIT { STRING_INIT, STRARRAY_INIT, STRBUILDER_INIT , -1}

TMacro* TMacro_Create();
void TMacro_Delete(TMacro*);

typedef Map TMacroMap;
#define TMACRP_MAP_INIT MAP_INIT

void TMacroMap_Init(TMacroMap* p);
void TMacroMap_Destroy(TMacroMap* p);
Result TMacroMap_Set(TMacroMap *map, const char*key, TMacro *data);
TMacro* TMacroMap_Find(TMacroMap* map, const char* key);
Result TMacroMap_DeleteItem(TMacroMap *map, const char* key);
void TMacroMap_Swap(TMacroMap *a, TMacroMap *b);

typedef struct
{
    String FullPath;
    int FileIndex;
    bool PragmaOnce;
	int FileLevel;
} TFile;
#define TFILE_INIT {STRING_INIT, 0, false, 0}

typedef Map TFileMap;

void TFileMap_Destroy(TFileMap* p);
Result TFileMap_Set(TFileMap *map, const char*key, TFile *data);
TFile* _opt TFileMap_Find(TFileMap* map, const char* key);
Result TFileMap_DeleteItem(TFileMap *map, const char* key);
void TFile_DeleteVoid(void* p);

typedef struct
{
	TFile** pItems;
	size_t size;
	size_t capacity;
} TFileArray;
#define TFILE_ARRAY_INIT {NULL, 0 , 0}
ARRAYOF(TFileArray, TFile)

typedef struct
{
    //Stack de basicscanner
    Array stack;

    //Mapa dos defines
    TMacroMap Defines;

    //Stack usado para #if #else etc
    ArrayInt StackIfDef;

    //lista de arquivos marcados com pragma once
    TFileMap FilesIncluded;

    //Lista de diretorios de include
    StrArray IncludeDir;

    //Expandindo dentro de um #if para considerar o defined
    bool bExpressionMode;

    //string para debug
    StrBuilder DebugString;

    //String que mantem o erro
    StrBuilder ErrorString;

    //True indica error
    bool bError;

    //Imprime includes no console
    bool bPrintIncludes;

    //Quando true o scanner retorna tambem espacos
    bool bIncludeSpaces;

    //Indica que foi feita uma leitura especulativa
    bool bHasLookAhead;

    //Valor lido na leitura especulativa
    ScannerItem LookAhead;

} Scanner;

void Scanner_SetError(Scanner* pScanner, const char* message);

const char* Scanner_GetStreamName(Scanner* pScanner);
Result Scanner_InitString(Scanner* pScanner,
                          const char* name,
                          const char* text,
                          bool bExpressionMode);

const char* Scanner_TokenString(Scanner* pScanner);
Tokens Scanner_Token(Scanner* pScanner);
const char* Scanner_Lexeme(Scanner* pScanner);

Result Scanner_Init(Scanner* pScanner);

typedef enum
{
    FileIncludeTypeQuoted,
    FileIncludeTypeIncludes,
    FileIncludeTypeFullPath,
} FileIncludeType;

void Scanner_IncludeFile(Scanner* pScanner,
                         const char* fileName,
                         FileIncludeType fileIncludeType);

void Scanner_Destroy(Scanner* pScanner);
int Scanner_GetCurrentLine(Scanner * pScanner);
int Scanner_GetFileIndex(Scanner * pScanner);

Tokens Scanner_Token(Scanner * pScanner);
const char * Scanner_Lexeme(Scanner * pScanner);

//BasicScanner * Scanner_Top(Scanner * pScanner);
int Scanner_Line(Scanner* scanner);
int Scanner_Col(Scanner* scanner);
void Scanner_Next(Scanner* pScanner);
void Scanner_Skip(Scanner* pScanner);

int EvalExpression(const char* s, Scanner* pScanner);
void Scanner_PrintDebug(Scanner* pScanner);
void Scanner_GetError(Scanner* pScanner, StrBuilder* str);


void PrintPreprocessedToConsole(const char* fileIn);

void Scanner_GetScannerItemCopy(Scanner* pScanner,
                                ScannerItem* scannerItem);

ScannerItem* Scanner_GetLookAhead(Scanner* pScanner);