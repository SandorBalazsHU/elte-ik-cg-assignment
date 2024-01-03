#version 430

// pipeline-b�l bej�v� per-fragment attrib�tumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

// kimen� �rt�k - a fragment sz�ne
out vec4 fs_out_col;

// text�ra mintav�telez� objektum
uniform sampler2D texImage;

uniform vec3 cameraPos;

// fenyforras tulajdonsagok
uniform vec4 lightPos = vec4( 0.0, 1.0, 0.0, 0.0);

uniform vec3 La = vec3(0.0, 0.0, 0.0 );
uniform vec3 Ld = vec3(1.0, 1.0, 1.0 );
uniform vec3 Ls = vec3(1.0, 1.0, 1.0 );

uniform float lightConstantAttenuation    = 1.0;
uniform float lightLinearAttenuation      = 0.0;
uniform float lightQuadraticAttenuation   = 0.0;

// anyag tulajdonsagok

uniform vec3 Ka = vec3( 1.0 );
uniform vec3 Kd = vec3( 1.0 );
uniform vec3 Ks = vec3( 1.0 );

uniform float Shininess = 1.0;

/* seg�ts�g:
	    - normaliz�l�s: http://www.opengl.org/sdk/docs/manglsl/xhtml/normalize.xml
	    - skal�ris szorzat: http://www.opengl.org/sdk/docs/manglsl/xhtml/dot.xml
	    - clamp: http://www.opengl.org/sdk/docs/manglsl/xhtml/clamp.xml
		- reflect: http://www.opengl.org/sdk/docs/manglsl/xhtml/reflect.xml
				reflect(be�rkez�_vektor, norm�lvektor);
		- pow: http://www.opengl.org/sdk/docs/manglsl/xhtml/pow.xml
				pow(alap, kitev�);
*/

void main()
{
	// A fragment norm�lvektora
	// MINDIG normaliz�ljuk!
	vec3 normal = normalize( vs_out_norm );
	
	vec3 ToLight; // A f�nyforr�sBA mutat� vektor
	float LightDistance=0.0; // A f�nyforr�st�l vett t�vols�g
	
	if ( lightPos.w == 0.0 ) // ir�ny f�nyforr�s (directional light)
	{
		// Ir�nyf�nyforr�s eset�n minden pont ugyan abb�la az ir�nyb�l van megvil�g�tva
		ToLight	= lightPos.xyz;
		// A t�vols�got 0-n hagyjuk, hogy az attenu�ci� ne v�ltoztassa a f�nyt
	}
	else				  // pont f�nyforr�s (point light)
	{
		// Pontf�nyforr�s eset�n kkisz�moljuk a fragment pontb�l a f�nyforr�sba mutat� vektort, ...
		ToLight	= lightPos.xyz - vs_out_pos;
		// ... �s a t�vols�got a f�nyforr�st�l
		LightDistance = length(ToLight);
	}
	//  Normaliz�ljuk a f�nyforr�sba mutat� vektort
	ToLight = normalize(ToLight);
	
	// Attenu�ci� (f�nyelhal�s) kisz�m�t�sa
	float Attenuation = 1.0 / ( lightConstantAttenuation + lightLinearAttenuation * LightDistance + lightQuadraticAttenuation * LightDistance * LightDistance);
	
	// Ambiens komponens
	// Ambiens f�ny mindenhol ugyanakkora
	vec3 Ambient = La * Ka;

	// Diff�z komponens
	// A diff�z f�nyforr�sb�l �rkez� f�ny mennyis�ge ar�nyos a f�nyforr�sba mutat� vektor �s a norm�lvektor skal�ris szorzat�val
	// �s az attenu�ci�val
	float DiffuseFactor = max(dot(ToLight,normal), 0.0) * Attenuation;
	vec3 Diffuse = DiffuseFactor * Ld * Kd;
	
	// Spekul�ris komponens
	vec3 viewDir = normalize( cameraPos - vs_out_pos ); // A fragmentb�l a kamer�ba mutat� vektor
	vec3 reflectDir = reflect( -ToLight, normal ); // T�k�letes visszaver�d�s vektora
	
	// A spekul�ris komponens a t�k�letes visszaver�d�s ir�nya �s a kamera ir�ny�t�l f�gg.
	// A koncentr�lts�ga cos()^s alakban sz�moljuk, ahol s a f�nyess�get meghat�roz� param�ter.
	// Szint�n f�gg az attenu�ci�t�l.
	float SpecularFactor = pow(max( dot( viewDir, reflectDir) ,0.0), Shininess) * Attenuation;
	vec3 Specular = SpecularFactor*Ls*Ks;

	// normal vector debug:
	// fs_out_col = vec4( normal * 0.5 + 0.5, 1.0 );
	fs_out_col = vec4( Ambient+Diffuse+Specular, 1.0 ) * texture(texImage, vs_out_tex);
}