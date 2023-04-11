#include <stdio.h>

#define MAX_TERMS 101
#define MAX_COL 101

// ��� ����� ��, ��, ���� �迭�� �������ָ� �ȴ�.
typedef struct {
	int row;
	int col;
	int value;
} element;

element a[11] = { { 6, 6, 8 },{ 0, 0, 15 },{ 0, 3, 22 },{ 0, 5, -15 }, { 1, 1, 11 }, { 1, 2, 3 }, { 2, 3, -6 }, { 4, 0, 91 }, { 5, 2, 28 } };
element b[11];
element result[MAX_TERMS];

/*// ����� ���� � ��� ���� �������� ����.
typedef struct {
	element data[MAX_TERMS];
	int rows;
	int cols;
	int terms; 
} matrix; */

/*
// ��� ����
matrix matrix_maker()
{
	matrix a;

	printf("�� ���� �Է� :");
	scanf("%d", &a.rows);
	printf("�� ���� �Է� :");
	scanf("%d", &a.cols);
	printf("���� ������ ���� �Է� :");
	scanf("%d", &a.terms);

	for (int i = 0; i < a.terms; i++)
	{
		printf("%d ��° �����Ͱ� ��ġ�� �� �Է� :", i + 1);
		scanf("%d", &a.data[i].row);
		if (a.data[i].row > 2)
			a.data[i].row = a.data[i].row % a.rows;
		printf("%d ��° �����Ͱ� ��ġ�� �� �Է� :", i + 1);
		scanf("%d", &a.data[i].col);
		if (a.data[i].col > 2)
			a.data[i].col = a.data[i].col % a.cols;
		printf("%d ��° �������� �� �Է� :", i + 1);
		scanf("%d", &a.data[i].value);
	}

	return a;
}


// ��� ��ġ
matrix matrix_transpose(matrix a) // ���� �������� ������
{
	matrix b; // ��ġ����� ����� ��� ����
	b.rows = a.cols;
	b.cols = a.rows;
	b.terms = a.terms;
	// ��ġ����� ���� ���� ������ ���� ��İ� �ٲ�� ��. �̶� ���� ������ ����.
	
	int index = 0;

	if (a.terms > 0)
	{
		for (int i = 0; i < a.cols; i++)	// ù ��° ������ a�� ������ ���� ������ ������(��ġ��Ŀ� ���� �ຸ�� ���� �켱����) 
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
		for (i = 0; i < numCols; i++) // �� ���� ��ŭ �ʱ�ȭ
		{
			rowTerms[i] = 0;
			printf("rowTerms[%d] = %d\n", i, rowTerms[i]);
		}
		printf("\n");

//		printf("rowTerms[a[0].col] = rowTerms[%d] %d\n", a[0].col, rowTerms[a[0].col]); // �����Ⱚ ���� 6���� ���� �� �������̶� ���� �������� �ʴ� ������ ����.
		for (i = 1; i <= numTerms; i++)
		{
			rowTerms[a[i].col]++; // �� ���� ���� (0~6����) �� �� �ִ��� ����
			printf("rowTerms[a[%d].col] = rowTerms[%d] %d\n", i, a[i].col, rowTerms[a[i].col]);
		}
		printf("\n");

		startingPos[0] = 1; // �ڱ� ���ڰ� �� ��° ������ �������� üũ
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
			printf("j = %d = startingPos[a[%d].col]++ = startingPos[%d]++\n", j, i, a[i].col); // ���⼭ j�� ���� ��ġ�� ����� �� ��° ��ġ�� �������� �˷���
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
	if (*sum) // sum�� �����ϸ� ���� ����
		if (*totalD < MAX_TERMS)
		{
			d[++*totalD].row = row;
			d[*totalD].col = column;
			d[*totalD].value = *sum;
			*sum = 0;
		}
		else
		{
			printf("��� ���� ���� ����");
			exit(1);
		}
}

void mmult(element a[], element b[], element d[])
{
	int i, j, column, totalD = 0;
	int rowsA = a[0].row, colsA = a[0].col, totalA = a[0].value;
	int colsB = b[0].col, totalB = b[0].value;
	int rowBegin = 1, row = a[1].row, sum = 0; // A�� ���� ����� ����
	element newB[MAX_TERMS];
	if (colsA != b[0].row) // A�� ������ B�� ����� �ٸ� �� ��� ���� ���� �Ҹ���
	{
		printf("��� �Ұ���"); // ��ġ��İ��� �����̱⿡ ����� ������ ���ɼ��� ���ٰ� ��
		exit();
	}
	fast_transpose(b, newB); // �ٽ� ��ġ�ؼ�
	a[totalA + 1].row = rowsA;
	newB[totalB + 1].row = colsB; // ��� ������ ��������
	newB[totalB + 1].col = 0;
	for (i = 1; i <= totalA; )
	{
		column = newB[1].row;
		for (j = 1; j <= totalB + 1;) // ���� A �� X B ���� ����
		{
			if (a[i].row != row) // �����ϴ� A ���� �����,
			{
				storeSum(d, &totalD, row, column, &sum);
				i = rowBegin; // A�� ����ġ��
				for (; newB[j].row == column; j++); // B�� ���� ����
				column = newB[j].row;
			}
			else if (newB[j].row != column) // �����ϴ� B ���� �����
			{
				storeSum(d, &totalD, row, column, &sum);
				i = rowBegin; // A�� ����ġ��
				column = newB[j].row; // B�� ���� ����
			}
			else switch (COMPARE(a[i].col, newB[j].col))
			{
				case -1: // a�� ������ A�� ����
					i++;
					break;
				case 0:
					sum += (a[i++].value*newB[j++].value);
					break;
				case 1: // b�� ������ B�� ����
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

// ��� ���(�ӽ�)
void matrix_printtemp(element x[])
{
	printf("=====================\n");
	printf(" ��\t ��\t  ��\n");
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
			if (x.data[index].row == i && x.data[index].col == j) // ���� ����: row�� col ���� ���� �� ó�� �Է��� ����� ����� ����Ʈ���� ����!
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

	// ���� ����� �ᱣ���� ������ �� ����� "��" �� ����� "��"�� ���´�.
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
		if (b1 == b.data[j].col) // a1 �־ ����
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
	printf("\n     �Է��� ���\n");
	matrix_printtemp(a);

	//matrix transpose;
	fast_transpose(a, b);
	printf("\n     ��ġ�� ���\n");
	matrix_printtemp(b);

	//matrix result;
	//result = matrix_multiply(a, transpose);
	mmult(a, b, result);
	printf("\n     ������ ���\n");
	matrix_printtemp(result);

	return 0;
}
