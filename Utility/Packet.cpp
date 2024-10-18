#include "Packet.h"

PacketFast::PacketFast()
{
	PushData(sizeof(HEADER_SIZE));
}

PacketFast::PacketFast(int capacity)
{
	packetData.reserve(capacity);
	PacketFast::PacketFast();
}

PacketFast::~PacketFast()
{

}

void PacketFast::PushData(const unsigned long long int& data)
{
	unsigned long long int netData = htonll(data);
	this->packetData.resize(this->packetData.size() + sizeof(netData));
	std::memcpy(&this->packetData[offset], &netData, sizeof(netData));
	offset += +sizeof(netData);

}

void PacketFast::PushData(const unsigned int& data)
{
	unsigned int netData = htonl(data);
	this->packetData.resize(this->packetData.size() + sizeof(netData));
	std::memcpy(&this->packetData[offset], &netData, sizeof(netData));
	offset += +sizeof(netData);
}

void PacketFast::PushData(const unsigned short& data)
{
	unsigned short netData = htons(data);
	this->packetData.resize(this->packetData.size() + sizeof(netData));
	std::memcpy(&this->packetData[offset], &netData, sizeof(netData));
	offset += +sizeof(netData);
}

void PacketFast::PushData(const unsigned char& data)
{
	this->packetData[offset++] = data;
}

void PacketFast::PushData(const long long int& data)
{
	PushData(*reinterpret_cast<const long long unsigned int*>(&data));
}

void PacketFast::PushData(const int& data)
{
	PushData(*reinterpret_cast<const unsigned int*>(&data));
}

void PacketFast::PushData(const short& data)
{
	PushData(*reinterpret_cast<const unsigned short*>(&data));
}

void PacketFast::PushData(const char& data)
{
	PushData(*reinterpret_cast<const unsigned char*>(&data));
}

void PacketFast::PushData(const float& data)
{
	PushData(*reinterpret_cast<const unsigned int*>(&data));
}

void PacketFast::PushData(const double& data)
{
	PushData(*reinterpret_cast<const unsigned long long int*>(&data));
}

template <class T, class>
void PacketFast::PushData(T begin, T end)
{
	for (; begin != end; ++begin)
		PushData(*begin);
}

void PacketFast::PushSize()
{
	unsigned int netData = htonl(static_cast<unsigned int>(GetBodySize()));
	std::memcpy(&this->packetData[offset], &netData, sizeof(netData));
}

void PacketFast::PopData(unsigned long long int& data)
{
	unsigned long long netData = 0;
	memcpy(&netData, &(this->packetData)[offset], sizeof(netData));
	data = ntohll(netData);
	offset += sizeof(netData);
}

void PacketFast::PopData(unsigned int& data)
{
	unsigned int netData = 0;
	memcpy(&netData, &(this->packetData)[offset], sizeof(netData));
	data = ntohll(netData);
	offset += sizeof(netData);
}

void PacketFast::PopData(unsigned short& data)
{
	unsigned short netData = 0;
	memcpy(&netData, &(this->packetData)[offset], sizeof(netData));
	data = ntohll(netData);
	offset += sizeof(netData);
}

void PacketFast::PopData(unsigned char& data)
{
	data = this->packetData[offset++];
}

void PacketFast::PopData(long long int& data)
{
	unsigned long long int temp;
	PopData(temp);
	data = *reinterpret_cast<long long int*>(&temp);
}

void PacketFast::PopData(int& data)
{
	unsigned int temp;
	PopData(temp);
	data = *reinterpret_cast<int*>(&temp);
}

void PacketFast::PopData(short& data)
{
	unsigned short temp;
	PopData(temp);
	data = *reinterpret_cast<short*>(&temp);
}

void PacketFast::PopData(char& data)
{
	unsigned char temp;
	PopData(temp);
	data = *reinterpret_cast<char*>(&temp);
}

void PacketFast::PopData(float& data)
{
	unsigned int temp;
	PopData(temp);
	data = *reinterpret_cast<float*>(&temp);
}

void PacketFast::PopData(double& data)
{
	unsigned long long int temp;
	PopData(temp);
	data = *reinterpret_cast<double*>(&temp);
}

template <class T>
T PacketFast::PopData()
{
	T temp;
	PopData(temp);
	return temp;
}

template <class T>
T PacketFast::PeekData()
{
	T temp;
	int prevOffset = this->offset;
	PopData(temp);
	this->offset = prevOffset;
	return temp;
}

template <class T>
void PacketFast::PeekData(T& t)
{
	int prevOffset = this->offset;
	PopData(t);
	this->offset = prevOffset;
}

void PacketFast::PeekData(char* data, int offset, int& size)
{
	CopyPeek(data, offset, size);
}

void PacketFast::PopData(char* data, int offset, int& size)
{
	CopyPeek(data, offset, size);
	this->offset += size;
}

template <class T, class>
void PacketFast::PopData(T iter, int& size)
{
	for (int i = 0; i < size; i++)
	{
		*iter = PopData<T>();
		offset += size;
		++iter;
	}
}

void PacketFast::Marking()
{
	marker = offset;
	offset = 0;
}

int PacketFast::GetBodySize() const
{
	return marker;
}

int PacketFast::GetBodyOffset()
{
	return sizeof(HEADER_SIZE);
}

int PacketFast::GetTotalSize() const
{
	return marker - GetBodyOffset();
}

int PacketFast::Offset()
{
	return offset;
}

void PacketFast::CopyPeek(char* data, int offset, int& size)
{
	size = min(max(GetTotalSize() - Offset(), 0), size);
	memcpy(&data[offset], &this->packetData[this->offset], size);
}

void PacketFast::Clear()
{
	offset = 0;
	marker = 0;
}

void PacketFast::OffsetClear()
{
	offset = 0;
}

void PacketFast::RewindOffset(int trySize, int realSize)
{
	RewindOffset(trySize - realSize);
}

void PacketFast::RewindOffset(int size)
{
	offset -= size;
}

void PacketFast::GotoMarker()
{
	offset = marker;
}
