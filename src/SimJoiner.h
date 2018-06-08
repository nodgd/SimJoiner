#ifndef __EXP2_SIMJOINER_H__
#define __EXP2_SIMJOINER_H__

#include <vector>

template <typename IDType, typename SimType>
struct JoinResult {
    IDType id1;
    IDType id2;
    SimType s;
};

typedef JoinResult < unsigned, double > JaccardJoinResult;
typedef JoinResult < unsigned, unsigned > EDJoinResult;
typedef JaccardJoinResult JCR;
typedef EDJoinResult EDR;
typedef std::vector < JCR > VJCR;
typedef std::vector < EDR > VEDR;

const int SUCCESS = 0;
const int FAILURE = 1;

class SimJoiner {

public:
    SimJoiner();
    ~SimJoiner();

public:
    /*
     * 找所有Jaccard不小于th的二元组
     * afn: in 文件A的路径
     * bfn: in 文件B的路径
     * th: in Jaccard下限
     * result: out 找到的二元组
     */
    int joinJaccard(
        const char * afn,
        const char * bfn,
        double th,
        VJCR & result
    );

    /*
     * 找所有编辑距离不超过th的二元组
     * afn: in 文件A的路径
     * bfn: in 文件B的路径
     * th: in 编辑距离上限
     * result: out 找到的二元组
     */
    int joinED(
        const char * afn,
        const char * bfn,
        unsigned th,
        VEDR & result
    );
};

#endif
