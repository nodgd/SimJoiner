#include "SimJoiner.h"






///类函数
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
    result.clear();
    return SUCCESS;
}

int SimJoiner::joinED(
    const char * afn,
    const char * bfn,
    unsigned th,
    VEDR & result
) {
    result.clear();
    return SUCCESS;
}
