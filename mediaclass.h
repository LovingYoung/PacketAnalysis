#ifndef MediaClass_H
#define MediaClass_H

#include <QObject>
#include "pcap_s.h"
#include <vector>
#include <string>
#include <pthread.h>
#include "homogenization.h"
#include "my_queue.h"
#include <arpa/inet.h>
#include <QList>

struct Option{
    std::string devname;
    int catchcount;
    std::string filterstring;
    Option(std::string devname_input, int catchcount_input, std::string filterstring_input):
            devname(devname_input), catchcount(catchcount_input), filterstring(filterstring_input){}
};
void * start_sniff(void *opt);

class MediaClass : public QObject{
    Q_OBJECT

    /* Alldevs */
    Q_PROPERTY(QStringList alldevs READ alldevs NOTIFY alldevsChanged)
    Q_PROPERTY(QStringList packetAnalysis READ packetAnalysis NOTIFY packetAnalysisChanged)

public:
    Q_INVOKABLE void refreshAllDevs(){
        m_alldevs.clear();
        Pcap::refreshAllDevs();
        std::vector<std::string> devs = Pcap::getAllDevs();
        std::vector<std::string> descriptions = Pcap::getAllDevsDescription();
        for(int i = 0; i < devs.size(); i++){
            m_alldevs.append(QString(devs[i].c_str()));
            m_alldevs.append(QString(descriptions[i].c_str()));
        }
        emit alldevsChanged();
    }

    Q_INVOKABLE void beginSniff(QString devname_in, QString catchcount_in, QString filterstring_in){
        std::string devname = devname_in.toUtf8().constData();
        int catchcount = catchcount_in.toInt();
        std::string filterstring = filterstring_in.toUtf8().constData();
        pthread_t sniffthread;
        void * option = new Option(devname, catchcount, filterstring);
        pthread_create(&sniffthread, NULL, Pcap::startSniffThread, option);
    }

    Q_INVOKABLE void refreshAnalysis(){
        while(!myQueue::homoqueue.empty()){
            Homo * homo = myQueue::homoqueue.front();
            myQueue::homoqueue.pop();
            while(packetAnalysis_mutex == 1);
            packetAnalysis_mutex = 1;
            m_packetAnalysis.append(QString::fromStdString(homo->protocol));
            m_packetAnalysis.append(QString::fromStdString(homo->src_ip));
            m_packetAnalysis.append(QString::fromStdString(homo->src_mac));
            m_packetAnalysis.append(QString::fromStdString(homo->dst_ip));
            m_packetAnalysis.append(QString::fromStdString(homo->dst_mac));
            m_packetAnalysis.append(QString::number(homo->packet_length));
            if(homo->protocol == "TCP"){
                m_packetAnalysis.append(QString::number(ntohs(homo->tcp_header->src_port)));
                m_packetAnalysis.append(QString::number(ntohs(homo->tcp_header->dst_port)));
                m_packetAnalysis.append(QString::number(ntohl(homo->tcp_header->seq)));
                m_packetAnalysis.append(QString::number(ntohl(homo->tcp_header->ack)));
                m_packetAnalysis.append(QString::number(homo->tcp_header->flags ));
                m_packetAnalysis.append(QString::number(ntohs(homo->tcp_header->window_size)));
                m_packetAnalysis.append(QString::number(ntohs(homo->tcp_header->checksum)));
                m_packetAnalysis.append(QString::number(ntohs(homo->tcp_header->urgent_p)));
            } else if(homo->protocol == "UDP"){
                m_packetAnalysis.append(QString::number(ntohs(homo->udp_header->uh_sport)));
                m_packetAnalysis.append(QString::number(ntohs(homo->udp_header->uh_dport)));
                m_packetAnalysis.append(QString::number(ntohs(homo->udp_header->uh_ulen)));
                m_packetAnalysis.append(QString::number(ntohs(homo->udp_header->uh_sum)));
            }
            packetAnalysis_mutex = 0;
        }
        emit packetAnalysisChanged();
    }

    Q_INVOKABLE void popAnalysis(int count){
        if(count == 14){
            printf("TCP->Delete\n");
        } else {
            printf("UDP->Delete\n");
        }
        fflush(stdout);
        while(packetAnalysis_mutex == 1);
        packetAnalysis_mutex = 1;
        for(int i = 0; i < count; i++){
            m_packetAnalysis.removeFirst();
        }
        packetAnalysis_mutex = 0;
    }

    QStringList alldevs(){
        return m_alldevs;
    }

    QStringList packetAnalysis(){
        while(packetAnalysis_mutex == 1);
        return m_packetAnalysis;
    }

signals:
    void alldevsChanged();
    void packetAnalysisChanged();
private:
    QStringList m_alldevs;
    QStringList m_packetAnalysis;
    int packetAnalysis_mutex = 0;

    /* REQUIRED */
private:
};

#endif
