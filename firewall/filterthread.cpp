#include "filterthread.h"
#include <QDebug>

#include <QString>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QVariant>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlError>


FilterThread::FilterThread(QObject *parent) :
    QThread(parent)
{
    enable_flag = 1;
}

//icmp query and process
int icmpQuery(char *srcstr, char *deststr,QString ippro,int type)
{
    QSqlQuery query;
    query.exec(QString("select * from rule where (SourceIP='%1' or SourceIP='all')"
                       "and (DestinationIP='%2' or DestinationIP = 'all') and "
                       "(protocol = 'all' or protocol = '%3') and (icmptype = 'all' or icmptype = '%4')and Pass = 0")
               .arg(srcstr).arg(deststr).arg(ippro).arg(type));

    int dropflag = 0;
    while(query.next())
    {
        dropflag = 1;
        qDebug()<<"Found match in DB";
        qDebug()<<query.value(0).toInt()<<endl
               <<query.value(1).toString()<<endl
              <<query.value(2).toString()<<endl
             <<query.value(3).toString()<<endl
            <<query.value(4).toString()<<endl
           <<query.value(5).toString()<<endl
          <<query.value(6).toString()<<endl;
    }
    if(dropflag == 1)
    {
        qDebug()<<"drop ICMP"<<endl;
        return NF_DROP;
    }
    else
    {
        qDebug()<<"accept ICMP"<<endl;
        return NF_ACCEPT;
    }
}

//tcp query and process
int tcpudpQuery(char *srcstr, char *deststr,QString ippro,int sourceP,int destP)
{
    QSqlQuery query;
    query.exec(QString("select * from rule where (SourceIP='%1' or SourceIP='all')"
                       "and (DestinationIP='%2' or DestinationIP = 'all') and "
                       "(Protocol = 'all' or Protocol = '%3') and"
                       "(SourcePort = 'all' or SourcePort = '%4') and"
                       "(DestinationPort = 'all' or DestinationPort = '%5')and Pass = 0")
               .arg(srcstr).arg(deststr).arg(ippro).arg(sourceP).arg(destP));

    int dropflag = 0;
    while(query.next())
    {
        dropflag = 1;
        qDebug()<<"Found match in DB";
        qDebug()<<query.value(0).toInt()<<endl
               <<query.value(1).toString()<<endl
              <<query.value(2).toString()<<endl
             <<query.value(3).toString()<<endl
            <<query.value(4).toString()<<endl
           <<query.value(5).toString()<<endl
          <<query.value(6).toString()<<endl;
    }
    if(dropflag == 1)
    {
        qDebug()<<"drop "<<ippro<<endl;
        return NF_DROP;
    }
    else
    {
        qDebug()<<"accept "<<ippro<<endl;
        return NF_ACCEPT;
    }
}

// filter process referring to one package
int callback(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfa, void *data)//使用netfilter_queue要用到的函数，含有对包的判决
{
    struct iphdr *piphdr;

    int id = 0;
    struct nfqnl_msg_packet_hdr *ph;
    unsigned char *pdata = NULL;
    int pdata_len;
    int dealmethod = NF_ACCEPT;
    char srcstr[32],deststr[32];
    QString ippro;
    ph = nfq_get_msg_packet_hdr(nfa);
    if (ph == NULL)
        return 1;
    id = ntohl(ph->packet_id);
    //if (enable_flag == 0)
    //return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
    pdata_len = nfq_get_payload(nfa, (char**)&pdata);
    if (pdata != NULL)
        piphdr = (struct iphdr *)pdata;
    else
        return 1;
    inet_ntop(AF_INET,&(piphdr->saddr),srcstr,32);
    inet_ntop(AF_INET,&(piphdr->daddr),deststr,32);
    //output part
    //cout basic information
    qDebug()<<"--------------------------PACKAGE DETECTED-------------------------------";
    qDebug()<<"get a packet:"<< srcstr<<"-> "<<deststr<<"\n";
    qDebug()<<"protocol info:"<<piphdr->protocol<<endl;

    //port cout
    if(piphdr->protocol == 6)
    {
        ippro = "tcp";
        struct tcphdr *ptcphdr;
        //printf("<0>This is an tcp packet.\n");
        ptcphdr = (struct tcphdr *)((char *)piphdr +(piphdr->ihl*4));
        qDebug()<<"TCP protocol detected! source:"<<ptcphdr->source<<"dest:"<<ptcphdr->dest<<endl;
        dealmethod = tcpudpQuery(srcstr,deststr,ippro,ptcphdr->source,ptcphdr->dest);
    }

    if(piphdr->protocol == 17)
    {
        ippro = "udp";
        struct udphdr *pudphdr;
        //printf("<0>This is an udp packet.\n");
        pudphdr = (struct udphdr *)((char *)piphdr +(piphdr->ihl*4));
        qDebug()<<"UDP protocol detected! source:"<<pudphdr->source<<"dest:"<<pudphdr->dest<<endl;
        dealmethod = tcpudpQuery(srcstr,deststr,ippro,pudphdr->source,pudphdr->dest);
    }

    if(piphdr->protocol == 1)
    {
        ippro = "icmp";
        struct icmphdr *picmphdr;
        picmphdr = (struct icmphdr *)((char *)piphdr+(piphdr->ihl*4));
        qDebug()<<"ICMP protocol detected!"<<endl;
        qDebug()<<"icmp type"<<picmphdr->type<<endl;
        dealmethod = icmpQuery(srcstr,deststr,ippro,picmphdr->type);
    }

    qDebug()<<"-------------------------------------------------------------------------";
    return nfq_set_verdict(qh, id, dealmethod, 0, NULL);
}

void FilterThread::run()
{
    qDebug()<<"reset the flag"<<endl;
    enable_flag = 1;

    qDebug()<<"start running"<<endl;
    static int fd;//使用netfilter_queue会使用到的变量
    static struct nfq_handle *h;//使用netfilter_queue会使用到的变量
    static struct nfq_q_handle *qh;//使用netfilter_queue会使用到的变量
    static struct nfnl_handle *nh;//使用netfilter_queue会使用到的变量

    char buf[1600];
    int length;

    //	printf("opening library handle\n");
    h = nfq_open();
    if (!h) {
        fprintf(stderr, "error during nfq_open()\n");
        exit(1);
    }

    //	printf("unbinding existing nf_queue handler for AF_INET (if any)\n");
    if (nfq_unbind_pf(h, AF_INET) < 0) {
        fprintf(stderr, "already nfq_unbind_pf()\n");
        exit(1);
    }

    //	printf("binding nfnetlink_queue as nf_queue handler for AF_INET\n");
    if (nfq_bind_pf(h, AF_INET) < 0) {
        fprintf(stderr, "error during nfq_bind_pf()\n");
        exit(1);
    }

    //	printf("binding this socket to queue '0'\n");
    qh = nfq_create_queue(h,0, &callback, NULL);
    if (!qh) {
        fprintf(stderr, "error during nfq_create_queue()\n");
        exit(1);
    }

    //	printf("setting copy_packet mode\n");
    if (nfq_set_mode(qh, NFQNL_COPY_PACKET, 0xffff) < 0) {
        fprintf(stderr, "can't set packet_copy mode\n");
        exit(1);
    }

    nh = nfq_nfnlh(h);
    fd = nfnl_fd(nh);
    //qDebug()<<"enter circulation"<<endl;
    while(1)
    {
        if(enable_flag == 0)
        {
            qDebug()<<"circulation stop"<<endl;
            break;
        }
        qDebug()<<"doing circulation"<<endl;
        length=recv(fd,buf,1600,0);//此处完成收包
        nfq_handle_packet(h, buf,length);//完成发包的真正函数
    }
    nfq_destroy_queue(qh);
    nfq_close(h);
    qDebug()<<"run process stoped"<<endl;
}

void FilterThread::stop()
{
    enable_flag = 0;
    //qDebug()<<"flag been set to stop"<<endl;
}
