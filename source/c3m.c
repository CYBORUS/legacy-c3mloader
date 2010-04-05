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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned int c3mError;

c3mModel* c3mOpen(const char* inFile)
{
    c3mError = C3M_NO_ERRORS;

    FILE* fp; /* "fp" = file pointer */
    fp = fopen(inFile, "r");
    if (fp == NULL)
    {
        c3mError = C3M_MISSING_FILE;
        return NULL;
    }

    char header[8];
    size_t fr;
    fr = fread(header, 1, 7, fp);
    header[7] = '\0';
    if (strcmp(header, "CYBORUS"))
    {
        c3mError = C3M_INVALID_HEADER;
        fclose(fp);
        return NULL;
    }

    c3mModel* outModel;
    outModel = malloc(sizeof(c3mModel));
    if (outModel == NULL)
    {
        c3mError = C3M_FAILED_ALLOCATION;
        fclose(fp);
        return NULL;
    }

    /* "constructor" */
    outModel->vertices.array = NULL;
    outModel->vertices.size = 0;
    outModel->normals.array = NULL;
    outModel->normals.size = 0;
    outModel->colors.array = NULL;
    outModel->colors.size = 0;
    outModel->secondaryColors.array = NULL;
    outModel->secondaryColors.size = 0;
    outModel->textureCoordinates.array = NULL;
    outModel->textureCoordinates.size = 0;
    outModel->indices.array = NULL;
    outModel->indices.size = 0;
    outModel->textureFile = NULL;

    unsigned short versionNumber;
    fr = fread(&versionNumber, 2, 1, fp);

    unsigned short numberOfBlocks;
    fr = fread(&numberOfBlocks, 2, 1, fp);

    unsigned short i;
    for (i = 0; i < numberOfBlocks; ++i)
    {
        unsigned short blockName;
        fr = fread(&blockName, 2, 1, fp);
        unsigned int blockSize;
        fr = fread(&blockSize, 4, 1, fp);

        switch (blockName)
        {
            case 0x4156: /* vertices */
            {
                outModel->vertices.size = blockSize / sizeof(float);
                outModel->vertices.array = calloc(outModel->vertices.size,
                    sizeof(float));
                fr = fread(outModel->vertices.array, 1, blockSize, fp);
                break;
            }

            case 0x414e: /* normals */
            {
                outModel->normals.size = blockSize / sizeof(float);
                outModel->normals.array = calloc(outModel->normals.size,
                    sizeof(float));
                fr = fread(outModel->normals.array, 1, blockSize, fp);
                break;
            }

            case 0x4143: /* colors */
            {
                outModel->colors.size = blockSize / sizeof(float);
                outModel->colors.array = calloc(outModel->colors.size,
                    sizeof(float));
                fr = fread(outModel->colors.array, 1, blockSize, fp);
                break;
            }

            case 0x3243: /* secondary colors */
            {
                outModel->secondaryColors.size = blockSize / sizeof(float);
                outModel->secondaryColors.array =
                    calloc(outModel->secondaryColors.size, sizeof(float));
                fr = fread(outModel->secondaryColors.array, 1, blockSize, fp);
                break;
            }

            case 0x4354: /* texture coordinates */
            {
                outModel->textureCoordinates.size = blockSize / sizeof(float);
                outModel->textureCoordinates.array =
                    calloc(outModel->textureCoordinates.size, sizeof(float));
                fr = fread(outModel->textureCoordinates.array, 1, blockSize,
                    fp);
                break;
            }

            case 0x4149: /* indices */
            {
                outModel->indices.size = blockSize / sizeof(unsigned int);
                outModel->indices.array = calloc(outModel->indices.size,
                    sizeof(unsigned int));
                fr = fread(outModel->indices.array, 1, blockSize, fp);
                break;
            }

            case 0x4654: /* texture file name */
            {
                outModel->textureFile = malloc(blockSize + 1);
                fr = fread(outModel->textureFile, 1, blockSize, fp);
                outModel->textureFile[blockSize] = '\0';
                break;
            }

            default: /* unidentified block */
            {
                if (blockName < 0x3000 || blockName > 0x5AFF)
                {
                    /** invalid block name -- block names consist of 0 to 9 and
                        A to Z */
                    c3mError = C3M_STRUCTURE_ERROR;
                    c3mClose(outModel);
                    outModel = NULL;
                    i = numberOfBlocks;
                }
                else
                {
                    /* skip the entire block */
                    fseek(fp, blockSize, SEEK_CUR);
                }
            }
        }
    }

    fclose(fp);

    return outModel;
}

void c3mClose(c3mModel* inModel)
{
    if (inModel == NULL) return;

    if (inModel->vertices.size > 0)
    {
        free(inModel->vertices.array);
        inModel->vertices.size = 0;
    }

    if (inModel->normals.size > 0)
    {
        free(inModel->normals.array);
        inModel->normals.size = 0;
    }

    if (inModel->colors.size > 0)
    {
        free(inModel->colors.array);
        inModel->colors.size = 0;
    }

    if (inModel->secondaryColors.size > 0)
    {
        free(inModel->secondaryColors.array);
        inModel->secondaryColors.size = 0;
    }

    if (inModel->textureCoordinates.size > 0)
    {
        free(inModel->textureCoordinates.array);
        inModel->textureCoordinates.size = 0;
    }

    if (inModel->indices.size > 0)
    {
        free(inModel->indices.array);
        inModel->indices.size = 0;
    }

    if (inModel->textureFile != NULL)
    {
        free(inModel->textureFile);
    }

    free(inModel);
}
