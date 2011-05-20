/* 
 * File:   main.cpp
 * Author: ron
 *
 * Created on December 16, 2010, 5:33 PM
 */

#include <stdlib.h>
#include "LedWand.h"

using namespace std;

void help();

/*
 * 
 */
int main(int argc, char** argv) {
    LedWand ld("172.23.42.120", 2342);
    //ld.Do();
    //exit(0);
    if (argc < 2)
        help();

    if (strcmp(argv[1], "--hardreset") == 0)
        ld.HardReset();
    else if (strcmp(argv[1], "--clear") == 0)
        ld.Clear();
    else if (strcmp(argv[1], "--refresh") == 0)
        ld.Refresh();
    else if (strcmp(argv[1], "--full") == 0)
        ld.FullWand();
    else if (strcmp(argv[1], "--ddos") == 0)
        ld.DDoS();
    else if (strcmp(argv[1], "--kill") == 0)
        ld.Kill();
    else if (strcmp(argv[1], "--benchmark") == 0) {
        if (argc >= 3)
            ld.Benchmark(atoi(argv[2]));
        else
            ld.Benchmark(50);
    } else if (strcmp(argv[1], "-i") == 0) {
        char c;
        int i = 0;
        int send_rep = 1;
        if (argc >= 3)
            if (atoi(argv[2]) > 0)
                send_rep = atoi(argv[2]);
        while (1) {
            system("stty raw");
            c = fgetc(stdin);
            system("stty cooked");
            ld.AppendBuffer(&c, 1);
            if (c == 0xFFFFFFFF) {
                ld.SendBuffer();
                exit(0);
            }
            printf("0x%02x ", c);
            i++;
            if (i % send_rep == 0)
                ld.SendBuffer();
        }
    } else if (strcmp(argv[1], "-l") == 0) {
        char buffer[COLS + 1];
        buffer[COLS] = '\0';
        int i = 0;
        int secs = 10;
        if (argc >= 3)
            if (atoi(argv[2]) > 0)
                secs = atoi(argv[2]);
        while (1) {
            int len;
            char c;
            for (int x = 0; x < COLS; x++) {
                if (cin.fail())
                    exit(0);
                cin.get(c);
                len = x;
                if (c == '\n' || c == 0) {
                    buffer[x] = 0;
                    break;
                } else
                    buffer[x] = c;
            }
            ld.AppendBuffer(buffer, len);
            cout << buffer << endl;
            if (i % (ROWS -1) == (ROWS -2)) {
                ld.SendBuffer();
                sleep(secs);
            }
            i++;
            ld.AppendBufferLine();
        }
    } else if (argc >= 3) {
        if (strcmp(argv[1], "--set-bright") == 0) {
            if (argc < 5)
                ld.SetBrightnessFull(atoi(argv[2]));
            else {
                if (argc < 7)
                    ld.SetBrighnessPos(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
                else
                    ld.SetBrighnessPos(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));

            }
        } else if (strcmp(argv[1], "--fill") == 0)
            ld.FillWand(argv[2][0]);
    } else
        cout << "Unknown command: " << argv[1] << endl;

    return (EXIT_SUCCESS);
}

void help() {
    cout << "Usage: ";
    exit(0);
}

