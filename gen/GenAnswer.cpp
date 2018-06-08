/*
 *  编辑距离：经过不断优化，得到getEd8在不加编译器优化和开启-O3优化时都是最快的 
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>

const int MAX_N = 200000 + 5;
const int MAX_LINE_LENGTH = 256 + 5;
const int INF_INT = 999999999;
const int SPEED_TEST_N = 500;
const char DATA_FILE_NAME[] = "data\\gen2.data.in";
const char ED_QUERY_FILE_NAME[] = "data\\gen2.ed.in";
const char JC_QUERY_FILE_NAME[] = "data\\gen2.jc.in";
char ED_ANSWER_FILE_NAME[100] = "data\\gen2.ed.ans";
char JC_ANSWER_FILE_NAME[100] = "data\\gen2.jc.ans";

int N;
std::string lineList[MAX_N];

void readData() {
    printf("readData()\n");
    FILE * fin = fopen(DATA_FILE_NAME, "r");
    static char buf[MAX_LINE_LENGTH];
    N = 0;
    while (fscanf(fin, "%[^\n]\n", buf) == 1) {
        lineList[N] = buf;
        N ++;
    }
    fclose(fin);
}

//没有优化 
int getEd1(std::string & a, std::string & b, int th) {
    static int f[MAX_LINE_LENGTH][MAX_LINE_LENGTH];
    int aLen = a.length();
    int bLen = b.length();
    for (int i = 0; i <= aLen; i ++) {
        f[i][0] = i;
    }
    for (int j = 0; j <= bLen; j ++) {
        f[0][j] = j;
    }
    for (int i = 1; i <= aLen; i ++) {
        char ai = a[i - 1];
        for (int j = 1; j <= bLen; j ++) {
            int t = f[i - 1][j - 1] + (ai != b[j - 1]);
            if (t > f[i - 1][j] + 1) {
                t = f[i - 1][j] + 1;
            }
            if (t > f[i][j - 1] + 1) {
                t = f[i][j - 1] + 1;
            }
            f[i][j] = t;
        }
    }
    if (f[aLen][bLen] <= th) {
        return f[aLen][bLen];
    } else {
        return th + 1;
    }
}

//基于getEd1，优化二维数组寻址 
int getEd2(std::string & a, std::string & b, int th) {
    static int f[2][MAX_LINE_LENGTH];
    int aLen = a.length();
    int bLen = b.length();
    int * f0 = f[0];
    for (int j = 0; j <= bLen; j ++) {
        f0[j] = j;
    }
    for (int i = 1; i <= aLen; i ++) {
        int * fi = f[i & 1];
        int * fi_ = f[(i & 1) ^ 1];
        fi[0] = i;
        char ai = a[i - 1];
        for (int j = 1; j <= bLen; j ++) {
            int t = fi_[j - 1] + (ai != b[j - 1]);
            if (t > fi_[j] + 1) {
                t = fi_[j] + 1;
            }
            if (t > fi[j - 1] + 1) {
                t = fi[j - 1] + 1;
            }
            fi[j] = t;
        }
    }
    if (f[aLen & 1][bLen] <= th) {
        return f[aLen & 1][bLen];
    } else {
        return th + 1;
    }
}

//基于getEd2，优化std::string调用 
int getEd3(std::string & a, std::string & b, int th) {
    static int f[2][MAX_LINE_LENGTH];
    const char * a_ = a.c_str();
    const char * b_ = b.c_str();
    int aLen = a.length();
    int bLen = b.length();
    int * f0 = f[0];
    for (int j = 0; j <= bLen; j ++) {
        f0[j] = j;
    }
    for (int i = 1; i <= aLen; i ++) {
        int * fi = f[i & 1];
        int * fi_ = f[(i & 1) ^ 1];
        fi[0] = i;
        char ai = a_[i - 1];
        for (int j = 1; j <= bLen; j ++) {
            int t = fi_[j - 1] + (ai != b_[j - 1]);
            if (t > fi_[j] + 1) {
                t = fi_[j] + 1;
            }
            if (t > fi[j - 1] + 1) {
                t = fi[j - 1] + 1;
            }
            fi[j] = t;
        }
    }
    if (f[aLen & 1][bLen] <= th) {
        return f[aLen & 1][bLen];
    } else {
        return th + 1;
    }
}

//基于getEd3，优化决策分支预测（负优化） 
int getEd4(std::string & a, std::string & b, int th) {
    static int f[2][MAX_LINE_LENGTH];
    const char * a_ = a.c_str();
    const char * b_ = b.c_str();
    int aLen = a.length();
    int bLen = b.length();
    int * f0 = f[0];
    for (int j = 0; j <= bLen; j ++) {
        f0[j] = j;
    }
    for (int i = 1; i <= aLen; i ++) {
        int * fi = f[i & 1];
        int * fi_ = f[(i & 1) ^ 1];
        fi[0] = i;
        char ai = a_[i - 1];
        for (int j = 1; j <= bLen; j ++) {
            if (ai != b_[j - 1]) {
                int t = fi_[j - 1] + 1;
                if (t > fi_[j] + 1) {
                    t = fi_[j] + 1;
                }
                if (t > fi[j - 1] + 1) {
                    t = fi[j - 1] + 1;
                }
                fi[j] = t;
            } else {
                fi[j] = fi_[j - 1];
            }
        }
    }
    if (f[aLen & 1][bLen] <= th) {
        return f[aLen & 1][bLen];
    } else {
        return th + 1;
    }
}

//基于getEd3，截断优化 
int getEd5(std::string & a, std::string & b, int th) {
    static int f[2][MAX_LINE_LENGTH];
    const char * a_ = a.c_str();
    const char * b_ = b.c_str();
    int aLen = a.length();
    int bLen = b.length();
    int * f0 = f[0];
    for (int j = 0; j <= bLen; j ++) {
        f0[j] = j;
    }
    for (int i = 1; i <= aLen; i ++) {
        int * fi = f[i & 1];
        int * fi_ = f[(i & 1) ^ 1];
        fi[0] = i;
        char ai = a_[i - 1];
        int minF = INF_INT;
        for (int j = 1; j <= bLen; j ++) {
            int t = fi_[j - 1] + (ai != b_[j - 1]);
            if (t > fi_[j] + 1) {
                t = fi_[j] + 1;
            }
            if (t > fi[j - 1] + 1) {
                t = fi[j - 1] + 1;
            }
            fi[j] = t;
            minF = minF > t ? t : minF;
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

//基于getEd5，优化j的取值范围 
int getEd6(std::string & a, std::string & b, int th) {
    static int f[2][MAX_LINE_LENGTH];
    const char * a_ = a.c_str();
    const char * b_ = b.c_str();
    int aLen = a.length();
    int bLen = b.length();
    if (aLen - bLen > th || bLen - aLen > th) {
        return th + 1;
    }
    int * f0 = f[0];
    for (int j = 0; j <= bLen && j <= th; j ++) {
        f0[j] = j;
    }
    for (int i = 1; i <= aLen; i ++) {
        int * fi = f[i & 1];
        int * fi_ = f[(i & 1) ^ 1];
        fi[0] = i;
        char ai = a_[i - 1];
        int minF = INF_INT;
        int jd1 = 1;
        int jd2 = i - th;
        int jd3 = i + bLen - aLen - th;
        int jd = jd1 >= jd2 ? jd1 >= jd3 ? jd1 : jd3 : jd2 >= jd3 ? jd2 : jd3;
        int ju1 = bLen;
        int ju2 = i + th;
        int ju3 = i + bLen - aLen + th;
        int ju = ju1 <= ju2 ? ju1 <= ju3 ? ju1 : ju3 : ju2 <= ju3 ? ju2 : ju3;
        for (int j = jd; j <= ju; j ++) {
            int t = fi_[j - 1] + (ai != b_[j - 1]);
            if (j < ju2 && j < ju3 && t > fi_[j] + 1) {
                t = fi_[j] + 1;
            }
            if (j > jd && t > fi[j - 1] + 1) {
                t = fi[j - 1] + 1;
            }
            fi[j] = t;
            minF = minF > t ? t : minF;
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

//基于getEd6，固定aLen <= bLen 
int getEd7(std::string & a, std::string & b, int th) {
    int aLen = a.length();
    int bLen = b.length();
    static int f[2][MAX_LINE_LENGTH];
    const char * a_ = a.c_str();
    const char * b_ = b.c_str();
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
            char ai = a_[i - 1];
            int minF = INF_INT;
            int jd1 = 1;
            int jd2 = i + bLen - aLen - th;
            int jd = jd1 >= jd2 ? jd1 : jd2;
            int ju1 = bLen;
            int ju2 = i + th;
            int ju = ju1 <= ju2 ? ju1 : ju2;
            for (int j = jd; j <= ju; j ++) {
                int t = fi_[j - 1] + (ai != b_[j - 1]);
                if (j < ju2 && t > fi_[j] + 1) {
                    t = fi_[j] + 1;
                }
                if (j > jd && t > fi[j - 1] + 1) {
                    t = fi[j - 1] + 1;
                }
                fi[j] = t;
                minF = minF > t ? t : minF;
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
            char ai = a_[i - 1];
            int minF = INF_INT;
            int jd1 = 1;
            int jd2 = i - th;
            int jd = jd1 >= jd2 ? jd1 : jd2;
            int ju1 = bLen;
            int ju2 = i + bLen - aLen + th;
            int ju = ju1 <= ju2 ? ju1 : ju2;
            for (int j = jd; j <= ju; j ++) {
                int t = fi_[j - 1] + (ai != b_[j - 1]);
                if (j < ju2 && t > fi_[j] + 1) {
                    t = fi_[j] + 1;
                }
                if (j > jd && t > fi[j - 1] + 1) {
                    t = fi[j - 1] + 1;
                }
                fi[j] = t;
                minF = minF > t ? t : minF;
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

//基于getEd7，优化局部变量数量，截断估价剪枝（-O3时不如getEd6） 
int getEd8(std::string & a, std::string & b, int th) {
    int aLen = a.length();
    int bLen = b.length();
    static int f[2][MAX_LINE_LENGTH];
    const char * a_ = a.c_str();
    const char * b_ = b.c_str();
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
            char ai = a_[i - 1];
            int minF = aLen - bLen - i;
            minF = i + (minF >= 0 ? minF : - minF);
            int jd2 = i + bLen - aLen - th;
            int jd = 1 >= jd2 ? 1 : jd2;
            int ju2 = i + th;
            int ju = bLen <= ju2 ? bLen : ju2;
            for (int j = jd; j <= ju; j ++) {
                int t = fi_[j - 1] + (ai != b_[j - 1]);
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
            char ai = a_[i - 1];
            int minF = aLen - bLen - i;
            minF = i + (minF >= 0 ? minF : - minF);
            int jd2 = i - th;
            int jd = 1 >= jd2 ? 1 : jd2;
            int ju2 = i + bLen - aLen + th;
            int ju = bLen <= ju2 ? bLen : ju2;
            for (int j = jd; j <= ju; j ++) {
                int t = fi_[j - 1] + (ai != b_[j - 1]);
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

//基于getEd6，截断估价剪枝（不如getEd8） 
int getEd9(std::string & a, std::string & b, int th) {
    static int f[2][MAX_LINE_LENGTH];
    const char * a_ = a.c_str();
    const char * b_ = b.c_str();
    int aLen = a.length();
    int bLen = b.length();
    if (aLen - bLen > th || bLen - aLen > th) {
        return th + 1;
    }
    int * f0 = f[0];
    for (int j = 0; j <= bLen && j <= th; j ++) {
        f0[j] = j;
    }
    for (int i = 1; i <= aLen; i ++) {
        int * fi = f[i & 1];
        int * fi_ = f[(i & 1) ^ 1];
        fi[0] = i;
        char ai = a_[i - 1];
        int minF = aLen - bLen - i;
        minF = i + (minF >= 0 ? minF : - minF);
        int jd1 = 1;
        int jd2 = i - th;
        int jd3 = i + bLen - aLen - th;
        int jd = jd1 >= jd2 ? jd1 >= jd3 ? jd1 : jd3 : jd2 >= jd3 ? jd2 : jd3;
        int ju1 = bLen;
        int ju2 = i + th;
        int ju3 = i + bLen - aLen + th;
        int ju = ju1 <= ju2 ? ju1 <= ju3 ? ju1 : ju3 : ju2 <= ju3 ? ju2 : ju3;
        for (int j = jd; j <= ju; j ++) {
            int t = fi_[j - 1] + (ai != b_[j - 1]);
            if (j < ju2 && j < ju3 && t > fi_[j] + 1) {
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


void solveEdQuery(std::string & query, int threshold, std::vector < std::pair < int, int > > & res, int I) {
    res.clear();
    for (int i = 0; i < N; i ++) {
        int ed = getEd8(query, lineList[i], threshold);
        if (ed <= threshold) {
            res.push_back(std::make_pair(i, ed));
        }
    }
}

void solveEdQuery(int ql, int qr) {
    printf("solveEdQuery(%d, %d)\n", ql, qr);
    FILE * fin = fopen(ED_QUERY_FILE_NAME, "r");
    FILE * fout = fopen(ED_ANSWER_FILE_NAME, "w");
    static char buf[MAX_LINE_LENGTH];
    std::string buf_s;
    int threshold;
    std::vector < std::pair < int, int > > res;
    for (int i = 0; fscanf(fin, "%[^\n]\n", buf) == 1; i ++) {
        fscanf(fin, "%d\n", & threshold);
        if (i < ql || i >= qr) {
            continue;
        }
        printf("solveEdQuery() %d / (%d, %d)\n", i, ql, qr);
        buf_s = buf;
        res.clear();
        solveEdQuery(buf_s, threshold, res, i);
        fprintf(fout, "%d: %d {", i, res.size());
        for (int i = 0; i < (int) res.size(); i ++) {
            fprintf(fout, "(%d, %d), ", res[i].first, res[i].second);
        }
        fprintf(fout, "}\n");
    }
    fclose(fout);
}

void checkEdAlgorithm() {
    printf("checkEdAlgorithm() check correctness\n");
    int (* func1)(std::string &, std::string &, int) = getEd1;
    int (* func2)(std::string &, std::string &, int) = getEd3;
    
    static char a[MAX_LINE_LENGTH];
    static char b[MAX_LINE_LENGTH];
    std::string a_s;
    std::string b_s;
    while (false) {
        scanf("%s", a);
        if (a[0] == ';') {
            break;
        }
        scanf("%s", b);
        int th;
        scanf("%d", &th);
        a_s = a;
        b_s = b;
        int ed1 = func1(a_s, b_s, th);
        int ed2 = func2(a_s, b_s, th);
        printf("-> %d %d\n", ed1, ed2);
        if (ed1 != ed2) {
            system("pause");
        }
    }
    
    printf("checkEdAlgorithm() test speed\n");
    int n = SPEED_TEST_N;
    static int th[SPEED_TEST_N][SPEED_TEST_N];
    static int ed1[SPEED_TEST_N][SPEED_TEST_N];
    static int ed2[SPEED_TEST_N][SPEED_TEST_N];
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < i; j ++) {
            th[i][j] = rand() % 50;
        }
    }
    int runTime1 = clock();
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < i; j ++) {
            ed1[i][j] = func1(lineList[i], lineList[j], th[i][j]);
        }
    }
    runTime1 = clock() - runTime1;
    int runTime2 = clock();
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < i; j ++) {
            ed2[i][j] = func2(lineList[i], lineList[j], th[i][j]);
        }
    }
    runTime2 = clock() - runTime2;
    printf("runTime1 = %d, runTime2 = %d\n", runTime1, runTime2);
    printf("r2/r1 = %.8lf\n", 1.0 * runTime2 / runTime1);
    bool currentness = true;
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < i; j ++) {
            if (ed1[i][j] != ed2[i][j]) {
                currentness = false;
                printf("(i, j) = (%d, %d) : th = %d, ed1 = %d, ed2 = %d\n", i, j, th[i][j], ed1[i][j], ed2[i][j]);
            }
        }
    }
    if (currentness) {
        printf("all current\n");
    }
}


int main(int argc, char ** argv) {
    srand(12345);
    readData();
    int ql = 0;
    int qr = 200000;
    if (argc >= 3) {
        sscanf(argv[1], "%d", &ql);
        sscanf(argv[2], "%d", &qr);
        sprintf(ED_ANSWER_FILE_NAME, "data\\gen2.ed.%d-%d.ans", ql, qr);
    }
    //solveEdQuery(ql, qr);
    checkEdAlgorithm();
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    return 0;
}
