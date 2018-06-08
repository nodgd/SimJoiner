#include <cstdio>
#include <cstring>
#include <assert.h>
#include <algorithm>
#include <cmath>

#include "SimJoiner.h"

#define PRINT_FLAG

typedef unsigned long long uint64;

/**
 * 常量定义
 */
//文本行数
const int MAX_N = 200000 + 8;
//每行长度
const int LINE_LENGTH = 256;
const int MAX_LINE_LENGTH = LINE_LENGTH + 8;
//ASCII字符集大小
const int MAX_Z = 128 + 8;
//编辑距离选用的Q-gram
const int Q = 3;
//单词散列的模数和基
const uint64 HASH_P = 7633587785259559ull;
const int HASH_B = 131;
//单词散列桶数和节点数
const int HASH_BUCKET = 4000037;
const int MAX_HASH_NODE = 200000 * 256;
//最大单词数量
const int MAX_W = 128 * 128 * 128;
//最大倒排表总大小
const int MAX_WORD_TABLE_SPACE = 200000 * 64;
//int无穷大
const int INF_INT = 999999999;
//浮点数精度容忍
const int EPS_DOUBLE = 1e-8;

/**
 * 变量定义
 */
//查询模式和参数
int queryMode;//1=jc, 2=ed
double jcTh;
double jcThd;
int edTh;

//输入文件A的所有行
int aN;
char aLine[MAX_N][MAX_LINE_LENGTH];
int aLineLength[MAX_N];
//输入文件B的所有行
int bN;
char bLine[MAX_N][MAX_LINE_LENGTH];
int bLineLength[MAX_N];

//字符集重新编码
int Z;
int ZToId[MAX_Z];
char ZToAscii[MAX_Z];

//单词的散列表
int wtTotal;
int wtHead[HASH_BUCKET];
uint64 wtKey[MAX_HASH_NODE];
char * wtPtr[MAX_HASH_NODE];

/**
 * 能用到的函数
 */
/*
 * 预处理Jaccard的常数
 * th: in 原始的查询参数
 */
static void initJcConstant(double th) {
    queryMode = 1;
    jcTh = th;
    jcThd = th / (1.0 + th);
}

/*
 * 预处理编辑距离的常数
 * th: in 原始的查询参数
 */
static void initEdConstant(int th) {
    queryMode = 2;
    edTh = th;
}

/*
 * 带限制的获取字符串的长度，并去掉行末\r
 * buf: in 字符串的指针
 * maxLength: in 长度限制
 * return: out 字符串长度
 */
static int getStringLength(char * buf, int maxLength) {
    int length = 0;
    while (length < maxLength && buf[length]) {
        length ++;
    }
    while (length > 0 && buf[length - 1] == '\r') {
        length --;
        buf[length] = 0;
    }
    return length;
}

/*
 * 读取输入文件
 * afn: in 文件A的路径
 * bfn: in 文件B的路径
 */
static void readInputFile(const char * afn, const char * bfn) {
    static char buf[MAX_LINE_LENGTH];
    memset(buf, 0, sizeof(buf));
    //读取文件A
    FILE * af = fopen(afn, "r");
    assert(af != NULL);
    aN = 0;
    while (fscanf(af, "%[^\n]\n", buf) == 1) {
        int length = getStringLength(buf, LINE_LENGTH);
        aLineLength[aN] = length;
        memcpy(aLine[aN], buf, sizeof(char) * length);
        aLine[aN][length] = 0;
        aN ++;
        memset(buf, 0, sizeof(buf));
    }
    fclose(af);
    //读取文件B
    FILE * bf = fopen(bfn, "r");
    assert(bf != NULL);
    bN = 0;
    while (fscanf(bf, "%[^\n]\n", buf) == 1) {
        int length = getStringLength(buf, LINE_LENGTH);
        bLineLength[bN] = length;
        memcpy(bLine[bN], buf, sizeof(char) * length);
        bLine[bN][length] = 0;
        bN ++;
        memset(buf, 0, sizeof(buf));
    }
    fclose(bf);
    printf("readInputFile() aN = %d, bN = %d\n", aN, bN);
}

/*
 * 字符集重新编号
 */
static void createAsciiId() {
    //统计出现过的字符
    memset(ZToId, 0, sizeof(ZToId));
    for (int i = 0; i < aN; i ++) {
        char * line = aLine[i];
        int length = aLineLength[i];
        for (int j = 0; j < length; j ++) {
            ZToId[(int) line[j]] = 1;
        }
    }
    for (int i = 0; i < bN; i ++) {
        char * line = bLine[i];
        int length = bLineLength[i];
        for (int j = 0; j < length; j ++) {
            ZToId[(int) line[j]] = 1;
        }
    }
    //给字符分配编号
    Z = 0;
    memset(ZToAscii, 0, sizeof(ZToAscii));
    for (int c = 0; c < MAX_Z; c ++) {
        if (ZToId[c] == 1) {
            ZToId[c] = Z;
            ZToAscii[Z] = c;
            Z ++;
        } else {
            ZToId[c] = -1;
        }
    }
}

/*
 * 清空单词的散列表
 */
static void wordHashTableClear() {
    memset(wtHead, 0, sizeof(wtHead));
    wtTotal = 0;
}

/*
 * 插入单词的散列表
 * ptr: in 单词的头指针
 * return: out 单词的流水编号
 */
static void wordHashTableInsert(const char * ptr, int len) {
    uint64 key = 0;
    for (int i = 0; i < len; ptr ++) {
        //TODO
    }
}

/*
 * 构建文件A的单词树以及临时元素序列
 */
static void createJcAWordTree() {
    static int wordList[MAX_LINE_LENGTH];
    //初始化单词散列表
    if (wtTotal != 0) {
        wordHashTableClear();
    }
    //处理文件A的每一行
    for (int i = 0; i < aN; i ++) {
        char * line = aLine[i];
        int length = aLineLength[i];
        //划分单词，插入单词树
        int wordCnt = 0;
        for (int l, r = 0; l < length;) {
            for (l = r; l < length && line[l] == ' '; l ++);
            for (r = l; r < length && line[r] != ' '; r ++);
            int wordId = wordHashTableInsert(line + l, r - l);
            wordList[wordCnt] = wordId;
            wordCnt ++;
        }
        //临时元素序列
        int * elem = AElem[i];
        int ec = 0;
        std::sort(wordList, wordList + wordCnt);
        for (int j = 0; j < wordCnt; j ++) {
            if (j == 0 || wordList[j] != wordList[j - 1]) {
                AWordTime[wordList[j]] ++;
                elem[ec] = wordList[j];
                ec ++;
            }
        }
        AElemCnt[i] = ec;
    }
}




static void joinJc(const char * afn, const char * bfn, double th, VJCR & result) {
    result.clear();
    initJcConstant(th);
    readInputFile(afn, bfn);
    createAsciiId();





}

static void joinEd(const char * afn, const char * bfn, int th, VEDR & result) {
    result.clear();
    initEdConstant(th);





}

/**
 * 类函数
 */
SimJoiner::SimJoiner() {
}

SimJoiner::~SimJoiner() {
}

int SimJoiner::joinJaccard(
    const char * afn,
    const char * bfn,
    double th,
    VJCR & result
) {
    joinJc(afn, bfn, th, result);
    return SUCCESS;
}

int SimJoiner::joinED(
    const char * afn,
    const char * bfn,
    unsigned th,
    VEDR & result
) {
    joinEd(afn, bfn, th, result);
    return SUCCESS;
}
