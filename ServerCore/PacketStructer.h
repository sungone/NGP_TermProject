#pragma once


//��� �����͸� ����������.
enum CMDCODE
{
	ClientInfoData = 0, // Ŭ���̾�Ʈ ��ġ,����,ũ�� ����
	BlockData, //��Ͽ� ���� ����
	ChattingData, // ä�ÿ� ��������
	ExitData // Ŭ���̾�Ʈ�� �����ȣ

};

struct MYCMD
{
	int Code;			//����ڵ� (CMDCODE)
	int Size;			//�������� ������� ������ ũ��
};

