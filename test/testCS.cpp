/*
   Copyright 2012 Corey Clayton, Eric Chan, Mathieu Dumais-Savard

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <common/ControlShell.h>
#include <common/KnowledgeSource.h>
#include "unistd.h"
#include "assert.h"
#include "stdio.h"
#include <common/minunit.h>

int tests_run = 0;

int tag = 490;

DataPoint p;

KnowledgeSource myKS(tag);

ControlShell myCS(tag);

/*
  need to be able to connect
*/
static const char * test_connect() {
    bool con = myCS.connectCS();
    mu_assert("connectCS returned false!", con == true);
    return 0;
}

/*
  Get the last n DataPoints.

  Todo: n = 0, n > total DPs
*/
static const char * test_getRecent() {
    std::deque<DataPoint> dq1 = myCS.getRecent(5);
    std::deque<DataPoint> dq2 = myCS.getRecent(5);

    mu_assert("Got a number of data points != 1", dq1.size() == 1);
    for (uint32_t i = 0; i < dq1[0].size(); ++i) {
        mu_assert("getRecent()1: Corrupt Data", p[i] == dq1[0][i]);
    }
    assert(dq2.size() == 1);
    for (uint32_t i = 0; i < dq1[0].size(); ++i) {
        mu_assert("getRecent()2: Corrupt Data", p[i] == dq2[0][i]);
    }
    return 0;
}

static const char * test_getMostRecent() {
    DataPoint dp = myCS.getMostRecent();

    assert(dp.size() == p.size());
    for (uint32_t i = 0; i < p.size(); ++i) {
        mu_assert("getMostRecent(): Corrupt Data", p[i] == dp[i]);
    }
    return 0;
}

static const char * all_tests() {
    mu_run_test(test_connect);
    mu_run_test(test_getRecent);
    mu_run_test(test_getMostRecent);
    return 0;
}

int main(int argc, char** argv) {

    /*
      in order to test the CS, we need to 
      have the BB in a known state.

      To do this we will first make a KS 
      and make some data.
     */

    if (myKS.connectKS()) {
        log("connected\n");
    } else {
        log("Could not connect, BB did not allow me to register for tag %d\n", tag);
        return 1;
    }

    p.resize(6);

    p[0] = 'M';
    p[1] = 'c';
    p[2] = 'G';
    p[3] = 'e';
    p[4] = 'r';
    p[5] = 'p';

    if (myKS.update(p)) {
        log("updated with ack\n");
    } else {
        log("update failed\n");
    }

    myKS.disconnectKS();

    /////////////////////////////////////////////////
    // setup complete
    /////////////////////////////////////////////////	

    const char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    myCS.disconnectCS();

    return 0;
}

