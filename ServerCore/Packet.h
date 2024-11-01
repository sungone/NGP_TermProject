
//TMEP  => 안쓸수도
class Packet
{
public:
	
	template <typename T>
	static BYTE* Alloc()
	{
		BYTE* ptr = new BYTE[sizeof(MYCMD) + sizeof(T)];
		return ptr;
	}

	template <typename T>
	static T* GetDataPointer(BYTE* ptr)
	{
		ptr += sizeof(MYCMD);
		return reinterpret_cast<T*>(ptr);
	}

};
