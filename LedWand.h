/* 
 * File:   LedWand.h
 * Author: ron
 *
 * Created on December 16, 2010, 5:42 PM
 */

#ifndef _LEDWAND_H
#define	_LEDWAND_H

#include <cstdio>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include "GfxHndl.h"
#include "GfxCntr.h"
#include "GfxObjs.h"

#define ROWS 20
#define COLS 56
#define MODULE_WIDTH 8
#define MODULE_HEIGHT 8
#define MODULE_HEIGHT_SPACE 12

class LedWand {
public:
    LedWand(char[], int);
    LedWand(const LedWand& orig);
    virtual ~LedWand();
    void Do();
    void HardReset();
    void Clear();
    void SendText(char*, int);
    void SendTextPos(char*, int, short, short);
    void SendTextPos(char*, int, short, short, short, short);
    void SetBrightnessFull(short);
    void SetBrighnessPos(short, short, short, short = 1, short = 1);
    void Refresh();
    void FillWand(char);
    void FullWand();
    void AppendBuffer(char*, int);
    void AppendBufferLine();
    void SetBuffer();
    void SendBuffer();
    void DDoS();
    void Kill();
    void Benchmark(int);
    void GFX_SendBuff();
    void GFX_SendBuff(char*);
    void GFX_LED_Set(int, int, bool = true);
    void GFX_ClrBuff();
    //void GFX_Rawput(char);

private:
    void udpSend(char*, int);
    void udpSendCmd(short, short, short, short, short, char*, int, bool = true);
    int setSocketTimeout(int, int);
    char* byteEncode(short&);
    char buffer[4096];
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char display_buffer[ROWS*COLS];
    int disp_buff_pos;
    char gfx_buff[ROWS*COLS*MODULE_WIDTH];


};

#endif	/* _LEDWAND_H */

