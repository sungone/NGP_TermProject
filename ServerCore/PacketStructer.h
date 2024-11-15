#pragma once

//��� �����͸� ����������.
enum CMDCODE
{
	Connect =0, //ä�ο� �������� ClientId �ο�����
	ClientInfoData , // Ŭ���̾�Ʈ ��ġ,����,ũ�� ����
	BlockData, //��Ͽ� ���� ����
	ChattingData, // ä�ÿ� ��������
	MatcingStartReady, //Ŭ���̾�Ʈ���� Matching �غ�Ϸᰡ ��ٴ� ��ȣ / �������� 3���� �Ǽ� Matching �� �غ� �Ϸ��ٴ� ��ȣ
	MatchingCancle, // Ŭ���̾�Ʈ���� Matching �� ����ϰڴٴ� ��ȣ�� �������� ����.
	//�ʿ��Ѱ��� �߰��ҿ���

	DisconnectClient, // �ΰ��� �� Ŭ���̾�Ʈ�� ������ �����Ͽ��� �� ������ �����ߴٴ� �� ������
	OtherClientIdData , // Ŭ���̾�Ʈ ID , ������ Ŭ���̾�Ʈ ���� ���� -> Ŭ�� �������� �� ������ Ŭ�󿡰� ������ ������ ����
};

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
	int random_seed[36][3];
};


struct MYCMD
{
	int Code = 0;			//����ڵ�
	int Size = 0;			//�������� ������� ������ ũ��
	int ClientID = 0;
	bool IsClientMaster = false;
	union
	{
		ClientInfoPacket clientInfoPacket;
		BlockCreateInfo blockRamdomSeedPacket;
	};
};





