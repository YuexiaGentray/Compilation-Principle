#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char res[20]="�ؼ���", iden[20]="��ʶ��", cons[20]="����", cal[20]="�����", deli[20]="���", err[20]="����", undef[20]="δ����";
typedef struct Word
{
    char type[20]; //���������� SYM
    char w[20]; //�ʵı�ʶʶ�����ֵ  ID
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
        };  //SYM�û��ؼ���,�����,���

int main() {
    char ch, word_temp[20] = "";  //������
    int i = 0, j = 0, v = 0, k = 0, key = 0, lineNum = 1, WordTemp = 0, temp = 0, iscons = 1, indef = 1, invar = 0, isres = 0, var_temp[20], vct = 0, isdef = 0, isinval=0;

    FILE *fp;
    Word word[100];
    Vari vari[100];

    fp = fopen("a.txt", "r");
    if (!fp) {
        printf("�ļ���ʧ��\n");
        exit(1);
    }

    while ((ch = fgetc(fp)) != EOF) {
        ch = tolower(ch); // ������ת��ΪСд
        if ((ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9')) {
            word_temp[key++] = ch;//����������ĸ,���ֵ����ɵ���
            word_temp[key] = '\0';
            continue;
        } else {
            if (strcmp(word_temp, "") != 0) {
                strcpy(word[i].w, word_temp);    //�����ʿ������ṹ������
                word[i].lineNum = lineNum;
                strcpy(word_temp, "");
                key = 0;                          //�ص���ʱ����Ŀ�ʼλ��
                i++;                            //�ṹ������±��1
                temp = 0;
            }
            if (ch == ' ' || ch == 10 || ch == 13 || ch == '	')//ȥ���ո񡢻س���tab��
            {
                // ����,�س�,������������
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
                        ungetc(ch, fp); // �����һ���ַ����� '='������Ż�������
                        word_temp[1] = '\0';
                    }
                } else {
                    word_temp[1] = '\0';
                }
                strcpy(word[i].w, word_temp);//������ĸ���ַ��ſ������ṹ������
                word[i].lineNum = lineNum;
                strcpy(word_temp, "");
                key = 0;//�ص���ʱ����Ŀ�ʼλ��
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
                                printf("���ִ���,���������%d��---->", word[j].lineNum);
                                printf(" %s ����,֮ǰȱ�ٱ�ʶ��\n", word[j].w);
                                strcpy(word[j].type, err);
                            }
                            invar = 3;
                        }
                        if (k == 26)
                            invar = 1;
                        if ((invar == 2) && (k == 30))
                            invar = 1;
                        strcpy(word[j].type, deli);//���
                    } else if (k >= 0 && k < 15) {
                        strcpy(word[j].type, res);//�ؼ���
                        if (k == 11)
                            indef = 0;
                        else if ((k == 0) && (!invar))
                            invar = 1;
                        else if (invar == 1) {
                            if (k >= 0 && k < 15) {
                                printf("���ִ���,���������%d��---->", word[j].lineNum);
                                printf(" %s ����,����������ʹ�ñ�����\n", word[j].w);
                                strcpy(word[j].type, err);
                            }
                        } else if (invar == 3) {
                            if ((k >= 1) && k < 5) {
                                invar = 4;
                                for (int c = 0; c < vct; c++) {
                                    int redef = 0;
                                    for (int n = 0; n < v; n++) {
                                        if (strcmp(vari[n].w, word[var_temp[c]].w) == 0) {
                                            printf("���ִ���,���������%d��---->", word[var_temp[c]].lineNum);
                                            printf(" %s ����,�����ض���\n", word[var_temp[c]].w);
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
                                printf("���ִ���,���������%d��---->", word[j].lineNum);
                                printf(" %s ����,ӦΪ��������\n", word[j].w);
                                strcpy(word[j].type, err);
                            }
                        } else if (invar == 4) {
                            printf("���ִ���,���������%d��---->", word[j].lineNum);
                            printf(" %s ����,֮ǰȱ��\";\"\n", word[j].w);
                            invar = 1;
                            j--;
                        }
                    }
                    isres = 1;
                    break;
                }
            }
            if (!invar) {
                printf("���ִ���,���������%d��---->", word[j].lineNum);
                printf(" %s ����,֮ǰȱ�������ؼ���var\n", word[j].w);
                exit(-1);
            }
            if (!isres) {
                if (invar == 4) {
                    printf("���ִ���,���������%d��---->", word[j].lineNum);
                    printf(" %s ����,֮ǰȱ��\";\"\n", word[j].w);
                    invar = 1;
                    j--;
                } else if (invar == 2) {
                    for (int c = 0; c < strlen(word[j].w); c++) {
                        if (!isalnum(word[j].w[c])) {
                            printf("���ִ���,���������%d��---->", word[j].lineNum);
                            printf(" %s ����,�������Ƿ��ַ�\n", word[j].w);
                            isinval=1;
                            break;
                        }
                    }
                    if(!isinval) {
                        printf("���ִ���,���������%d��---->", word[j].lineNum);
                        printf(" %s ����,�����ȱ�ָ�����������зǷ��ո�\n", word[j].w);
                    }
                    isinval=0;
                    strcpy(word[j].type, err);
                } else if (!isdigit(word[j].w[0])) {

                    if ((strcmp(word[j].type, err) != 0) && (invar == 1))
                        strcpy(word[j].type, iden);//��ʶ��
                    invar = 2;
                    var_temp[vct++] = j;
                } else {
                    printf("���ִ���,���������%d��---->", word[j].lineNum);
                    printf(" %s ��ʶ�����������ֿ�ͷ\n", word[j].w);
                    strcpy(word[j].type, err);
                }
            }
        } else {
            for (k = 0; k < 31; k++) {
                if ((strcmp(word[j].w, ktt[k])) == 0) {
                    if (k >= 0 && k < 15)
                        strcpy(word[j].type, res);//�ؼ���
                    else if (k >= 15 && k < 26)
                        strcpy(word[j].type, cal);//�����
                    else if (k >= 26 && k < 31)
                        strcpy(word[j].type, deli);//���
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
                    strcpy(word[j].type, cons);//����
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
                        printf("���ִ���,���������%d��---->", word[j].lineNum);
                        printf(" %s δ���������\n", word[j].w);
                    } else
                        strcpy(word[j].type, iden);//��ʶ��
                    iscons = 1;
                    isdef = 0;
                }
            }
        }
    }
    for (j = 0; j < i; j++)//����ʽҪ���ӡ���
    {
        printf("���:%s, ����:'%s', ��������:%d\n", word[j].type, word[j].w, word[j].lineNum);
    }
    fclose(fp);

    return 0;
}