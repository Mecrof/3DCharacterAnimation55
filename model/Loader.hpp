#ifndef LOADER_H
#define LOADER_H

#include "glm/vec3.hpp"
#include <vector>

using namespace std;

class Loader {
        public:
            //Load a .obj jile
            bool loadObj(const char*, vector<glm::vec3>&, vector<glm::vec3>&, vector<unsigned int>&, vector<unsigned int>&);
};

#endif // LOADER_H
