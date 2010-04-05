#ifndef CYBORUS_C3MREADER
#define CYBORUS_C3MREADER

#ifdef __cplusplus
extern "C" {
#endif

#define C3ML_MAJOR_VERSION 1
#define C3ML_MINOR_VERSION 0

#define C3M_NO_ERRORS 0
#define C3M_MISSING_FILE 1
#define C3M_INVALID_HEADER 2
#define C3M_FAILED_ALLOCATION 3
#define C3M_STRUCTURE_ERROR 4

typedef struct {
    float* array;
    unsigned int size;
} c3mFloatBlock;

typedef struct {
    unsigned int* array;
    unsigned int size;
} c3mIndexBlock;

typedef struct {
    c3mFloatBlock vertices;
    c3mFloatBlock normals;
    c3mFloatBlock colors;
    c3mFloatBlock secondaryColors;
    c3mFloatBlock textureCoordinates;
    c3mIndexBlock indices;
    char* textureFile;
} c3mModel;

extern unsigned int c3mError;
extern c3mModel* c3mOpen(const char* inFile);
extern void c3mClose(c3mModel* inModel);

#ifdef __cplusplus
}
#endif

#endif
