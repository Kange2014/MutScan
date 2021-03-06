#ifndef PE_SCANNNER_H
#define PE_SCANNNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "read.h"
#include "mutation.h"
#include <cstdlib>
#include <condition_variable>
#include <mutex>
#include <thread>
#include "rollinghash.h"


using namespace std;

struct ReadPairPack {
    ReadPair** data;
    int count;
};

typedef struct ReadPairPack ReadPairPack;

struct ReadPairRepository {
    ReadPairPack** packBuffer;
    size_t readPos;
    size_t writePos;
    size_t readCounter;
    std::mutex mtx;
    std::mutex readCounterMtx;
    std::condition_variable repoNotFull;
    std::condition_variable repoNotEmpty;
};

typedef struct ReadPairRepository ReadPairRepository;

class PairEndScanner{
public:
    PairEndScanner(string mutationFile, string refFile, string read1File, string read2File, string html="", int threadnum=1);
    ~PairEndScanner();
    bool scan();
    void textReport(vector<Mutation>& mutationList, vector<Match*> *mutationMatches);
    void htmlReport(vector<Mutation>& mutationList, vector<Match*> *mutationMatches);

private:
    bool scanPairEnd(ReadPairPack* pack);
    bool scanRead(Read* r, ReadPair* originalRead, bool reversed);
    void initPackRepository();
    void destroyPackRepository();
    void producePack(ReadPairPack* pack);
    void consumePack();
    void producerTask();
    void consumerTask();
    void pushMatch(int i, Match* m, bool needStoreReadToDelete);

private:
    string mMutationFile;
    string mRefFile;
    string mRead1File;
    string mRead2File;
    string mHtmlFile;
    ReadPairRepository mRepo;
    bool mProduceFinished;
    vector<Mutation> mutationList;
    vector<Match*> *mutationMatches;
    std::mutex mMutationMtx;
    int mThreadNum;
    RollingHash* mRollingHash;
    vector<Read*> mReadToDelete;
    vector<char*> mBufToDelete;
};


#endif