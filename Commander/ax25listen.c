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
#include <sys/socket.h>
#include <netax25/ax25.h>

struct full_sockaddr_ax25 sa_bind;

void rot13(char* buf, int len) {
	int i;
	for (i = 0; i < len; ++i) {
		char up = buf[i] &~ 0x20;
		if (up >= 'A' && up <= 'M') {
			buf[i] += 13;
		}
		if (up >= 'N' && up <= 'Z') {
			buf[i] -= 13;
		}
	}
}

int main(int argc, char** argv) {
	int sock_ax;
	int conn_sock_ax;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <callsign>\n", (argc>0)?argv[0]:"???");
		exit(-1);
	}

	const char* local_call = argv[1];

	const int bufferlen = 1024;
	char buffer[bufferlen];
	int recvlen;

	puts("socket...");

	// create socket
	if ((sock_ax = socket(PF_AX25, SOCK_SEQPACKET, 0)) == -1) {
		perror("socket");
		exit(-1);
	}

	puts("sockaddr...");

	// define address
	sa_bind.fsa_ax25.sax25_family = AF_AX25;
	sa_bind.fsa_ax25.sax25_ndigis = 0;
	ax25_aton_entry(local_call, sa_bind.fsa_ax25.sax25_call.ax25_call);

	puts("bind...");

	// bind socket to address
	if (bind(sock_ax, (struct sockaddr*)&sa_bind, sizeof(struct full_sockaddr_ax25)) == -1) {
		perror("bind");
		exit(-1);
	}

	puts("listen...");

	// attempt to connect to address
	if (listen(sock_ax, SOMAXCONN) != 0) {
		perror("listen");
		exit(-1);
	}

	puts("accept...");
	
	// hope for a connection
	for/*ever*/ (;;) {

		conn_sock_ax = accept(sock_ax, NULL, NULL);
		if (conn_sock_ax < 0) {
			perror("accept");
			exit(-1);
		}

		puts("accepted!");
		
		while ((recvlen = recv(conn_sock_ax, buffer, bufferlen, 0)) > 0) {

			printf("recv %d bytes\n", recvlen);

			rot13(buffer, recvlen);

			recvlen = send(conn_sock_ax, buffer, recvlen, 0);
			if (recvlen < 0) {
				perror("recv");
				exit(-1);
			}

			printf("send %d bytes\n", recvlen);
			
		}
		if (recvlen < 0) {
			perror("recv");
			exit(-1);
		}

		puts("close...");
		if (close(conn_sock_ax) != 0) {
			perror("close");
			exit(-1);
		}
	}

	return 0;
}
