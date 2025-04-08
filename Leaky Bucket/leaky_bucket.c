#include <stdio.h>
#include <stdlib.h>

struct Frame {
    int data;
    int seqNum;
};

struct Frame currentFrame;
int ackNum;
int timer = 5;
int delay;
int terminate = 0;
char currentTurn = 's'; // Start with sender
int frameError = 1;
int ackError = 1;

void sender();
void receiver();

int main() {
    currentFrame.data = 0;
    currentFrame.seqNum = 0;

    while (!terminate) {
        sender();
        for (delay = 1; delay <= 10000000; delay++); // Simulated delay
        receiver();
    }

    return 0;
}

void sender() {
    static int ackReceivedFlag = 0;

    if (currentTurn == 's') {
        if (ackError == 0) { // ACK lost, resend last frame
            printf("SENDER: Resending packet with Seq No: %d\n", currentFrame.seqNum);
            frameError = rand() % 4;
            if (frameError == 0)
                printf("SENDER: Error while sending packet\n");

            currentTurn = 'r';
        } else {
            if (ackReceivedFlag == 1)
                printf("SENDER: Received ACK for packet %d\n", ackNum);

            if (currentFrame.seqNum == 5) {
                terminate = 1;
                return;
            }

            currentFrame.data += 1;
            currentFrame.seqNum += 1;

            printf("SENDER: Sending packet with Seq No: %d\n", currentFrame.seqNum);

            frameError = rand() % 4;
            if (frameError == 0)
                printf("SENDER: Error while sending packet\n");

            currentTurn = 'r';
            ackReceivedFlag = 1;
        }
    } else {
        timer--;
        printf("SENDER: Timer decreasing to %d\n", timer);
        if (timer == 0) {
            currentTurn = 's';
            ackError = 0; // simulate ACK lost
            timer = 5;
        }
    }
}

void receiver() {
    static int expectedSeq = 1;

    if (currentTurn == 'r') {
        if (frameError != 0) {
            if (currentFrame.seqNum == expectedSeq) {
                printf("RECEIVER: Received packet with Seq No: %d\n", currentFrame.seqNum);
                ackNum = currentFrame.seqNum;
                expectedSeq++;
            } else {
                printf("RECEIVER: Duplicate packet received with Seq No: %d\n", expectedSeq - 1);
                ackNum = expectedSeq - 1;
            }

            currentTurn = 's';
            ackError = rand() % 4;
            if (ackError == 0)
                printf("RECEIVER: Error while sending ACK\n");

        } else {
            printf("RECEIVER: Frame lost or corrupted. Waiting for retransmission.\n");
        }
    }
}
