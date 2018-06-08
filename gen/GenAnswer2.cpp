#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <ctime>

const int MAX_N = 200000 + 5;
const int MAX_S = 50 + 5;
const int MAX_W = 200 + 5;
const int MAX_Q = 5000 + 5;

int N;
int lineSize[MAX_N];
int lineElem[MAX_N][MAX_S];
int W;
int Q;
int ms[MAX_Q][MAX_Q];
double jc[MAX_Q][MAX_Q];

double jcValue[MAX_S << 1][MAX_S];

double getAbs(double x) {
    return x >= 0 ? x : - x;
}

int randInt() {
    return rand() << 15 | rand();
}

double getJc1(const int * a, int aSize, const int * b, int bSize, int minSame) {
    int curSame = 0;
    for (int i = 0, j = 0; i < aSize && j < bSize;) {
        int ai = a[i];
        int bj = b[j];
        if (ai == bj) {
            curSame ++;
            i ++;
            j ++;
        } else if (ai < bj) {
            i ++;
        } else {
            j ++;
        }
    }
    if (curSame < minSame) {
        return -1;
    } else {
        return (double) curSame / (aSize + bSize - curSame);
    }
}

double getJc2(const int * a, int aSize, const int * b, int bSize, int minSame) {
    int curSame = 0;
    for (int i = aSize - 1, j = bSize - 1; i >= 0 && j >= 0;) {
        int d = a[i] - b[j];
        if (d == 0) {
            curSame ++;
            i --;
            j --;
        } else if (d > 0) {
            i --;
        } else {
            j --;
        }
        if (curSame + i + 1 < minSame || curSame + j + 1 < minSame) {
            return -1;
        }
    }
    if (curSame < minSame) {
        return -1;
    } else {
        return (double) curSame / (aSize + bSize - curSame);
    }
}

double getJc3(const int * a, int aSize, const int * b, int bSize, int minSame) {
    int curSame = 0;
    for (int i = aSize - 1, j = bSize - 1; i >= 0 && j >= 0;) {
        int d = a[i] - b[j];
        curSame += d == 0;
        i -= d >= 0;
        j -= d <= 0;
        if (curSame + i + 1 < minSame || curSame + j + 1 < minSame) {
            return -1;
        }
    }
    if (curSame < minSame) {
        return -1;
    } else {
        return (double) curSame / (aSize + bSize - curSame);
    }
}

int getJcSame4(const int * a, int aSize, const int * b, int bSize, int minSame) {
    int curSame = 0;
    for (int i = aSize - 1, j = bSize - 1; i >= 0 && j >= 0;) {
        int d = a[i] - b[j];
        curSame += d == 0;
        i -= d >= 0;
        j -= d <= 0;
        if (curSame + i + 1 < minSame || curSame + j + 1 < minSame) {
            return 0;
        }
    }
    if (curSame < minSame) {
        return 0;
    } else {
        return curSame;
    }
}

int main() {
    srand(12345);
    
    N = 200000;
    W = 200;
    for (int i = 0; i < N; i ++) {
        lineSize[i] = 30 + randInt() % 21;
        for (int j = 0; j < lineSize[i]; j ++) {
            int t;
            do {
                t = randInt() % W;
                bool flag = true;
                for (int k = 0; k < j; k ++) {
                    if (t == k) {
                        flag = false;
                    }
                }
                if (flag == true) {
                    break;
                }
            } while (true);
            lineElem[i][j] = t;
        }
        std::sort(lineElem[i], lineElem[i] + lineSize[i]);
    }
    
    Q = 4000;
    for (int i = 0; i < Q; i ++) {
        for (int j = 0; j < i; j ++) {
            int t = lineSize[i];
            t = t > lineSize[j] ? lineSize[j] : t;
            t = randInt() % t + 1;
            ms[i][j] = t;
        }
    }
    
    for (int i = 0; i < (MAX_S << 1); i ++) {
        jcValue[i][0] = -1;
        for (int j = 1; j <= i && j < MAX_S; j ++) {
            jcValue[i][j] = (double) j / (double) (i - j);
        }
    }
    
    int time_1 = clock();
    for (int i = 0; i < Q; i ++) {
        for (int j = 0; j < i; j ++) {
            jc[i][j] = getJc3(lineElem[i], lineSize[i], lineElem[j], lineSize[j], ms[i][j]);
        }
    }
    time_1 = clock() - time_1;
    
    double error = 0;
    int time_2 = clock();
    for (int i = 0; i < Q; i ++) {
        for (int j = 0; j < i; j ++) {
            int s = getJcSame4(lineElem[i], lineSize[i], lineElem[j], lineSize[j], ms[i][j]);
            double t = jcValue[lineSize[i] + lineSize[j]][s];
            error += getAbs(t - jc[i][j]);
        }
    }
    time_2 = clock() - time_2;
    
    printf("error = %lf\n", error);
    printf("time1 = %d\ntime2 = %d\n", time_1, time_2);
    
    return 0;
}
