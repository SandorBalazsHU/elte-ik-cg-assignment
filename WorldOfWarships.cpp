/*
 * ELTE IK Számítógépes grafika
 * World of Warships
 * Sándor Balázs
 * AZA6NL
 */

#include "WorldOfWarships.h"

WorldOfWarships::WorldOfWarships() {};
WorldOfWarships::~WorldOfWarships() {};

void WorldOfWarships::InitShaders()
{
	m_programID = glCreateProgram();
	AssembleProgram( m_programID, "Vert_PosNormTex.vert", "Frag_LightingNoFaceCull.frag" );
	m_programWaterID = glCreateProgram();
	AssembleProgram( m_programWaterID, "Vert_Water.vert", "Frag_Lighting_w_Spot_XZPlaneNormalMap.frag" );
	m_programAxesID = glCreateProgram();
	AssembleProgram( m_programAxesID, "Vert_axes.vert", "Frag_PosCol.frag" );
	m_programTrajectoryID = glCreateProgram();
	AssembleProgram( m_programTrajectoryID, "Vert_traj.vert", "Frag_PosCol.frag" );
	
	InitSkyboxShaders();
}

void WorldOfWarships::InitSkyboxShaders()
{
	m_programSkyboxID = glCreateProgram();
	AssembleProgram( m_programSkyboxID, "Vert_skybox.vert", "Frag_skybox.frag" );
}

void WorldOfWarships::CleanShaders()
{
	glDeleteProgram( m_programID );
	glDeleteProgram( m_programWaterID );
	glDeleteProgram( m_programAxesID );
	glDeleteProgram( m_programTrajectoryID );

	CleanSkyboxShaders();
}

void WorldOfWarships::CleanSkyboxShaders()
{
	glDeleteProgram( m_programSkyboxID );
}

struct Param
{
	glm::vec3 GetPos(float u, float v) const noexcept
	{
        return glm::vec3(u, v, 0.0);
    }

	glm::vec3 GetNorm(float u, float v) const noexcept
	{
        return glm::vec3(0.0, 0.0, 1.0);
    }

	glm::vec2 GetTex( float u, float v ) const noexcept
	{
        return glm::vec2(u, v);
    }
};

struct Water
{
	glm::vec3 GetPos(float u, float v) const noexcept
	{
		glm::vec3 pos = glm::vec3(-10.0, 0.0, 10.0) + glm::vec3( 20.0, 0.0, -20.0) * glm::vec3(u, 0.0, v);
		pos.y = sinf(pos.z);

		return pos;
	}

	glm::vec3 GetNorm(float u, float v) const noexcept
	{
		glm::vec3 du = GetPos(u + 0.01f, v) - GetPos(u - 0.01f, v);
		glm::vec3 dv = GetPos(u, v + 0.01f) - GetPos(u, v - 0.01f);

		return glm::normalize(glm::cross(du, dv));
	}

	glm::vec2 GetTex( float u, float v ) const noexcept
	{
        return glm::vec2(u, v);
    }
};

void WorldOfWarships::InitGeometry()
{

	const std::initializer_list<VertexAttributeDescriptor> vertexAttribList =
	{
		{ 0, offsetof( Vertex, position ), 3, GL_FLOAT },
		{ 1, offsetof( Vertex, normal   ), 3, GL_FLOAT },
		{ 2, offsetof( Vertex, texcoord ), 2, GL_FLOAT },
	};

	// Suzanne

	MeshObject<Vertex> suzanneMeshCPU = ObjParser::parse("Assets/ship.obj");

	m_SuzanneGPU = CreateGLObjectFromMesh( suzanneMeshCPU, vertexAttribList );

	// quad
	MeshObject<Vertex> quadMeshCPU;
    quadMeshCPU.vertexArray =
	{
        { glm::vec3(-1.0, -1.0, 0.0), glm::vec3(0.0, 0.0,  1.0), glm::vec2(0.0,0.0) }, // első lap
        { glm::vec3( 1.0, -1.0, 0.0), glm::vec3(0.0, 0.0,  1.0), glm::vec2(1.0,0.0) },
        { glm::vec3( 1.0,  1.0, 0.0), glm::vec3(0.0, 0.0,  1.0), glm::vec2(1.0,1.0) },
        { glm::vec3(-1.0,  1.0, 0.0), glm::vec3(0.0, 0.0,  1.0), glm::vec2(0.0,1.0) }
    };

    quadMeshCPU.indexArray =
	{
        0, 1, 2, // első lap
        2, 3, 0
    };
    
	m_quadGPU = CreateGLObjectFromMesh( quadMeshCPU, vertexAttribList );

	// Skybox
	InitSkyboxGeometry();

	// Water
	MeshObject<glm::vec2> waterCPU;
	{
		MeshObject<Vertex> surfaceMeshCPU = GetParamSurfMesh( Param(), 1000, 1000 );
		for ( const Vertex& v : surfaceMeshCPU.vertexArray )
		{
			waterCPU.vertexArray.emplace_back( glm::vec2( v.position.x, v.position.y ) );
		}
		waterCPU.indexArray = surfaceMeshCPU.indexArray;
	}
	m_waterGPU = CreateGLObjectFromMesh( waterCPU, { { 0, offsetof( glm::vec2,x), 2, GL_FLOAT}});
}

void WorldOfWarships::CleanGeometry()
{
	CleanOGLObject( m_SuzanneGPU );
	CleanSkyboxGeometry();
}

void WorldOfWarships::InitSkyboxGeometry()
{
	// skybox geo
	MeshObject<glm::vec3> skyboxCPU =
	{
		std::vector<glm::vec3>
		{
			// hátsó lap
			glm::vec3(-1, -1, -1),
			glm::vec3( 1, -1, -1),
			glm::vec3( 1,  1, -1),
			glm::vec3(-1,  1, -1),
			// elülső lap
			glm::vec3(-1, -1, 1),
			glm::vec3( 1, -1, 1),
			glm::vec3( 1,  1, 1),
			glm::vec3(-1,  1, 1),
		},

		std::vector<GLuint>
		{
			// hátsó lap
			0, 1, 2,
			2, 3, 0,
			// elülső lap
			4, 6, 5,
			6, 4, 7,
			// bal
			0, 3, 4,
			4, 3, 7,
			// jobb
			1, 5, 2,
			5, 6, 2,
			// alsó
			1, 0, 4,
			1, 4, 5,
			// felső
			3, 2, 6,
			3, 6, 7,
		}
	};

	m_SkyboxGPU = CreateGLObjectFromMesh( skyboxCPU, { { 0, offsetof( glm::vec3,x ), 3, GL_FLOAT } } );
}

void WorldOfWarships::CleanSkyboxGeometry()
{
	CleanOGLObject( m_SkyboxGPU );
}

void WorldOfWarships::InitTextures()
{
	// diffuse texture

	glGenTextures( 1, &m_SuzanneTextureID );
	TextureFromFile( m_SuzanneTextureID, "Assets/ship.png" );
	SetupTextureSampling( GL_TEXTURE_2D, m_SuzanneTextureID );

	glGenTextures( 1, &m_waterTextureID );
	TextureFromFile( m_waterTextureID, "Assets/water_texture.jpg" );
	SetupTextureSampling( GL_TEXTURE_2D, m_waterTextureID );

	glGenTextures(1, &m_woodNormalMapTextureID);
	//TextureFromFile(m_woodNormalMapTextureID, "Assets/normal.jpg");
	TextureFromFile(m_woodNormalMapTextureID, "Assets/normal.jpg");
	SetupTextureSampling(GL_TEXTURE_2D, m_woodNormalMapTextureID);

	InitSkyboxTextures();
}

void WorldOfWarships::CleanTextures()
{
	glDeleteTextures( 1, &m_SuzanneTextureID );
	glDeleteTextures( 1, &m_waterTextureID );
	glDeleteTextures( 1, &m_woodNormalMapTextureID);
	CleanSkyboxTextures();
}

void WorldOfWarships::InitSkyboxTextures()
{
	// skybox texture

	glGenTextures( 1, &m_skyboxTextureID );
	TextureFromFile(m_skyboxTextureID, "Assets/xpos.png", GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	TextureFromFile( m_skyboxTextureID, "Assets/xneg.png", GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_NEGATIVE_X );
	TextureFromFile( m_skyboxTextureID, "Assets/ypos.png", GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_POSITIVE_Y );
	TextureFromFile( m_skyboxTextureID, "Assets/yneg.png", GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y );
	TextureFromFile( m_skyboxTextureID, "Assets/zpos.png", GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_POSITIVE_Z );
	TextureFromFile( m_skyboxTextureID, "Assets/zneg.png", GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z );
	SetupTextureSampling( GL_TEXTURE_CUBE_MAP, m_skyboxTextureID, false );

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	//glGenerateMipmap(GL_TEXTURE_2D); // Mipmap generálása

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // bilineáris szürés nagyításkor (ez az alapértelmezett)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// mi legyen az eredmény, ha a textúrán kívülröl próbálunk mintát venni?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // vízszintesen
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // függölegesen

	glGenerateMipmap(GL_TEXTURE_2D); // Mipmap generálása

}

void WorldOfWarships::CleanSkyboxTextures()
{
	glDeleteTextures( 1, &m_skyboxTextureID);
}

bool WorldOfWarships::Init()
{
	setupDebugCallback();

	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);
	
	// Nem minden driver támogatja a vonalak és pontok vastagabb megjelenítését, ezért
	// lekérdezzük, hogy támogatott-e a GL_LINE_WIDTH_RANGE és GL_POINT_SIZE_RANGE tokenek.
	{
		// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glPointSize.xhtml
		GLfloat pointSizeRange[2] = { 0.0f, 0.0f };
		glGetFloatv(GL_POINT_SIZE_RANGE, pointSizeRange); // lekérdezzük a támogatott pontméretek tartományát
		glPointSize( std::min( 16.0f, pointSizeRange[ 1 ] ) ); // nagyobb pontok
	}

	/* {
		// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glLineWidth.xhtml
		GLfloat lineWidthRange[2] = { 0.0f, 0.0f };
		glGetFloatv(GL_LINE_WIDTH_RANGE, lineWidthRange); // lekérdezzük a támogatott vonalvastagság tartományát
		glLineWidth( std::min( 4.0f, lineWidthRange[ 1 ] ) ); // vastagabb vonalak
	}*/

	InitShaders();
	InitGeometry();
	InitTextures();

	//
	// egyéb inicializálás
	//

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hátrafelé néző lapok eldobását
	glCullFace(GL_BACK);    // GL_BACK: a kamerától "elfelé" néző lapok, GL_FRONT: a kamera felé néző lapok

	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

	// kamera
	m_camera.SetView(
		glm::vec3(0.0, 25.0, 25.0),	// honnan nézzük a színteret	   - eye
		glm::vec3(0.0, 0.0, 0.0),   // a színtér melyik pontját nézzük - at
		glm::vec3(0.0, 1.0, 0.0));  // felfelé mutató irány a világban - up

	return true;
}

void WorldOfWarships::Clean()
{
	CleanShaders();
	CleanGeometry();
	CleanTextures();
}

void WorldOfWarships::Update( const SUpdateInfo& updateInfo )
{
	m_ElapsedTimeInSec = updateInfo.ElapsedTimeInSec;

	m_camera.Update( updateInfo.DeltaTimeInSec );
}

void WorldOfWarships::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT)...
	// ... és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Suzanne

	glBindVertexArray( m_SuzanneGPU.vaoID );

	// - Textúrák beállítása, minden egységre külön
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, m_SuzanneTextureID );


	glUseProgram( m_programID );

	glm::vec3 pos = glm::vec3(0.0);
	pos.y = sin((pos.z + m_ElapsedTimeInSec) / 8.0) + sin((pos.y + pos.x + m_ElapsedTimeInSec) / 6.0);
	glm::mat4 matWorld = glm::translate(pos);

	glUniformMatrix4fv( ul( "world" ),    1, GL_FALSE, glm::value_ptr( matWorld ) );
	glUniformMatrix4fv( ul( "worldIT" ),  1, GL_FALSE, glm::value_ptr( glm::transpose( glm::inverse( matWorld ) ) ) );

	glUniformMatrix4fv( ul( "viewProj" ), 1, GL_FALSE, glm::value_ptr( m_camera.GetViewProj() ) );

	// - Fényforrások beállítása
	glUniform3fv( ul( "cameraPos" ), 1, glm::value_ptr( m_camera.GetEye() ) );
	glUniform4fv( ul( "lightPos" ),  1, glm::value_ptr( m_lightPos ) );

	glUniform3fv( ul( "La" ),		 1, glm::value_ptr( m_La ) );
	glUniform3fv( ul( "Ld" ),		 1, glm::value_ptr( m_Ld ) );
	glUniform3fv( ul( "Ls" ),		 1, glm::value_ptr( m_Ls ) );

	glUniform1f( ul( "lightConstantAttenuation"	 ), m_lightConstantAttenuation );
	glUniform1f( ul( "lightLinearAttenuation"	 ), m_lightLinearAttenuation   );
	glUniform1f( ul( "lightQuadraticAttenuation" ), m_lightQuadraticAttenuation);

	// - Anyagjellemzők beállítása
	glUniform3fv( ul( "Ka" ),		 1, glm::value_ptr( m_Ka ) );
	glUniform3fv( ul( "Kd" ),		 1, glm::value_ptr( m_Kd ) );
	glUniform3fv( ul( "Ks" ),		 1, glm::value_ptr( m_Ks ) );

	glUniform1f( ul( "Shininess" ),	m_Shininess );


	// - textúraegységek beállítása
	glUniform1i( ul( "texImage" ), 0 );

	glDrawElements( GL_TRIANGLES,    
					m_SuzanneGPU.count,			 
					GL_UNSIGNED_INT,
					nullptr );

	// Viz

	glDisable(GL_CULL_FACE);

	glBindVertexArray( m_waterGPU.vaoID );

	// - Textúrák beállítása, minden egységre külön
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, m_waterTextureID );

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_woodNormalMapTextureID);


	glUseProgram( m_programWaterID );

	matWorld = glm::translate(glm::vec3(0.0,0.1,0.0)); // toljuk lejjebb a vizet
	//matWorld = glm::translate(glm::vec3(0.0,-2.0,0.0)); // toljuk lejjebb a vizet
	//matWorld = glm::identity<glm::mat4>(); // toljuk lejjebb a vizet

	// Mivel másik shader-t használunk, ezért újra be kell állítani a uniform paramétereket
	glUniformMatrix4fv( ul( "world" ),    1, GL_FALSE, glm::value_ptr( matWorld ) );
	glUniformMatrix4fv( ul( "worldIT" ),  1, GL_FALSE, glm::value_ptr( glm::transpose( glm::inverse( matWorld ) ) ) );

	glUniformMatrix4fv( ul( "viewProj" ), 1, GL_FALSE, glm::value_ptr( m_camera.GetViewProj() ) );

	// - Fényforrások beállítása
	glUniform3fv( ul( "cameraPos" ), 1, glm::value_ptr( m_camera.GetEye() ) );
	glUniform4fv( ul( "lightPos" ),  1, glm::value_ptr( m_lightPos ) );

	glUniform3fv( ul( "La" ),		 1, glm::value_ptr( m_La ) );
	glUniform3fv( ul( "Ld" ),		 1, glm::value_ptr( m_Ld ) );
	glUniform3fv( ul( "Ls" ),		 1, glm::value_ptr( m_Ls ) );

	glUniform1f( ul( "lightConstantAttenuation"	 ), m_lightConstantAttenuation );
	glUniform1f( ul( "lightLinearAttenuation"	 ), m_lightLinearAttenuation   );
	glUniform1f( ul( "lightQuadraticAttenuation" ), m_lightQuadraticAttenuation);

	// - Anyagjellemzők beállítása
	glUniform3fv( ul( "Ka" ),		 1, glm::value_ptr( m_Ka ) );
	glUniform3fv( ul( "Kd" ),		 1, glm::value_ptr( m_Kd ) );
	glUniform3fv( ul( "Ks" ),		 1, glm::value_ptr( m_Ks ) );

	glUniform1f( ul( "Shininess" ),	m_Shininess );

	glUniform1f( ul( "ElapsedTimeInSec" ),	m_ElapsedTimeInSec );

	glUniform1i(ul("texImage"), 0);
	glUniform1i(ul("texNormalMap"), 1);

	glDrawElements( GL_TRIANGLES,    
					m_waterGPU.count,			 
					GL_UNSIGNED_INT,
					nullptr );

	glEnable(GL_CULL_FACE);

	//
	// skybox
	//

	// - VAO
	glBindVertexArray( m_SkyboxGPU.vaoID );

	// - Textura
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_CUBE_MAP, m_skyboxTextureID );

	// - Program
	glUseProgram( m_programSkyboxID );

	// - uniform parameterek
	glUniformMatrix4fv( ul("world"),    1, GL_FALSE, glm::value_ptr( glm::translate( m_camera.GetEye() ) ) );
	glUniformMatrix4fv( ul("viewProj"), 1, GL_FALSE, glm::value_ptr( m_camera.GetViewProj() ) );

	// - textúraegységek beállítása
	glUniform1i( ul( "skyboxTexture" ), 0 );

	// mentsük el az előző Z-test eredményt, azaz azt a relációt, ami alapján update-eljük a pixelt.
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);

	// most kisebb-egyenlőt használjunk, mert mindent kitolunk a távoli vágósíkokra
	glDepthFunc(GL_LEQUAL);

	// - Rajzolas
	glDrawElements( GL_TRIANGLES, m_SkyboxGPU.count, GL_UNSIGNED_INT, nullptr );

	glDepthFunc(prevDepthFnc);

	// shader kikapcsolasa
	glUseProgram( 0 );

	// - Textúrák kikapcsolása, minden egységre külön
	glActiveTexture( GL_TEXTURE0 );
	glActiveTexture(GL_TEXTURE1);
	glBindTexture( GL_TEXTURE_2D, 0 );
	glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );

	// VAO kikapcsolása
	glBindVertexArray( 0 );
}

void WorldOfWarships::RenderGUI()
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

GLint WorldOfWarships::ul( const char* uniformName ) noexcept
{
	GLuint programID = 0;

	// Kérdezzük le az aktuális programot!
	// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGet.xhtml
	glGetIntegerv( GL_CURRENT_PROGRAM, reinterpret_cast<GLint*>( &programID ) );
	// A program és a uniform név ismeretében kérdezzük le a location-t!
	// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetUniformLocation.xhtml
	return glGetUniformLocation( programID, uniformName );
}