// Version du GLSL

#version 150 core


// Entrées

in vec2 in_Vertex;
in vec3 in_Color;


// Sortie

out vec3 color;


// Fonction main

void main()
{
    // Position finale du vertex

    gl_Position = vec4(in_Vertex, 0.0, 1.0);


    // Envoi de la couleur au Fragment Shader

    color = in_Color;
}
