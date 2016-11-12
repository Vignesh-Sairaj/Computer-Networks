#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <stdexcept>
#include <cstdlib>
#include <cmath>
#include <cstdlib>
#include <ctime>


using namespace std;

//CRC-8  --- x^{8}+x^{7}+x^{6}+x^{4}+x^{2}+1  = {1, 1, 1, 0, 1, 0, 1, 0, 1}
//CRC-12 ---x^{{12}}+x^{{11}}+x^{3}+x^{2}+x+1 = {1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1}


//VRC Word size = 512/8 = 64 bits
//LRC Column size = 512/8 = 64 bits => Word size = 8 bits
//Sum Column size = 512/8 = 64 bits => Word size = 8 bits

pair<vector<int>, vector<int> > xorDividePbyQ(vector<int> dividend, vector<int> divisor);

//result stored in b
void xorBus(vector<int>::iterator aBegin, vector<int>::iterator aEnd, vector<int>::iterator bBegin, vector<int>::iterator bEnd);
//

vector<int> crcAppend(vector<int> msg, vector<int> genPoly);
bool crcCheck(vector<int> rcvMsg, vector<int> genPoly);



long long unsigned bin2dec(vector<int> bin);
vector<int> dec2bin(long long unsigned dec, int numBits);

vector<int> onesCompAdd(vector<int>::iterator aBegin, vector<int>::iterator aEnd, vector<int>::iterator bBegin, vector<int>::iterator bEnd);

std::vector<int> checkSumEncode(std::vector<int> msg, int wrdSize);
bool checkSumDecode(std::vector<int> rcvMsg, int wrdSize);


vector<int> lrcEncode(std::vector<int> msg, int wrdSize);
bool lrcDecode(std::vector<int> rcvMsg, int wrdSize);


vector<int> vrcEncode(vector<int> msg, int wrdSize);
bool vrcDecode(std::vector<int> rcvMsg, int wrdSize);


void simulation(void);


int main(void)
{
/*  //CHECK DIVIDE & CRC
    vector<int> dividend {1, 1, 1, 1};
    vector<int> divisor {1, 1, 1};

    pair<vector<int>, vector<int> > ans = xorDividePbyQ(dividend, divisor);

    cout << "\nquotient: {";
    for (auto x : ans.first)
    {
        cout << " " << x << " ";
    }
    cout << "}" << endl;

    cout << "\nremainder: {";
    for (auto x : ans.second)
    {
        cout << " " << x << " ";
    }
    cout << "}" << endl;

    vector<int> crcMsg = crcAppend(dividend, divisor);
    cout << "\nTransmitted message: {";
    for (auto x : crcMsg)
    {
        cout << " " << x << " ";
    }
    cout << "}" << endl;    

    cout << "crcCheck on msg returned: " << (crcCheck(crcMsg, divisor)? "True" : "False") << endl;*/

/*  
    //CHECK CONVERSION
    std::vector<int> bin = {1, 0, 0, 0};
    long long unsigned dec = bin2dec(bin);
    std::vector<int> binRe = dec2bin(dec, 4);

    cout << "dec: " << dec << endl;

    cout << "\nBinary: {";
    for (auto x : binRe)
    {
        cout << " " << x << " ";
    }
    cout << "}" << endl;*/

/*  
    //CHECK ONE'S COMPL
    std::vector<int> a = {0, 1, 0, 0};
    std::vector<int> b = {1, 1, 1, 0};


    std::vector<int> ans  = onesCompAdd(a.begin(), a.end(), b.begin(), b.end());

    cout << "size of final ans: " << ans.size() << endl;


    cout << "\nans: {";
    for (auto x : ans)
    {
        cout << " " << x << " ";
    }
    cout << "}" << endl;*/

/*    //CHECK SUM
    std::vector<int> msg = {0, 1, 0, 1, 1, 0, 1, 0};

    std::vector<int> rcvMsg = checkSumEncode(msg, 4);

    cout << "\nrcvMsg: {";
    for (auto x : rcvMsg)
    {
        cout << " " << x << " ";
    }
    cout << "}" << endl;

    //corrupting...
    rcvMsg[0] = rcvMsg[0] == 0;
    rcvMsg[2] = rcvMsg[2] == 0;

    rcvMsg[5] = rcvMsg[5] == 0;
    rcvMsg[7] = rcvMsg[7] == 0;

    cout << "\nrcvMsg: {";
    for (auto x : rcvMsg)
    {
        cout << " " << x << " ";
    }
    cout << "}" << endl;


    bool correct = checkSumDecode(rcvMsg, 4);
    cout << "The modified message is: " << (correct? "correct" : "corrupted") << endl;
*/


/*    std::vector<int> msg = {1, 1, 1, 1, 1, 0, 1, 0};

    std::vector<int> rcvMsg = lrcEncode(msg, 4);

    cout << "\norgMsg: {";
    for (auto x : msg)
    {
        cout << " " << x << " ";
    }
    cout << "}" << endl;

    cout << "\nrcvMsg: {";
    for (auto x : rcvMsg)
    {
        cout << " " << x << " ";
    }
    cout << "}" << endl;

    //corrupting...
    rcvMsg[0] = rcvMsg[0] == 0;
    rcvMsg[2] = rcvMsg[2] == 0;

    rcvMsg[5] = rcvMsg[5] == 0;
    rcvMsg[7] = rcvMsg[7] == 0;

    cout << "\nrcvMsg: {";
    for (auto x : rcvMsg)
    {
        cout << " " << x << " ";
    }
    cout << "}" << endl;


    bool correct = lrcDecode(rcvMsg, 4);
    cout << "The modified message is detected as: " << (correct? "correct" : "corrupted") << endl;*/

/*
    //CHECK VRC
    std::vector<int> msg = {0, 1, 0, 1, 1, 0, 0, 1};

    std::vector<int> rcvMsg = vrcEncode(msg, 4);

    cout << "\norgMsg: {";
    for (auto x : msg)
    {
        cout << " " << x << " ";
    }
    cout << "}" << endl;

    cout << "\nrcvMsg: {";
    for (auto x : rcvMsg)
    {
        cout << " " << x << " ";
    }
    cout << "}" << endl;

    //corrupting...
    rcvMsg[0] = rcvMsg[0] == 0;
    //rcvMsg[2] = rcvMsg[2] == 0;

    rcvMsg[4] = rcvMsg[4] == 0;
    //rcvMsg[7] = rcvMsg[7] == 0;

    cout << "\nrcvMsg: {";
    for (auto x : rcvMsg)
    {
        cout << " " << x << " ";
    }
    cout << "}" << endl;


    bool correct = vrcDecode(rcvMsg, 4);
    cout << "The modified message is detected as: " << (correct? "correct" : "corrupted") << endl;*/

    simulation();

    return 0;
}




void simulation(void) {

    srand(time(NULL));

    int NUM_ERROR_BITS = 20;

    int NUM_TRIALS = 500;
    int MSG_LEN = 512;
    int vrcWrdSize = 64;
    int lrcWrdSize = 8;

    vector<int>crc8Poly = {1, 1, 1, 0, 1, 0, 1, 0, 1};
    vector<int>crc12Poly = {1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1};

    int vrcCount=0, lrcCount=0, crc8Count=0, crc12Count=0, checkSumCout=0;

    for (int i = 0; i < NUM_TRIALS; ++i)
    {
        vector<int> msg(MSG_LEN);

        for(int j = 0 ; j < static_cast<int>(msg.size()); j++) {

            msg[i] = rand()%2;
        }

        vector<int> vrcMsg = vrcEncode(msg, vrcWrdSize);
        vector<int> lrcMsg = lrcEncode(msg, lrcWrdSize);
        vector<int> crc8Msg = crcAppend(msg, crc8Poly);
        vector<int> crc12Msg = crcAppend(msg, crc12Poly);
        vector<int> checkSumMsg = checkSumEncode(msg, lrcWrdSize);

        //find(vector.begin(), vector.end(), item) != vector.end()
        vector<int> listOfInd8;

        for (int j = 0; j < NUM_ERROR_BITS; ++j) 
        {
            int index;
            do {
                index = rand()%(MSG_LEN+lrcWrdSize); //lrcWrd size = num extra bits
            } while( find(listOfInd8.begin(), listOfInd8.end(), index) != listOfInd8.end() ); //to avoid repitition

            listOfInd8.push_back(index);
        }

        for (int j = 0; j < MSG_LEN+lrcWrdSize; ++j)
        {
            if(find(listOfInd8.begin(), listOfInd8.end(), j) != listOfInd8.end()) { //j is an index to be flipped
                vrcMsg[j] = vrcMsg[j] == 0;
                lrcMsg[j] = lrcMsg[j] == 0;
                crc8Msg[j] = crc8Msg[j] == 0;
                crc12Msg[j] = crc12Msg[j] == 0;
                checkSumMsg[j] = checkSumMsg[j] == 0;
            }
        }

        vrcCount += vrcDecode(vrcMsg, vrcWrdSize);
        lrcCount += lrcDecode(lrcMsg, lrcWrdSize);
        crc8Count += crcCheck(crc8Msg, crc8Poly);
        crc12Count += crcCheck(crc12Msg, crc12Poly);
        checkSumCout += checkSumDecode(checkSumMsg, lrcWrdSize);
    }

    cout << "Result of simulation: " << endl;

    cout <<  "- vrcCount: " << vrcCount << endl;
    cout <<  "- lrcCount: " << lrcCount << endl;
    cout <<  "- crc8Count: " << crc8Count << endl;
    cout <<  "- crc12Count: " << crc12Count << endl;
    cout <<  "- checkSumCout: " << checkSumCout << endl;

    return;
}









pair<vector<int>, vector<int> > xorDividePbyQ(vector<int> dividend, vector<int> divisor) {

    vector<int> quotient;
    vector<int> rem;

    if (dividend.size() >= divisor.size()) {

        vector<int> arr (dividend.begin(), dividend.end());

        int i=0;

        while(  i <= static_cast<int>( arr.size() ) - static_cast<int>( divisor.size() )  ) { //while bits are left in arr (dividend)
            if (arr[i] == 0) {
                quotient.push_back(0);
                i++;
            }

            else if (arr[i] == 1) {
                xorBus(divisor.begin(), divisor.end(), arr.begin()+i, arr.begin()+i+divisor.size());
                quotient.push_back(1);
                i++;
            }

            else {
                cout << "Values in array must be 0/1, encountered value: " << arr[i] << " at index: " << i;
                throw runtime_error("Expected value 0/1.\n");
            }
        }

        vector<int> remainder1 (arr.end() - static_cast<int>(divisor.size()), arr.end());
        rem = remainder1;
    }

    else {
        quotient.push_back(0);
        vector<int> remainder1 (divisor.size(), 0);
        copy(dividend.begin(), dividend.end(), remainder1.end() - dividend.size());
        rem = remainder1;
    }

    std::vector<int> remainder (divisor.size()-1);

    copy(rem.begin()+1, rem.end(), remainder.begin());

    pair<vector<int>, vector<int> > ans = make_pair(quotient, remainder);
    return ans;
}


// XOR of a and b stored in b
void xorBus(vector<int>::iterator aBegin, vector<int>::iterator aEnd, vector<int>::iterator bBegin, vector<int>::iterator bEnd) {

    if(aEnd-aBegin != bEnd-bBegin) {
        throw runtime_error("ERROR\nxorBus: Widths do not match!\n");
    }

    for (vector<int>::iterator aPtr = aBegin, bPtr = bBegin; aPtr != aEnd; ++aPtr, ++bPtr)
    {
        *bPtr = ((*aPtr) != 0) != ((*bPtr) != 0); //Logical xor
    }

    return;
}





vector<int> crcAppend(vector<int> msg, vector<int> genPoly) {
    vector<int> ans (msg.size() + genPoly.size()-1, 0);

    copy(msg.begin(), msg.end(), ans.begin());

    pair<vector<int>, vector<int> > divs = xorDividePbyQ(ans, genPoly);

    copy(divs.second.begin(), divs.second.end(), ans.begin()+msg.size());

    return ans;
}


bool crcCheck(vector<int> rcvMsg, vector<int> genPoly) {

    pair<vector<int>, vector<int> > divs = xorDividePbyQ(rcvMsg, genPoly);


/*    cout << "Inside check:" << endl;

    cout << "\nquotient: {";
    for (auto x : divs.first)
    {
        cout << " " << x << " ";
    }
    cout << "}" << endl;


    cout << "\nremainder: {";
    for (auto x : divs.second)
    {
        cout << " " << x << " ";
    }
    cout << "}" << endl;
*/

    bool ans = true;

    for (auto x : divs.second) {
        if(x != 0) {
            ans = false;
            break;
        }
    }

    return ans;
}



long long unsigned bin2dec(vector<int> bin) {

    if (! (bin.size() > 0) )
    {
        throw runtime_error("Error in bin2dec: vector of size zero!\n");
    }

    long long unsigned dec = 0;
    for (int i = 0; i < static_cast<int>(bin.size()); ++i) {
        dec += bin[i] * pow(2, (bin.size()-1 - i));
    }
    return dec;
}


vector<int> dec2bin(long long unsigned dec, int numBits) {

    vector<int> bin(numBits);

    for (int i = numBits-1; i >= 0; --i) {
        bin[i] = dec%2;
        dec /= 2;
    }

    return bin;
}


vector<int> onesCompAdd(vector<int>::iterator aBegin, vector<int>::iterator aEnd, vector<int>::iterator bBegin, vector<int>::iterator bEnd) {
    if(aEnd-aBegin != bEnd-bBegin) {
        throw runtime_error("ERROR\nxorBus: Widths do not match!\n");
    }

    std::vector<int> aVector (aBegin, aEnd);
    std::vector<int> bVector (bBegin, bEnd);

    int size = static_cast<int>(aEnd-aBegin);

    long long unsigned aDec = bin2dec(aVector);
    long long unsigned bDec = bin2dec(bVector);

    long long unsigned sum = aDec + bDec;

    long long unsigned radix = pow(2, size);

    while(sum >= radix) {
        long long unsigned quotient = sum/radix;
        sum = sum % radix;
        sum = sum + quotient; 
    }


    vector<int> ans = dec2bin(sum, size);
    return ans;
}


std::vector<int> checkSumEncode(std::vector<int> msg, int wrdSize) {

    if(msg.size()%wrdSize != 0) {
        throw runtime_error("Msg not compatible with wrdSize\n");
    }

    std::vector<int> sum(wrdSize, 0);

    for (int i = 0; i < static_cast<int>(msg.size())/wrdSize; ++i)
    {
        sum = onesCompAdd(sum.begin(), sum.end(), msg.begin()+(wrdSize*i), msg.begin()+(wrdSize*(i+1)) );
    }

    vector<int> transMsg(msg.begin(), msg.end());

    for (int i = 0; i < static_cast<int>(sum.size()); ++i)
    {
        transMsg.push_back(sum[i] == 0); //One' Complement, Negating
    }

    return transMsg;
}

bool checkSumDecode(std::vector<int> rcvMsg, int wrdSize) {

    if(rcvMsg.size()%wrdSize != 0) {
        throw runtime_error("rcvMsg not compatible with wrdSize\n");
    }

    std::vector<int> sum(wrdSize, 0);

    for (int i = 0; i < static_cast<int>(rcvMsg.size()/wrdSize); ++i)
    {
        sum = onesCompAdd(sum.begin(), sum.end(), rcvMsg.begin()+(wrdSize*i), rcvMsg.begin()+(wrdSize*(i+1)) );
    }

    bool correct = true;

    for (int i = 0; i < static_cast<int>(sum.size()); ++i)
    {
        if(sum[i] == 0) {
            correct = false;
            break;
        }
    }

/*    cout << "\nsum: {";
    for (auto x : sum)
    {
        cout << " " << x << " ";
    }
    cout << "}" << endl;*/

    return correct;
}

vector<int> lrcEncode(std::vector<int> msg, int wrdSize) {

    if(msg.size()%wrdSize != 0) {
        throw runtime_error("Msg not compatible with wrdSize\n");
    }

    std::vector<int> parity(wrdSize, 0);

    for (int i = 0; i < static_cast<int>(msg.size())/wrdSize; ++i)
    {
        xorBus(msg.begin()+(wrdSize*i), msg.begin()+(wrdSize*(i+1)), parity.begin(), parity.end());
    }

    vector<int> transMsg(msg.begin(), msg.end());

    for (int i = 0; i < static_cast<int>(parity.size()); ++i)
    {
        transMsg.push_back(parity[i]); //One' Complement, Negating
    }

    return transMsg;
}

bool lrcDecode(std::vector<int> rcvMsg, int wrdSize) {

    if(rcvMsg.size()%wrdSize != 0) {
        throw runtime_error("rcvMsg not compatible with wrdSize\n");
    }

    std::vector<int> parity(wrdSize, 0);

    for (int i = 0; i < static_cast<int>(rcvMsg.size())/wrdSize; ++i)
    {
        xorBus(rcvMsg.begin()+(wrdSize*i), rcvMsg.begin()+(wrdSize*(i+1)), parity.begin(), parity.end());
    }

    bool correct = true;

    for (int i = 0; i < static_cast<int>(parity.size()); ++i)
    {
        if(parity[i] != 0) {
            correct = false;
            break;
        }
    }

/*    cout << "\nparity: {";
    for (auto x : parity)
    {
        cout << " " << x << " ";
    }
    cout << "}" << endl;*/

    return correct;
}


vector<int> vrcEncode(vector<int> msg, int wrdSize) {
    if(msg.size()%(wrdSize) != 0) { 
        throw runtime_error("rcvMsg not compatible with wrdSize\n");
    }

    int numWrds = msg.size()/wrdSize;
    std::vector<int> transMsg;

    transMsg.reserve(msg.size() + numWrds);

    for (int i = 0; i < numWrds; ++i)
    {
        int parity = 0;

        for (int j = wrdSize*i; j < wrdSize*(i+1); ++j)
        {
            parity = (msg[j] != 0) != (parity != 0); //xor
            transMsg.push_back(msg[j]);
        }

        transMsg.push_back(parity);
    }

    return transMsg;
}


bool vrcDecode(std::vector<int> rcvMsg, int wrdSize) {
    if(rcvMsg.size()%(wrdSize+1) != 0) { //One extra bit per word
        throw runtime_error("rcvMsg not compatible with wrdSize\n");
    }

    int numWrds = rcvMsg.size()/(wrdSize+1);

    bool correct = true;

    for (int i = 0; i < numWrds; ++i)
    {
        int parity = 0;

        for (int j = (wrdSize+1)*i; j < (wrdSize+1)*(i+1); ++j)
        {
            parity = (rcvMsg[j] != 0) != (parity != 0); //xor
        }

        if (parity != 0) {
            correct = false;
            break;
        }
    }

    return correct;
}