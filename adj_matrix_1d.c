#include <stdio.h>
#include <stdlib.h>

// 인접 행렬 구조체
typedef struct {
    int numVertices;      // 정점(V)의 개수
    int* matrix;          // 1차원 배열로 펼쳐서 저장~ 
} AdjacencyMatrix1D;

// 1. 행렬 생성 및 초기화
AdjacencyMatrix1D* createMatrix(int vertices) {
    AdjacencyMatrix1D* mat = (AdjacencyMatrix1D*)malloc(sizeof(AdjacencyMatrix1D));
    mat->numVertices = vertices;
    
    // V * V 메모리를 한 번에 할당
    mat->matrix = (int*)calloc(vertices * vertices, sizeof(int));
    
    return mat;
}

// 2. 메모리 해제 함수
void freeMatrix(AdjacencyMatrix1D* mat) {
    if (mat == NULL) return;
    free(mat->matrix);
    //한 방 정리 ㄹㅈㄷ 속시원 
    free(mat);
}

// 3. 간선 추가 (i -> j)
void addEdge(AdjacencyMatrix1D* mat, int i, int j, int weight) {
    if (i >= 0 && i < mat->numVertices && j >= 0 && j < mat->numVertices) {
        // 2차원 인덱스를 1차원으로 바꿀 땐... i*C+j 이용(모르겠으면 필기 참고) 
        mat->matrix[i * mat->numVertices + j] = weight;
    }
}

// 4. 행렬 곱셈 연산 (A * B) - 경로 수 계산
AdjacencyMatrix1D* multiply(AdjacencyMatrix1D* a, AdjacencyMatrix1D* b) {
    if (a->numVertices != b->numVertices) return NULL;
    
    int n = a->numVertices;
    AdjacencyMatrix1D* result = createMatrix(n);
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                // C_ij += A_ik * B_kj
                result->matrix[i * n + j] += a->matrix[i * n + k] * b->matrix[k * n + j];
            }
        }
    }
    return result;
}

// 5. 행렬 출력 
void printMatrix(AdjacencyMatrix1D* mat) {
    int n = mat->numVertices;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", mat->matrix[i * n + j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    AdjacencyMatrix1D* M = createMatrix(4);
    
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
    
    AdjacencyMatrix1D* M2 = multiply(M, M);
    printf("M^2\n");
    printMatrix(M2);
    
    AdjacencyMatrix1D* M3 = multiply(M2, M);
    printf("M^3\n");
    printMatrix(M3);
    
    freeMatrix(M);
    freeMatrix(M2);
    freeMatrix(M3);
    
    return 0;
}
