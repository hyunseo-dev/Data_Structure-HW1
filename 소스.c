#include <stdio.h>

#define MAX_TERMS 101
#define MAX_COL 101

// 희소 행렬은 행, 영, 값만 배열에 저장해주면 된다.
typedef struct {
	int row;
	int col;
	int value;
} element;

element a[11] = { { 6, 6, 8 },{ 0, 0, 15 },{ 0, 3, 22 },{ 0, 5, -15 }, { 1, 1, 11 }, { 1, 2, 3 }, { 2, 3, -6 }, { 4, 0, 91 }, { 5, 2, 28 } };
element b[11];
element result[MAX_TERMS];

/*// 행렬이 몇행 몇열 몇개의 항을 가지는지 저장.
typedef struct {
	element data[MAX_TERMS];
	int rows;
	int cols;
	int terms; 
} matrix; */

/*
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
*/

void fast_transpose(element a[], element b[])
{
	int rowTerms[MAX_COL], startingPos[MAX_COL];
	int i, j;
	int numCols = a[0].col, numTerms = a[0].value;
	b[0].row = numCols;
	b[0].col = a[0].row;
	b[0].value = numTerms;
	if (numTerms > 0)
	{
		for (i = 0; i < numCols; i++) // 열 개수 만큼 초기화
		{
			rowTerms[i] = 0;
			printf("rowTerms[%d] = %d\n", i, rowTerms[i]);
		}
		printf("\n");

//		printf("rowTerms[a[0].col] = rowTerms[%d] %d\n", a[0].col, rowTerms[a[0].col]); // 쓰레기값 나옴 6열의 값은 맨 마지막이라서 굳이 저장하지 않는 것으로 보임.
		for (i = 1; i <= numTerms; i++)
		{
			rowTerms[a[i].col]++; // 각 열의 숫자 (0~6까지) 몇 개 있는지 저장
			printf("rowTerms[a[%d].col] = rowTerms[%d] %d\n", i, a[i].col, rowTerms[a[i].col]);
		}
		printf("\n");

		startingPos[0] = 1; // 자기 숫자가 몇 번째 열부터 나오는지 체크
		printf("startingPos[0] = %d\n", startingPos[0]);

		for (i = 1; i < numCols; i++)
		{
			startingPos[i] = startingPos[i - 1] + rowTerms[i - 1];
			printf("startingPos[%d] = %d\n", i, startingPos[i]);
		}
		printf("\n");

		for (i = 1; i <= numTerms; i++)
		{
			j = startingPos[a[i].col]++;
			printf("j = %d = startingPos[a[%d].col]++ = startingPos[%d]++\n", j, i, a[i].col); // 여기서 j의 값은 전치된 행렬의 몇 번째 위치에 들어가는지를 알려줌
			b[j].row = a[i].col;
			b[j].col = a[i].row;
			b[j].value = a[i].value;
		}
	}
}

int COMPARE(int a, int b)
{
	if (a > b)
		return 1;
	else if (a = b)
		return 0;
	else
		return -1;
}

void storeSum(element d[], int *totalD, int row, int column, int *sum)
{
	if (*sum) // sum이 존재하면 값을 저장
		if (*totalD < MAX_TERMS)
		{
			d[++*totalD].row = row;
			d[*totalD].col = column;
			d[*totalD].value = *sum;
			*sum = 0;
		}
		else
		{
			printf("출력 개수 보다 많음");
			exit(1);
		}
}

void mmult(element a[], element b[], element d[])
{
	int i, j, column, totalD = 0;
	int rowsA = a[0].row, colsA = a[0].col, totalA = a[0].value;
	int colsB = b[0].col, totalB = b[0].value;
	int rowBegin = 1, row = a[1].row, sum = 0; // A의 현재 행부터 시작
	element newB[MAX_TERMS];
	if (colsA != b[0].row) // A의 열수와 B의 행수가 다를 때 행렬 곱셈 조건 불만족
	{
		printf("계산 불가능"); // 전치행렬과의 곱셈이기에 절대로 오류날 가능성이 없다고 봄
		exit();
	}
	fast_transpose(b, newB); // 다시 전치해서
	a[totalA + 1].row = rowsA;
	newB[totalB + 1].row = colsB; // 경계 조건을 설정해줌
	newB[totalB + 1].col = 0;
	for (i = 1; i <= totalA; )
	{
		column = newB[1].row;
		for (j = 1; j <= totalB + 1;) // 현재 A 행 X B 열을 수행
		{
			if (a[i].row != row) // 곱셈하던 A 행을 벗어나면,
			{
				storeSum(d, &totalD, row, column, &sum);
				i = rowBegin; // A는 원위치로
				for (; newB[j].row == column; j++); // B는 다음 열로
				column = newB[j].row;
			}
			else if (newB[j].row != column) // 곱셈하던 B 열을 벗어나면
			{
				storeSum(d, &totalD, row, column, &sum);
				i = rowBegin; // A는 원위치로
				column = newB[j].row; // B는 다음 열로
			}
			else switch (COMPARE(a[i].col, newB[j].col))
			{
				case -1: // a가 작으면 A쪽 증가
					i++;
					break;
				case 0:
					sum += (a[i++].value*newB[j++].value);
					break;
				case 1: // b가 작으면 B쪽 증가
					j++;
			}
		}
		for (; a[i].row == row; i++);
		rowBegin = i;
		row = a[i].row;
	}
	d[0].row = rowsA;
	d[0].col = colsB;
	d[0].value = totalD;
}

// 행렬 출력(임시)
void matrix_printtemp(element x[])
{
	printf("=====================\n");
	printf(" 행\t 열\t  값\n");
	for (int i = 0; i < 9; i++)
	{
		printf("%2d\t %d\t %3d \n", x[i].row, x[i].col, x[i].value);
	}
	printf("=====================\n");
}

/* void matrix_print(matrix x)
{
	int index = 0;

	printf("=========\n");
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
	printf("=========\n");
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
			printf("%d\n", sum_temp);
			c.terms++;
			k++;
		}
		sum_temp = 0;
		while (j< a.cols && a1 == a.data[j].col)
		{
			j++;
		}
		if (b1 == b.data[j].col) // a1 넣어도 무관
		{
			while (i<b.rows && b1 == b.data[i].row) 
			{
				i++;
				j = 0;
			}
			if (b1 == b.data[i].row) 
			{
				break;
			}
		}
	}

	return c;
}
*/

// 
int main()
{
	//matrix a = { a_el , 2, 2, 3 };
	//a = matrix_maker();
	printf("\n     입력한 행렬\n");
	matrix_printtemp(a);

	//matrix transpose;
	fast_transpose(a, b);
	printf("\n     전치된 행렬\n");
	matrix_printtemp(b);

	//matrix result;
	//result = matrix_multiply(a, transpose);
	mmult(a, b, result);
	printf("\n     곱해진 행렬\n");
	matrix_printtemp(result);

	return 0;
}
