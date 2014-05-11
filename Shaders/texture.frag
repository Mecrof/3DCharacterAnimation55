// Version du GLSL

#version 150 core


// Entrée

in vec2 coordTexture;


// Uniform

uniform sampler2D texture;


// Sortie 

out vec4 out_Color;


// Fonction main

void main()
{
    // Couleur du pixel

    out_Color = texture(texture, coordTexture);
}
