#include "AnimMesh.h"

#include <assimp/matrix4x4.h>

/*
//MeshEntry
MeshEntry::MeshEntry()
{
    VB = INVALID_VALUE;
    IB = INVALID_VALUE;
    m_numIndices  = 0;
    m_materialIndex = INVALID_MATERIAL;
};

MeshEntry::~MeshEntry()
{
    if (VB != INVALID_VALUE)
    {
        glDeleteBuffers(1, &VB);
    }

    if (IB != INVALID_VALUE)
    {
        glDeleteBuffers(1, &IB);
    }
}

void MeshEntry::init(const std::vector<Vertex>& Vertices,
                          const std::vector<unsigned int>& Indices)
{
    m_numIndices = Indices.size();

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_numIndices, &Indices[0], GL_STATIC_DRAW);
}
*/

void cloneMatrix4x4ToGLM(glm::mat4 & mat1, const aiMatrix4x4 & mat2)
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

void cloneMatrix4x4ToASSIMP(aiMatrix4x4 & mat1, glm::mat4 & mat2)
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

//AnimMesh
AnimMesh::AnimMesh()
{
    this->m_light = new scene::SpotLight::Light;

    this->m_shader = new scene::Shader("./Shaders/light.vert", "./Shaders/light.frag");
    this->m_shader->bind();

    m_idModelView = glGetUniformLocation(m_shader->getProgramID(), "modelview");
    m_idProjection = glGetUniformLocation(m_shader->getProgramID(), "projection");

    for (uint i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_boneLocation); i++)
    {
        char Name[128];
        memset(Name, 0, sizeof(Name));
        snprintf(Name, sizeof(Name), "gBones[%d]", i);
        m_boneLocation[i] = glGetUniformLocation(m_shader->getProgramID(), Name);
    }

    m_runningTime = 0.01f;

}

AnimMesh::~AnimMesh()
{
    glDeleteVertexArrays(1, &m_VAO);
    delete m_light;

}

bool AnimMesh::loadMesh(const std::string& filename)
{
    // Release the previously loaded mesh (if it exists)
    //Clear();

    // Create the VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Create the buffers for the vertices atttributes
    glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

    bool ret = false;
    m_pScene = m_Importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_FixInfacingNormals | aiProcess_GenSmoothNormals);
    m_pAnimation = m_pScene->mAnimations[0];
    m_pRootNode = m_pScene->mRootNode;

    if (m_pScene) {
        cloneMatrix4x4ToGLM(m_GlobalInverseTransform, m_pScene->mRootNode->mTransformation);
        std::cout << "m_GlobalInverseTransform="<<std::endl;
        std::cout << m_GlobalInverseTransform[0][0] << "\t" << m_GlobalInverseTransform[0][1] << "\t" << m_GlobalInverseTransform[0][2] << "\t" << m_GlobalInverseTransform[0][3] << std::endl;
        std::cout << m_GlobalInverseTransform[1][0] << "\t" << m_GlobalInverseTransform[1][1] << "\t" << m_GlobalInverseTransform[1][2] << "\t" << m_GlobalInverseTransform[1][3] << std::endl;
        std::cout << m_GlobalInverseTransform[2][0] << "\t" << m_GlobalInverseTransform[2][1] << "\t" << m_GlobalInverseTransform[2][2] << "\t" << m_GlobalInverseTransform[2][3] << std::endl;
        std::cout << m_GlobalInverseTransform[3][0] << "\t" << m_GlobalInverseTransform[3][1] << "\t" << m_GlobalInverseTransform[3][2] << "\t" << m_GlobalInverseTransform[3][3] << std::endl;

        m_GlobalInverseTransform = glm::inverse(m_GlobalInverseTransform);

        std::cout << "m_GlobalInverseTransform=inversed"<<std::endl;
        std::cout << m_GlobalInverseTransform[0][0] << "\t" << m_GlobalInverseTransform[0][1] << "\t" << m_GlobalInverseTransform[0][2] << "\t" << m_GlobalInverseTransform[0][3] << std::endl;
        std::cout << m_GlobalInverseTransform[1][0] << "\t" << m_GlobalInverseTransform[1][1] << "\t" << m_GlobalInverseTransform[1][2] << "\t" << m_GlobalInverseTransform[1][3] << std::endl;
        std::cout << m_GlobalInverseTransform[2][0] << "\t" << m_GlobalInverseTransform[2][1] << "\t" << m_GlobalInverseTransform[2][2] << "\t" << m_GlobalInverseTransform[2][3] << std::endl;
        std::cout << m_GlobalInverseTransform[3][0] << "\t" << m_GlobalInverseTransform[3][1] << "\t" << m_GlobalInverseTransform[3][2] << "\t" << m_GlobalInverseTransform[3][3] << std::endl;

        ret = initFromScene(m_pScene, filename);
    }
    else {
        printf("Error parsing '%s': '%s'\n", filename.c_str(), m_Importer.GetErrorString());
    }

    // Make sure the VAO is not changed from outside code
    glBindVertexArray(0);

    return ret;
}

bool AnimMesh::initFromScene(const aiScene* pScene, const std::string& filename)
{
    m_Entries.resize(pScene->mNumMeshes);
    //m_Textures.resize(pScene->mNumMaterials);

    // Prepare vectors for vertex attributes and indices
    std::vector<glm::vec3> Positions;
    std::vector<glm::vec3> Normals;
    std::vector<glm::vec2> TexCoords;
    std::vector<VertexBoneData> bones;
    std::vector<unsigned int> Indices;

    unsigned int NumVertices = 0;
    unsigned int NumIndices = 0;

    // Count the number of vertices and indices
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
        m_Entries[i].materialIndex  = pScene->mMeshes[i]->mMaterialIndex;
        m_Entries[i].numIndices     = pScene->mMeshes[i]->mNumFaces * 3;
        m_Entries[i].baseVertex     = NumVertices;
        m_Entries[i].baseIndex      = NumIndices;

        NumVertices += pScene->mMeshes[i]->mNumVertices;
        NumIndices  += m_Entries[i].numIndices;
    }

    // Reserve space in the vectors for the vertex attributes and indices
    Positions.reserve(NumVertices);
    Normals.reserve(NumVertices);
    TexCoords.reserve(NumVertices);
    bones.resize(NumVertices);
    Indices.reserve(NumIndices);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        initMesh(i, paiMesh, Positions, Normals, TexCoords, bones, Indices);
    }

    if (!initMaterials(pScene, filename)) {
        return false;
    }


    // Generate and populate the buffers with vertex attributes and the indices
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
    glBufferData(   GL_ARRAY_BUFFER,
                    sizeof(Positions[0]) * Positions.size(),
                    &Positions[0],
                    GL_STATIC_DRAW);
    GLint idVertex = glGetAttribLocation(m_shader->getProgramID(), "in_Vertex");
    glEnableVertexAttribArray(idVertex);
    glVertexAttribPointer(idVertex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
    glBufferData(   GL_ARRAY_BUFFER,
                    sizeof(TexCoords[0]) * TexCoords.size(),
                    &TexCoords[0],
                    GL_STATIC_DRAW);
    GLint idTexCoord = glGetAttribLocation(m_shader->getProgramID(), "in_TexCoord");
    glEnableVertexAttribArray(idTexCoord);
    glVertexAttribPointer(idTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
    glBufferData(   GL_ARRAY_BUFFER,
                    sizeof(Normals[0]) * Normals.size(),
                    &Normals[0],
                    GL_STATIC_DRAW);
    GLint idNormal = glGetAttribLocation(m_shader->getProgramID(), "in_Normal");
    glEnableVertexAttribArray(idNormal);
    glVertexAttribPointer(idNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bones[0]) * bones.size(), &bones[0], GL_STATIC_DRAW);
    GLint idBone = glGetAttribLocation(m_shader->getProgramID(), "in_BoneIDs");
    glEnableVertexAttribArray(idBone);
        glVertexAttribPointer(idBone, 4, GL_INT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)0);
//    glVertexAttribIPointer(idBone, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
    GLint idBoneWeight = glGetAttribLocation(m_shader->getProgramID(), "in_BoneWeight");
    glEnableVertexAttribArray(idBoneWeight);
    glVertexAttribPointer(idBoneWeight, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
    glBufferData(   GL_ELEMENT_ARRAY_BUFFER,
                    sizeof(Indices[0]) * Indices.size(),
                    &Indices[0],
                    GL_STATIC_DRAW);

    return true;
}

/*
bool AnimMesh::initFromScene(const aiScene* pScene, const std::string& filename)
{
    m_Entries.resize(pScene->mNumMeshes);
    //m_Textures.resize(pScene->mNumMaterials);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        initMesh(i, paiMesh);
    }

    return true;//InitMaterials(pScene, Filename);
}*/

void AnimMesh::initMesh(uint meshIndex,
                        const aiMesh* paiMesh,
                        std::vector<glm::vec3> &positions,
                        std::vector<glm::vec3> &normals,
                        std::vector<glm::vec2> &texCoords,
                        std::vector<VertexBoneData> &bones,
                        std::vector<unsigned int> &indices)
{
    const aiVector3D zero3D(0.0f, 0.0f, 0.0f);


   // Populate the vertex attribute vectors
   for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
       const aiVector3D* pPos = &(paiMesh->mVertices[i]);
       const aiVector3D* pNormal = paiMesh->HasNormals() ? &(paiMesh->mNormals[i]) : &zero3D;
       const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &zero3D;

       positions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
       normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
       texCoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));
   }

   loadBones(meshIndex, paiMesh, bones);

   // Populate the index buffer
   for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
       const aiFace& face = paiMesh->mFaces[i];
       assert(face.mNumIndices == 3);
       indices.push_back(face.mIndices[0]);
       indices.push_back(face.mIndices[1]);
       indices.push_back(face.mIndices[2]);
   }
}

/*
void AnimMesh::initMesh(unsigned int index, const aiMesh* paiMesh)
{
    m_Entries[index].m_materialIndex = paiMesh->mMaterialIndex;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++)
    {
        const aiVector3D* pPos = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal = paiMesh->HasNormals() ? &(paiMesh->mNormals[i]) : &zero3D;
        //const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

        Vertex v(glm::vec3(pPos->x, pPos->y, pPos->z),
               glm::vec2(0.0, 0.0),
               glm::vec3(pNormal->x, pNormal->y, pNormal->z));

        vertices.push_back(v);
    }

    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++)
    {
        const aiFace& Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        indices.push_back(Face.mIndices[0]);
        indices.push_back(Face.mIndices[1]);
        indices.push_back(Face.mIndices[2]);
    }

    m_Entries[index].init(vertices, indices);
}
*/

bool AnimMesh::initMaterials(const aiScene* pScene, const std::string& filename)
{
    /*for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++)
    {
        const aiMaterial* pMaterial = pScene->mMaterials[i];
        m_Textures[i] = NULL;
        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                std::string FullPath = Dir + "/" + Path.data;
                m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

                if (!m_Textures[i]->Load()) {
                    printf("Error loading texture '%s'\n", FullPath.c_str());
                    delete m_Textures[i];
                    m_Textures[i] = NULL;
                    Ret = false;
                }
            }
        }

        if (!m_Textures[i]) {
            m_Textures[i] = new Texture(GL_TEXTURE_2D, "./white.png");
            Ret = m_Textures[i]->Load();
        }
    }

    return Ret;*/
    return true;
}

void AnimMesh::loadBones(uint meshIndex, const aiMesh *pMesh, std::vector<AnimMesh::VertexBoneData> &bones)
{
    for (uint i = 0 ; i < pMesh->mNumBones ; i++) {
        uint BoneIndex = 0;
        std::string BoneName(pMesh->mBones[i]->mName.data);

        if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
            BoneIndex = m_NumBones;
            m_NumBones++;
            BoneInfo bi;
            m_BoneInfo.push_back(bi);
            cloneMatrix4x4ToGLM(m_BoneInfo[BoneIndex].BoneOffset, pMesh->mBones[i]->mOffsetMatrix);
            m_BoneMapping[BoneName] = BoneIndex;
        }
        else {
            BoneIndex = m_BoneMapping[BoneName];
        }

        m_BoneMapping[BoneName] = BoneIndex;
        cloneMatrix4x4ToGLM(m_BoneInfo[BoneIndex].BoneOffset, pMesh->mBones[i]->mOffsetMatrix);
       // m_BoneInfo[BoneIndex].BoneOffset = m_BoneInfo[BoneIndex].BoneOffset * glm::inverse(m_GlobalInverseTransform);

        for (uint j = 0 ; j < pMesh->mBones[i]->mNumWeights ; j++) {
            uint VertexID = m_Entries[meshIndex].baseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
            float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
            bones[VertexID].addBoneData(BoneIndex, Weight);
        }
    }

}

const aiNodeAnim *AnimMesh::FindNodeAnim(const aiAnimation *pAnimation, const std::string NodeName)
{
    for (uint i = 0 ; i < pAnimation->mNumChannels ; i++) {
        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

        if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
            return pNodeAnim;
        }
    }

    return NULL;
}

uint AnimMesh::FindPosition(float AnimationTime, const aiNodeAnim *pNodeAnim)
{
    for (uint i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
            return i;
        }
    }

    assert(0);

    return 0;
}

uint AnimMesh::FindRotation(float AnimationTime, const aiNodeAnim *pNodeAnim)
{
    assert(pNodeAnim->mNumRotationKeys > 0);

    for (uint i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
            return i;
        }
    }

    assert(0);

    return 0;
}

uint AnimMesh::FindScaling(float AnimationTime, const aiNodeAnim *pNodeAnim)
{
    assert(pNodeAnim->mNumScalingKeys > 0);

    for (uint i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
            return i;
        }
    }

    assert(0);

    return 0;
}

void AnimMesh::CalcInterpolatedScaling(aiVector3D &Out, float AnimationTime, const aiNodeAnim *pNodeAnim)
{
    if (pNodeAnim->mNumScalingKeys == 1) {
        Out = pNodeAnim->mScalingKeys[0].mValue;
        return;
    }

    uint ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
    uint NextScalingIndex = (ScalingIndex + 1);
    assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
    float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;

    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
    const aiVector3D& End   = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}

void AnimMesh::CalcInterpolatedRotation(aiQuaternion &Out, float AnimationTime, const aiNodeAnim *pNodeAnim)
{
    // we need at least two values to interpolate...
    if (pNodeAnim->mNumRotationKeys == 1) {
        Out = pNodeAnim->mRotationKeys[0].mValue;
        return;
    }

    uint RotationIndex = FindRotation(AnimationTime, pNodeAnim);
    uint NextRotationIndex = (RotationIndex + 1);
    assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
    float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
    const aiQuaternion& EndRotationQ   = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
    aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
    Out = Out.Normalize();
}

void AnimMesh::CalcInterpolatedPosition(aiVector3D &Out, float AnimationTime, const aiNodeAnim *pNodeAnim)
{
    if (pNodeAnim->mNumPositionKeys == 1) {
        Out = pNodeAnim->mPositionKeys[0].mValue;
        return;
    }

    uint PositionIndex = FindPosition(AnimationTime, pNodeAnim);
    uint NextPositionIndex = (PositionIndex + 1);
    assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
    float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
    const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}

void AnimMesh::readNodeHierarchy(float AnimationTime, const aiNode *pNode, const glm::mat4 &ParentTransform)
{
   std::string NodeName(pNode->mName.data);
   const aiAnimation* pAnimation = m_pAnimation;
   glm::mat4 NodeTransformation;
   cloneMatrix4x4ToGLM(NodeTransformation, pNode->mTransformation);
   //NodeTransformation = glm::inverse(NodeTransformation);
   const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);
   if (pNodeAnim) {
       // Interpolate scaling and generate scaling transformation matrix
       aiVector3D Scaling;
       CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
       glm::mat4 ScalingM;
       ScalingM[0][0] = Scaling.x;
       ScalingM[1][1] = Scaling.y;
       ScalingM[2][2] = Scaling.z;
       ScalingM[3][3] = 1;

       // Interpolate rotation and generate rotation transformation matrix
       aiQuaternion RotationQ;
       CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
       glm::quat tmpQuat;
       tmpQuat.w = RotationQ.w; tmpQuat.x = RotationQ.x; tmpQuat.y = RotationQ.y; tmpQuat.z = RotationQ.z;
       tmpQuat = glm::inverse(tmpQuat);
       glm::mat4 RotationM = glm::mat4_cast(tmpQuat);

       // Interpolate translation and generate translation transformation matrix
       aiVector3D Translation;
       CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
       glm::mat4 TranslationM;
       TranslationM[0][0] = 1;
       TranslationM[1][1] = 1;
       TranslationM[2][2] = 1;
       TranslationM[3][3] = 1;
       TranslationM[0][3] = Translation.x;
       TranslationM[1][3] = Translation.y;
       TranslationM[2][3] = Translation.z;
       //std::cout << Translation.x  << " " << Translation.y << " " << Translation.z << std::endl;
       // Combine the above transformations
       glm::mat4 tmp;
       tmp[0][0]= -1; tmp[0][0]= 0; tmp[0][0]= 0; tmp[0][0]= 0;
       tmp[0][0]= 0; tmp[1][1]= -1; tmp[0][0]= 0; tmp[0][0]= 0;
       tmp[0][0]= 0; tmp[0][0]= 0; tmp[0][0]= -1; tmp[0][0]= 0;
       tmp[0][0]= 0; tmp[0][0]= 0; tmp[0][0]= 0; tmp[0][0]= 1;
       NodeTransformation = RotationM * TranslationM;
       /*
       std::cout << "Name="<<NodeName<<std::endl;
       std::cout << NodeTransformation[0][0] << " " << NodeTransformation[0][1] << " " << NodeTransformation[0][2] << " " << NodeTransformation[0][3] << std::endl;
       std::cout << NodeTransformation[1][0] << " " << NodeTransformation[1][1] << " " << NodeTransformation[1][2] << " " << NodeTransformation[1][3] << std::endl;
       std::cout << NodeTransformation[2][0] << " " << NodeTransformation[2][1] << " " << NodeTransformation[2][2] << " " << NodeTransformation[2][3] << std::endl;
       std::cout << NodeTransformation[3][0] << " " << NodeTransformation[3][1] << " " << NodeTransformation[3][2] << " " << NodeTransformation[3][3] << std::endl;
       */
   }
   glm::mat4 GlobalTransformation =  NodeTransformation * ParentTransform;
   if (m_BoneMapping.find(NodeName) != m_BoneMapping.end())
   {
       uint BoneIndex = m_BoneMapping[NodeName];
       m_BoneInfo[BoneIndex].FinalTransformation =  m_BoneInfo[BoneIndex].BoneOffset * GlobalTransformation;
   }
   for (uint i = 0 ; i < pNode->mNumChildren ; i++)
   {
       readNodeHierarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
   }
}

void
AnimMesh::update(float tpf)
{
    m_runningTime += tpf;
}

void
AnimMesh::render(const glm::mat4 &modelView, const glm::mat4 &projection)
{
    glBindVertexArray(m_VAO);
    glUseProgram(m_shader->getProgramID());
        this->m_light->render(m_shader->getProgramID());
        glUniformMatrix4fv(m_idModelView, 1, GL_FALSE, glm::value_ptr(modelView) );
        glUniformMatrix4fv(m_idProjection, 1, GL_FALSE, glm::value_ptr(projection) );
        std::vector<glm::mat4> transforms;
        this->boneTransform(m_runningTime, transforms);
        for(uint i = 0; i<transforms.size(); i++)
        {
            assert(i < MAX_BONES);
            /*std::cout << "i="<<i<<std::endl;
            std::cout << transforms[i][0][0] << " " << transforms[i][0][1] << " " << transforms[i][0][2] << " " << transforms[i][0][3] << std::endl;
            std::cout << transforms[i][1][0] << " " << transforms[i][1][1] << " " << transforms[i][1][2] << " " << transforms[i][1][3] << std::endl;
            std::cout << transforms[i][2][0] << " " << transforms[i][2][1] << " " << transforms[i][2][2] << " " << transforms[i][2][3] << std::endl;
            std::cout << transforms[i][3][0] << " " << transforms[i][3][1] << " " << transforms[i][3][2] << " " << transforms[i][3][3] << std::endl;*/
            glUniformMatrix4fv(m_boneLocation[i], 1, GL_TRUE, (const GLfloat*)glm::value_ptr(transforms[i]));
        }


        for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {

            //const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;
            //assert(MaterialIndex < m_Textures.size());
//            if (m_Textures[MaterialIndex]) {
//                m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
 //           }
            glDrawElementsBaseVertex(GL_TRIANGLES,
                                      m_Entries[i].numIndices,
                                      GL_UNSIGNED_INT,
                                      (void*)(sizeof(unsigned int) * m_Entries[i].baseIndex),
                                      m_Entries[i].baseVertex);
        }
        // Make sure the VAO is not changed from the outside

        glUseProgram(0);
        glBindVertexArray(0);
}

void AnimMesh::boneTransform(float timeInSecond, std::vector<glm::mat4> &transforms)
{
    glm::mat4 Identity;
    Identity[0][0] = 1;/* Identity[0][1] = 0; Identity[0][2] = 0; Identity[0][3] = 0;
    Identity[1][0] = 0;*/ Identity[1][1] = 1; /*Identity[1][2] = 0; Identity[1][3] = 0;
    Identity[2][0] = 0; Identity[2][1] = 0; */Identity[2][2] = 1;/* Identity[2][3] = 0;
    Identity[3][0] = 0; Identity[3][1] = 0; Identity[3][2] = 0; */Identity[3][3] = 1;

    float TicksPerSecond = (float)(m_pAnimation->mTicksPerSecond != 0 ? m_pAnimation->mTicksPerSecond : 25.0f);
    float TimeInTicks = timeInSecond * TicksPerSecond;
    float AnimationTime = fmod(TimeInTicks, (float)m_pAnimation->mDuration);
    readNodeHierarchy(AnimationTime, m_pRootNode, Identity);
    transforms.resize(m_NumBones);
    for (uint i = 0 ; i < m_NumBones ; i++) {
        transforms[i] = m_BoneInfo[i].FinalTransformation;// * Identity;
    }
}

/*
void
AnimMesh::render(const glm::mat4 &modelView, const glm::mat4 &projection)
{
    glUseProgram(m_shader->getProgramID());

        this->m_light->render(m_shader->getProgramID());

        GLint idVertex = glGetAttribLocation(m_shader->getProgramID(), "in_Vertex");
        GLint idNormal = glGetAttribLocation(m_shader->getProgramID(), "in_Normal");
        GLint idModelView = glGetUniformLocation(m_shader->getProgramID(), "modelview");
        GLint idProjection = glGetUniformLocation(m_shader->getProgramID(), "projection");

        glEnableVertexAttribArray(idVertex);
        glEnableVertexAttribArray(idNormal);

        glUniformMatrix4fv(idModelView, 1, GL_FALSE, glm::value_ptr(modelView) );
        glUniformMatrix4fv(idProjection, 1, GL_FALSE, glm::value_ptr(projection) );

        for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
            glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
            glVertexAttribPointer(idVertex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
            glVertexAttribPointer(idNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

            //const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

//            if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
//                m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
 //           }

            glDrawElements(GL_TRIANGLES, m_Entries[i].m_numIndices, GL_UNSIGNED_INT, 0);
        }

       // glDrawArrays(GL_TRIANGLES, 0, this->m_numFaces);

        glDisableVertexAttribArray(idVertex);
        glDisableVertexAttribArray(idNormal);

        glUseProgram(0);
}
*/


void AnimMesh::VertexBoneData::addBoneData(uint boneID, float weight)
{
    for (uint i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(IDs) ; i++) {
        if (Weights[i] == 0.0) {
            IDs[i] = boneID;
            Weights[i] = weight;
            return;
        }
    }

    // should never get here - more bones than we have space for
    assert(0);
}
