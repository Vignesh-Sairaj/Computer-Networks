compile with -std=c++11 option

Make multiple calls (8) to sendFrame, which sends a single frame.
Choice provided at every frame whether to corrupt or not.


SWS = 7
RWS = 1

Therefore, num of sequence numbers = 7+1 = 8
Update seq number as seqno = (seqno+1)%(SWS+RWS)

A nack halts and resets the group of frames.


Problem: goes into infinite loop for some reason
