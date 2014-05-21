#version 130


struct DirectionalLight {
    vec3 direction;
        vec3 halfplane;
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 specularColor;
};

struct Material {
    vec4 ambientFactor;
    vec4 diffuseFactor;
    vec4 specularFactor;
    float shininess;
};

// Light
uniform DirectionalLight u_directionalLight;

// Material
uniform Material u_material;

varying vec3 v_posVertex;
varying vec3 v_ecNormal;

void main() {
    if ( dot(v_posVertex,v_ecNormal) < 0 )
    {
        vec3 inverseVertex = normalize(-v_posVertex);
        vec3 r = normalize(reflect(-u_directionalLight.direction,v_ecNormal));

        vec4 ambiant = u_directionalLight.ambientColor * u_material.ambientFactor;
        vec4 diffuse = clamp( u_directionalLight.diffuseColor * u_material.diffuseFactor * max(dot(v_ecNormal, u_directionalLight.direction), 0.0) , 0.0, 1.0);
        vec4 spec = clamp( u_directionalLight.specularColor * u_material.specularFactor * pow(max(dot(r, inverseVertex), 0.0), 0.3*u_material.shininess), 0.0, 1.0);
        gl_FragColor = ambiant + diffuse + spec;
    }
}

/*
struct DirectionalLight {
    vec3 direction; 
	vec3 halfplane;
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 specularColor; 
};

struct Material {
    vec4 ambientFactor;
    vec4 diffuseFactor; 
    vec4 specularFactor; 
    float shininess;
};

// Light
uniform DirectionalLight u_directionalLight;

// Material
uniform Material u_material;

varying vec3 v_ecNormal;

void main() {
    vec3 ecNormal = v_ecNormal;
    float ecNormalDotLightDirection = max(0.0, dot(ecNormal, u_directionalLight.direction));
    float ecNormalDotLightHalfplane = max(0.0, dot(ecNormal, u_directionalLight.halfplane));

    vec4 ambientLight = u_directionalLight.ambientColor * u_material.ambientFactor;
    vec4 diffuseLight = ecNormalDotLightDirection * u_directionalLight.diffuseColor * u_material.diffuseFactor;
    vec4 specularLight = vec4(0.0);

    if (ecNormalDotLightHalfplane > 0.0)
    {
        specularLight = pow(ecNormalDotLightHalfplane, u_material.shininess) * u_directionalLight.specularColor * u_material.specularFactor;
    }
	 
    gl_FragColor = ambientLight + diffuseLight + specularLight;
}
*/
