#version 330 core

//in vec3 fColor;
in vec2 fTexture;
in vec3 fPos; //--- 위치값
in vec3 fNormal; //--- 버텍스 세이더에서 받은 노멀값

out vec4 fragColor;

uniform sampler2D tex;
uniform vec3 fColor;

uniform vec3 lightPos; //--- 조명의 위치
uniform vec3 lightColor; //--- 응용 프로그램에서 설정한 조명 색상
uniform vec3 viewPos; //--- viewPos 값 전달: 카메라 위치

void main()
{
	float ambientLight = 0.8; //--- 주변 조명 계수: 0.0 ≤ ambientLight ≤ 1.0
	vec3 ambient = ambientLight * lightColor; //--- 주변 조명값

	vec3 normalVector = normalize(fNormal); //--- 노말값을 정규화한다.
	vec3 lightDir = normalize(lightPos-fPos); //--- 표면과 조명의 위치로 조명의 방향을 결정한다.
	float diffuseLight = max(dot (fNormal, lightDir), 0.0); //--- N과 L의 내적 값으로 강도 조절 (음의 값을 가질 수 없게 한다.)

	float lc;
	if(lightColor.x>0)
		lc=lightColor.x;
	else if(lightColor.y>0)
		lc=lightColor.y;
	else if(lightColor.z>0)
		lc=lightColor.z;
	
	float diffuse = diffuseLight * lc; //--- 산란반사조명값=산란반사값*조명색상값

	float shininess = 128; //--- 광택 계수
	vec3 viewDir = normalize(viewPos-fPos); //--- 관찰자의 방향
	vec3 reflectDir = reflect(-lightDir, normalVector); //--- 반사 방향: reflect 함수 - 입사 벡터의 반사 방향 계산
	float specularLight = max (dot (viewDir, reflectDir), 0.0); //--- V와 R의 내적값으로 강도 조절: 음수 방지
	specularLight = pow(specularLight, shininess); //--- shininess 승을 해주어 하이라이트를 만들어준다.
	vec3 specular = vec3(specularLight * lightColor.x,specularLight * lightColor.y,specularLight * lightColor.z); //--- 거울 반사 조명값: 거울반사값 * 조명색상값


	vec3 result = (ambient + diffuse + specular) * fColor; //--- 최종 조명 설정된 픽셀 색상: (주변+산란반사+거울반사조명)*객체 색상

    if (fTexture.x>0) {
	
        fragColor = texture(tex, fTexture);
        return;
    }

    fragColor = vec4(result, 0.5);

	//fragColor=vec4(fColor,1.0);
}