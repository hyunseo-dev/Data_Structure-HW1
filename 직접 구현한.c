#include <stdio.h>

#define MAX_TERMS 101

// 희소 행렬은 행, 영, 값만 배열에 저장해주면 된다.
typedef struct {
	int row;
	int col;
	int value;
} element;

// 행렬이 몇행 몇열 몇개의 항을 가지는지 저장.
typedef struct {
	element data[MAX_TERMS];
	int rows;
	int cols;
	int terms;
} matrix;


// 행렬 생성
matrix matrix_maker()
{
	matrix a;

	printf("행 개수 입력 :");
	scanf("%d", &a.rows);
	printf("열 개수 입력 :");
	scanf("%d", &a.cols);
	printf("값을 가지는 개수 입력 :");
	scanf("%d", &a.terms);
	printf("\n\n");

	for (int i = 0; i < a.terms; i++)
	{
		printf("%d 번째 데이터가 위치한 행 입력 :", i + 1);
		scanf("%d", &a.data[i].row);
		if (a.data[i].row > 2)
			a.data[i].row = a.data[i].row % a.rows;
		printf("%d 번째 데이터가 위치한 열 입력 :", i + 1);
		scanf("%d", &a.data[i].col);
		if (a.data[i].col > 2)
			a.data[i].col = a.data[i].col % a.cols;
		printf("%d 번째 데이터의 값 입력 :", i + 1);
		scanf("%d", &a.data[i].value);
		printf("\n");
	}

	return a;
}


// 행렬 전치
matrix matrix_transpose(matrix a) // 기존 희소행렬을 가져옴
{
	matrix b; // 전치행렬이 저장될 행렬 선언
	b.rows = a.cols;
	b.cols = a.rows;
	b.terms = a.terms;
	// 전치행렬의 열과 행의 개수는 기존 행렬과 바뀌게 됨. 이때 항의 개수는 동일.

	int index = 0;

	if (a.terms > 0)
	{
		for (int i = 0; i < a.cols; i++)	// 첫 번째 열부터 a의 열개수 까지 열별로 가져옴(전치행렬에 의해 행보다 열이 우선순위)
		{
			for (int j = 0; j < a.terms; j++)
			{
				if (a.data[j].col == i)
				{
					b.data[index].row = a.data[j].col;
					b.data[index].col = a.data[j].row;
					b.data[index].value = a.data[j].value;
					index++;
				}
			}
		}
	}

	return b;
}

// 행렬 출력(임시)
void matrix_printtemp(element x[])
{
	printf("====================\n");
	for (int i = 0; i < MAX_TERMS; i++)
	{
		printf("(%d, %d, %d) \n", x[i].row, x[i].col, x[i].value);
	}
	printf("====================\n");
}

void matrix_print(matrix x)
{
int index = 0;

printf("========\n");
for (int i = 0; i < x.rows; i++)
{
printf("  ");
for (int j = 0; j < x.cols; j++)
{
if (x.data[index].row == i && x.data[index].col == j) // 문제 존재: row가 col 보다 높을 때 처음 입력한 행렬이 제대로 프린트되지 않음!
{
printf("%d ", x.data[index].value);
index++;
}
else
{
printf("0 ");
}
}
printf("\n");
}
printf("========\n");
}


matrix matrix_multiply(matrix a, matrix b)
{
	matrix c;
	int i = 0, j = 0, k = 0;
	int sum_temp = 0;
	int index = 0;

	// 곱한 행렬의 결괏값은 무조건 앞 행렬의 "행" 뒷 행렬의 "열"로 나온다.
	c.rows = a.rows;
	c.cols = b.cols;

	while (i<a.rows || j<b.cols)
	{
		int a1 = a.data[i].row;
		int b1 = b.data[j].row;
		int tempi = i;
		int tempj = j;
		while (a.data[tempi].row == a1)
		{
			tempj = j;
			while (b.data[tempj].col == b1)
			{
				if (a.data[tempi].col == b.data[tempj].row)
					//if (a.data[tempi].col == b.data[tempj].col)
				{
					sum_temp += (a.data[tempi].value * b.data[tempj].value);
				}
				tempj++;
			}
			tempi++;
		}
		if (sum_temp != 0)
		{
			c.data[k].row = a.data[i].row;
			c.data[k].col = b.data[j].row;
			c.data[k].value = sum_temp;
//			printf("%d\n", sum_temp);
			c.terms++;
			k++;
		}
		sum_temp = 0;
		while (j< a.rows && a1 == b.data[j].col) // 이 부분부터의 반복문을 수정해야 할 것으로 보임
		{
			j++;
		}
		if (b1 == b.data[j].col) 
		{
			while (i<b.cols && a1 == a.data[i].row)
			{
				i++;
				j = 0;
			}
			if (a1 == a.data[i].row)
			{
				break;
			}
		}
	}

	return c;
}

// 
int main()
{
	matrix a;
	a = matrix_maker();
	printf("입력된 행렬\n");
	matrix_print(a);

	matrix transpose;
	transpose = matrix_transpose(a);
	printf("전치된 행렬\n");
	matrix_print(transpose);
	// 이 부분 까지 정상 작동
	
	matrix result;
	result = matrix_multiply(a, transpose);
	printf("곱해진 행렬\n");
	matrix_print(result);

	return 0;
}
