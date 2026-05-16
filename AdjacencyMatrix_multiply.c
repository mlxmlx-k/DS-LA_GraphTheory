#include <stdio.h>
#include <stdlib.h>

// 인접 행렬을 표현하는 구조체
typedef struct {
    int numVertices;      // 정점(V)의 개수... 이걸로 V*V 행렬 생성하겠죠.. 
    int** matrix;         // n x n 2차원 배열 포인터... 
} AdjacencyMatrix;

// 1. 행렬 생성 및 초기화
AdjacencyMatrix* createMatrix(int vertices) {
    AdjacencyMatrix* mat = (AdjacencyMatrix*)malloc(sizeof(AdjacencyMatrix));
    mat->numVertices = vertices;
    
    // 2차원 배열 동적 할당 및 0으로 초기화 (calloc 사용)
    mat->matrix = (int**)malloc(vertices * sizeof(int*));
    for (int i = 0; i < vertices; i++) {
        mat->matrix[i] = (int*)calloc(vertices, sizeof(int));
    }
    return mat;
}

// 2. 메모리 해제 함수
void freeMatrix(AdjacencyMatrix* mat) {
    if (mat == NULL) return;
    for (int i = 0; i < mat->numVertices; i++) {
        free(mat->matrix[i]);
    }
    free(mat->matrix);
    free(mat);
}

// 3. 간선 추가 (i-> j)
void addEdge(AdjacencyMatrix* mat, int i, int j, int weight) {
	//numVertices는 지금 4고... i랑 j는 0부터 3... 
    if (i >= 0 && i < mat->numVertices && j >= 0 && j < mat->numVertices) {
        mat->matrix[i][j] = weight;
    }
}

// 4. 행렬 곱셈 연산 (A * B) - 경로 수 계산
AdjacencyMatrix* multiply(AdjacencyMatrix* a, AdjacencyMatrix* b) {
    // 두 행렬의 크기가 같은지 확인~ 
    if (a->numVertices != b->numVertices) return NULL;
    
    int n = a->numVertices;
    AdjacencyMatrix* result = createMatrix(n);
    
    //곱셈은 선대 2장에서 배운걸로... A_ik * B_kj
	//근데 이거 삼중 포문이 맞냐... 
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                result->matrix[i][j] += a->matrix[i][k] * b->matrix[k][j];
            }
        }
    }
    return result;
}

// 5. 행렬 출력 
void printMatrix(AdjacencyMatrix* mat) { 
    for (int i = 0; i < mat->numVertices; i++) {
        for (int j = 0; j < mat->numVertices; j++) {
            printf("%d ", mat->matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    AdjacencyMatrix* M = createMatrix(4);
    
    addEdge(M, 0, 1, 1);
    addEdge(M, 0, 2, 1);
    addEdge(M, 1, 0, 1);
	  addEdge(M, 1, 2, 1);
	  addEdge(M, 2, 0, 1);
    addEdge(M, 2, 3, 1);
    addEdge(M, 3, 1, 1);
    addEdge(M, 3, 2, 1);
    
    printf("M^1\n");
    printMatrix(M);
	
    AdjacencyMatrix* M2 = multiply(M, M);
    printf("M^2\n");
    printMatrix(M2);
	
    AdjacencyMatrix* M3 = multiply(M2, M);
    printf("M^3\n");
    printMatrix(M3);
	
    freeMatrix(M);
    freeMatrix(M2);
    freeMatrix(M3);
	
    return 0;
}
