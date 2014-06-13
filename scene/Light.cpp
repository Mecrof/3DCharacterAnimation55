#include "Light.hpp"

namespace scene
{
namespace SpotLight
{

    Light::Light()
    {
        this->m_Light = new DirectionalLight;
        this->m_Material = new Material;

        this->m_Light->ambientColor[0] = 1.0f;
        this->m_Light->ambientColor[1] = 1.0f;
        this->m_Light->ambientColor[2] = 1.0f;

        this->m_Light->diffuseColor[0] = 0.8f;
        this->m_Light->diffuseColor[1] = 0.8f;
        this->m_Light->diffuseColor[2] = 0.8f;

        this->m_Light->specularColor[0] = 0.0f;
        this->m_Light->specularColor[1] = 0.0f;
        this->m_Light->specularColor[2] = 0.0f;

        this->m_Light->direction[0] = 1.0f;
        this->m_Light->direction[1] = 1.0f;
        this->m_Light->direction[2] = 1.0f;

        this->m_Light->halfPlane[0] = 1.0f;
        this->m_Light->halfPlane[1] = 1.0f;
        this->m_Light->halfPlane[2] = 1.0f;

        this->m_Material->ambientFactor[0] = 10.0f;
        this->m_Material->ambientFactor[1] = 10.0f;
        this->m_Material->ambientFactor[2] = 10.0f;

        this->m_Material->diffuseFactor[0] = 1.0f;
        this->m_Material->diffuseFactor[1] = 1.0f;
        this->m_Material->diffuseFactor[2] = 1.0f;

        this->m_Material->specularFactor[0] = 1.0f;
        this->m_Material->specularFactor[1] = 1.0f;
        this->m_Material->specularFactor[2] = 1.0f;

        this->m_Material->shininess = 1;
    }

    Light::Light(DirectionalLight *light, Material *material) : m_Light(light), m_Material(material)
    {

    }

    Light::~Light()
    {

    }

    DirectionalLight* Light::getDirectionalLight()
    {
        return this->m_Light;
    }

    Material* Light::getMaterial()
    {
        return this->m_Material;
    }

    void Light::render(GLuint shaderId)
    {
        GLint lightId1 = glGetUniformLocation(shaderId, "u_directionalLight.ambientColor");
        GLint lightId2 = glGetUniformLocation(shaderId, "u_directionalLight.diffuseColor");
        GLint lightId3 = glGetUniformLocation(shaderId, "u_directionalLight.specularColor");
        GLint lightId4 = glGetUniformLocation(shaderId, "u_directionalLight.direction");
        GLint lightId5 = glGetUniformLocation(shaderId, "u_directionalLight.halfplane");

        glUniform4f(lightId1, this->m_Light->ambientColor[0],  this->m_Light->ambientColor[1],  this->m_Light->ambientColor[2],  1.0f);
        glUniform4f(lightId2, this->m_Light->diffuseColor[0],  this->m_Light->diffuseColor[1],  this->m_Light->diffuseColor[2],  1.0f);
        glUniform4f(lightId3, this->m_Light->specularColor[0], this->m_Light->specularColor[1], this->m_Light->specularColor[2], 1.0f);
        glUniform3f(lightId4, this->m_Light->direction[0],     this->m_Light->direction[1],     this->m_Light->direction[2]);
        glUniform3f(lightId5, this->m_Light->halfPlane[0],     this->m_Light->halfPlane[1],     this->m_Light->halfPlane[2]);

        GLint matId1 = glGetUniformLocation(shaderId, "u_material.ambientFactor");
        GLint matId2 = glGetUniformLocation(shaderId, "u_material.diffuseFactor");
        GLint matId3 = glGetUniformLocation(shaderId, "u_material.specularFactor");
        GLint matId4 = glGetUniformLocation(shaderId, "u_material.shininess");

        glUniform4f(matId1, this->m_Material->ambientFactor[0],  this->m_Material->ambientFactor[1],  this->m_Material->ambientFactor[2],  1.0f);
        glUniform4f(matId2, this->m_Material->diffuseFactor[0],  this->m_Material->diffuseFactor[1],  this->m_Material->diffuseFactor[2],  1.0f);
        glUniform4f(matId3, this->m_Material->specularFactor[0], this->m_Material->specularFactor[1], this->m_Material->specularFactor[2], 1.0f);
        glUniform1f(matId4, this->m_Material->shininess);
    }
}
}
