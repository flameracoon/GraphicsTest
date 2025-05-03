#pragma once
#ifndef  TEXTURE_H
#define TEXTURE_H

/*****************************************************************
FILENAME:   Texture.h
AUTHOR(S):  Gabe (100%)
BRIEF:      Loads a texture form file path.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
******************************************************************/
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>std::
#include <map>
#include <string>
#include <vector>
#include <stdlib.h>

enum TextureType{
	REGULAR=0,
	DIFFUSE=1,
	SPECULAR=2,
	NORMAL=3,
	HEIGHT=4,
	ROUGHNESS
};
class Texture {

public:
	//Set texture
	/************************************************************************/
	/*!
	\brief
	Constructor for the `Texture` class. Loads a texture from a file and sets texture parameters.

	\param texturePath
	Path to the texture image file.

	\param texName
	The name of the texture.

	\return
	NIL
	*/
	/************************************************************************/
	Texture(const char* texturePath, std::string texName) : name{ texName }, texture{ 0 } {
		//Set texture
		glGenTextures(1, &texture);
		//Bind textures
		glBindTexture(GL_TEXTURE_2D, texture);
		//Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Load texture
		int nrChannels;
		unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
		//Load texture
		if (!data) {
			std::cout << "FAILED TO LOAD TEXURE";
		}
		std::cout << "Channel of [" << texName << "] is " << nrChannels << std::endl;
		if (nrChannels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else if (nrChannels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
		//Free texture
		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	Texture(std::string texName, unsigned char* data, int newWidth, int newHeight, int nrChannels)
		:name{ texName }, width{ newWidth }, height{ newHeight } {

		glGenTextures(1, &texture);
		//Bind textures
		glBindTexture(GL_TEXTURE_2D, texture);
		//Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (nrChannels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else if (nrChannels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

	}
	//void LoadTextureFromJSON(const std::string& filePath);

	/************************************************************************/
	/*!
	\brief
	Frees the texture data and deletes the texture from OpenGL.

	\return
	NIL
	*/
	/************************************************************************/
	void FreeTexture() {
		//std::cout << "Freeing data";
		//textureList.erase(name);
		glDeleteTextures(1, &texture);
	}

	/************************************************************************/
	/*!
	\brief
	Activates the texture and binds it to the current OpenGL context.

	\return
	NIL
	*/
	/************************************************************************/
	void Use() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	/************************************************************************/
	/*!
	\brief
	Returns the OpenGL texture ID.

	\return
	The texture ID (`GLuint`).
	*/
	/************************************************************************/
	//Access width and height
	unsigned int RetrieveTexture() {
		return texture;
	}
	/************************************************************************/
	/*!
	\brief
	Returns the OpenGL texture ID pointer.

	\return
	The texture ID (`GLuint`) pointer.
	*/
	/************************************************************************/
	//Access width and height
	unsigned int* RetrieveTexturePointer() {
		return &texture;
	}

	/************************************************************************/
	/*!
	\brief
	Retrieves the width of the texture.

	\return
	The width of the texture in pixels.
	*/
	/************************************************************************/
	int RetrieveWidth() const {
		return width;
	}
	/************************************************************************/
	/*!
	\brief
	Retrieves the height of the texture.

	\return
	The height of the texture in pixels.
	*/
	/************************************************************************/
	int RetrieveHeight() const {
		return height;
	}
	/************************************************************************/
	/*!
	\brief
	Retrieves the width and height of the texture as a pair.

	\return
	A `std::pair<int, int>` containing the width and height of the texture.
	*/
	/************************************************************************/
	std::pair<int, int>RetrieveWH()const {
		return std::pair<int, int>{ width, height };
	}
	/************************************************************************/
	/*!
	\brief
	Returns the name of the texture.

	\return
	The name of the texture (`std::string`).
	*/
	/************************************************************************/
	std::string RetrieveName()const {
		return name;
	};
	TextureType RetrieveType() { return type;; }
	void SetType(TextureType typ) { type = typ; }
private:
	unsigned int texture;
	int width, height;
	std::string name;
	TextureType type{ TextureType::REGULAR};
};

class TileSet {
public:
	TileSet(const char* texturePath, std::string texName, int newRow = 1, int newCol = 1)
		:texPath{ texturePath }, name{ texName }, row{ newRow }, col{ newCol } {
		//open data
		int nrChannels;
		unsigned char* data = stbi_load(texturePath, &ogWidth, &ogHeight, &nrChannels, 0);
		//Find each tile set interval
		int widthInterval{ ogWidth / row }, heightInterval{ ogHeight / col };

		int lastWidth{ widthInterval }, lastHeight{ heightInterval };
		//Get new width/height if not properly divisible by number
		lastWidth = ogWidth % col ? widthInterval - (ogWidth - widthInterval * col) : lastWidth;
		lastHeight = ogHeight % row ? heightInterval - (ogHeight - heightInterval * row) : lastHeight;
		//Iterate through to create different textures based on tileset
		for (int r{ row - 1 }; r >= 0; r--) {
			for (int c{ col - 1 }; c >= 0; c--) {
				std::string newName = name + std::string{ '(' + std::to_string((c + 1) + (r * col)) + ")" };
				std::cout << newName << '\n';
				//Copy pixel data over 
				unsigned char* newData = new unsigned char[widthInterval * heightInterval * nrChannels];
				for (int nR = 0; nR < heightInterval; ++nR) {
					for (int nC = 0; nC < widthInterval; ++nC) {

						int x{ c * widthInterval }, y{ r * heightInterval };
						//std::cout << x<<' ' << y << '\n';
						int img_index = (((x + nR)) * ogWidth + (y + nC)) * nrChannels;
						int quarter_index = (nR * widthInterval + nC) * nrChannels;

						{
							for (int chan = 0; chan < nrChannels; ++chan) {
								newData[quarter_index + chan] = data[img_index + chan];
							}
						}
					}
				}
				//Generate and add new texture to the tile set
				tilesetTextures.push_back(Texture{ newName,newData,widthInterval
					,heightInterval,nrChannels });
				//Free data
				delete[] newData;
			}
		}
		stbi_image_free(data);

	}
	void UpdateTileSet(int newRow, int newCol) {
		//Clear old data
		row = newRow;
		col = newCol;
		for (Texture& tex : tilesetTextures) {
			tex.FreeTexture();
		}
		tilesetTextures.clear();

		//open data
		int nrChannels;
		unsigned char* data = stbi_load(texPath.c_str(), &ogWidth, &ogHeight, &nrChannels, 0);
		//Find each tile set interval
		int widthInterval{ ogWidth / row }, heightInterval{ ogHeight / col };
		//Iterate through to create different textures based on tileset
		for (int r{ row - 1 }; r >= 0; r--) {
			for (int c{ col - 1 }; c >= 0; c--) {
				std::string newName = name + std::string{ '(' + std::to_string((c + 1) + (r * col)) + ")" };

				//Copy pixel data over 
				unsigned char* newData = new unsigned char[widthInterval * heightInterval * nrChannels];
				for (int nR = 0; nR < heightInterval; ++nR) {

					int img_index = (nR * ogWidth) * nrChannels;
					int quarter_index = (nR * widthInterval) * nrChannels;


					memcpy(newData + quarter_index, data + img_index, widthInterval * nrChannels);
				}
				//Generate and add new texture to the tile set
				tilesetTextures.push_back(Texture{ newName,newData,widthInterval
					,heightInterval,nrChannels });
				//Free data
				delete[] newData;
			}
		}
		stbi_image_free(data);
	}
	void FreeTexture() {
		//std::cout << "Freeing data";
		//textureList.erase(name);
		//glDeleteTextures(1, &texture);
		for (Texture& tex : tilesetTextures) {
			tex.FreeTexture();
		}
		tilesetTextures.clear();
	}

	void Use(unsigned int) {
		//glActiveTexture(GL_TEXTURE0);
		//std::find_if(tilesetTextures.begin(), tilesetTextures.end(), [](auto& val) {return val.RetrieveName();})->RetrieveTexture()
		//
	}

	int RetrieveRow()const { return row; };
	int RetireveCol()const { return col; };
	std::vector<Texture>& RetrieveTextureList() {
		return tilesetTextures;
	}

private:
	std::vector<Texture>tilesetTextures;
	int row, col;
	int ogWidth, ogHeight;
	std::string name;
	std::string texPath;
};


#endif