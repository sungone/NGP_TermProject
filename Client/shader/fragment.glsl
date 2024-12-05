#version 330 core

//in vec3 fColor;
in vec2 fTexture;
in vec3 fPos; //--- ��ġ��
in vec3 fNormal; //--- ���ؽ� ���̴����� ���� ��ְ�

out vec4 fragColor;

uniform sampler2D tex;
uniform vec3 fColor;

uniform vec3 lightPos; //--- ������ ��ġ
uniform vec3 lightColor; //--- ���� ���α׷����� ������ ���� ����
uniform vec3 viewPos; //--- viewPos �� ����: ī�޶� ��ġ

void main()
{
	float ambientLight = 0.8; //--- �ֺ� ���� ���: 0.0 �� ambientLight �� 1.0
	vec3 ambient = ambientLight * lightColor; //--- �ֺ� ����

	vec3 normalVector = normalize(fNormal); //--- �븻���� ����ȭ�Ѵ�.
	vec3 lightDir = normalize(lightPos-fPos); //--- ǥ��� ������ ��ġ�� ������ ������ �����Ѵ�.
	float diffuseLight = max(dot (fNormal, lightDir), 0.0); //--- N�� L�� ���� ������ ���� ���� (���� ���� ���� �� ���� �Ѵ�.)

	float lc;
	if(lightColor.x>0)
		lc=lightColor.x;
	else if(lightColor.y>0)
		lc=lightColor.y;
	else if(lightColor.z>0)
		lc=lightColor.z;
	
	float diffuse = diffuseLight * lc; //--- ����ݻ�����=����ݻ簪*�������

	float shininess = 128; //--- ���� ���
	vec3 viewDir = normalize(viewPos-fPos); //--- �������� ����
	vec3 reflectDir = reflect(-lightDir, normalVector); //--- �ݻ� ����: reflect �Լ� - �Ի� ������ �ݻ� ���� ���
	float specularLight = max (dot (viewDir, reflectDir), 0.0); //--- V�� R�� ���������� ���� ����: ���� ����
	specularLight = pow(specularLight, shininess); //--- shininess ���� ���־� ���̶���Ʈ�� ������ش�.
	vec3 specular = vec3(specularLight * lightColor.x,specularLight * lightColor.y,specularLight * lightColor.z); //--- �ſ� �ݻ� ����: �ſ�ݻ簪 * �������


	vec3 result = (ambient + diffuse + specular) * fColor; //--- ���� ���� ������ �ȼ� ����: (�ֺ�+����ݻ�+�ſ�ݻ�����)*��ü ����

    if (fTexture.x>0) {
	
        fragColor = texture(tex, fTexture);
        return;
    }

    fragColor = vec4(result, 0.5);

	//fragColor=vec4(fColor,1.0);
}