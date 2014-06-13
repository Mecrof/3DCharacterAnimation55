#ifndef SUNLIGHT_H
#define SUNLIGHT_H

#include "GL/glew.h"

namespace scene
{

    typedef struct DirectionalLight DirectionalLight;
    typedef struct Material Material;

    struct DirectionalLight {
        float ambientColor[3];
        float diffuseColor[3];
        float specularColor[3];

        float halfPlane[3];
        float direction[3];
    };

    struct Material {
        float ambientFactor[3];
        float diffuseFactor[3];
        float specularFactor[3];
        float shininess;
    };

    namespace SpotLight
    {

        class Light
        {
            private :
                DirectionalLight *m_Light;
                Material *m_Material;

            public :
                explicit Light();
                explicit Light(DirectionalLight*, Material*);
                ~Light();
                DirectionalLight* getDirectionalLight();
                Material* getMaterial();
                void render(GLuint);
        };

    }

}
#endif // SUNLIGHT_H
