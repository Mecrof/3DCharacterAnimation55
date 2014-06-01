#ifndef ANIMMESH_H
#define ANIMMESH_H

#include <map>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <assimp/scene.h>
//#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "SceneObject.hpp"
#include "Light.hpp"

typedef unsigned int uint;

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

struct Vertex
{
    glm::vec3 m_pos;
    glm::vec2 m_tex;
    glm::vec3 m_normal;

    Vertex() {}

    Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal)
    {
        m_pos    = pos;
        m_tex    = tex;
        m_normal = normal;
    }
};

void cloneMatrix4x4ToGLM(glm::mat4 & mat1, const aiMatrix4x4 & mat2);

void cloneMatrix4x4ToASSIMP(aiMatrix4x4 & mat1, glm::mat4 & mat2);


/*
struct MeshEntry {
    MeshEntry();

    ~MeshEntry();

    void init(const std::vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices);

    GLuint VB;
    GLuint IB;
    unsigned int m_numIndices;
    unsigned int m_materialIndex;
};
*/

class AnimMesh : public scene::SceneObject {

    public:
        AnimMesh();

        ~AnimMesh();

        bool loadMesh(const std::string& filename);


        void update(float);
        void render(const glm::mat4 & modelView, const glm::mat4 & projection);

        void boneTransform(float timeInSecond, std::vector<glm::mat4> & transforms);

    private:

#define INVALID_MATERIAL 0xFFFFFFFF
#define INVALID_VALUE 0xFFFFFFFF

#define INDEX_BUFFER 0
#define POS_VB 1
#define NORMAL_VB 2
#define TEXCOORD_VB 3
#define BONE_VB 4
#define NUM_VBs 5
#define MAX_BONES 100

    #define NUM_BONES_PER_VEREX 4

        struct MeshEntry {
            MeshEntry()
            {
                numIndices = 0;
                baseVertex = 0;
                baseIndex = 0;
                materialIndex = INVALID_MATERIAL;
            }

            unsigned int baseVertex;
            unsigned int baseIndex;
            unsigned int numIndices;
            unsigned int materialIndex;
        };

        struct VertexBoneData
        {
            uint IDs[NUM_BONES_PER_VEREX];
            float Weights[NUM_BONES_PER_VEREX];

            void addBoneData(uint boneID, float weight);
        };

        struct BoneInfo
        {
            glm::mat4 BoneOffset;
            glm::mat4 FinalTransformation;
        };

        bool initFromScene(const aiScene* pScene, const std::string& filename);
        void initMesh(uint meshIndex, const aiMesh* paiMesh,
                      std::vector<glm::vec3>  & positions,
                      std::vector<glm::vec3>  & normals,
                      std::vector<glm::vec2>  & texCoords, std::vector<VertexBoneData> &bones,
                      std::vector<unsigned int>  & indices);
        //void initMesh(unsigned int index, const aiMesh* paiMesh);
        bool initMaterials(const aiScene* pScene, const std::string& filename);
        void loadBones(uint meshIndex, const aiMesh * pMesh, std::vector<VertexBoneData> & bones);
        const aiNodeAnim * FindNodeAnim(const aiAnimation * pAnimation, const std::string NodeName);
        uint FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
        uint FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
        uint FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
        void CalcInterpolatedScaling(aiVector3D &Out, float AnimationTime, const aiNodeAnim * pNodeAnim);
        void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
        void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
        void readNodeHierarchy(float AnimationTime, const aiNode * pNode, const glm::mat4 & ParentTransform);
        void clear();



        GLuint m_VAO;
        GLuint m_Buffers[NUM_VBs];

        scene::SpotLight::Light *m_light;
        GLint m_idModelView;
        GLint m_idProjection;
        float m_runningTime;

        std::vector<MeshEntry> m_Entries;
        std::vector<BoneInfo> m_BoneInfo;
        glm::mat4 m_GlobalInverseTransform;
        //std::vector<Texture*> m_Textures;

        const aiScene * m_pScene;
        Assimp::Importer m_Importer;
        const aiAnimation * m_pAnimation;
        const aiNode * m_pRootNode;
        std::map<std::string, uint> m_BoneMapping; // maps a bone name to its index
        uint m_NumBones;
         GLuint m_boneLocation[MAX_BONES];
};

#endif // ANIMMESH_H
