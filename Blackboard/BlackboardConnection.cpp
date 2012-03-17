#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

#include "common.h"
#include "Packet.h"
#include "BlackboardConnection.h"

bool BlackboardConnection::connectBB() {
    const char* address = BB_SOCK_PATH;
    
    sockaddr_un local;

    memset(&local, 0, sizeof (sockaddr_un));

    // request socket
#ifdef __APPLE__
    log("create stream socket on %s\n", address);
    exitOnFail((bbfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1, "socket");
#else
    log("create seqpacket socket on %s\n", address);
    exitOnFail((bbfd = socket(AF_UNIX, SOCK_SEQPACKET, 0)) == -1, "socket");
#endif

    // set socket path
    local.sun_family = AF_UNIX;
    strncpy(local.sun_path, address, sizeof (local.sun_path));

    exitOnFail(connect(bbfd, (sockaddr*) & local, sizeof (sockaddr_un)) == -1, "connect");
    return (bbfd >= 0);
}

void BlackboardConnection::disconnectBB() {
    // TODO: clear buffers, we assume at this point, they are empty

    log("disconnect\n");
    
    if (bbfd >= 0)
        close(bbfd);
    bbfd = -1;
}

void BlackboardConnection::sendPacket(const Packet& buffer) {
    sendQueue.push_back(buffer);
}

bool BlackboardConnection::recvPacket(Packet& buffer) {
    if (recvQueue.size() == 0) {
        return false;
    }
    buffer = recvQueue.front();
    recvQueue.pop_front();
    return true;
}

/*
  This function will "flush" the send queue and "fill" the recv queue.
  This function will not block and should be periodically called in 
  and event loop. 
 */ 
void BlackboardConnection::processMsgQueue() {
    log("processMsgQueue (send)\n");
    
    int ret;
    // send any queued outgoing packets
    while (sendQueue.size() > 0) {
        ret = send(bbfd, &(sendQueue.front().front()), sendQueue.front().size(), MSG_DONTWAIT);

        if (ret >= 0) {
            sendQueue.pop_front();
            log("send\n");
        } else {
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                break;
            } else {
                errorOnFail(FAIL, "send")
            }
        }
    }

    log("processMsgQueue (recv)\n");
    // attempt to get any new packets and queue them to be handled
    Packet buffer;
    while (true) {
        buffer.resize(MAX_BUFFER_SIZE);
        ret = recv(bbfd, &(buffer.front()), buffer.size(), MSG_DONTWAIT);

        if (ret == -1) {
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                break;
            } else {
                errorOnFail(FAIL, "recv");
            }
        }
        if (ret == 0) {
            // connection closed;
            bbfd = -1;
        }

        buffer.resize(ret);
        recvQueue.push_back(buffer);
    }
}

void BlackboardConnection::waitForEvents() {
    fd_set fds;
    while (recvQueue.size() == 0) {
        // wait to unblock
        FD_ZERO(&fds);
        FD_SET(bbfd, &fds);
        select(bbfd + 1, &fds, NULL, NULL, NULL);
    }
}
