/*
 * ELTE IK Számítógépes grafika
 * World of Warships
 * Sándor Balázs
 * AZA6NL
 */

#include "WorldOfWarships.h"

WorldOfWarships::WorldOfWarships() {};
WorldOfWarships::~WorldOfWarships() {};

bool WorldOfWarships::init()
{
	setupDebugCallback();
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);
	GLfloat pointSizeRange[2] = { 0.0f, 0.0f };
	glGetFloatv(GL_POINT_SIZE_RANGE, pointSizeRange);
	glPointSize(std::min(16.0f, pointSizeRange[1]));

	initShaders();
	initGeometry();
	initTextures();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	camera.SetView(glm::vec3(0.0, 25.0, 25.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	return true;
};

void WorldOfWarships::clean()
{
	cleanShaders();
	cleanGeometry();
	cleanTextures();
};

void WorldOfWarships::update(const SUpdateInfo& updateInfo)
{
	elapsedTimeInSec = updateInfo.ElapsedTimeInSec;
	camera.Update(updateInfo.DeltaTimeInSec);
};

void WorldOfWarships::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sceneRender();
	waterRender();
	skyBoxRender();
	renderClean();
}

void WorldOfWarships::renderClean() {
	glUseProgram(0);

	glActiveTexture(GL_TEXTURE0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glBindVertexArray(0);
}

void WorldOfWarships::renderGUI()
{
	//ImGui::ShowDemoWindow();
	if ( ImGui::Begin( "Lighting settings" ) )
	{		
		ImGui::InputFloat("Shininess", &m_Shininess, 0.1f, 1.0f, "%.1f" );
		static float Kaf = 1.0f;
		static float Kdf = 1.0f;
		static float Ksf = 1.0f;
		if ( ImGui::SliderFloat( "Ka", &Kaf, 0.0f, 1.0f ) )
		{
			m_Ka = glm::vec3( Kaf );
		}
		if ( ImGui::SliderFloat( "Kd", &Kdf, 0.0f, 1.0f ) )
		{
			m_Kd = glm::vec3( Kdf );
		}
		if ( ImGui::SliderFloat( "Ks", &Ksf, 0.0f, 1.0f ) )
		{
			m_Ks = glm::vec3( Ksf );
		}

		{
			static glm::vec2 lightPosXZ = glm::vec2( 0.0f );
			lightPosXZ = glm::vec2( m_lightPos.x, m_lightPos.z );
			if ( ImGui::SliderFloat2( "Light Position XZ", glm::value_ptr( lightPosXZ ), -1.0f, 1.0f ) )
			{
				float lightPosL2 = lightPosXZ.x * lightPosXZ.x + lightPosXZ.y * lightPosXZ.y;
				if ( lightPosL2 > 1.0f ) // Ha kívülre esne a körön, akkor normalizáljuk
				{
					lightPosXZ /= sqrtf( lightPosL2 );
					lightPosL2 = 1.0f;
				}

				m_lightPos.x = lightPosXZ.x;
				m_lightPos.z = lightPosXZ.y;
				m_lightPos.y = sqrtf( 1.0f - lightPosL2 );
			}
			ImGui::LabelText( "Light Position Y", "%f", m_lightPos.y );
		}
	}
	ImGui::End();
}