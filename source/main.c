/**
 *  This file is part of c3mloader.
 *
 *  c3mloader is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  c3mloader is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with c3mloader.  If not, see <http://www.gnu.org/licenses/>.
 */

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

            case C3M_UNSUPPORTED_SPEC:
            {
                printf("the file spec is newer than this loader supports");
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
    printf("%d secondary colors\n", m->secondaryColors.size / 4u);
    printf("%d texture coordinates\n", m->textureCoordinates.size / 2u);
    printf("%d triangles\n", m->indices.size / 3u);

    if (m->textureFile != NULL)
    {
        printf("texture file: %s\n", m->textureFile);
    }

    c3mClose(m);
    return 0;
}
