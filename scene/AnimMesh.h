#ifndef ANIMMESH_H
#define ANIMMESH_H

#include <map>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <scene.h>
#include <Importer.hpp>
#include <postprocess.h>

#include "SceneObject.hpp"
#include "Light.hpp"
#include "Texture.hpp"

#define INVALID_MATERIAL 0xFFFFFFFF  //invalid material id
#define INVALID_VALUE 0xFFFFFFFF     //invalid value id

//position of the buffers in the VAO
#define INDEX_BUFFER 0
#define POS_VB 1
#define NORMAL_VB 2
#define TEXCOORD_VB 3
#define BONE_VB 4
#define NUM_VBS 5

#define MAX_BONES 100
#define NUM_BONES_PER_VEREX 4

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0])) //return the size of an array
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }      //delete a pointer only if it exists

//Vertex structure that define a vertex
struct Vertex
{
    glm::vec3 m_Pos;
    glm::vec2 m_Tex;
    glm::vec3 m_Normal;

    Vertex() {}

    Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal)
    {
        m_Pos    = pos;
        m_Tex    = tex;
        m_Normal = normal;
    }
};

//MeshEntry structure that contains the mesh data
struct MeshEntry {

    GLuint m_BaseVertex;
    GLuint m_BaseIndex;
    GLuint m_NumIndices;
    GLuint m_MaterialIndex;

    MeshEntry()
    {
        m_BaseVertex = 0;
        m_BaseIndex = 0;
        m_NumIndices = 0;
        m_MaterialIndex = INVALID_MATERIAL;
    }
};

//VextexBonneData structure that contains the bones ids and weight of each bone attached to the vertex
struct VertexBoneData
{
    GLuint m_IDs[NUM_BONES_PER_VEREX];
    float m_Weights[NUM_BONES_PER_VEREX];

    void addBoneData(GLuint, float);
};

//BoneInfo structure that contains the bone informations
struct BoneInfo
{
    glm::mat4 m_BoneOffset;
    glm::mat4 m_FinalTransformation;
};

void cloneMatrix4x4ToGLM(glm::mat4& dest, const aiMatrix4x4& source);
void cloneMatrix4x4ToASSIMP(aiMatrix4x4& dest, glm::mat4& source);

class AnimMesh : public scene::SceneObject {

    public:
        AnimMesh();

        ~AnimMesh();

        void update(float);
        void render(const glm::mat4& model_view, const glm::mat4& projection);

        bool loadMesh(const std::string& file_name);
        void boneTransform(float time_in_second, std::vector<glm::mat4>& transforms);

        std::string addAnimation(const std::string& file_name);
        bool runAnimation(int index);
        const std::vector<const aiAnimation *> getAnimations();

        scene::SpotLight::Light* getLight();

    private:
        void clear();

        bool initFromScene(const aiScene* scene, const std::string& file_name);
        void initMesh(GLuint mesh_index, const aiMesh* paiMesh,
                      std::vector<glm::vec3>& positions,
                      std::vector<glm::vec3>& normals,
                      std::vector<glm::vec2>& tex_coords, std::vector<VertexBoneData>& bones,
                      std::vector<unsigned int>& indices);
        bool initMaterials(const aiScene* scene, const std::string& file_name);
        void loadBones(GLuint mesh_index, const aiMesh * pMesh, std::vector<VertexBoneData> & bones);

        GLuint findPosition(float animation_time, const aiNodeAnim* node_anim);
        GLuint findRotation(float animation_time, const aiNodeAnim* node_anim);
        GLuint findScaling(float animation_time, const aiNodeAnim* node_anim);

        void calcInterpolatedScaling(aiVector3D& out,    float animation_time, const aiNodeAnim* node_anim);
        void calcInterpolatedRotation(aiQuaternion& out, float animation_time, const aiNodeAnim* node_anim);
        void calcInterpolatedPosition(aiVector3D& out,   float animation_time, const aiNodeAnim* node_anim);

        const aiNodeAnim * findNodeAnim(const aiAnimation * animation, const std::string node_name);
        void readNodeHierarchy(float animation_time, const aiNode * node, const glm::mat4 & parent_transform);

        scene::SpotLight::Light *m_Light;

        GLuint m_VAO;
        GLuint m_Buffers[NUM_VBS];

        GLint m_IdModelView;
        GLint m_IdProjection;
        float m_RunningTime;

        std::vector<MeshEntry> m_Entries;
        std::vector<BoneInfo> m_BoneInfo;
        std::vector<Texture*> m_Textures;


        Assimp::Importer *m_Importer;
        std::vector<Assimp::Importer*> m_Importers;
        std::vector<const aiAnimation* > m_Animations;
        bool m_AnimationSelected;
        const aiScene * m_Scene;
        const aiAnimation *m_Animation;
        const aiNode *m_RootNode;
        std::map<std::string, GLuint> m_BoneMapping;
        GLuint m_NumBones;
        GLuint m_BoneLocation[MAX_BONES];

        bool m_IsLoading;
};

#endif // ANIMMESH_H
