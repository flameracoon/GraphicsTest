#include "Mesh.h"

void Cube::CreateMesh() {

    float verticeCube[] = {
        // Back face
        -0.5f, -0.5f, -0.5f,   0, 0, -1,   0,0,   1,0,0,   0,1,0,
         0.5f, -0.5f, -0.5f,   0, 0, -1,   1,0,   1,0,0,   0,1,0,
         0.5f,  0.5f, -0.5f,   0, 0, -1,   1,1,   1,0,0,   0,1,0,
         0.5f,  0.5f, -0.5f,   0, 0, -1,   1,1,   1,0,0,   0,1,0,
        -0.5f,  0.5f, -0.5f,   0, 0, -1,   0,1,   1,0,0,   0,1,0,
        -0.5f, -0.5f, -0.5f,   0, 0, -1,   0,0,   1,0,0,   0,1,0,

        // Front face
        -0.5f, -0.5f, 0.5f,    0,0,1,   0,0,   1,0,0,   0,-1,0,
         0.5f, -0.5f, 0.5f,    0,0,1,   1,0,   1,0,0,   0,-1,0,
         0.5f,  0.5f, 0.5f,    0,0,1,   1,1,   1,0,0,   0,-1,0,
         0.5f,  0.5f, 0.5f,    0,0,1,   1,1,   1,0,0,   0,-1,0,
        -0.5f,  0.5f, 0.5f,    0,0,1,   0,1,   1,0,0,   0,-1,0,
        -0.5f, -0.5f, 0.5f,    0,0,1,   0,0,   1,0,0,   0,-1,0,

        // Left face
        -0.5f,  0.5f,  0.5f, -1,0,0,   1,0,   0,0,-1,   0,1,0,
        -0.5f,  0.5f, -0.5f, -1,0,0,   1,1,   0,0,-1,   0,1,0,
        -0.5f, -0.5f, -0.5f, -1,0,0,   0,1,   0,0,-1,   0,1,0,
        -0.5f, -0.5f, -0.5f, -1,0,0,   0,1,   0,0,-1,   0,1,0,
        -0.5f, -0.5f,  0.5f, -1,0,0,   0,0,   0,0,-1,   0,1,0,
        -0.5f,  0.5f,  0.5f, -1,0,0,   1,0,   0,0,-1,   0,1,0,

        // Right face
         0.5f,  0.5f,  0.5f, 1,0,0,   1,0,   0,0,1,   0,1,0,
         0.5f,  0.5f, -0.5f, 1,0,0,   1,1,   0,0,1,   0,1,0,
         0.5f, -0.5f, -0.5f, 1,0,0,   0,1,   0,0,1,   0,1,0,
         0.5f, -0.5f, -0.5f, 1,0,0,   0,1,   0,0,1,   0,1,0,
         0.5f, -0.5f,  0.5f, 1,0,0,   0,0,   0,0,1,   0,1,0,
         0.5f,  0.5f,  0.5f, 1,0,0,   1,0,   0,0,1,   0,1,0,

         // Bottom face
         -0.5f, -0.5f, -0.5f, 0,-1,0,   0,1,   1,0,0,   0,0,1,
          0.5f, -0.5f, -0.5f, 0,-1,0,   1,1,   1,0,0,   0,0,1,
          0.5f, -0.5f,  0.5f, 0,-1,0,   1,0,   1,0,0,   0,0,1,
          0.5f, -0.5f,  0.5f, 0,-1,0,   1,0,   1,0,0,   0,0,1,
         -0.5f, -0.5f,  0.5f, 0,-1,0,   0,0,   1,0,0,   0,0,1,
         -0.5f, -0.5f, -0.5f, 0,-1,0,   0,1,   1,0,0,   0,0,1,

         // Top face
         -0.5f,  0.5f, -0.5f, 0,1,0,   0,1,   1,0,0,   0,0,-1,
          0.5f,  0.5f, -0.5f, 0,1,0,   1,1,   1,0,0,   0,0,-1,
          0.5f,  0.5f,  0.5f, 0,1,0,   1,0,   1,0,0,   0,0,-1,
          0.5f,  0.5f,  0.5f, 0,1,0,   1,0,   1,0,0,   0,0,-1,
         -0.5f,  0.5f,  0.5f, 0,1,0,   0,0,   1,0,0,   0,0,-1,
         -0.5f,  0.5f, -0.5f, 0,1,0,   0,1,   1,0,0,   0,0,-1
    };
    //Set up VBO
    GLuint VBO;
    glGenVertexArrays(1, &vaoId);
    glGenBuffers(1, &VBO);

    // Bind VAO first
    glBindVertexArray(vaoId);

    // Bind and upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticeCube), verticeCube, GL_STATIC_DRAW);

    // Set vertex attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14* sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 *sizeof(float)));
    glEnableVertexAttribArray(2);

    // Tangent
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8* sizeof(float)));
    glEnableVertexAttribArray(3);

    // Binormal
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11* sizeof(float)));
    glEnableVertexAttribArray(4);

    // Unbind VAO
    glBindVertexArray(0);

    primitiveType = GL_TRIANGLES;
}

void Cube::DrawMesh()
{
	//std::cout << "DRAWOING SQUARE \n";
	//Draw element
    glBindVertexArray(vaoId);
    glDrawArrays(GL_TRIANGLES, 0, 36);//glDepthMask(GL_TRUE);
    glBindVertexArray(0);

}