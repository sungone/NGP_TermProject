#pragma once


//��� �����͸� ����������.
enum CMDCODE
{
	ClientInfoData = 0, // Ŭ���̾�Ʈ ��ġ,����,ũ�� ����
	BlockData, //��Ͽ� ���� ����
	ChattingData // ä�ÿ� ��������

};




struct MYCMD
{
	int Code;			//����ڵ�
	int Size;			//�������� ������� ������ ũ��
};

