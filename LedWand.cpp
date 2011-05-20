/* 
 * File:   LedWand.cpp
 * Author: ron
 * 
 * Created on December 16, 2010, 5:42 PM
 */

#include "LedWand.h"
#include "GfxHndl.h"
using namespace std;

LedWand::LedWand(char* hostname, int portnr) {
    for (int i = 0; i < ROWS * COLS; i++)
        display_buffer[i] = 0;
    for (int i = 0; i < ROWS * COLS * MODULE_WIDTH; i++)
        gfx_buff[i] = 0;
    disp_buff_pos = -1;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
        std::cerr << "Error: Can't open socket!";
    else
        cout << "Open socket... Done.\n";

    server = gethostbyname(hostname);
    if (server == NULL) {
        std::cerr << "Error: Can't resolve hostname.";
    } else
        cout << "Hostname resolved.\n";
    cout << "Connection now to " << hostname << "... ";
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr,
            (char *) & serv_addr.sin_addr.s_addr,
            server->h_length);
    serv_addr.sin_port = htons(portnr);
    if (connect(sockfd, (sockaddr const*) & serv_addr, sizeof (serv_addr)) < 0)
        std::cerr << "Error: Can't connect.";
    else
        cout << "Done.\n";

    setSocketTimeout(sockfd, 1000);
}

LedWand::LedWand(const LedWand& orig) {
}

LedWand::~LedWand() {
}

void LedWand::udpSend(char* byteSend, int len) {
    // << "Sending now: " << byteSend + 10 << "\n";
    //printf("%x", byteSend + 11);
    //printf("Sending now: %x \n",byteSend+10);
    //write(sockfd,byteSend,len);
    //this-> n = read(sockfd,this->buffer,4096);
    send(sockfd, byteSend, len, 0);
    recv(sockfd, this->buffer, 4096, 0);
    return;
}

int LedWand::setSocketTimeout(int connectSocket, int milliseconds) {
    struct timeval tv;
    tv.tv_sec = milliseconds / 1000;
    tv.tv_usec = (milliseconds % 1000) * 1000;
    cout << "Set timeout to: " << milliseconds << endl;
    return setsockopt(connectSocket, SOL_SOCKET, SO_RCVTIMEO, (char*) & tv, sizeof tv);
}

void LedWand::udpSendCmd(short cmd, short xpos, short ypos, short xsize, short ysize, char* str, int len, bool nullbyte) {
    if (len > 1400)
        cout << "Warning! You are exceeding the maximum package length!" << endl;

    char result[11 + len];
    char* tmp = byteEncode(cmd);
    memcpy(result, tmp, 2);
    delete tmp;

    tmp = byteEncode(xpos);
    memcpy(result + 2, tmp, 2);
    delete tmp;

    tmp = byteEncode(ypos);
    memcpy(result + 4, tmp, 2);
    delete tmp;

    tmp = byteEncode(xsize);
    memcpy(result + 6, tmp, 2);
    delete tmp;

    tmp = byteEncode(ysize);
    memcpy(result + 8, tmp, 2);
    delete tmp;

    if (len > 0 && str != NULL)
        memcpy(result + 10, str, len);

    if (nullbyte)result[10 + len] = 0;

    udpSend(result, 10 + len + ((nullbyte) ? 1 : 0));
}

char* LedWand::byteEncode(short &input) {
    char* result = new char[2];
    result[0] = input & 0xFF;
    result[1] = (input >> 8) & 0xFF;
    return result;
}

void LedWand::SendText(char* sendByte, int len) {
    udpSendCmd(4, 0, 0, 0, 0, sendByte, len);
    cout << "Using SendText(char*,int). Warning, this command is deprecated!" << endl;
}

void LedWand::SendTextPos(char* c, int l, short x, short y) {
    short sizex = (l + x - 1 > COLS - x) ? COLS - x : l + x - 1;
    short sizey = ((l / sizex) + y - 1 > ROWS - y) ? ROWS - y : (l / sizex) + y - 1;
    udpSendCmd(3, x, y, sizex, sizey, c, l, false);
}

void LedWand::SendTextPos(char* c, int len, short x, short y, short sizex, short sizey) {
    udpSendCmd(3, x, y, sizex, sizey, c, len);
}

void LedWand::HardReset() {
    udpSendCmd(11, 0, 0, 0, 0, NULL, 0);
}

void LedWand::SetBrighnessPos(short brightness, short x, short y, short sizex, short sizey) {
    char b = brightness % 0xFF;
    char buffer[sizex * sizey];
    for (int i = 0; i < sizex * sizey; i++) {
        buffer[i] = b;
    }

    udpSendCmd(5, x, y, sizex, sizey, buffer, sizex * sizey);
}

void LedWand::SetBrightnessFull(short brightness) {

    char buff = brightness % 0xFF;
    udpSendCmd(7, 0, 0, 0, 0, &buff, 1);
}

void LedWand::FillWand(char c) {
    char buff[ROWS * COLS];
    for (int i = 0; i < ROWS * COLS; i++) {

        buff[i] = c;
    }
    udpSendCmd(3, 0, 0, COLS, ROWS, buff, ROWS * COLS);
}

void LedWand::FullWand() {
    FillWand(0xDB);
}

void LedWand::Clear() {

    udpSendCmd(2, 0, 0, 0, 0, NULL, 0);
}

void LedWand::Refresh() {

    udpSendCmd(8, 0, 0, 0, 0, NULL, 0);
}

void LedWand::AppendBuffer(char* str, int len) {
    for (int i = 0; i < len; i++) {
        disp_buff_pos++;
        if (disp_buff_pos >= ROWS * COLS) {
            disp_buff_pos = 0;

        }
        if (disp_buff_pos % COLS == 0)
            for (int i = 0; i < COLS; i++)
                display_buffer[disp_buff_pos + i] = 0;
        if (str[i] == '\n')
            AppendBufferLine();
        else
            display_buffer[disp_buff_pos] = str[i];
    }
}

void LedWand::SendBuffer() {
    //SendTextPos(display_buffer, ROWS*COLS, 0, 0, COLS, ROWS);
    char sendbuff[ROWS * COLS];
    int fullrow = disp_buff_pos + 1 + COLS - ((disp_buff_pos + 1) % COLS);
    //printf("%d", fullrow);
    for (int i = fullrow; i < fullrow + ROWS * COLS; i++)
        sendbuff[i - fullrow] = display_buffer[i % (ROWS * COLS)];
    SendTextPos(sendbuff, ROWS*COLS, 0, 0, COLS, ROWS);
}

void LedWand::AppendBufferLine() {
    char c = ' ';
    while (((disp_buff_pos + 1) % COLS) != 0)
        AppendBuffer(&c, 1);
}

void LedWand::DDoS() {
    char sendb[10];
    sendb[0] = 0x08;
    sendb[1] = 0x00;
    while (1)
        send(sockfd, sendb, 10, 0);
}

void LedWand::Kill() {
    for (int i = 0; i < 100; i++) {
        char bf[30000];
        for (int i = 0; i < 10000; i++)
            bf[i] = 0x00;
        udpSendCmd(0x0010, 0, 10000, 0, 0, gfx_buff, 10000);
    }
}

void LedWand::Benchmark(int n) {
    cout << "Creating buffer... ";
    char buf1[ROWS * COLS * MODULE_WIDTH];
    char buf2[ROWS * COLS * MODULE_WIDTH];
    for (int i = 0; i < ROWS * COLS * MODULE_WIDTH; i++) {
        buf1[i] = (i % 2 == 0) ? 85 : 170;
        buf2[i] = (i % 2 == 1) ? 85 : 170;
    }
    cout << "Done." << endl;
    cout << "Starting benchmark with " << n << " frames..." << endl;
    timeval start, end;
    gettimeofday(&start, 0);
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0)
            GFX_SendBuff(buf1);
        else
            GFX_SendBuff(buf2);
        if (i % 10 == 9) {
            gettimeofday(&end, 0);
            long long diff = end.tv_sec * 1000000 + end.tv_usec - start.tv_sec * 1000000 - start.tv_usec;
            cout << "Send " << (i + 1) << " frames in " << diff / 1000000.0 << " seconds (" << (i + 1) / ((double) diff)*1000000.0 << " fps)." << endl;
        }
    }
    gettimeofday(&end, 0);
    long long diff = end.tv_sec * 1000000 + end.tv_usec - start.tv_sec * 1000000 - start.tv_usec;
    cout << "Completed..." << endl;
    cout << "Send " << n << " frames in " << diff / 1000000.0 << " seconds (" << n / ((double) diff)*1000000.0 << " fps)." << endl;

}

void LedWand::GFX_LED_Set(int x, int y, bool state) {
    if (x >= MODULE_WIDTH * COLS)
        x = MODULE_WIDTH * COLS - 1;
    if (y >= MODULE_HEIGHT * ROWS)
        y = MODULE_HEIGHT * ROWS - 1;
    if (state)
        gfx_buff[(y / MODULE_HEIGHT) * COLS * MODULE_WIDTH + x] |= (1 << (7 - (y % 8)));
    else
        gfx_buff[(y / MODULE_HEIGHT) * COLS * MODULE_WIDTH + x] &= ~(1 << (7 - (y % 8)));
}

void LedWand::GFX_SendBuff() {
    GFX_SendBuff(gfx_buff);
}

void LedWand::GFX_SendBuff(char* buf) {
    int partSize = ROWS * COLS * MODULE_WIDTH / 7;
    for (int i = 0; i < 6; i++) {
        udpSendCmd(0x0010, i*partSize, partSize, 0, 0, buf + partSize*i, partSize);
    }
    udpSendCmd(0x0010, 6 * partSize, partSize, 2342, 2342, buf + partSize * 6, partSize);
    //udpSendCmd(0x0011, 0, 0, 0, 0, 0, 0);
}

void LedWand::GFX_ClrBuff() {
    for (int i = 0; i < ROWS * COLS * MODULE_WIDTH; i++)
        gfx_buff[i] = 0;
}

void LedWand::Do() {
    //Clear();
    Refresh();
    Benchmark(50);
    SetBrightnessFull(2);

    GfxHndl gfx(gfx_buff);
    GfxClock cl;
    GfxCntr co;
    co.Add(&cl);
    gfx.DrawContainer(&co);
    while (1) {
        cl.Update();
        GFX_ClrBuff();
        gfx.DrawContainer(&co);

        GFX_SendBuff();
        sleep(1);
    }

    return;
    GfxCntr cnt;
    GfxCircle cir;
    GfxRectangle rec;
    cir.SetPosition(300, 100);
    cir.SetRadius(100);
    cir.SetPenWidth(6);
    rec.SetWidth(200);
    rec.SetHeight(100);
    rec.SetPosition(200, 100);
    rec.SetPenWidth(5);
    GfxChess ch;
    GfxLine li;
    li.SetPosition(10, 10);
    li.SetRelEndPos(200, 150);
    li.SetPenWidth(6);
    //cnt.Add(&rec);
    //cnt.Add(&cir);
    cnt.Add(&ch);
    GFX_ClrBuff();
    gfx.DrawContainer(&cnt);

    GFX_SendBuff();
    while (1) {
        GFX_ClrBuff();
        gfx.DrawContainer(&cnt);

        GFX_SendBuff();

        ch.Move(-1, 0);
    }

    return;

    gfx.DrawLine(1, 1, 201, 201);
    gfx.DrawLine(201, 1, 1, 201);
    gfx.DrawLine(101, 1, 101, 201);
    gfx.DrawLine(1, 101, 201, 101);

    gfx.DrawLine(1, 59, 201, 142);
    gfx.DrawLine(1, 142, 201, 59);
    gfx.DrawLine(59, 1, 142, 201);
    gfx.DrawLine(142, 1, 59, 201);

    gfx.DrawCircle(101, 101, 100, 3);
    gfx.DrawCircle(101, 101, 75, 2);
    gfx.DrawCircle(101, 101, 50, 2);
    gfx.DrawCircle(101, 101, 25, 1);

    gfx.DrawCircle(1, 1, 1, 201);

    gfx.DrawLine(1, 1, 201, 1);
    gfx.DrawLine(1, 1, 1, 201);
    gfx.DrawLine(201, 1, 201, 201);
    gfx.DrawLine(1, 201, 201, 201);
    //gfx.DrawCircle(300,150,90,3);
    //gfx.DrawEllipse(150,100,120,60,2);
    GFX_SendBuff();
    //for (int i = 0; i < 0xFFFF; i++) {
    char c[2];
    c[0] = 0xFF;
    c[1] = 0xF0;
    //udpSendCmd(0x0010, 56 * 8 - 12, 2, 0, 0, c, 2);
    //udpSendCmd(0x0011, 0, 0, 0, 0, 0, 0);
    //  }
    //while (1)
    //HardReset();
    //udpSendCmd(4, 0, 0, 0, 0, "Hallo Welt!", 11);
    // FullWand();
    // SetBrighnessPos(12, 8, 8);
    //FullWand();
    //SendTextPos("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX", 54, 1, 5);
    //Refresh();
}


