#include "decode.h"
#include "ops.h"
#include "memory.h"

#include "sys.h"
#include <string.h>

#define MAX_LABEL_COUNT 64
#define MAX_LABEL_SIZE 9

enum ARGTYPE {
    ARG_IMM,
    ARG_REG
};

enum REGISTER {
    a,
    b,
    c,
    d,
    f, // flags
    w, // write segment
    r, // read segment
    ih, // instruction ptr high
    il, // instruction ptr low
    sh, // stack pointer high
    sl, // stack pointer low
    bh, // base pointer high
    bl, // base pointer low
    eh, // extra reg high
    el, // extra reg low
    REGISTER_COUNT
};

char * strRegisters[] = {
    "a",
    "b",
    "c",
    "d",
    "f", 
    "w", 
    "r", 
    "ih",
    "il",
    "sh",
    "sl",
    "bh",
    "bl",
    "eh",
    "el"
};

/* determine argument type (if valid) for argument without spaces*/
enum ARGTYPE determineArgType(char const * const argStr) {
    bool alreadyContainsNumber = false;
    for(size_t i = 0; argStr[i] != 0; i++)
    {
        bool isNumerical = ( '0' <= argStr[i] && argStr[i] <= '9'  );
        
        if( alreadyContainsNumber ) 
        {
            ASSERT(isNumerical);
        }

        if(isNumerical)
        {
            alreadyContainsNumber = true;
        }
        else
        {
            ASSERT(!alreadyContainsNumber);
        }
    }

    return alreadyContainsNumber ? ARG_IMM : ARG_REG;
}

uint16_t encodeOp(char * in) {
    char * const firstSpace = strchr(in, ' ');

    ASSERT(firstSpace != NULL);
    ASSERT(firstSpace - in != 3);

    char const opName[3];
    memcpy(in, opName, 3);

    //find op name
    bool found = false;
    enum OP op = 0;
    for(; op < OP_COUNT; op++) 
    {
        if(strcmp(opInfos[op].name, in) == 0)
        {
            found = true;
            break;
        }
    }

    ASSERT(found);

    //get arguments as strings

    char* strArg1 = NULL; 
    char* strArg2 = NULL;

    char const * const secondSpace = strchr(firstSpace + 1, ' ');
    size_t const sizeArg1 = secondSpace - firstSpace;
    if( secondSpace &&  sizeArg1 > 1 )
    {
        strArg1 = malloc(sizeof(char) * 8);
        memcpy(strArg1, firstSpace + 1, sizeArg1);

        char const * const thirdSpace = strchr(secondSpace + 1, ' ');
        size_t const sizeArg2 = secondSpace - firstSpace;
        if( thirdSpace && sizeArg2 > 1)
        {
            strArg2 = malloc(sizeof(char) * 8);
            memcpy(strArg2, firstSpace + 1, sizeArg2);
        }
    }

    enum ARGTYPE typeArg1, typeArg2;

    if(strArg1){
        typeArg1 = determineArgType(strArg1);
    }

    if(strArg2){
        typeArg2 = determineArgType(strArg2);
    }

    struct OpInfo opInfo = opInfos[op];

    union Raw raw = {.high8 = opInfo.OpEncoded};

    //check argument format and evaluate
    switch(opInfo.OpFormat) {
    case F_REG_IMM: //first 4 ops use special encoding RawRA
        ASSERT(strArg1 && strArg2);
        ASSERT(typeArg1 == ARG_REG && typeArg2 == ARG_IMM);

        return ((union RawRA){
                    .op = op,
                    .reg = findInStrList(strArg1, strRegisters, REGISTER_COUNT),
                    .addr = strToInt(strArg2)})
            .bitsIn;

    case F_REG_REG:
        ASSERT(strArg1 && strArg2);
        ASSERT(typeArg1 == ARG_REG && typeArg2 == ARG_REG);

        raw.reg = findInStrList(strArg1, strRegisters, REGISTER_COUNT );
        raw.reg2 = findInStrList(strArg2, strRegisters, REGISTER_COUNT );

        break;

    case F_SIM_REG:
        ASSERT(strArg1 && strArg2);
        ASSERT(typeArg1 == ARG_REG && typeArg2 == ARG_IMM);

        raw.reg = findInStrList(strArg1, strRegisters, REGISTER_COUNT );
        raw.reg2 = strToInt(strArg2);

        break;

    case F_REG:
        ASSERT(strArg1 && !strArg2);
        ASSERT(typeArg1 == ARG_REG);

        raw.reg = findInStrList(strArg1, strRegisters, REGISTER_COUNT );

        break;

    case F_IMM:
        ASSERT(strArg1 && !strArg2);
        ASSERT(typeArg1 == ARG_IMM);

        raw.addr = strToInt(strArg2);

        break;

    case F_NONE:
        ASSERT(!strArg1 && !strArg2);
        break;
    };

    return raw.bitsIn;
}

static uint16_t memoryCount = 0;

struct Label {
    char name[10];
    uint16_t addr;
};

static struct Label labels[64] = {0};
static size_t labelCount = 0;

void registerLabel(char const * const label) {
    ASSERT(labelCount <= MAX_LABEL_COUNT);
    strncat(labels[labelCount].name, label, MAX_LABEL_SIZE);
    labels[labelCount].addr = memoryCount;
    labelCount++;
}

void handleLabel(char const * const line ) {
    ASSERT(line[0] == '@');
    registerLabel(line + 1);
}

void handleDB(char const * db) {
    //TODO add out of bounds warning instead of just cutting of anything that goes beyond memory size

    // DB -> human readable string
    // DX -> hex encoded string
    ASSERT(db[1] == 'B' || db[1] == 'X');
    ASSERT(db[2] == ' ');
    if(db[1] == 'B') {
        //copy from db to image until terminating \0 or out of bounds
        memccpy(memory + memoryCount, db + 3, 0, MEMORY_SIZE - memoryCount);
    }

    for(uint16_t i = 0; db[i] != 0; i++) {
        if(db[i] == '\') {
            //TODO: add out of bounds check
            strchr(db + i, )
            i+=2;
        }
    }
}

void parseLine(char const * const line) {


    //terminate input at EOL or first semicolon
    *strpbrk(line, ";\n") = 0;

    //handle labels
    if(line[0] == '@') {
        registerLabel(line + 1);
    };
};

i16main ()
{
    //handle labels, db, repeat, fill assembler commands

    //outputs a binary image that can be loaded into memory and executed (add rom to CPU later?)
}
