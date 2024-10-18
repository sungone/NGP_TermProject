#pragma once

#include "stdafx.h"

class Packet
{
public:
	static unsigned long long int HEADER_SIZE;
public:
	std::vector<uint8_t> packetData;
	int offset = 0;
	int marker = 0;

	Packet();

	Packet(int capacity);

	virtual ~Packet();

	void PushData(const unsigned long long int& data);

	void PushData(const unsigned int& data);

	void PushData(const unsigned short& data);

	void PushData(const unsigned char& data);

	void PushData(const long long int& data);

	void PushData(const int& data);

	void PushData(const short& data);

	void PushData(const char& data);

	void PushData(const float& data);

	void PushData(const double& data);

	template<class T, class = std::enable_if_t<std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<T>::iterator_category>>>
	void PushData(T begin, T end);

	void PushSize();

	void PopData(unsigned long long int& data);

	void PopData(unsigned int& data);

	void PopData(unsigned short& data);

	void PopData(unsigned char& data);

	void PopData(long long int& data);

	void PopData(int& data);

	void PopData(short& data);

	void PopData(char& data);

	void PopData(float& data);

	void PopData(double& data);

	template<class T>
	T PopData();

	template<class T>
	T PeekData();

	template<class T>
	void PeekData(T& t);

	void PeekData(char* data, int offset, int& size);

	void PopData(char* data, int offset, int& size);

	template<class T, class = std::enable_if_t<std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<T>::iterator_category>>>
	void PopData(T iter, int& size);

	void Marking();

	int GetBodySize() const;

	static int GetBodyOffset();

	int GetTotalSize() const;

	int Offset();

	void CopyPeek(char* data, int offset, int& size);

	void Clear();

	void OffsetClear();

	void RewindOffset(int trySize, int realSize);

	void RewindOffset(int size);

	void GotoMarker();
};