#include "Loader.hpp"

#include <cstring>
#include <iostream>
#include <fstream>

bool
Loader::loadObj(const char *fileName, vector<glm::vec3> &vertices, vector<glm::vec3> &normals, vector<unsigned int> &facesVertices, vector<unsigned int> &facesNormals)
{
   // Open OBJ file
    FILE * file = fopen(fileName, "r");
    if( file == NULL )
    {
        std::cout << "Impossible to open the file !\n";
        return false;
    }

    char lineHeader[256];
    int res, matches;
    glm::vec3 vertex, normal;
    unsigned int vertexIndex[3], normalIndex[3];

    while( 1 )
    {
        // read the first word of the line
        res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;

        // else : parse lineHeader
        if ( strcmp( lineHeader, "v" ) == 0 )
        {
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            vertices.push_back(vertex);
        }
        /*else if ( strcmp( lineHeader, "vt" ) == 0 )
        {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            temp_uvs.push_back(uv);
        }*/
        else if ( strcmp( lineHeader, "vn" ) == 0 )
        {
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            normals.push_back(normal);
        }
        else if ( strcmp( lineHeader, "f" ) == 0 )
        {
            matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2] );

            if (matches != 6)
            {
                printf("Wrong format, should be : v//vn v//vn v//vn\n");
                return false;
            }

            facesVertices.push_back(vertexIndex[0] - 1);
            facesVertices.push_back(vertexIndex[1] - 1);
            facesVertices.push_back(vertexIndex[2] - 1);
            facesNormals.push_back(normalIndex[0] - 1);
            facesNormals.push_back(normalIndex[1] - 1);
            facesNormals.push_back(normalIndex[2] - 1);
        }
    }

    return true;
}
