#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexture;
//layout(location = 3) in vec3 vColor;

out vec3 fPos; //--- 객체의 위치값을 프래그먼트 세이더로 보낸다.
out vec3 fNormal; //--- 노멀값을 프래그먼트 세이더로 보낸다.
out vec2 fTexture;
out vec3 fColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() 
{

    gl_Position = projection * view * model * vec4(vPos, 1.0);

    fPos = vec3(model * vec4(vPos, 1.0)); //--- 객체에 대한 조명 계산을 프래그먼트 셰이더에서 한다. 
    //--- 따라서 월드공간에 있는 버텍스 값을 프래그먼트 셰이더로 보낸다.
   // fNormal = vec3(model * vec4(vNormal, 1.0));
    fNormal = vNormal; //--- 노멀값을 프래그먼트 세이더로 보낸다.

    fTexture = vTexture;
   // fColor = vColor;

}