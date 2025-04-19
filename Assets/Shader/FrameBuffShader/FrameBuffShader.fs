/*
 FILENAME: FreeTypeText.fs
 AUTHOR(S): Keith (100%)
 @version 460 core
 BRIEF: Fragment shader for rendering text with customizable color.
 
 \ParamType TexCoord Input texture coordinates for the character texture.
 \ParamType character A uniform sampler2D representing the character texture.
 \ParamType  textColor A uniform vec3 specifying the RGB color of the text.
 
 This shader takes the texture coordinates, samples the character 
 texture at the specified coordinates, and colors the character based 
 on the provided text color.

 All content � 2024 DigiPen Institute of Technology Singapore. All
 rights reserved.
 */
#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;
in vec4 ourColor;

uniform sampler2D screenTexture;

void main()
{ 
    FragColor =vec4(1.f,1.f,1.f,1.f)*texture(screenTexture, TexCoords); 
}