#include "pch.h"
#include "GeoMetryHelper.h"
#include "Mesh.h"
#include "ResourceManager.h"

shared_ptr<Mesh> GeoMetryHelper::LoadRectangleBox(const float scale)
{

    std::wstringstream wss;
    wss << scale;
    wstring temp = wstring(wss.str());

    shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Get<Mesh>(L"BOX" + temp);

    if (mesh)
    {
        return mesh;
    }

    vector<Vertex> v;
    v.reserve(24); 

    // ����
    v.push_back({ vec3(-1.0f, 1.0f, -1.0f) * scale, vec2(0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f) });
    v.push_back({ vec3(-1.0f, 1.0f, 1.0f) * scale, vec2(1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f) });
    v.push_back({ vec3(1.0f, 1.0f, 1.0f) * scale, vec2(1.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f) });
    v.push_back({ vec3(1.0f, 1.0f, -1.0f) * scale, vec2(0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f) });

    // �Ʒ���
    v.push_back({ vec3(-1.0f, -1.0f, -1.0f) * scale, vec2(0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f) });
    v.push_back({ vec3(1.0f, -1.0f, -1.0f) * scale, vec2(1.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f) });
    v.push_back({ vec3(1.0f, -1.0f, 1.0f) * scale, vec2(1.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f) });
    v.push_back({ vec3(-1.0f, -1.0f, 1.0f) * scale, vec2(0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f) });

    // �ո�
    v.push_back({ vec3(-1.0f, -1.0f, -1.0f) * scale, vec2(0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f) });
    v.push_back({ vec3(-1.0f, 1.0f, -1.0f) * scale, vec2(1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f) });
    v.push_back({ vec3(1.0f, 1.0f, -1.0f) * scale, vec2(1.0f, 1.0f), vec3(0.0f, 0.0f, -1.0f) });
    v.push_back({ vec3(1.0f, -1.0f, -1.0f) * scale, vec2(0.0f, 1.0f), vec3(0.0f, 0.0f, -1.0f) });

    // �޸�
    v.push_back({ vec3(-1.0f, -1.0f, 1.0f) * scale, vec2(0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f) });
    v.push_back({ vec3(1.0f, -1.0f, 1.0f) * scale, vec2(1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f) });
    v.push_back({ vec3(1.0f, 1.0f, 1.0f) * scale, vec2(1.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f) });
    v.push_back({ vec3(-1.0f, 1.0f, 1.0f) * scale, vec2(0.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f) });

    // ����
    v.push_back({ vec3(-1.0f, -1.0f, 1.0f) * scale, vec2(0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f) });
    v.push_back({ vec3(-1.0f, 1.0f, 1.0f) * scale, vec2(1.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f) });
    v.push_back({ vec3(-1.0f, 1.0f, -1.0f) * scale, vec2(1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f) });
    v.push_back({ vec3(-1.0f, -1.0f, -1.0f) * scale, vec2(0.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f) });

    // ������
    v.push_back({ vec3(1.0f, -1.0f, 1.0f) * scale, vec2(0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f) });
    v.push_back({ vec3(1.0f, -1.0f, -1.0f) * scale, vec2(1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f) });
    v.push_back({ vec3(1.0f, 1.0f, -1.0f) * scale, vec2(1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f) });
    v.push_back({ vec3(1.0f, 1.0f, 1.0f) * scale, vec2(0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f) });

    // �ε��� ����
    vector<uint32> index = {
        0,  1,  2,  0,  2,  3,  // ����
        4,  5,  6,  4,  6,  7,  // �Ʒ���
        8,  9,  10, 8,  10, 11, // �ո�
        12, 13, 14, 12, 14, 15, // �޸�
        16, 17, 18, 16, 18, 19, // ����
        20, 21, 22, 20, 22, 23  // ������
    };

  
    mesh = make_shared<Mesh>();
    mesh->Init(v, index);

    ResourceManager::GetInstance()->Add(L"BOX"+temp, mesh);

    return mesh;

}

shared_ptr<Mesh> GeoMetryHelper::LoadRectangleMesh(const float scale)
{

    std::wstringstream wss;
    wss << scale;
    wstring temp = wstring(wss.str());

    shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Get<Mesh>(L"RECTANGLE" + temp);

    if (mesh)
    {
        return mesh;
    }

    vector<Vertex> vec(4);

    // �ո�
    vec[0] = Vertex(vec3(-scale, -scale, 0), vec2(0.0f, 1.0f), vec3(0.0f, 0.0f, -1.0f), vec3(1.0f, 0.0f, 0.0f));
    vec[1] = Vertex(vec3(-scale, +scale, 0), vec2(0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(1.0f, 0.0f, 0.0f));
    vec[2] = Vertex(vec3(+scale, +scale, 0), vec2(1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(1.0f, 0.0f, 0.0f));
    vec[3] = Vertex(vec3(+scale, -scale, 0), vec2(1.0f, 1.0f), vec3(0.0f, 0.0f, -1.0f), vec3(1.0f, 0.0f, 0.0f));

    vector<uint32> idx(6);

    // �ո�
    idx[0] = 0; idx[1] = 1; idx[2] = 2;
    idx[3] = 0; idx[4] = 2; idx[5] = 3;

    mesh = make_shared<Mesh>();
    mesh->Init(vec, idx);

    ResourceManager::GetInstance()->Add(L"RECTANGLE" + temp, mesh);

    return mesh;
}
