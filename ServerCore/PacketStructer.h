#pragma once

//��� �����͸� ����������.
namespace ENUM
{
	enum CMDCODE
	{
		Connect = 0, //ä�ο� �������� ClientId �ο�����
		ClientInfoData, // Ŭ���̾�Ʈ ��ġ,����,ũ�� ����
		BlockData, //��Ͽ� ���� ����
		ChattingData, // ä�ÿ� ��������
		StartGame, //Ŭ���̾�Ʈ���� Matching �غ�Ϸᰡ ��ٴ� ��ȣ / �������� 3���� �Ǽ� Matching �� �غ� �Ϸ��ٴ� ��ȣ
		MatchingCancle, // Ŭ���̾�Ʈ���� Matching �� ����ϰڴٴ� ��ȣ�� �������� ����.
		//�ʿ��Ѱ��� �߰��ҿ���
		BlockDataRecv,
		BlockCollision,
		HPSync,
		DisconnectClient, // �ΰ��� �� Ŭ���̾�Ʈ�� ������ �����Ͽ��� �� ������ �����ߴٴ� �� ������
		DisConnectClientInfo, // Ŭ���̾�Ʈ ID , ������ Ŭ���̾�Ʈ ���� ���� -> Ŭ�� �������� �� ������ Ŭ�󿡰� ������ ������ ����
		GAMEOVER
	};
}

// �÷��̾� ��ġ ���� ��Ŷ
struct ClientInfoPacket
{
	float pos_x;

	float color_r;
	float color_g;
	float color_b;

	float scale_x;
	float scale_y;
};

// ����� ������ �� ����ϴ� ���� �õ尪
struct BlockCreateInfo
{
	int	random_num[36][3];
	int cur_idx = 0;
	float color[3][3][3];
};

struct HPInfo
{
	int hp = 3;
};


struct MYCMD
{
	int Code = 0;			//����ڵ�
	int Size = 0;			//�������� ������� ������ ũ��
	int ClientID = 0;
	bool IsClientMaster = false;
};





