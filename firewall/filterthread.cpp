#include "filterthread.h"
#include <QDebug>

FilterThread::FilterThread(QObject *parent) :
    QThread(parent)
{
    enable_flag = 1;
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

    //cout basic information
    qDebug()<<"get a packet:"<< srcstr<<"-> "<<deststr<<"\n";
    qDebug()<<"protocol info:"<<piphdr->protocol<<endl;

    //port cout
    if(piphdr->protocol == 6)
    {
        struct tcphdr *ptcphdr;
        //printf("<0>This is an tcp packet.\n");
        ptcphdr = (struct tcphdr *)((char *)piphdr +(piphdr->ihl*4));
        qDebug()<<"TCP protocol detected! source:"<<ptcphdr->source<<"dest:"<<ptcphdr->dest<<endl;
    }

    if(piphdr->protocol == 17)
    {
        struct udphdr *pudphdr;
        //printf("<0>This is an udp packet.\n");
        pudphdr = (struct udphdr *)((char *)piphdr +(piphdr->ihl*4));
        qDebug()<<"UDP protocol detected! source:"<<pudphdr->source<<"dest:"<<pudphdr->dest<<endl;
    }
    return nfq_set_verdict(qh, id, dealmethod, 0, NULL);
}

void FilterThread::run()
{
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
            //qDebug()<<"circulation stop"<<endl;
            break;
        }
        //qDebug()<<"doing circulation"<<endl;
        length=recv(fd,buf,1600,0);//此处完成收包
        nfq_handle_packet(h, buf,length);//完成发包的真正函数
    }

    //qDebug()<<"finish circulation"<<endl;
    nfq_destroy_queue(qh);
    nfq_close(h);

    qDebug()<<"run process stoped"<<endl;
    //qDebug()<<"reset the flag"<<endl;
    enable_flag = 1;
}

void FilterThread::stop()
{
    enable_flag = 0;
    //qDebug()<<"flag been set to stop"<<endl;
}
