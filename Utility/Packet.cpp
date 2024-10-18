#include "Packet.h"

Packet::Packet()
{
	packetData.reserve(GetBodyOffset());
	PushData(static_cast<HEADER_TYPE>(0));
}

Packet::Packet(int capacity) : Packet()
{
	packetData.reserve(capacity);
}

Packet::~Packet()
{

}

void Packet::PushData(const unsigned long long int& data)
{
	unsigned long long int netData = htonll(data);
	this->packetData.resize(this->packetData.size() + sizeof(netData));
	std::memcpy(&this->packetData[offset], &netData, sizeof(netData));
	offset += sizeof(netData);

}

void Packet::PushData(const unsigned int& data)
{
	unsigned int netData = htonl(data);
	this->packetData.resize(this->packetData.size() + sizeof(netData));
	std::memcpy(&this->packetData[offset], &netData, sizeof(netData));
	offset += sizeof(netData);
}

void Packet::PushData(const unsigned short& data)
{
	unsigned short netData = htons(data);
	this->packetData.resize(this->packetData.size() + sizeof(netData));
	std::memcpy(&this->packetData[offset], &netData, sizeof(netData));
	offset += sizeof(netData);
}

void Packet::PushData(const unsigned char& data)
{
	this->packetData.resize(this->packetData.size() + 1);
	this->packetData[offset++] = data;
}

void Packet::PushData(const long long int& data)
{
	PushData(*reinterpret_cast<const long long unsigned int*>(&data));
}

void Packet::PushData(const int& data)
{
	PushData(*reinterpret_cast<const unsigned int*>(&data));
}

void Packet::PushData(const short& data)
{
	PushData(*reinterpret_cast<const unsigned short*>(&data));
}

void Packet::PushData(const char& data)
{
	PushData(*reinterpret_cast<const unsigned char*>(&data));
}

void Packet::PushData(const float& data)
{
	PushData(*reinterpret_cast<const unsigned int*>(&data));
}

void Packet::PushData(const double& data)
{
	PushData(*reinterpret_cast<const unsigned long long int*>(&data));
}

void Packet::PushData(std::string& data)
{
	PushData(static_cast<size_t>(data.size()));
	for(int i=0;i< data.size();i++)
		PushData(data[i]);
}

void Packet::PushSize()
{
	int offset = this->offset;
	this->offset = 0;
	PushData(static_cast<HEADER_TYPE>(GetBodySize()));
	this->offset = offset;
}

void Packet::PopData(unsigned long long int& data)
{
	unsigned long long netData = 0;
	memcpy(&netData, &(this->packetData)[offset], sizeof(netData));
	data = ntohll(netData);
	offset += sizeof(netData);
}

void Packet::PopData(unsigned int& data)
{
	unsigned int netData = 0;
	memcpy(&netData, &(this->packetData)[offset], sizeof(netData));
	data = ntohl(netData);
	offset += sizeof(netData);
}

void Packet::PopData(unsigned short& data)
{
	unsigned short netData = 0;
	memcpy(&netData, &(this->packetData)[offset], sizeof(netData));
	data = ntohs(netData);
	offset += sizeof(netData);
}

void Packet::PopData(unsigned char& data)
{
	data = this->packetData[offset++];
}

void Packet::PopData(long long int& data)
{
	unsigned long long int temp;
	PopData(temp);
	data = *reinterpret_cast<long long int*>(&temp);
}

void Packet::PopData(int& data)
{
	unsigned int temp;
	PopData(temp);
	data = *reinterpret_cast<int*>(&temp);
}

void Packet::PopData(short& data)
{
	unsigned short temp;
	PopData(temp);
	data = *reinterpret_cast<short*>(&temp);
}

void Packet::PopData(char& data)
{
	unsigned char temp;
	PopData(temp);
	data = *reinterpret_cast<char*>(&temp);
}

void Packet::PopData(float& data)
{
	unsigned int temp;
	PopData(temp);
	data = *reinterpret_cast<float*>(&temp);
}

void Packet::PopData(double& data)
{
	unsigned long long int temp;
	PopData(temp);
	data = *reinterpret_cast<double*>(&temp);
}

void Packet::PopData(std::string& data)
{
	int size = static_cast<int>(PopData<size_t>());
	data.resize(size);
	for (int i = 0; i < size; i++)
		PopData(data[i]);
}


void Packet::PeekData(char* data, int offset, int& size)
{
	CopyPeek(data, offset, size);
}

void Packet::PopData(char* data, int offset, int& size)
{
	CopyPeek(data, offset, size);
	this->offset += size;
}

void Packet::Marking()
{
	marker = offset;
	offset = 0;
}

int Packet::GetBodySize() const
{
	return marker - GetBodyOffset();
}

int Packet::GetBodyOffset()
{
	return sizeof(HEADER_TYPE);
}

int Packet::GetTotalSize() const
{
	return marker;
}

int Packet::Offset(int setOffset)
{
	if (setOffset >= 0)
		offset = setOffset;
	return offset;
}

void Packet::CopyPeek(char* data, int offset, int& size)
{
	size = min(max(GetTotalSize() - Offset(), 0), size);
	memcpy(&data[offset], &this->packetData[this->offset], size);
}

void Packet::Clear()
{
	offset = 0;
	marker = 0;
}

void Packet::OffsetClear()
{
	offset = 0;
}

void Packet::RewindOffset(int trySize, int realSize)
{
	RewindOffset(trySize - realSize);
}

void Packet::RewindOffset(int size)
{
	offset -= size;
}

void Packet::GotoMarker()
{
	offset = marker;
}

void Packet::GotoBodyBegin()
{
	offset = GetBodyOffset();
}
