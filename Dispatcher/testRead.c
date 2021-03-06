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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>
#include <common/common.h>
#include <unistd.h>

double aquire_sample();

struct sigaction* sig_usr1;

int usr1_called = 0;

void sig_usr1_handler(int signal) {
    usr1_called++;
}

int main() {

    log("Starting READ\n");

    log("Registering USR1 Handler\n");

    // create space for sig_usr1
    sig_usr1 = malloc(sizeof (struct sigaction));
    // TODO: fail on malloc fail;

    // define it
    sig_usr1->sa_handler = sig_usr1_handler;
    sig_usr1->sa_flags = 0;
    sigemptyset(&sig_usr1->sa_mask);

    // create the handler
    sigaction(SIGUSR1, sig_usr1, NULL);

    log("[Read]: Start!\n");

    while (1) {
        double point, t;

        point = aquire_sample();

        if (usr1_called) {
            log("[Read]: USR1\n");
            break;
        }

        printf("[Read]: ");

        for (t = -1; t < point; t += 1.0 / 32) {
            putchar(' ');
        }

        printf("+\n");
    }


    return 0;
}

double aquire_sample() {
    // keep last sample reference
    static short int p = 0;
    double point;

    // simulate a blocking read delay
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 20000;
    select(0, 0, 0, 0, &tv);

    // generate a sample and return it
    p = (p + 27) % 1024;
    point = cos(p * PI / 512);
    return point;
}

