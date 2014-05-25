#version 150
// Attributes
in vec3 in_Vertex;
in vec2 in_TexCoord;
in vec3 in_Normal;
in ivec4 in_BoneIDs;
in vec4 in_BoneWeight;

// Matrices
uniform mat4 modelview;
uniform mat4 projection;
//in vec3 in_Vertex;
//in vec3 in_Normal;

// Varying
varying vec3 v_posVertex;
varying vec3 v_ecNormal;

void main() {
    v_posVertex = vec3(modelview * vec4(in_Vertex,1.0));
    v_ecNormal = normalize(vec3(modelview * vec4(in_Normal, 0.0)));

    gl_Position =  projection * modelview * vec4( in_Vertex, 1.0f );
}

/*
// Matrices
uniform mat4 modelview;
uniform mat4 projection;

// Attributes
in vec3 in_Vertex;
in vec3 in_Normal;

// Varying
varying vec3 v_ecNormal;

void main() {
    // Calculate and normalize eye space normal
    vec3 ecNormal = vec3(modelview * vec4(in_Normal, 0.0));
    v_ecNormal = ecNormal / length(ecNormal);
    vec4 posVertex = modelview * vec4( in_Vertex, 1.0f );

    gl_Position =  projection * modelview * vec4( in_Vertex, 1.0f );
}
*/
/*
uniform mat4 MVP;
in vec3 position;


out vec3 fColor;
out float intensity;

in vec3 normal;
out vec3 normal_;

void main()
{
	normal_ = gl_NormalMatrix * normal;
	gl_Position = MVP * vec4( position, 1.0f );
}
*/
