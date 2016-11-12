compile with -std=c++11 option

inputs a file named "multiport.cpp"

outputs 3 files - receiverout.txt, senderout.txt and multiportCopy.cpp


Each character byte is broken into its constituent bits and the whole array is appended with vrc bits at every 32 bit intervals.
Each word of size 33 bits is sent to client. The client reconstructs the bytes and outputs into a copy of the file.
The transaction is recorded in the other 2 files.