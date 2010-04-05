#include "c3m.h"
#include <stdio.h>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("usage: %s <path/to/target.c3m>\n", argv[0]);
        return 0;
    }

    c3mModel* m;
    m = c3mOpen(argv[1]);
    if (c3mError)
    {
        printf("error: ");
        switch (c3mError)
        {
            case C3M_MISSING_FILE:
            {
                printf("unable to load file -- %s", argv[1]);
                break;
            }

            case C3M_INVALID_HEADER:
            {
                printf("invalid C3M file header");
                break;
            }

            case C3M_FAILED_ALLOCATION:
            {
                printf("failed to allocate memory");
                break;
            }

            case C3M_STRUCTURE_ERROR:
            {
                printf("problem parsing internal data");
                break;
            }

            default:
            {
                printf("unknown");
            }
        }

        printf("\n");

        return 1;
    }

    printf("%d vertices\n", m->vertices.size / 3u);
    printf("%d normals\n", m->normals.size / 3u);
    printf("%d colors\n", m->colors.size / 4u);
    printf("%d texture coordinates\n", m->textureCoordinates.size / 2u);
    printf("%d triangles\n", m->indices.size / 3u);

    if (m->textureFile != NULL)
    {
        printf("texture file: %s\n", m->textureFile);
    }

    c3mClose(m);
    return 0;
}
