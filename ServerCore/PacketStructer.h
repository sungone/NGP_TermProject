#pragma once


//��� �����͸� ����������.
enum CMDCODE
{
	ClientInfoData = 0, // Ŭ���̾�Ʈ ��ġ,����,ũ�� ����
	BlockData, //��Ͽ� ���� ����
	ChattingData, // ä�ÿ� ��������
	MatcingStartReady, //Ŭ���̾�Ʈ���� Matching �غ�Ϸᰡ ��ٴ� ��ȣ / �������� 3���� �Ǽ� Matching �� �غ� �Ϸ��ٴ� ��ȣ

};

struct MYCMD
{
	int Code;			//����ڵ�
	int Size;			//�������� ������� ������ ũ��
};

struct Chat
{
	char message[256];
};
