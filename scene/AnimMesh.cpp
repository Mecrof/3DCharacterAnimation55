#include "AnimMesh.h"

#include <assimp/matrix4x4.h>

/**************************************************************************
* Name: cloneMatrix4x4ToGLM
* Description: clone an assimp 4x4 matrix to a glm 4x4 matrix
* Inputs:
- parameter1: a dest glm matrix
- parameter2: a source assimp matrix
* Returns:
- value: void
**************************************************************************/
void clone_matrix4x4_to_GLM(glm::mat4 & mat1, const aiMatrix4x4 & mat2)
{
    mat1[0][0] = mat2[0][0];
    mat1[0][1] = mat2[0][1];
    mat1[0][2] = mat2[0][2];
    mat1[0][3] = mat2[0][3];

    mat1[1][0] = mat2[1][0];
    mat1[1][1] = mat2[1][1];
    mat1[1][2] = mat2[1][2];
    mat1[1][3] = mat2[1][3];

    mat1[2][0] = mat2[2][0];
    mat1[2][1] = mat2[2][1];
    mat1[2][2] = mat2[2][2];
    mat1[2][3] = mat2[2][3];

    mat1[3][0] = mat2[3][0];
    mat1[3][1] = mat2[3][1];
    mat1[3][2] = mat2[3][2];
    mat1[3][3] = mat2[3][3];
}

/**************************************************************************
* Name: cloneMatrix4x4ToASSIMP
* Description: clone an glm 4x4 matrix to a assimp 4x4 matrix
* Inputs:
- parameter1: a dest assimp matrix
- parameter2: a source glm matrix
* Returns:
- value: void
**************************************************************************/
void clone_matrix4x4_to_ASSIMP(aiMatrix4x4 & mat1, glm::mat4 & mat2)
{
    mat1[0][0] = mat2[0][0];
    mat1[0][1] = mat2[0][1];
    mat1[0][2] = mat2[0][2];
    mat1[0][3] = mat2[0][3];

    mat1[1][0] = mat2[1][0];
    mat1[1][1] = mat2[1][1];
    mat1[1][2] = mat2[1][2];
    mat1[1][3] = mat2[1][3];

    mat1[2][0] = mat2[2][0];
    mat1[2][1] = mat2[2][1];
    mat1[2][2] = mat2[2][2];
    mat1[2][3] = mat2[2][3];

    mat1[3][0] = mat2[3][0];
    mat1[3][1] = mat2[3][1];
    mat1[3][2] = mat2[3][2];
    mat1[3][3] = mat2[3][3];
}

/////////////////////////////// PUBLIC ///////////////////////////////////////
//============================= LIFECYCLE ====================================

/**************************************************************************
* Name: AnimMesh
* Description: constructor
**************************************************************************/
AnimMesh::AnimMesh()
{
    this->m_Light = new scene::SpotLight::Light;

    this->m_Shader = new scene::Shader("./Shaders/light.vert", "./Shaders/light.frag");
    this->m_Shader->bind();

    m_IdModelView = glGetUniformLocation(m_Shader->getProgramID(), "modelview");
    m_IdProjection = glGetUniformLocation(m_Shader->getProgramID(), "projection");

    for (GLuint i = 0; i < ARRAY_SIZE_IN_ELEMENTS(this->m_BoneLocation); i++)
    {
        char name[128];
        memset(name, 0, sizeof(name));
        snprintf(name, sizeof(name), "gBones[%d]", i);
        this->m_BoneLocation[i] = glGetUniformLocation(m_Shader->getProgramID(), name);
    }

    this->m_RunningTime = 0.01f;

    this->m_IsLoading = false;
}

/**************************************************************************
* Name: ~AnimMesh
* Description: destructor
**************************************************************************/
AnimMesh::~AnimMesh()
{
    this->clear();
    delete  this->m_Light;
    delete  this->m_Importer;
}

//============================= OPERATIONS ===================================

/**************************************************************************
* Name: loadMesh
* Description: load a mesh and initialize the data
* Inputs:
- parameter1: the path to the mesh
* Returns:
- value: true if the mesh is successfully loaded
**************************************************************************/
bool AnimMesh::loadMesh(const std::string& file_name)
{
    this->m_IsLoading = true;

    // Release the previously loaded mesh (if it exists)
    this->clear();

    //create a new importer
    this->m_Importer = new Assimp::Importer;

    // Create the VAO
    glGenVertexArrays(1, &this->m_VAO);
    glBindVertexArray(this->m_VAO);

    // Create the buffers for the vertices atttributes
    glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(this->m_Buffers), this->m_Buffers);

    bool ret = false;
    this->m_Scene = this->m_Importer->ReadFile(file_name.c_str(), aiProcess_FixInfacingNormals | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    if(this->m_Scene->HasAnimations())
    {
        this->m_Animations.push_back(this->m_Scene->mAnimations[0]);
        this->m_Animation = this->m_Scene->mAnimations[0];
        this->m_AnimationSelected = true;
    }

    this->m_RootNode = this->m_Scene->mRootNode;

    if(this->m_Scene)
    {
        ret = this->initFromScene(this->m_Scene, file_name);
    }
    else
    {
        printf("Error parsing '%s': '%s'\n", file_name.c_str(), m_Importer->GetErrorString());
    }

    // Make sure the VAO is not changed from outside code
    glBindVertexArray(0);

    this->m_IsLoading = false;

    return ret;
}

/**************************************************************************
* Name: addAnimation
* Description: initialize the data from the scene
* Inputs:
- parameter1: a pointer to the current scene
* Returns:
- value:
**************************************************************************/
std::string AnimMesh::addAnimation(const std::string &file_name)
{
    Assimp::Importer * importer = new Assimp::Importer();
    this->m_Importers.push_back(importer);
    const aiScene * tmp_scene = importer->ReadFile(file_name.c_str(), 0);

    if(tmp_scene)
    {
        if(tmp_scene->HasAnimations())
        {
            this->m_Animations.push_back(tmp_scene->mAnimations[0]);
            return tmp_scene->mAnimations[0]->mName.C_Str();
        }
    }
    else
    {
        printf("Error parsing '%s': '%s'\n", file_name.c_str(), importer->GetErrorString());
    }

    return NULL;
}

/**************************************************************************
* Name: runAnimation
* Description: initialize the data from the scene
* Inputs:
- parameter1: a pointer to the current scene
* Returns:
- value:
**************************************************************************/
bool AnimMesh::runAnimation(int index)
{
    const aiAnimation * animation = this->m_Animations[index];

    if(animation)
    {
        this->m_Animation = animation;
        this->m_AnimationSelected = true;
        return true;
    }

    return false;
}


/**************************************************************************
* Name: initMaterials
* Description: initialize the textures of the mesh
* Inputs:
- parameter1: the current mesh index
- parameter2: the mesh loaded from assimp
- parameter3: the vector of positions
- parameter4: the vector of normals
- parameter5: the vector of textures
- parameter6: the vector of bones
- parameter6: the vector of indices
* Returns:
- value: void
**************************************************************************/
void AnimMesh::update(float tpf)
{
    this->m_RunningTime += tpf;
}

/**************************************************************************
* Name: initMaterials
* Description: initialize the textures of the mesh
* Inputs:
- parameter1: the current mesh index
- parameter2: the mesh loaded from assimp
- parameter3: the vector of positions
- parameter4: the vector of normals
- parameter5: the vector of textures
- parameter6: the vector of bones
- parameter6: the vector of indices
* Returns:
- value: void
**************************************************************************/
void AnimMesh::boneTransform(float time_in_second, std::vector<glm::mat4> &transforms)
{
    glm::mat4 identity;
    identity[0][0] = 1;
    identity[1][1] = 1;
    identity[2][2] = 1;
    identity[3][3] = 1;

    float ticks_per_second = (float)(this->m_Animation->mTicksPerSecond != 0 ? this->m_Animation->mTicksPerSecond : 25.0f);
    float time_in_ticks = time_in_second * ticks_per_second;
    float animation_time = fmod(time_in_ticks, (float)this->m_Animation->mDuration);

    readNodeHierarchy(animation_time, this->m_RootNode, identity);
    transforms.resize(this->m_NumBones);

    for (GLuint i = 0 ; i < this->m_NumBones ; i++)
    {
        transforms[i] = this->m_BoneInfo[i].m_FinalTransformation;
    }
}

/**************************************************************************
* Name: initMaterials
* Description: initialize the textures of the mesh
* Inputs:
- parameter1: the current mesh index
- parameter2: the mesh loaded from assimp
- parameter3: the vector of positions
- parameter4: the vector of normals
- parameter5: the vector of textures
- parameter6: the vector of bones
- parameter6: the vector of indices
* Returns:
- value: void
**************************************************************************/
void AnimMesh::render(const glm::mat4 &model_view, const glm::mat4 &projection)
{
    if(this->m_IsLoading)
        return;

    glBindVertexArray(m_VAO);
    glUseProgram(m_Shader->getProgramID());

    this->m_Light->render(m_Shader->getProgramID());

    glUniformMatrix4fv(this->m_IdModelView, 1, GL_FALSE, glm::value_ptr(model_view));
    glUniformMatrix4fv(this->m_IdProjection, 1, GL_FALSE, glm::value_ptr(projection));

    GLint id_has_animation = glGetUniformLocation(m_Shader->getProgramID(), "hasAnimation");

    if (this->m_AnimationSelected)
    {
        glUniform1f(id_has_animation, 1.0f );

        std::vector<glm::mat4> transforms;
        this->boneTransform(this->m_RunningTime, transforms);

        for(GLuint i = 0; i<transforms.size(); i++)
        {
            glUniformMatrix4fv(this->m_BoneLocation[i], 1, GL_TRUE, (const GLfloat*)glm::value_ptr(transforms[i]));
        }
    }
    else
    {
        glUniform1i(id_has_animation, 0.0f );
    }

    for (unsigned int i = 0 ; i < this->m_Entries.size() ; i++)
    {
        const GLuint material_index = this->m_Entries[i].m_MaterialIndex;

        if (m_Textures[material_index])
        {
           m_Textures[material_index]->bind(GL_TEXTURE0);
        }

        glDrawElementsBaseVertex(GL_TRIANGLES, m_Entries[i].m_NumIndices, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * m_Entries[i].m_BaseIndex), m_Entries[i].m_BaseVertex);
    }

    glUseProgram(0);
    glBindVertexArray(0);
}

scene::SpotLight::Light* AnimMesh::getLight()
{
    return this->m_Light;
}

//============================= ATTRIBUTE ACCESSORS ==========================

/**************************************************************************
* Name: getAnimations
* Description: return the available animations
* Inputs: void
* Returns:
- std::vector<const aiAnimation*>: the available animations
**************************************************************************/
const std::vector<const aiAnimation*> AnimMesh::getAnimations()
{
    return this->m_Animations;
}

/////////////////////////////// PRIVATE ///////////////////////////////////////
//============================= OPERATIONS ===================================
/**************************************************************************
* Name: clear
* Description: clear the data loaded for the current mesh
* Returns:
- value: void
**************************************************************************/
void AnimMesh::clear()
{
    for (GLuint i = 0 ; i < this->m_Textures.size() ; i++) {
        SAFE_DELETE(m_Textures[i]);
    }

    if (m_Buffers[0] != 0) {
        glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);
    }

    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }

    if(this->m_Importer)
    {
        this->m_Importer->FreeScene();
        delete m_Importer;
    }

    this->m_Animations.clear();
}

/**************************************************************************
* Name: initFromScene
* Description: initialize the data from the scene
* Inputs:
- parameter1: a pointer to the current scene
- parameter2: the path to the mesh
* Returns:
- value: true if the mesh is successfully initialized
**************************************************************************/
bool AnimMesh::initFromScene(const aiScene* scene, const std::string& file_name)
{
    this->m_Entries.resize(scene->mNumMeshes);
    this->m_Textures.resize(scene->mNumMaterials);

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textures;
    std::vector<VertexBoneData> bones;
    std::vector<GLuint> indices;

    GLuint num_vertices = 0;
    GLuint num_indices = 0;

    for (GLuint i = 0 ; i < this->m_Entries.size() ; i++)
    {
        this->m_Entries[i].m_MaterialIndex  = scene->mMeshes[i]->mMaterialIndex;
        this->m_Entries[i].m_NumIndices     = scene->mMeshes[i]->mNumFaces * 3;
        this->m_Entries[i].m_BaseVertex     = num_vertices;
        this->m_Entries[i].m_BaseIndex      = num_indices;

        num_vertices += scene->mMeshes[i]->mNumVertices;
        num_indices  += m_Entries[i].m_NumIndices;
    }

    positions.reserve(num_vertices);
    normals.reserve(num_vertices);
    textures.reserve(num_vertices);
    bones.resize(num_vertices);
    indices.reserve(num_indices);

    for (GLuint i = 0 ; i < this->m_Entries.size() ; i++)
    {
        const aiMesh* ai_mesh = scene->mMeshes[i];
        initMesh(i, ai_mesh, positions, normals, textures, bones, indices);
    }

    if (!initMaterials(scene, file_name))
    {
        return false;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);

    GLint id_vertex = glGetAttribLocation(m_Shader->getProgramID(), "in_Vertex");
    glEnableVertexAttribArray(id_vertex);
    glVertexAttribPointer(id_vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textures[0]) * textures.size(), &textures[0], GL_STATIC_DRAW);

    GLint id_tex_coord = glGetAttribLocation(m_Shader->getProgramID(), "in_TexCoord");
    glEnableVertexAttribArray(id_tex_coord);
    glVertexAttribPointer(id_tex_coord, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    GLint id_normal = glGetAttribLocation(m_Shader->getProgramID(), "in_Normal");
    glEnableVertexAttribArray(id_normal);
    glVertexAttribPointer(id_normal, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bones[0]) * bones.size(), &bones[0], GL_STATIC_DRAW);

    GLint id_bone = glGetAttribLocation(m_Shader->getProgramID(), "in_BoneIDs");
    glEnableVertexAttribArray(id_bone);
    glVertexAttribPointer(id_bone, 4, GL_INT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)0);

    GLint id_bone_weight = glGetAttribLocation(m_Shader->getProgramID(), "in_BoneWeight");
    glEnableVertexAttribArray(id_bone_weight);
    glVertexAttribPointer(id_bone_weight, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

    return true;
}

/**************************************************************************
* Name: initMesh
* Description: initialize the data of the mesh
* Inputs:
- parameter1: the current mesh index
- parameter2: the mesh loaded from assimp
- parameter3: the vector of positions
- parameter4: the vector of normals
- parameter5: the vector of textures
- parameter6: the vector of bones
- parameter6: the vector of indices
* Returns:
- value: void
**************************************************************************/
void AnimMesh::initMesh(GLuint mesh_index,
                        const aiMesh* ai_mesh,
                        std::vector<glm::vec3> &positions,
                        std::vector<glm::vec3> &normals,
                        std::vector<glm::vec2> &textures,
                        std::vector<VertexBoneData> &bones,
                        std::vector<GLuint> &indices)
{

   const aiVector3D null_vector(0.0f, 0.0f, 0.0f);

   for (GLuint i = 0 ; i < ai_mesh->mNumVertices ; i++)
   {
       const aiVector3D* pos = &(ai_mesh->mVertices[i]);
       const aiVector3D* norms = ai_mesh->HasNormals() ? &(ai_mesh->mNormals[i]) : &null_vector;
       const aiVector3D* texs = ai_mesh->HasTextureCoords(0) ? &(ai_mesh->mTextureCoords[0][i]) : &null_vector;

       positions.push_back(glm::vec3(pos->x, pos->y, pos->z));
       normals.push_back(glm::vec3(norms->x, norms->y, norms->z));
       textures.push_back(glm::vec2(texs->x, texs->y));
   }

   loadBones(mesh_index, ai_mesh, bones);

   for (unsigned int i = 0 ; i < ai_mesh->mNumFaces ; i++)
   {
       const aiFace& face = ai_mesh->mFaces[i];
       indices.push_back(face.mIndices[0]);
       indices.push_back(face.mIndices[1]);
       indices.push_back(face.mIndices[2]);
   }
}

/**************************************************************************
* Name: loadBones
* Description: initialize the data of the mesh
* Inputs:
- parameter1: the current mesh index
- parameter2: the mesh loaded from assimp
- parameter3: the vector of positions
* Returns:
- value: void
**************************************************************************/
void AnimMesh::loadBones(GLuint mesh_index, const aiMesh *ai_mesh, std::vector<VertexBoneData> &bones)
{
    for(GLuint i = 0 ; i < ai_mesh->mNumBones ; i++)
    {
        GLuint bone_index = 0;
        std::string bone_name(ai_mesh->mBones[i]->mName.data);

        if (this->m_BoneMapping.find(bone_name) == this->m_BoneMapping.end())
        {
            bone_index = this->m_NumBones;
            this->m_NumBones++;
            BoneInfo bone_info;
            this->m_BoneInfo.push_back(bone_info);
            clone_matrix4x4_to_GLM(this->m_BoneInfo[bone_index].m_BoneOffset, ai_mesh->mBones[i]->mOffsetMatrix);
            this->m_BoneMapping[bone_name] = bone_index;
        }
        else
        {
            bone_index = this->m_BoneMapping[bone_name];
        }

        this->m_BoneMapping[bone_name] = bone_index;
        clone_matrix4x4_to_GLM(this->m_BoneInfo[bone_index].m_BoneOffset, ai_mesh->mBones[i]->mOffsetMatrix);

        for (GLuint j = 0 ; j < ai_mesh->mBones[i]->mNumWeights ; j++)
        {
            GLuint vertex_id = this->m_Entries[mesh_index].m_BaseVertex + ai_mesh->mBones[i]->mWeights[j].mVertexId;
            float weight = ai_mesh->mBones[i]->mWeights[j].mWeight;
            bones[vertex_id].addBoneData(bone_index, weight);
        }
    }
}

/**************************************************************************
* Name: initMaterials
* Description: initialize the textures of the mesh
* Inputs:
- parameter1: the current mesh index
- parameter2: the mesh loaded from assimp
- parameter3: the vector of positions
- parameter4: the vector of normals
- parameter5: the vector of textures
- parameter6: the vector of bones
- parameter6: the vector of indices
* Returns:
- value: void
**************************************************************************/
bool AnimMesh::initMaterials(const aiScene* scene, const std::string& file_name)
{
    bool ret;

    for (unsigned int i = 0 ; i < scene->mNumMaterials ; i++)
    {
        const aiMaterial* material = scene->mMaterials[i];

        this->m_Textures[i] = NULL;

        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString path;
            std::string dir = file_name.substr(0, file_name.find_last_of("/"));

            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
            {
                std::string p(path.data);

                std::string full_path = dir + "/" + p.substr(p.find_last_of("/") + 1, file_name.length() + 1);

                m_Textures[i] = new Texture(GL_TEXTURE_2D, full_path.c_str());

                if (!m_Textures[i]->load())
                {
                    printf("Error loading texture '%s'\n", full_path.c_str());
                    delete m_Textures[i];
                    m_Textures[i] = NULL;
                    ret = false;
                }
            }
        }

        if (!m_Textures[i])
        {
            m_Textures[i] = new Texture(GL_TEXTURE_2D, "./white.png");
            ret = m_Textures[i]->load();
        }
    }

    return ret;
}

/**************************************************************************
* Name: initMaterials
* Description: initialize the textures of the mesh
* Inputs:
- parameter1: the current mesh index
- parameter2: the mesh loaded from assimp
- parameter3: the vector of positions
- parameter4: the vector of normals
- parameter5: the vector of textures
- parameter6: the vector of bones
- parameter6: the vector of indices
* Returns:
- value: void
**************************************************************************/
const aiNodeAnim *AnimMesh::findNodeAnim(const aiAnimation *animation, const std::string node_name)
{
    for (GLuint i = 0 ; i < animation->mNumChannels ; i++)
    {
        const aiNodeAnim* node_anim = animation->mChannels[i];

        if (std::string(node_anim->mNodeName.data) == node_name)
            return node_anim;
    }

    return NULL;
}

/**************************************************************************
* Name: initMaterials
* Description: initialize the textures of the mesh
* Inputs:
- parameter1: the current mesh index
- parameter2: the mesh loaded from assimp
- parameter3: the vector of positions
- parameter4: the vector of normals
- parameter5: the vector of textures
- parameter6: the vector of bones
- parameter6: the vector of indices
* Returns:
- value: void
**************************************************************************/
GLuint AnimMesh::findPosition(float animation_time, const aiNodeAnim *node_anim)
{
    for (GLuint i = 0 ; i < node_anim->mNumPositionKeys - 1 ; i++)
    {
        if (animation_time < (float)node_anim->mPositionKeys[i + 1].mTime)
            return i;
    }

    return 0;
}

/**************************************************************************
* Name: initMaterials
* Description: initialize the textures of the mesh
* Inputs:
- parameter1: the current mesh index
- parameter2: the mesh loaded from assimp
- parameter3: the vector of positions
- parameter4: the vector of normals
- parameter5: the vector of textures
- parameter6: the vector of bones
- parameter6: the vector of indices
* Returns:
- value: void
**************************************************************************/
GLuint AnimMesh::findRotation(float animation_time, const aiNodeAnim *node_anim)
{
    for (GLuint i = 0 ; i < node_anim->mNumRotationKeys - 1 ; i++)
    {
        if (animation_time < (float)node_anim->mRotationKeys[i + 1].mTime)
            return i;
    }

    return 0;
}

/**************************************************************************
* Name: initMaterials
* Description: initialize the textures of the mesh
* Inputs:
- parameter1: the current mesh index
- parameter2: the mesh loaded from assimp
- parameter3: the vector of positions
- parameter4: the vector of normals
- parameter5: the vector of textures
- parameter6: the vector of bones
- parameter6: the vector of indices
* Returns:
- value: void
**************************************************************************/
GLuint AnimMesh::findScaling(float animation_time, const aiNodeAnim *node_anim)
{
    for (GLuint i = 0 ; i < node_anim->mNumScalingKeys - 1 ; i++)
    {
        if (animation_time < (float)node_anim->mScalingKeys[i + 1].mTime)
            return i;
    }

    return 0;
}

/**************************************************************************
* Name: initMaterials
* Description: initialize the textures of the mesh
* Inputs:
- parameter1: the current mesh index
- parameter2: the mesh loaded from assimp
- parameter3: the vector of positions
- parameter4: the vector of normals
- parameter5: the vector of textures
- parameter6: the vector of bones
- parameter6: the vector of indices
* Returns:
- value: void
**************************************************************************/
void AnimMesh::calcInterpolatedScaling(aiVector3D &out, float animation_time, const aiNodeAnim *node_anim)
{
    if (node_anim->mNumScalingKeys == 1)
    {
        out = node_anim->mScalingKeys[0].mValue;
        return;
    }

    GLuint scaling_index = this->findScaling(animation_time, node_anim);
    GLuint next_scaling_index = (scaling_index + 1);

    float delta_time = (float)(node_anim->mScalingKeys[next_scaling_index].mTime - node_anim->mScalingKeys[scaling_index].mTime);
    float factor = (animation_time - (float)node_anim->mScalingKeys[scaling_index].mTime) / delta_time;

    const aiVector3D& start = node_anim->mScalingKeys[scaling_index].mValue;
    const aiVector3D& end   = node_anim->mScalingKeys[next_scaling_index].mValue;

    aiVector3D delta = end - start;
    out = start + factor * delta;
}

/**************************************************************************
* Name: initMaterials
* Description: initialize the textures of the mesh
* Inputs:
- parameter1: the current mesh index
- parameter2: the mesh loaded from assimp
- parameter3: the vector of positions
- parameter4: the vector of normals
- parameter5: the vector of textures
- parameter6: the vector of bones
- parameter6: the vector of indices
* Returns:
- value: void
**************************************************************************/
void AnimMesh::calcInterpolatedRotation(aiQuaternion &out, float animation_time, const aiNodeAnim *node_anim)
{
    // we need at least two values to interpolate...
    if (node_anim->mNumRotationKeys == 1)
    {
        out = node_anim->mRotationKeys[0].mValue;
        return;
    }

    GLuint rotation_index = this->findRotation(animation_time, node_anim);
    GLuint next_rotation_index = (rotation_index + 1);

    float delta_time = (float)(node_anim->mRotationKeys[next_rotation_index].mTime - node_anim->mRotationKeys[rotation_index].mTime);
    float factor = (animation_time - (float)node_anim->mRotationKeys[rotation_index].mTime) / delta_time;

    const aiQuaternion& start_rotation = node_anim->mRotationKeys[rotation_index].mValue;
    const aiQuaternion& end_otation    = node_anim->mRotationKeys[next_rotation_index].mValue;

    aiQuaternion::Interpolate(out, start_rotation, end_otation, factor);
    out = out.Normalize();
}

/**************************************************************************
* Name: initMaterials
* Description: initialize the textures of the mesh
* Inputs:
- parameter1: the current mesh index
- parameter2: the mesh loaded from assimp
- parameter3: the vector of positions
- parameter4: the vector of normals
- parameter5: the vector of textures
- parameter6: the vector of bones
- parameter6: the vector of indices
* Returns:
- value: void
**************************************************************************/
void AnimMesh::calcInterpolatedPosition(aiVector3D &out, float animation_time, const aiNodeAnim *node_anim)
{
    if (node_anim->mNumPositionKeys == 1)
    {
        out = node_anim->mPositionKeys[0].mValue;
        return;
    }

    GLuint position_index = this->findPosition(animation_time, node_anim);
    GLuint next_position_index = (position_index + 1);


    float delta_time = (float)(node_anim->mPositionKeys[next_position_index].mTime - node_anim->mPositionKeys[position_index].mTime);
    float factor = (animation_time - (float)node_anim->mPositionKeys[position_index].mTime) / delta_time;

    const aiVector3D& start = node_anim->mPositionKeys[position_index].mValue;
    const aiVector3D& end   = node_anim->mPositionKeys[next_position_index].mValue;

    aiVector3D delta = end - start;
    out = start + factor * delta;
}

/**************************************************************************
* Name: initMaterials
* Description: initialize the textures of the mesh
* Inputs:
- parameter1: the current mesh index
- parameter2: the mesh loaded from assimp
- parameter3: the vector of positions
- parameter4: the vector of normals
- parameter5: the vector of textures
- parameter6: the vector of bones
- parameter6: the vector of indices
* Returns:
- value: void
**************************************************************************/
void AnimMesh::readNodeHierarchy(float animation_time, const aiNode *node, const glm::mat4 &parent_transform)
{
   std::string node_name(node->mName.data);
   const aiAnimation* animation = this->m_Animation;
   glm::mat4 node_transformation;

   clone_matrix4x4_to_GLM(node_transformation, node->mTransformation);

   const aiNodeAnim* node_anim = this->findNodeAnim(animation, node_name);

   if (node_anim)
   {
       // Interpolate scaling and generate scaling transformation matrix
       aiVector3D scaling;
       this->calcInterpolatedScaling(scaling, animation_time, node_anim);
       glm::mat4 scaling_m;
       scaling_m[0][0] = scaling.x;
       scaling_m[1][1] = scaling.y;
       scaling_m[2][2] = scaling.z;
       scaling_m[3][3] = 1;

       // Interpolate rotation and generate rotation transformation matrix
       aiQuaternion rotation;
       this->calcInterpolatedRotation(rotation, animation_time, node_anim);
       glm::quat tmp_quat;
       tmp_quat.w = rotation.w;
       tmp_quat.x = rotation.x;
       tmp_quat.y = rotation.y;
       tmp_quat.z = rotation.z;
       tmp_quat = glm::inverse(tmp_quat);
       glm::mat4 rotation_m = glm::mat4_cast(tmp_quat);

       // Interpolate translation and generate translation transformation matrix
       aiVector3D translation;
       this->calcInterpolatedPosition(translation, animation_time, node_anim);
       glm::mat4 translation_m;
       translation_m[0][0] = 1;
       translation_m[1][1] = 1;
       translation_m[2][2] = 1;
       translation_m[3][3] = 1;
       translation_m[0][3] = translation.x;
       translation_m[1][3] = translation.y;
       translation_m[2][3] = translation.z;

       // Combine the above transformations
       node_transformation = rotation_m * translation_m;
   }

   glm::mat4 global_transformation =  node_transformation * parent_transform;
   if (m_BoneMapping.find(node_name) != m_BoneMapping.end())
   {
       GLuint bone_index = m_BoneMapping[node_name];
       m_BoneInfo[bone_index].m_FinalTransformation =  m_BoneInfo[bone_index].m_BoneOffset * global_transformation;
   }

   for (GLuint i = 0 ; i < node->mNumChildren ; i++)
   {
       readNodeHierarchy(animation_time, node->mChildren[i], global_transformation);
   }
}




/**************************************************************************
* Name: initMaterials
* Description: initialize the textures of the mesh
* Inputs:
- parameter1: the current mesh index
- parameter2: the mesh loaded from assimp
- parameter3: the vector of positions
- parameter4: the vector of normals
- parameter5: the vector of textures
- parameter6: the vector of bones
- parameter6: the vector of indices
* Returns:
- value: void
**************************************************************************/
void VertexBoneData::addBoneData(GLuint bone_id, float weight)
{
    for (GLuint i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(this->m_IDs) ; i++)
    {
        if(this->m_Weights[i] == 0.0)
        {
            this->m_IDs[i] = bone_id;
            this->m_Weights[i] = weight;
            return;
        }
    }
}
