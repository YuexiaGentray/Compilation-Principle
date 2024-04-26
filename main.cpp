#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char res[20]="关键字", iden[20]="标识符", cons[20]="常数", cal[20]="运算符", deli[20]="界符", err[20]="错误", undef[20]="未定义";
typedef struct Word
{
    char type[20]; //词所属类型 SYM
    char w[20]; //词的标识识别符的值  ID
    int lineNum;
}Word;

typedef struct Vari
{
    char type[20];
    char w[20];
}Vari;

char ktt[32][20] =
        {
                "var", "integer", "longint", "bool", "real", "if", "then", "else", "while", "do", "for", "begin", "end", "and", "or", "+", "-", "*", "/", ":=", "<", ">", "<>", ">=", "<=", "==", ";", ":", "(", ")", ","
        };  //SYM用户关键字,运算符,界符

int main() {
    char ch, word_temp[20] = "";  //缓存区
    int i = 0, j = 0, v = 0, k = 0, key = 0, lineNum = 1, WordTemp = 0, temp = 0, iscons = 1, indef = 1, invar = 0, isres = 0, var_temp[20], vct = 0, isdef = 0, isinval=0;

    FILE *fp;
    Word word[100];
    Vari vari[100];

    fp = fopen("a.txt", "r");
    if (!fp) {
        printf("文件打开失败\n");
        exit(1);
    }

    while ((ch = fgetc(fp)) != EOF) {
        ch = tolower(ch); // 将输入转换为小写
        if ((ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9')) {
            word_temp[key++] = ch;//连续几个字母,数字的连成单词
            word_temp[key] = '\0';
            continue;
        } else {
            if (strcmp(word_temp, "") != 0) {
                strcpy(word[i].w, word_temp);    //将单词拷贝到结构数组中
                word[i].lineNum = lineNum;
                strcpy(word_temp, "");
                key = 0;                          //回到临时数组的开始位置
                i++;                            //结构数组的下标加1
                temp = 0;
            }
            if (ch == ' ' || ch == 10 || ch == 13 || ch == '	')//去掉空格、回车和tab键
            {
                // 换行,回车,所在行数增加
                if (ch == '\n') {
                    lineNum++;
                }
                continue;
            } else {
                word_temp[0] = ch;
                if (word_temp[0] == ':' || word_temp[0] == '>' || word_temp[0] == '<') {
                    ch = fgetc(fp);
                    if (ch == '=') {
                        word_temp[1] = ch;
                        word_temp[2] = '\0';
                    } else {
                        ungetc(ch, fp); // 如果下一个字符不是 '='，则将其放回输入流
                        word_temp[1] = '\0';
                    }
                } else {
                    word_temp[1] = '\0';
                }
                strcpy(word[i].w, word_temp);//将非字母数字符号拷贝到结构数组中
                word[i].lineNum = lineNum;
                strcpy(word_temp, "");
                key = 0;//回到临时数组的开始位置
                i++;
            }
        }
    }
    for (j = 0; j < i; j++) {
        isres = 0;
        if (indef) {
            for (k = 0; k < 31; k++) {
                if ((strcmp(word[j].w, ktt[k])) == 0) {
                    if (k >= 26 && k < 31) {
                        if (k == 27) {
                            if (invar != 2) {
                                printf("发现错误,错误出现在%d行---->", word[j].lineNum);
                                printf(" %s 错误,之前缺少标识符\n", word[j].w);
                                strcpy(word[j].type, err);
                            }
                            invar = 3;
                        }
                        if (k == 26)
                            invar = 1;
                        if ((invar == 2) && (k == 30))
                            invar = 1;
                        strcpy(word[j].type, deli);//界符
                    } else if (k >= 0 && k < 15) {
                        strcpy(word[j].type, res);//关键字
                        if (k == 11)
                            indef = 0;
                        else if ((k == 0) && (!invar))
                            invar = 1;
                        else if (invar == 1) {
                            if (k >= 0 && k < 15) {
                                printf("发现错误,错误出现在%d行---->", word[j].lineNum);
                                printf(" %s 错误,变量名不能使用保留字\n", word[j].w);
                                strcpy(word[j].type, err);
                            }
                        } else if (invar == 3) {
                            if ((k >= 1) && k < 5) {
                                invar = 4;
                                for (int c = 0; c < vct; c++) {
                                    int redef = 0;
                                    for (int n = 0; n < v; n++) {
                                        if (strcmp(vari[n].w, word[var_temp[c]].w) == 0) {
                                            printf("发现错误,错误出现在%d行---->", word[var_temp[c]].lineNum);
                                            printf(" %s 错误,变量重定义\n", word[var_temp[c]].w);
                                            strcpy(word[var_temp[c]].type, err);
                                            redef = 1;
                                            break;
                                        }
                                    }
                                    if (!redef) {
                                        strcpy(vari[v].type, word[var_temp[c]].type);
                                        strcpy(vari[v].w, word[var_temp[c]].w);
                                        v++;
                                    }
                                }
                                vct = 0;
                            } else {
                                printf("发现错误,错误出现在%d行---->", word[j].lineNum);
                                printf(" %s 错误,应为变量类型\n", word[j].w);
                                strcpy(word[j].type, err);
                            }
                        } else if (invar == 4) {
                            printf("发现错误,错误出现在%d行---->", word[j].lineNum);
                            printf(" %s 错误,之前缺少\";\"\n", word[j].w);
                            invar = 1;
                            j--;
                        }
                    }
                    isres = 1;
                    break;
                }
            }
            if (!invar) {
                printf("发现错误,错误出现在%d行---->", word[j].lineNum);
                printf(" %s 错误,之前缺少声明关键字var\n", word[j].w);
                exit(-1);
            }
            if (!isres) {
                if (invar == 4) {
                    printf("发现错误,错误出现在%d行---->", word[j].lineNum);
                    printf(" %s 错误,之前缺少\";\"\n", word[j].w);
                    invar = 1;
                    j--;
                } else if (invar == 2) {
                    for (int c = 0; c < strlen(word[j].w); c++) {
                        if (!isalnum(word[j].w[c])) {
                            printf("发现错误,错误出现在%d行---->", word[j].lineNum);
                            printf(" %s 错误,变量含非法字符\n", word[j].w);
                            isinval=1;
                            break;
                        }
                    }
                    if(!isinval) {
                        printf("发现错误,错误出现在%d行---->", word[j].lineNum);
                        printf(" %s 错误,多变量缺分隔符或变量中有非法空格\n", word[j].w);
                    }
                    isinval=0;
                    strcpy(word[j].type, err);
                } else if (!isdigit(word[j].w[0])) {

                    if ((strcmp(word[j].type, err) != 0) && (invar == 1))
                        strcpy(word[j].type, iden);//标识符
                    invar = 2;
                    var_temp[vct++] = j;
                } else {
                    printf("发现错误,错误出现在%d行---->", word[j].lineNum);
                    printf(" %s 标识符不能以数字开头\n", word[j].w);
                    strcpy(word[j].type, err);
                }
            }
        } else {
            for (k = 0; k < 31; k++) {
                if ((strcmp(word[j].w, ktt[k])) == 0) {
                    if (k >= 0 && k < 15)
                        strcpy(word[j].type, res);//关键字
                    else if (k >= 15 && k < 26)
                        strcpy(word[j].type, cal);//运算符
                    else if (k >= 26 && k < 31)
                        strcpy(word[j].type, deli);//界符
                    isres = 1;
                    break;
                }
            }
            if (!isres) {
                for (int c = 0; c < strlen(word[j].w); c++) {
                    if (!isdigit(word[j].w[c])) {
                        iscons = 0;
                        break;
                    }
                }
                if (iscons)
                    strcpy(word[j].type, cons);//常量
                else {
                    for (vct = 0; vct < v; vct++) {
                        if ((strcmp(word[j].w, vari[vct].w)) == 0) {
                            strcpy(word[j].type, vari[vct].type);
                            isdef = 1;
                            break;
                        }
                    }
                    if (!isdef) {
                        strcpy(word[j].type, undef);
                        printf("发现错误,错误出现在%d行---->", word[j].lineNum);
                        printf(" %s 未定义的内容\n", word[j].w);
                    } else
                        strcpy(word[j].type, iden);//标识符
                    iscons = 1;
                    isdef = 0;
                }
            }
        }
    }
    for (j = 0; j < i; j++)//按格式要求打印输出
    {
        printf("类别:%s, 单词:'%s', 所在行数:%d\n", word[j].type, word[j].w, word[j].lineNum);
    }
    fclose(fp);

    return 0;
}