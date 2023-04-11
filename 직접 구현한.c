#include <stdio.h>

#define MAX_TERMS 101

// ��� ����� ��, ��, ���� �迭�� �������ָ� �ȴ�.
typedef struct {
	int row;
	int col;
	int value;
} element;

// ����� ���� � ��� ���� �������� ����.
typedef struct {
	element data[MAX_TERMS];
	int rows;
	int cols;
	int terms;
} matrix;


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
	printf("\n\n");

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
		printf("\n");
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

// ��� ���(�ӽ�)
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
printf("========\n");
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
//			printf("%d\n", sum_temp);
			c.terms++;
			k++;
		}
		sum_temp = 0;
		while (j< a.rows && a1 == b.data[j].col) // �� �κк����� �ݺ����� �����ؾ� �� ������ ����
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
	printf("�Էµ� ���\n");
	matrix_print(a);

	matrix transpose;
	transpose = matrix_transpose(a);
	printf("��ġ�� ���\n");
	matrix_print(transpose);
	// �� �κ� ���� ���� �۵�
	
	matrix result;
	result = matrix_multiply(a, transpose);
	printf("������ ���\n");
	matrix_print(result);

	return 0;
}
