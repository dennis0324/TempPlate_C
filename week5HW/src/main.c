#pragma warning(disable:4996)

/**
 * @file main.c
 * @author dennis ko (https://github.com/dennis0324)
 * @brief 이진트리에 관련된 알고리즘을 다양하게 실행해보는 프로그램이다.
 * @date 2022-09-29
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TESTING_COUNT 1000


#define DATA_COUNT 32500

 /**
  * @brief 기록된 시간을 저장할때 어느 인덱스에 저장되는지에 대한 내용
  *
  */
typedef enum action_type {
	insert_iterative = 0,
	insert_recursive,
	traversal_iterative,
	traversal_recursive
}ActionType;

/**
 * @brief 노드 개수 관련해서 함수 사용 시에 사용하는 기본 플래그 값
 *
 */
typedef enum node_count_option {

	terminal_node = 0,
	all_node
}NodeCountOption;

/**
 * @brief 지나갈 노드 횟수를 저장할 변수
 *
 */
int recur_count = 0;
int iter_count = 0;

/**
 * @brief 이진트리 노드 자료형
 *
 */
typedef struct Tree_Node {
	int data;
	struct Tree_Node* left, *right;
}TreeNode;

/**
 * @brief 이진트리 노드를 생성합니다.
 *
 * @param data 이진 트리의 시작지점입니다.
 * @return TreeNode*
 */
TreeNode* addNode(int data) {
	TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
	node->left = NULL;
	node->right = NULL;
	node->data = data;
	return node;
}

/**
 * @brief 이진트리 노드를 추가합니다.
 *
 * @param root 이진트리의 시작지점
 * @param data 추가할 노드 키 값
 */
void node_insert_iter(TreeNode ** root, int data) {
	TreeNode* p = NULL; //말단 노드에서 데이터를 삽입해주는 역할을 한다.
	TreeNode* c = *root;

	if (c == NULL) {
		*root = addNode(data);
		return;
	}

	while (1) {
		if (c->data > data) { // 들어온 데이터가 비교한 데이터보다 작을 때
			p = c;
			c = c->left;
			if (c == NULL) break;
		}
		else if (c->data < data) { // 클 때
			p = c;
			c = c->right;
			if (c == NULL) break;
		}
	}

	if (p->data > data) { // previous 노드와 비교하고 previous노드에서 삽입하는것이 중요하다.
		p->left = addNode(data);
	}
	else if (p->data < data) {
		p->right = addNode(data);
	}
}

/**
 * @brief 재귀적 이진트리 삽입 함수
 *
 * @param node 이진트리의 루트 지점
 * @param value 추가할 노드의 키값
 * @return TreeNode* 변경된 이진트리의 시작 지점
 */
TreeNode * node_insert_recur(TreeNode * node, int value) {
	if (node == NULL) return addNode(value);

	if (node->data > value) {
		node->left = node_insert_recur(node->left, value);
	}
	else if (node->data < value) {
		node->right = node_insert_recur(node->right, value);
	}

	return node;
}

/**
 * @brief 재귀적인 방법으로 중위순회를 하는 함수입니다.
 *
 * @param temp 이진트리의 시작 지점입니다.
 */
void inorder_recur(TreeNode * temp) {
	if (temp == NULL) return;
	inorder_recur(temp->left);
	recur_count++;
	inorder_recur(temp->right);
}

/**
 * @brief 반복적인 방법으로 중위순회를 하는 함수입니다.
 *
 * @param temp 이진트리의 시작 지점입니다.
 * @param stack 반복 순회에서 사용하는 스택을 넣는 매개변수입니다.
 */
void inorder_iter(TreeNode * temp, TreeNode* stack[]) {
	int top = -1;
	int total = 0;

	while (1) {
		for (; temp; temp = temp->left)
			stack[++top] = temp;
		temp = stack[top--];
		if (top == -2) break;
		iter_count++;
		temp = temp->right;
	}
}
/**
 * @brief 들어오는 2개의 수 중에서 큰 수를 반환합니다.
 *
 * @param a 정수형 변수 1
 * @param b 정수형 변수 2
 * @return int
 */
static inline int bigger(const int a, const int b) {

	return a > b ? a : b;
}

/**
 * @brief 이진 트리의 노드의 개수를 구합니다.
 *
 * @param temp 노드의 개수를 세고 싶은 이진 트리를 넣는 매개변수입니다.
 * @param mode 모드가 1이냐 0이냐에 따라서 단말노드를 구할지, 전체 노드를 구할지 정할 수 있습니다.
 * @return int 노드의 개수가 반환됩니다.
 */
int get_node_count(TreeNode * temp, int mode) {
	if (temp == NULL) return 0;

	if (temp->right == NULL && temp->left == NULL) return 1;

	return mode + get_node_count(temp->left, mode) + get_node_count(temp->right, mode);
}

/**
 * @brief 이진트리의 높이를 구하는 함수입니다.
 *
 * @param node 높이를 구할 이진 트리를 넣는 매개변수입니다.
 * @return int 높이를 반환합니다.
 */
int get_tree_height(TreeNode * node) {
	if (node == NULL) return 0;

	if (node->left == NULL && node->right == NULL) return 1;

	return 1 + bigger(get_tree_height(node->left), get_tree_height(node->right));
}


/**
 * @brief 사용을 다한 이진트리의 할당을 해제합니다.
 *
 * @param temp 동적 할당을 해제할 이진트리의 시작 지점입니다.
 */
void free_TreeNode(TreeNode * temp) {
	if (temp == NULL) return;
	free_TreeNode(temp->left);
	free_TreeNode(temp->right);
	free(temp);
}

/**
 * @brief 반복적인 테스트 해볼 내용들을 적는 장소
 *
 * @param arr 실행해볼 테스트 내용
 * @param time_spend_result 한 사이클 당 걸린 시간
 * @param count 현재 실험 횟수
 */
void testing(int * arr, double time_spend_result[][4], int count) {
	clock_t start, end;
	TreeNode * tree1 = NULL;

	// 반복 삽입의 기록 구간
	start = clock();
	for (int i = 0; i < 32500; i++) {
		node_insert_iter(&tree1, arr[i]);
	}
	end = clock();
	// 0번째 인덱스에 넣음
	time_spend_result[count][insert_iterative] = (double)(end - start) / CLOCKS_PER_SEC;

	// 재귀 삽입의 기록 구간
	TreeNode * tree2 = NULL;
	start = clock();
	for (int i = 0; i < 32500; i++) {
		tree2 = node_insert_recur(tree2, arr[i]);
	}
	end = clock();
	// 1번째 인덱스에 넣음
	time_spend_result[count][insert_recursive] = (double)(end - start) / CLOCKS_PER_SEC;

	// 사용하지 않을 이진 트리 동적 할당 해제
	free_TreeNode(tree2);


	// 시간을 재기 전에 간단한 스택 구현
	TreeNode * stack[200] = { 0 };

	// 반복 순회의 기록 구간
	start = clock();
	inorder_iter(tree1, stack);
	end = clock();
	// 2번째 인덱스에 넣음
	time_spend_result[count][traversal_iterative] = (double)(end - start) / CLOCKS_PER_SEC;

	// 재귀 순회의 기록 구간
	start = clock();
	inorder_recur(tree1);
	end = clock();
	// 3번째 인덱스에 넣음
	time_spend_result[count][traversal_recursive] = (double)(end - start) / CLOCKS_PER_SEC;
}
/**
 * @brief 메인함수
 *
 * @return int
 */
int main() {

	FILE * fp = fopen("data.txt", "r");
	FILE * resultfp = fopen("result.txt", "w+");

	if (fp == NULL || resultfp == NULL) {
		printf("파일을 읽거나 생성할 수 없습니다.");
		return 1;
	}
	// 사용할 데이터 받아오기
	int arr[DATA_COUNT] = { 0 };
	// 테스트 횟수 만큼 결과 저장 배열 선언 및 초기화
	double time_spend_result[TESTING_COUNT][4] = { 0 };

	// 32500개 읽어오기
	int i = 0;
	while (!feof(fp)) {
		fscanf(fp, "%d", &arr[i++]);
	}
	fclose(fp);

	if (i < DATA_COUNT) printf("[WARNING] 명시한 데이터 양보다 적은 양이 들어왔습니다.\n");
	else if (i > DATA_COUNT) {
		printf("[ERROR] 명시한 데이터 양보다 많은 양이 들어와 종료합니다.\n");
		return 1;
	}

	TreeNode * root = NULL;

	for (int i = 0; i < DATA_COUNT; i++) {
		node_insert_iter(&root, arr[i]);
	}

	// 테스트 testing_count만큼 실행
	for (int i = 0; i < TESTING_COUNT; i++) {
		testing(arr, time_spend_result, i);
	}

	// 실험한 결과의 총 걸린 시간을 위함
	double time_total_result[4] = { 0 };

	for (int i = 0; i < TESTING_COUNT; i++) {
		for (int j = 0; j < 4; j++) {
			time_total_result[j] += time_spend_result[i][j];
			// 모든 실험의 횟수의 데이터를 시각화하기 위해 파일 입출력
			fprintf(resultfp, "%lf ", time_spend_result[i][j]);
		}
		fprintf(resultfp, "\n");
	}
	fclose(resultfp);

	// 결과 출력
	printf("실험 횟수: %d\n", TESTING_COUNT);
	printf("반복 삽입 - 소요 시간: %lf\n", time_total_result[0] / TESTING_COUNT);
	printf("순환 삽입 - 소요 시간: %lf\n", time_total_result[1] / TESTING_COUNT);

	printf("반복 순회 거친 노드의 개수 / 테스트 횟수: %d\n", recur_count / TESTING_COUNT);
	printf("반복 순회 - 소요 시간: %lf\n", time_total_result[2] / TESTING_COUNT);

	printf("순환 순회 거친 노드의 개수 / 테스트 횟수: %d\n", iter_count / TESTING_COUNT);
	printf("순환 순회 - 소요 시간: %lf\n", time_total_result[3] / TESTING_COUNT);

	printf("전체 노드 개수: %d\n", get_node_count(root, all_node));

	printf("단말 노드 개수: %d\n", get_node_count(root, terminal_node));

	printf("이진 트리 높이: %d", get_tree_height(root));

	free_TreeNode(root);
	return 0;
}