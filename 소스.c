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

// 행렬 출력
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


int main()
{
	printf("\n     입력한 행렬\n");
	matrix_printtemp(a);

	fast_transpose(a, b);
	printf("\n     전치된 행렬\n");
	matrix_printtemp(b);

	mmult(a, b, result);
	printf("\n     곱해진 행렬\n");
	matrix_printtemp(result);

	return 0;
}
