#ifndef RESULT_H
#define RESULT_H

typedef enum {
    OK = 0,
    ERR_GENERIC,
    ERR_LOAD_DLL,
    ERR_FILE_IO,
    ERR_MEMORY,
    ERR_ASSET_LOAD,
    ERR_CONFIG_PARSE,
    ERR_RESOLVE_SYMBOL,
} Result;

#endif
