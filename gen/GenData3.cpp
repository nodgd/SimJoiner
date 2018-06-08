#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

const int N = 100;
const int MAX_N = N + 5;
const int LINE_LENGTH = 50;
const int MAX_LINE_LENGTH = LINE_LENGTH + 5;
const int Z = 26;
const int MAX_Z = Z + 5;
const int Q = 100;
const int MAX_Q = Q + 5;
const int INF_INT = 999999999;

int lineLength[MAX_N];
char lineList[MAX_N][MAX_LINE_LENGTH];

int edQueryLength[MAX_N];
char edQueryList[MAX_N][MAX_LINE_LENGTH];
int edQueryTh[MAX_N];

void initRandom() {
    FILE * f = fopen("data/gen3.srand.in", "r");
    int base = 0;
    if (f != NULL) {
        fscanf(f, "%d", & base);
        fclose(f);
    }
    srand(base);
    f = fopen("data/gen3.srand.in", "w");
    fprintf(f, "%d\n", base + 1);
    fclose(f);
}

int randInt() {
    int t = rand() << 15 | rand();
    return t >= 0 ? t : ~ t;
}

int getEd(const char * a, int aLen, const char * b, int bLen, int th) {
    static int f[2][MAX_LINE_LENGTH];
    if (aLen - bLen > th || bLen - aLen > th) {
        return th + 1;
    }
    int * f0 = f[0];
    for (int j = 0; j <= bLen && j <= th; j ++) {
        f0[j] = j;
    }
    if (aLen <= bLen) {
        for (int i = 1; i <= aLen; i ++) {
            int * fi = f[i & 1];
            int * fi_ = f[(i & 1) ^ 1];
            fi[0] = i;
            char ai = a[i - 1];
            int minF = aLen - bLen - i;
            minF = i + (minF >= 0 ? minF : - minF);
            int jd2 = i + bLen - aLen - th;
            int jd = 1 >= jd2 ? 1 : jd2;
            int ju2 = i + th;
            int ju = bLen <= ju2 ? bLen : ju2;
            for (int j = jd; j <= ju; j ++) {
                int t = fi_[j - 1] + (ai != b[j - 1]);
                if (j < ju2 && t > fi_[j] + 1) {
                    t = fi_[j] + 1;
                }
                if (j > jd && t > fi[j - 1] + 1) {
                    t = fi[j - 1] + 1;
                }
                fi[j] = t;
                int td = aLen - bLen - i + j;
                td = td >= 0 ? td : - td;
                minF = minF > t + td ? t + td : minF;
            }   
            if (minF > th) {
                return th + 1;
            }
        }
        if (f[aLen & 1][bLen] <= th) {
            return f[aLen & 1][bLen];
        } else {
            return th + 1;
        }
    } else {
        for (int i = 1; i <= aLen; i ++) {
            int * fi = f[i & 1];
            int * fi_ = f[(i & 1) ^ 1];
            fi[0] = i;
            char ai = a[i - 1];
            int minF = aLen - bLen - i;
            minF = i + (minF >= 0 ? minF : - minF);
            int jd2 = i - th;
            int jd = 1 >= jd2 ? 1 : jd2;
            int ju2 = i + bLen - aLen + th;
            int ju = bLen <= ju2 ? bLen : ju2;
            for (int j = jd; j <= ju; j ++) {
                int t = fi_[j - 1] + (ai != b[j - 1]);
                if (j < ju2 && t > fi_[j] + 1) {
                    t = fi_[j] + 1;
                }
                if (j > jd && t > fi[j - 1] + 1) {
                    t = fi[j - 1] + 1;
                }
                fi[j] = t;
                int td = aLen - bLen - i + j;
                td = td >= 0 ? td : - td;
                minF = minF > t + td ? t + td : minF;
            }   
            if (minF > th) {
                return th + 1;
            }
        }
        if (f[aLen & 1][bLen] <= th) {
            return f[aLen & 1][bLen];
        } else {
            return th + 1;
        }
    }
}

void genLine(int I) {
    char * line = lineList[I];
    int length = randInt() % LINE_LENGTH + 1;
    lineLength[I] = length;
    for (int i = 0; i < length; i ++) {
        int t = randInt() % Z + 'a';
        line[i] = t;
    }
    int cntA = randInt() % (length + 1);
    for (int i = 0; i < cntA; i ++) {
        int j = randInt() % length;
        line[j] = 'a';
    }
}

void genLine() {
    for (int i = 0; i < N; i ++) {
        genLine(i);
    }
    FILE * fout = fopen("data/gen3.data.in", "w");
    for (int i = 0; i < N; i ++) {
        fprintf(fout, "%s\n", lineList[i]);
    }
    fclose(fout);
}

void genEdQuery(int I) {
    char * line = edQueryList[I];
    int length = randInt() % LINE_LENGTH + 1;
    edQueryLength[I] = length;
    for (int i = 0; i < length; i ++) {
        int t = randInt() % Z + 'a';
        line[i] = t;
    }
    int cntA = randInt() % (length + 1);
    for (int i = 0; i < cntA; i ++) {
        int j = randInt() % length;
        line[j] = 'a';
    }
    int minEd = INF_INT;
    for (int j = 0; j < N; j ++) {
        int ed = getEd(lineList[j], lineLength[j], edQueryList[I], edQueryLength[I], MAX_LINE_LENGTH);
        minEd = minEd > ed ? ed : minEd;
    }
    edQueryTh[I] = minEd + randInt() % (LINE_LENGTH / 10);
}

void genEdQuery() {
    for (int i = 0; i < Q; i ++) {
        genEdQuery(i);
    }
    FILE * fout = fopen("data/gen3.ed.in", "w");
    for (int i = 0; i < Q; i ++) {
        fprintf(fout, "%s\n", edQueryList[i]);
        fprintf(fout, "%d\n", edQueryTh[i]);
    }
    fclose(fout);
}

int main() {
    initRandom();
    genLine();
    genEdQuery();
    return 0;
}
