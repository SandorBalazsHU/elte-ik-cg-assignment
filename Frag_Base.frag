#version 430
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;
out vec4 fs_out_col;
uniform sampler2D texImage;
uniform vec3 cameraPos;

//light00
uniform vec4 lightPos_00 = vec4(0.0f, 1.0f, 0.0f, 0.0f);
uniform vec3 lightAmbient_00 = vec3(0.125f);
uniform vec3 lightDiffuse_00 = vec3(0.0, 1.0, 1.0);
uniform vec3 lightSpecular_00 = vec3(0.0, 1.0, 1.0);
uniform float lightConstantAttenuation_00 = 1.0;
uniform float lightLinearAttenuation_00 = 0.0;
uniform float lightQuadraticAttenuation_00 = 0.0;
uniform vec3 materialAmbient_00 = vec3(1.0);
uniform vec3 materialDiffuse_00 = vec3(1.0);
uniform vec3 materialSpecular_00 = vec3(1.0);
uniform float shininess_00 = 1.0;

//light01
uniform vec4 lightPos_01 = vec4(0.0f, 1.0f, 0.0f, 0.0f);
uniform vec3 lightAmbient_01 = vec3(0.125f);
uniform vec3 lightDiffuse_01 = vec3(0.0, 1.0, 1.0);
uniform vec3 lightSpecular_01 = vec3(0.0, 1.0, 1.0);
uniform float lightConstantAttenuation_01 = 1.0;
uniform float lightLinearAttenuation_01 = 0.0;
uniform float lightQuadraticAttenuation_01 = 0.0;
uniform vec3 materialAmbient_01 = vec3(1.0);
uniform vec3 materialDiffuse_01 = vec3(1.0);
uniform vec3 materialSpecular_01 = vec3(1.0);
uniform float shininess_01 = 1.0;

//light02
uniform vec4 lightPos_02 = vec4(0.0f, 1.0f, 0.0f, 0.0f);
uniform vec3 lightAmbient_02 = vec3(0.125f);
uniform vec3 lightDiffuse_02 = vec3(0.0, 1.0, 1.0);
uniform vec3 lightSpecular_02 = vec3(0.0, 1.0, 1.0);
uniform float lightConstantAttenuation_02 = 1.0;
uniform float lightLinearAttenuation_02 = 0.0;
uniform float lightQuadraticAttenuation_02 = 0.0;
uniform vec3 materialAmbient_02 = vec3(1.0);
uniform vec3 materialDiffuse_02 = vec3(1.0);
uniform vec3 materialSpecular_02 = vec3(1.0);
uniform float shininess_02 = 1.0;


vec3 calculateLight(vec4 lightPos,
					vec3 La,
					vec3 Ld,
					vec3 Ls,
					float lightConstantAttenuation,
					float lightLinearAttenuation,
					float lightQuadraticAttenuation,
					vec3 Ka,
					vec3 Kd,
					vec3 Ks,
					float Shininess) {

	vec3 normal = normalize( vs_out_norm );
	vec3 ToLight;
	float LightDistance=0.0;
	
	if ( lightPos.w == 0.0 )
	{
		// irány fényforrás (directional light)
		ToLight	= lightPos.xyz;
	}
	else				  
	{
		// pont fényforrás (point light)
		ToLight	= lightPos.xyz - vs_out_pos;
		LightDistance = length(ToLight);
	}

	//  Normalizáljuk a fényforrásba mutató vektort
	ToLight = normalize(ToLight);
	
	// Attenuáció (fényelhalás) kiszámítása
	float Attenuation = 1.0 / ( lightConstantAttenuation + lightLinearAttenuation * LightDistance + lightQuadraticAttenuation * LightDistance * LightDistance);
	
	// Ambiens komponens
	vec3 Ambient = La * Ka;

	// Diffúz komponens
	float DiffuseFactor = max(dot(ToLight,normal), 0.0) * Attenuation;
	vec3 Diffuse = DiffuseFactor * Ld * Kd;
	
	// Spekuláris komponens
	vec3 viewDir = normalize( cameraPos - vs_out_pos ); // A fragmentből a kamerába mutató vektor
	vec3 reflectDir = reflect( -ToLight, normal ); // Tökéletes visszaverődés vektora
	float SpecularFactor = pow(max( dot( viewDir, reflectDir) ,0.0), Shininess) * Attenuation;
	vec3 Specular = SpecularFactor*Ls*Ks;

	return Ambient+Diffuse+Specular;
}

void main()
{
	vec3 light00 = calculateLight(lightPos_00,
				lightAmbient_00,
				lightDiffuse_00,
				lightSpecular_00,
				lightConstantAttenuation_00,
				lightLinearAttenuation_00,
				lightQuadraticAttenuation_00,
				materialAmbient_00,
				materialDiffuse_00,
				materialSpecular_00,
				shininess_00);

	vec3 light01 = calculateLight(lightPos_01,
				lightAmbient_01,
				lightDiffuse_01,
				lightSpecular_01,
				lightConstantAttenuation_01,
				lightLinearAttenuation_01,
				lightQuadraticAttenuation_01,
				materialAmbient_01,
				materialDiffuse_01,
				materialSpecular_01,
				shininess_01);

	vec3 light02 = calculateLight(lightPos_02,
				lightAmbient_02,
				lightDiffuse_02,
				lightSpecular_02,
				lightConstantAttenuation_02,
				lightLinearAttenuation_02,
				lightQuadraticAttenuation_02,
				materialAmbient_02,
				materialDiffuse_02,
				materialSpecular_02,
				shininess_02);

	fs_out_col = vec4( light00+light01+light02, 1.0 ) * texture(texImage, vs_out_tex);
}