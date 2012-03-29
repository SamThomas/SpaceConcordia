#ifndef BLACKBOARDCONNECTION_H
#define	BLACKBOARDCONNECTION_H

#include <stdint.h>
#include <deque>
#include <vector>
#include <time.h>

#include "common.h"
#include "Packet.h"

class BlackboardConnection {
private:
    int bbfd;
    std::deque<Packet> recvQueue;
    std::deque<Packet> sendQueue;

protected:
    bool connectBB();
    void disconnectBB();
    void sendPacket(const Packet&);
    bool recvPacket(Packet&);
    void processOutgoing();
    void processIncoming();


public:
    void processMsgQueue();
    void waitForEvents();
    
    static int multiWait(std::deque<BlackboardConnection*>, timeval* = 0);
};


#endif	/* BLACKBOARDCONNECTION_H */