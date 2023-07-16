# Socket-Programming
The project aims to develop a chat application using socket programming in the C programming language<br>  <br>
Socket programming enables communication between two or more computers over a network, allowing them to exchange data. The application will be implemented using the client-server model, where clients can connect to a central server and exchange messages with each other. <br> <br>
The project has 2 socket programs, sender.c and receiver.c, that communicate together using “Datagram Sockets (UDP)" <br> <br>
1. Sender: Sender gets a command line argument for the number of packets to be sent, say P. The sender sends the packet to the receiver and starts a retransmission timer. Let the sequence number in this packet be x. <br> <br>
   (a) If the retransmission timer expires and there is no acknowledgment from the receiver, the sender will retransmit the same packet.<br> <br>
   (b) If the sender receives an acknowledgment before the retransmission timer expires and
the acknowledgment contains x+1 as its sequence number then the sender transmits
the packet with sequence number x+1. <br> <br>
   (c) If there is an acknowledgment from the receiver containing a sequence number other than
x+1 then the sender has to ignore the acknowledgment. <br> <br> <br>
2. Receiver: The receiver responds to the packet received from the sender with an acknowledgment
that it has received the packet. The steps followed by the receiver (each time it receives a
packet) are: <br> <br>
   (a) Receive the packet and check if the packet with the correct sequence number is received (the
receiver expects the very first packet to have sequence number 1). <br> <br>
   (b) If the sequence no. is incorrect, an acknowledgment is sent with the expected packet
sequence number (which is x+1, where x is the sequence no. of the last packet received
from the sender). <br> <br>
   (c) If the sequence no. is correct, the following steps are done: <br>
        - A uniformly distributed random variable is generated between 0 and 1. If this number
is less than the PacketDropProbability then no acknowledgement is generated. The
receiver will wait for the sender to retransmit the packet with the same Sequence
number. <br>
        - Otherwise, anacknowledgmentt will be sent to the sendercontaining the sequence no. of the packet the receiver expectsg next (format mentioned
below). <br>
