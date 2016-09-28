#ifndef PCAP
#define PCAP

#include <pcap.h>
#include <vector>
#include <string>
#include <queue>


class Pcap{
public:
    static std::vector<std::string> getAllDevs(); //获得所有设备，核心函数pcap_findalldevs
    static std::vector<std::string> getAllDevsDescription(); // 所有设备的描述
    static void refreshAllDevs();
    static void startSniff(std::string devname, int catchcount, std::string filter_string);
    static void * startSniffThread(void * Opt);


private: /* Attribute */
    static std::vector<std::string> m_alldevs;
    static std::vector<std::string> m_alldevsdescription;
    static char errbuf[PCAP_ERRBUF_SIZE];
    static std::queue<std::string> m_queue;


private: /* Method */
};

#endif // PCAP

