#include "pch.h"
#include "Terrain.h"
#include "Mesh.h"
#include "Transform.h"
#include "Material.h"
#include "Core.h"
#include "BufferPool.h"
#include "Shader.h"
#include "Texture.h"
/////////////////////////////////////////////////////////

Terrain::Terrain():CustomObject(PlayerType::Terrain)
{
}

Terrain::~Terrain()
{
    if (_HeightMapPixels)
    {
        delete[] _HeightMapPixels;
    }
}

void Terrain::Init()
{
    LoadFIle(L"../Resources/Texture/heightMap/heightMap.raw", vec3(30.0f, 20.0f, 30.0f));
    CreateMesh();

    ShaderInfo info;
  
    shared_ptr<GraphicsShader> shader = make_shared<GraphicsShader>();
    shader->Init(L"terrain.hlsl", info);
    this->SetShader(shader);

    {
        shared_ptr<Texture> texture = make_shared<Texture>();
        texture->Init(L"heightMap/Base_Texture.dds");
        _material->SetDiffuseTexture(texture);
    }

    {
        shared_ptr<Texture> texture = make_shared<Texture>();
        texture->Init(L"heightMap/diffuse.tga");
        _material->SetNormalTexture(texture);
    }

    {
        shared_ptr<Texture> texture = make_shared<Texture>();
        texture->Init(L"heightMap/Lava(Emissive).dds");
        _material->SetSpecularTexture(texture);
    }

}

void Terrain::Update()
{
    _transform->Update();
}

void Terrain::Render()
{
    _shader->SetPipelineState();
    _transform->PushData();
    _material->PushData();
    core->GetBufferManager()->GetGraphicsTableHeap()->SetGraphicsRootDescriptorTable();
    _mesh->Render();
}

#define _WITH_APPROXIMATE_OPPOSITE_CORNER
vec3 Terrain::GetHeight(vec3 pos)
{
    float normalizedX = pos.x + (_width * _scale.x) / 2;
    float normalizedZ = -pos.z + (_length * _scale.z) / 2;

    // ����Ʈ���� �ε��� ���
    int xIndex = static_cast<int>(normalizedX / _scale.x);
    int zIndex = static_cast<int>(normalizedZ / _scale.z);

    // �ε��� ��ȿ�� �˻�
    if (xIndex < 0 || xIndex >= _width - 1 || zIndex < 0 || zIndex >= _length - 1)
    {
        return vec3(pos.x, 0.0f, pos.z); // ������ ��� ��� �⺻ ���� ��ȯ
    }

    // ����Ʈ�ʿ��� ���� ���
    float fxPercent = (normalizedX / _scale.x) - xIndex;
    float fzPercent = (normalizedZ / _scale.z) - zIndex;

    // ����Ʈ�ʿ��� ���̰� ��������
    int heightIndex = zIndex * _width + xIndex;

    float bottomLeft = static_cast<float>(_HeightMapPixels[heightIndex]);
    float bottomRight = static_cast<float>(_HeightMapPixels[heightIndex + 1]);
    float topLeft = static_cast<float>(_HeightMapPixels[heightIndex + _width]);
    float topRight = static_cast<float>(_HeightMapPixels[heightIndex + _width + 1]);

    // ���� ����
#ifdef _WITH_APPROXIMATE_OPPOSITE_CORNER
    // �簢���� ���� ��Ŀ� ���� ������ ����
    if (fzPercent < (1.0f - fxPercent))
    {
        // ������ �ﰢ���� ���̰� ����
        topRight = topLeft + (bottomRight - bottomLeft);
    }
    else
    {
        // ���� �ﰢ���� ���̰� ����
        bottomLeft = topLeft + (bottomRight - topRight);
    }
#endif

    // ���� ���̰� ���
    float topHeight = topLeft * (1 - fxPercent) + topRight * fxPercent;
    float bottomHeight = bottomLeft * (1 - fxPercent) + bottomRight * fxPercent;
    float height = bottomHeight * (1 - fzPercent) + topHeight * fzPercent;

    // ���̰��� ������ vec3 ��ȯ (x, height, z)
    return vec3(pos.x, height * _scale.y, pos.z);
};

void Terrain::CreateMesh()
{

  
    int size = 1;
    vector<Vertex> v;

    for (int i = 0; i <= _length; ++i)
    {
        for (int j = 0; j <= _width; ++j)
        {
            Vertex vertex;

            vertex.position.x = (j - (_width / 2)) * size * _scale.x;
            vertex.position.y = 0; // �ʱ�ȭ
            vertex.position.z = (i - (_length / 2)) * -size * _scale.z;

            int heightIndex = i * _width + j;

            if (heightIndex < _width * _length) {
                vec3 Height = GetHeight(vec3(vertex.position.x, 0, vertex.position.z));
                vertex.position.y = Height.y ;
            }

            vertex.uv.x = static_cast<float>(j) / _width;
            vertex.uv.y = static_cast<float>(i) / _length;

            v.push_back(vertex);
        }
    }

    for (int i = 0; i <= _length; ++i)
    {
        for (int j = 0; j <= _width; ++j)
        {
            Vertex& vertex = v[i * (_width + 1) + j]; // ���� ����

            if (i > 0 && i < _length - 1 && j > 0 && j < _width - 1)
            {
                vec3 left = v[i * (_width + 1) + (j - 1)].position;
                vec3 right = v[i * (_width + 1) + (j + 1)].position;
                vec3 top = v[(i - 1) * (_width + 1) + j].position;
                vec3 bottom = v[(i + 1) * (_width + 1) + j].position;

                vec3 v1 = right - left;
                vec3 v2 = bottom - top;

                vec3 normal = v1.Cross(v2);
                normal.Normalize();
                vertex.normal = normal;
            }
            else
            {
                vertex.normal = vec3(0, 1, 0); // �⺻ �븻 ����
            }
        }
    }

    vector<uint32_t> index;

    for (int i = 0; i < _length - 1; ++i)
    {
        for (int j = 0; j < _width - 1; ++j)
        {
            index.push_back(i * (_width + 1) + j);
            index.push_back(i * (_width + 1) + (j + 1));
            index.push_back((i + 1) * (_width + 1) + j);

            index.push_back(i * (_width + 1) + (j + 1));
            index.push_back((i + 1) * (_width + 1) + (j + 1));
            index.push_back((i + 1) * (_width + 1) + j);
        }
    }

    _mesh->Init(v, index);
}

void Terrain::LoadFIle(LPCTSTR pFileName, vec3 scale)
{

    _scale = scale;

    // ���� ����
    HANDLE hFile = ::CreateFile(pFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_READONLY, NULL);

    // ���� ���� ���� �� ũ���� �߻�
    assert(hFile != INVALID_HANDLE_VALUE && "Failed to open height map file!");

    // ���� ũ�� ��������
    DWORD fileSize = ::GetFileSize(hFile, NULL);
    assert(fileSize != INVALID_FILE_SIZE && "Failed to get height map file size!");

    // �ȼ��� ����Ʈ �� (8��Ʈ = 1����Ʈ, 16��Ʈ = 2����Ʈ)
    int bytesPerPixel = 1; // RAW ������ 8��Ʈ��� ����

    // �ػ� ���
    _width = _length = static_cast<int>(sqrt(fileSize / bytesPerPixel));

    // ���� ũ��� ���� ũ�Ⱑ �´��� Ȯ��
    assert(_width * _length * bytesPerPixel == fileSize && "File size doesn't match calculated dimensions!");

    // ����Ʈ�� �����͸� ���� �޸� �Ҵ�
    if (_HeightMapPixels==nullptr)
    {
        _HeightMapPixels = new BYTE[_width * _length];
    }
    // ���Ͽ��� ������ �б�
    DWORD dwBytesRead;
    BOOL bReadSuccess = ::ReadFile(hFile, _HeightMapPixels, fileSize, &dwBytesRead, NULL);

    // ���� �б� ���� �� ũ���� �߻�
    assert(bReadSuccess && "Failed to read height map file!");
    assert(dwBytesRead == fileSize && "Incomplete file read!");

    // ���� �ڵ� �ݱ�
    ::CloseHandle(hFile);

}


