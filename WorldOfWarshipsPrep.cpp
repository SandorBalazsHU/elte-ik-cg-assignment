/*
 * ELTE IK Sz�m�t�g�pes grafika
 * World of Warships
 * S�ndor Bal�zs
 * AZA6NL
 */

#include "WorldOfWarships.h"

void WorldOfWarships::initShaders()
{
	shaderBase = glCreateProgram();
	AssembleProgram(shaderBase, "Vert_Base.vert", "Frag_Base.frag");
	shaderWater = glCreateProgram();
	AssembleProgram(shaderWater, "Vert_Water.vert", "Frag_Water.frag");
	shaderSkyBox = glCreateProgram();
	AssembleProgram(shaderSkyBox, "Vert_SkyBox.vert", "Frag_SkyBox.frag");
}

void WorldOfWarships::cleanShaders()
{
	glDeleteProgram(shaderBase);
	glDeleteProgram(shaderWater);
	glDeleteProgram(shaderSkyBox);
}

void WorldOfWarships::initTextures()
{
	glGenTextures(1, &shipTexture);
	TextureFromFile(shipTexture, "Assets/ship.png");
	SetupTextureSampling(GL_TEXTURE_2D, shipTexture);

	glGenTextures(1, &waterTexture);
	TextureFromFile(waterTexture, "Assets/water_texture.jpg");
	SetupTextureSampling(GL_TEXTURE_2D, waterTexture);

	glGenTextures(1, &waterNormalMapTexture);
	TextureFromFile(waterNormalMapTexture, "Assets/normal.jpg");
	SetupTextureSampling(GL_TEXTURE_2D, waterNormalMapTexture);

	// skybox texture

	glGenTextures(1, &skyboxTexture);
	TextureFromFile(skyboxTexture, "Assets/xpos.png", GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	TextureFromFile(skyboxTexture, "Assets/xneg.png", GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	TextureFromFile(skyboxTexture, "Assets/ypos.png", GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	TextureFromFile(skyboxTexture, "Assets/yneg.png", GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	TextureFromFile(skyboxTexture, "Assets/zpos.png", GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	TextureFromFile(skyboxTexture, "Assets/zneg.png", GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
	SetupTextureSampling(GL_TEXTURE_CUBE_MAP, skyboxTexture, false);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	//glGenerateMipmap(GL_TEXTURE_2D); // Mipmap gener�l�sa

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // biline�ris sz�r�s nagy�t�skor (ez az alap�rtelmezett)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// mi legyen az eredm�ny, ha a text�r�n k�v�lr�l pr�b�lunk mint�t venni?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // v�zszintesen
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // f�gg�legesen

	glGenerateMipmap(GL_TEXTURE_2D); // Mipmap gener�l�sa
}

void WorldOfWarships::cleanTextures()
{
	glDeleteTextures(1, &shipTexture);
	glDeleteTextures(1, &waterTexture);
	glDeleteTextures(1, &waterNormalMapTexture);
	glDeleteTextures(1, &skyboxTexture);
}