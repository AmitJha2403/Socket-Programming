//Stop and wait ARQ is also referred to as the alternating protocol is a method used in two-way communication
//systems to send information between two connected devices (sender and a receiver). It is referred to as stop
//and wait ARQ because the function of this protocol is to send one frame at a time . After sending a frame or
//packet, the sender doesn’t send any further packets until it receives an acknowledgement from the receiver.
//Moreover, the sender keeps a copy of the sent packet. After receiving the desired frame, the receiver sends an
//acknowledgement. If the acknowledgement does not reach the sender before the specified time, known as the
//timeout, the sender sends the same packet again. The timeout is reset after each frame transmission. The
//above scenario depicts a Stop and wait situation, so this control mechanism is termed as Stop and wait ARQ. 
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#define TIMEOUT 5
#define MAX_SEQ 1
#define TOT_PACKETS 3

#define inc(k) if(k<MAX_SEQ) k++; else k=0;

typedef struct
{
    int data;
}packet;

typedef struct
{
    int kind;
    int seq;
    int ack;
    packet info;
    int err;
}frame;
frame DATA;
typedef enum{frame_arrival,err,timeout,no_event} event_type;

void from_network_layer(packet *);
void to_network_layer(packet *);
void to_physical_layer(frame *);
void from_physical_layer(frame *);
void wait_for_event_sender(event_type *);
void wait_for_event_receiver(event_type *);
void receiver();
void sender();

int i=1;
char turn;
int DISCONNECT=0;

int main()
{
    while(DISCONNECT)
    {
        sender();

        for(long int i =0;i<1000000;i++);
        receiver();
    }

    return 0;
}

void sender()
{
    static int sn=0;
    static frame s;
    packet buffer;
    event_type event;
    static int flag=0;

    if(flag==0){
        from_network_layer(&buffer);
        s.info = buffer;
        s.seq = sn;
        printf("\nSENDER : Info = %d    seq No = %d     ", s.info, s.seq);
        turn = 'r';
        to_physical_layer(&s);
        flag = 1;
    }
    wait_for_event_sender(&event);
    if (turn == 's')
    {
        if (event == frame_arrival)
        {
            from_network_layer(&buffer);
            inc(sn);
            s.info = buffer;
            s.seq = sn;
            printf("\nSENDER : Info = %d    seq No = %d     ", s.info, s.seq);
            turn = 'r';
            to_physical_layer(&s);      
        }
        if (event == timeout)
        {
            printf("\nSENDER : Resending Frame          ");
            turn = 'r';
            to_physical_layer(&s);
        }
    }
}

void receiver()
{
    static int rn=0;
    frame fr,fs;
    event_type event;
    wait_for_event_receiver(&event);
    if(turn=='r')
    {
        if(event==frame_arrival)
        {
            from_physical_layer(&fr);
            if(fr.seq==rn)
            {
                to_network_layer(&fr.info);
                inc(rn);
            }       
            else{
                printf("\nRECEIVER : Duplicate Frame Acknowledge Resent\n");    
            }

            turn = 's';
            to_physical_layer(&fs);
        }
        if(event==err)
        {
            printf("\nRECEIVER : Corrupted Frame\n");
            turn = 's';
        }
    }
}

void from_network_layer(packet *buffer)
{
    (*buffer).data = i;
    i++;
}

void to_physical_layer(frame *s)
{
    s->err = rand()%2;
    printf("\n\terror rate =%d", s->err);
    DATA = *s;
}

void to_network_layer(packet *buffer)
{
    printf("\nRECEIVER : Packet %d received, Ack Sent \n", (*buffer).data);
    if(i>TOT_PACKETS)
    {
        DISCONNECT = 1;
        printf("\nDISCONNECT");
    }
}
void from_physical_layer(frame *buffer)

{
    *buffer =DATA;
}
void wait_for_event_sender(event_type *e)
{
    static int timer=0;

    if(turn=='s')
    {
        timer++;
        if(timer==TIMEOUT)
        {
            *e=timeout;
            printf("\nSENDER:Ack not received=> TIMEOUT\n");
            timer = 0;
            return;
        }
        if (DATA.err==0){
            *e=err;
        }
        else
        {
            timer = 0;
            *e = frame_arrival;
        }
    }
}

void wait_for_event_receiver(event_type *e)
{
    if(turn=='r'){
        if(DATA.err==0){
            *e = err;
        }
        else{
            *e = frame_arrival;
        }
    }
}



