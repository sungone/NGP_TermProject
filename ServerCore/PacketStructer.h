#pragma once


//��� �����͸� ����������.
enum CMDCODE
{
	Connect =0, //ä�ο� �������� ClientId �ο�����
	ClientInfoData , // Ŭ���̾�Ʈ ��ġ,����,ũ�� ����
	BlockData, //��Ͽ� ���� ����
	ChattingData, // ä�ÿ� ��������
	MatcingStartReady, //Ŭ���̾�Ʈ���� Matching �غ�Ϸᰡ ��ٴ� ��ȣ / �������� 3���� �Ǽ� Matching �� �غ� �Ϸ��ٴ� ��ȣ
	MatchingCancle // Ŭ���̾�Ʈ���� Matching �� ����ϰڴٴ� ��ȣ�� �������� ����.
	
	//�ʿ��Ѱ��� �߰��ҿ���
};

struct MYCMD
{
	int Code=0;			//����ڵ�
	int Size=0;			//�������� ������� ������ ũ��
	int ClientID = 0;
};


