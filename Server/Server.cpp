#include <iostream>
#include <Packet.h>


int main()
{
    std::cout << "Hello World!\n";

	Packet packet(1024);
    //TestCase
	{
        std::vector<int> textData{};
        textData.resize(10);
        std::iota(textData.begin(), textData.end(), 0);

        packet.PushData(1234); // 4
        packet.PushData(12.34); // 8
        packet.PushData(textData.begin(), textData.end());  // 4 * 10 40

        std::string str = "Hello World!";
    	packet.PushData(str);  // 4 * 10 40
        packet.Marking();
        packet.PushSize(); // 8
        // total : 8 + 52 = 60

        std::cout << packet.GetBodySize() << "\n";
        std::cout << packet.GetTotalSize() << "\n\n";

        std::cout << packet.PopData<Packet::HEADER_TYPE>() << "\n";
        std::cout << packet.PopData<int>() << "\n";
        std::cout << packet.PopData<double>() << "\n";
        for(int i=0;i<10;i++)
			std::cout << packet.PopData<int>() << " ";
        std::cout << "\n";

        std::cout << packet.PopData<std::string>() << "\n";
    }

}
