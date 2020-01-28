/* 建立一个大小自定义的二维数组 
并进行高斯消除法*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void update(double **a, int total_column_number, int start_column_number, int row_number);    //进行row_operation的函数
void echelon_form(double **a, int begin_position, int number_of_row, int number_of_column);   //将矩阵改写为echelon_form的函数
void Move_zero_down(double **a, int begin_position, int number_of_row, int number_of_column); //调换两行的元素
void calculation(double **a, double *result, int number_of_solution, int number_of_column);   //计算出方程的解
int det(double **a, int length);                                                              //求方形矩阵对应的行列式值
void factorization(double **a, int numebr_of_row, int number_of_column);                      //对一个矩阵进行分解

int main(void)
{
    /* 关于动态二维动态数组的理解：
    首先定义一个指针**a,那么a表示指向存储m行指针的空间*a
    *a指向存储每行n个double变量的空间 */

    double **a;     /* a用于指向动态创建的二维数组 */
    double *result; /* result用于指向动态创建的存放方程组解的数组 */
    int i, j, n, m, choice;
    double det_result = 0;
    printf("Enter the number of rows:");
    scanf("%d", &m);
    a = (double **)malloc(sizeof(double *) * m); /*分配m个一维指针类型空间，用来存放各行的地址*/

    if (a == NULL)
    {
        fprintf(stderr, "fail!\n");
        return -1;
    }

    printf("Enter the number of columns:");
    scanf("%d", &n);
    result = (double *)malloc(sizeof(double) * (n - 1));
    /* 分配m个存储result的指针类型空间，用来存放每个结果的地址 */

    if (result == NULL)
    {
        fprintf(stderr, "fail!\n");
        return -1;
    }

    if (m < 0 && n < 0) //不合法的行数和列数
        return -1;

    for (i = 0; i < m; i++)                            /* 创建动态二维数组 */
    {                                                  /*为各行分配空间*/
        a[i] = (double *)malloc(sizeof(double) * (n)); /*为第i行分配n个double类型的空间*/
        if (a[i] == NULL)
        {
            fprintf(stderr, "fail!\n");
            return -1;
        }
        for (j = 0; j <= n - 1; j++)
        { /*为第i行的n个元素赋值*/
            a[i][j] = 0;
        }
    }

    /* 输入想要计算操作的矩阵方程组 */
    printf("Please enter the entries of your matrix:\n");
    for (i = 0; i <= m - 1; i++)
    {
        for (j = 0; j <= n - 1; j++)
        {
            scanf("%lf", &a[i][j]); //输入第i行第j列的数据。其中&a[i][j]也可以写作a[i]+j或者是 *(a+i) + j. 功能相同。
        }
    }

    /* 先将矩阵进行化简 */
    if (m == n - 1) //m个方程m个未知量
    {
        for (i = 0; i < n - 1; i++) //从第一行到第m行进行消除
        {
            echelon_form(a, i, m, n);
        }
    }
    else if (m > n - 1) //所给方程数多余需要求的解个数
    {
        for (i = 0; i < n - 1; i++)
        {
            echelon_form(a, i, m, n);
        }
    }
    else if (m < n - 1) //所给方程不够解出准确的值
    {
        for (i = 0; i < m; i++)
        {
            echelon_form(a, i, m, n);
        }
    }

    printf("The echelon form of the matrix is:\n");
    for (i = 0; i < m; i++)
    { /*输出这个二维数组的内容*/
        for (j = 0; j < n; j++)
        {
            printf("%8.2f", a[i][j]);
        }
        putchar('\n');
    }
    putchar('\n');

    while (1)
    {
        printf("Enter the operations you want to do :\n");
        printf("1. calculate the multivariable equations.\n");
        printf("2. compute the  determinant of the square matirx.\n");
        printf("0. End the program.\n");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            if (m >= n - 1) //可计算出方程的解
            {
                calculation(a, result, n - 1, n);
            }
            else
            {
                printf("The equations have infinite solutions or have no solutions.\n");
            }

            if (m > n - 1)
            {
                for (i = n; i < m; i++)
                {
                    double total = 0;
                    for (j = 0; j < n - 1; j++)
                    {
                        total += result[j] * a[i][j];
                    }

                    if (total != a[i][n - 1]) //若下面的方程中存在不符合结果的，则提示方程组无解。
                    {
                        printf("The multiple equations have no solutions.\n");
                        goto label;
                    }
                }

                printf("The solutions to the matrix are:\n");
                for (j = 0; j < n - 1; j++) //打印方程组的解
                {
                    if (j == n - 1)
                    {
                        printf("|");
                    }
                    printf("x[%d] = %5.2f\n", j, result[j]);
                }
                printf("\n");
            }
            else if (m == n - 1)
            {
                printf("The solutions to the matrix are:\n");
                for (j = 0; j < n - 1; j++) //打印方程组的解
                {
                    if (j == n - 1)
                    {
                        printf("|");
                    }
                    printf("x[%d] = %5.2f\n", j, result[j]);
                }
                printf("\n");
            }
            break;
        case 2:
            if (m == n)
            {
                det_result = det(a, m);
                printf("The determinant of the matrix is %lf", det_result);
            }
            else
                printf("The matrix is not a square matrix.\n");
            break;
        default:
            goto label;
            break;
        }
    }

label:
    for (i = 0; i < m; i++)
    { /*释放为a数组的每一行分配的空间*/
        free(a[i]);
    }
    free(a);      /*释放a指针指向的空间*/
    free(result); /* 释放result指针指向的空间 */
    return 0;
}

void echelon_form(double **a, int begin_position, int number_of_row, int number_of_column)
{
    int j;
    Move_zero_down(a, begin_position, number_of_row, number_of_column);
    for (j = begin_position + 1; j < number_of_row; j++) //从第i行的下一行到第n行进行row operation
    {
        if (a[j][begin_position] != 0)
        {
            update(a, number_of_column, begin_position, j);
        }
    }

    return;
}

void update(double **a, int total_column_number, int start_column_number, int row_number)
{
    /* 将开始位置所在列下面所有的数进行row operation将首位全部变为0 */
    int k;
    double temp;
    temp = a[start_column_number][start_column_number] / a[row_number][start_column_number];
    for (k = start_column_number; k < total_column_number; k++)
    {
        a[row_number][k] = a[start_column_number][k] - temp * a[row_number][k];
    }

    return;
}

/* begin_position实际上可以理解为列数，因为高斯消除法是一列一列进行的 */
void Move_zero_down(double **a, int begin_position, int number_of_row, int number_of_column)
{
    /* 从开始的位置在同一列往下依次判断是否有非零的数
    若开始位置即为非零数或所有数都非零，则不变；若开始位置为0，那么将
    找到的非零的那一行与开始位置所在行替换 */

    int row = begin_position;
    for (int l = begin_position; l < number_of_row; l++)
    {
        if (a[l][begin_position] != 0)
        {
            row = l;
            break;
        }
    }
    if (row != begin_position)
    {
        for (int l = begin_position; l < number_of_column; l++) //第row行和第i行交换
        {
            double temp;
            temp = a[begin_position][l];
            a[begin_position][l] = a[row][l];
            a[row][l] = temp;
        }
    }
}

void calculation(double **a, double *result, int number_of_solution, int number_of_column)
{
    int i, j;
    for (i = number_of_solution - 1; i >= 0; i--)
    {
        result[i] = a[i][number_of_column - 1];
        for (j = number_of_solution - 1; j >= i + 1; j--)
        {
            result[i] -= a[i][j] * result[j];
        }
        result[i] /= a[i][i];
    }
}

int det(double **a, int length)
{
    int i;
    double result = 1;
    for (i = 0; i < length; i++)
    {
        echelon_form(a, i, length, length);
    }

    for (i = 0; i < length; i++)
    {
        result *= a[i][i];
    }

    return result;
}

void factorization(double **a, int number_of_row, int number_of_column)
{
    double **L;
    double **U;
    L = (double **)malloc(sizeof(double *) * number_of_row);
    U = (double **)malloc(sizeof(double *) * number_of_row);

    if (L == NULL || U == NULL)
    {
        fprintf(stderr, "fail!\n");
        return;
    }

    for (int i = 0; i < number_of_row; i++)
    {
        L[i] = (double *)malloc(sizeof(double) * number_of_row);
        U[i] = (double *)malloc(sizeof(double) * number_of_column);
        if (L[i] == NULL || U[i] == NULL)
        {
            fprintf(stderr, "fail!\n");
            return;
        }
        for (int j = 0; j < number_of_row; j++)
        {
            L[i][j] = 0;
        }
        for (int j = 0; j < number_of_column; j++)
        {
            L[i][j] = 0;
        }
    }

    // int p = 1;
    // for (int i = 0; i < m; i++)
    // {
    //     int alpha;
    //     alpha = a[i - 1][p - 1];
    //     for (int j = 0; j < number_of_column; j++)
    //     {
    //         if(j<=i)
    //         {
    //             L   [][]
    //         }
    //         else
    //         {

    //         }

    //     }
    // }

    /* 释放创建的两个数组 */
    for (int i = 0; i < number_of_row; i++)
    {
        free(L[i]);
        free(U[i]);
    }
    free(L);
    free(U);
    return;
}