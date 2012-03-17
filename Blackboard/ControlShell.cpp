/* 
 * File:   ControlShell.cpp
 * Author: chanman
 * 
 * Created on March 16, 2012, 9:45 PM
 */

#include "ControlShell.h"
#include "BlackboardConnection.h"
#include <set>
#include "bbdef.h"
#include "Packet.h"
#include <cassert>

ControlShell::ControlShell(bbtag _tag) {
    tag = _tag;
}

ControlShell::~ControlShell() {
}

bool ControlShell::connectCS() {
    log("connect\n");
    return connectBB();
}

void ControlShell::disconnectCS() {
    log("disconnect\n");
    disconnectBB();
}

std::deque<DataPoint> ControlShell::getRecent(int numRequested) {
    // assume that this is connection is only used for this tag
    // and that all GET_RECENT requests have completed (i.e. no data left in the recf queue)

    log("get recent %d\n", numRequested);

    // send request
    Packet request;
    request.resize(12);
    request.setU32(0, BO_CS_GET_RECENT);
    request.setU32(4, tag);
    request.setU32(8, numRequested);
    sendPacket(request);

    Packet response;
    std::deque<DataPoint> ret;

    // wait for response
    while (true) {
        waitForEvents();
        processMsgQueue();

        // if we have a packet to work with, deal with it
        if (recvPacket(response)) {
            if (response.getU32(0) == BO_CS_RECENT) {
                assert(response.getU32(4) == tag);
                int numDataPoints = response.getU32(8);
                int PacketPos = 12;

                //while (PacketPos < )

            }
        }
    }

    return ret;
}

DataPoint ControlShell::getMostRecent() {
    std::deque<DataPoint> retSet = getRecent(1);
    if (retSet.size() == 0) {
        return DataPoint();
    }
    return retSet.front();
}

void ControlShell::registerCallback(void (*callback)(bbtag, DataPoint)) {
    callbacks.insert(callback);
}

void ControlShell::releaseCallback(void (*callback)(bbtag, DataPoint)) {
    callbacks.erase(callback);
}

