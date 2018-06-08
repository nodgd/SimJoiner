#include <cstdio>

#include "SimJoiner.h"

using namespace std;

void run0(int argc, char ** argv) {
    SimJoiner joiner;

    vector<EDJoinResult> resultED;
    vector<JaccardJoinResult> resultJaccard;

    unsigned edThreshold = 2;
    double jaccardThreshold = 0.85;

    joiner.joinJaccard(argv[1], argv[2], jaccardThreshold, resultJaccard);
    joiner.joinED(argv[1], argv[2], edThreshold, resultED);
}

void run1() {
    SimJoiner sj;
    VJCR vjcr;
    sj.joinJaccard("data/gen4.A.in", "data/gen4.B.in", 0.85, vjcr);
    printf("run1() vjcr.size = %d\n", (int) vjcr.size());
    for (int i = 0, sz = vjcr.size(); i < sz; i ++) {
        printf("run1() vjcr[%d] = { %d, %d, %f }\n", vjcr[i].id1, vjcr[i].id2, vjcr[i].s);
    }
}

int main(int argc, char **argv) {
    if (argc == 3) {
        run0(argc, argv);
    } else {
        run1();
    }
    return 0;
}
