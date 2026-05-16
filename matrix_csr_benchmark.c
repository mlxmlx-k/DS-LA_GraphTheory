#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 인접 행렬 구조체
typedef struct {
    int numVertices;// 정점(V)의 개수
    int* matrix;// 1차원 배열로 펼쳐서 저장~ 
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

// 원래는 이게 4. 였으요~ 
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

// CSR
typedef struct {
    int numVertices;
    int numNonZeros;
    
    int* values;
    int* colIndices;
    int* rowPointers;
} CSRMatrix;

// 인접 행렬 -> CSR
CSRMatrix* convertToCSR(AdjacencyMatrix1D* mat) {
    int V = mat->numVertices;
    int nnz = 0; 
    
    for (int i = 0; i < V * V; i++) {
        if (mat->matrix[i] != 0) {
            nnz++;
        }
    }
    
    CSRMatrix* csr = (CSRMatrix*)malloc(sizeof(CSRMatrix));
    csr->numVertices = V;
    csr->numNonZeros = nnz;
    
    csr->values = (int*)malloc(nnz * sizeof(int));
    csr->colIndices = (int*)malloc(nnz * sizeof(int));
    csr->rowPointers = (int*)malloc((V + 1) * sizeof(int)); 
    
    int k = 0; 
    csr->rowPointers[0] = 0; 
    
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            int val = mat->matrix[i * V + j];
            if (val != 0) { 
                csr->values[k] = val;
                csr->colIndices[k] = j;
                k++;
            }
        }
        csr->rowPointers[i + 1] = k; 
    }
    
    return csr;
}

// CSR 메모리 해제 함수
void freeCSR(CSRMatrix* csr) {
    if (csr == NULL) return;
    free(csr->values);
    free(csr->colIndices);
    free(csr->rowPointers);
    free(csr);
}

// CSR 출력
void printCSR(CSRMatrix* csr) {
    printf("CSR\n");
    printf("Values: ");
    for (int i = 0; i < csr->numNonZeros; i++) printf("%d ", csr->values[i]);
    printf("\n");
    
    printf("Col Indices: ");
    for (int i = 0; i < csr->numNonZeros; i++) printf("%d ", csr->colIndices[i]);
    printf("\n");
    
    printf("Row Pointers: ");
    for (int i = 0; i <= csr->numVertices; i++) printf("%d ", csr->rowPointers[i]);
    printf("\n\n");
}

// CSR로 곱하기 ㄹㅊㄱ 
AdjacencyMatrix1D* multiplyCSR(CSRMatrix* a, CSRMatrix* b) {
    if (a->numVertices != b->numVertices) return NULL;
    
    int V = a->numVertices;
    AdjacencyMatrix1D* result = createMatrix(V);
    
    for (int i = 0; i < V; i++) {
        for (int p = a->rowPointers[i]; p < a->rowPointers[i+1]; p++) {
            int k = a->colIndices[p]; 
            int valA = a->values[p];  
            
            for (int q = b->rowPointers[k]; q < b->rowPointers[k+1]; q++) {
                int j = b->colIndices[q]; 
                int valB = b->values[q];  
                
                result->matrix[i * V + j] += valA * valB;
            }
        }
    }
    
    return result;
}


int main() {
    int V = 1000;
    
    AdjacencyMatrix1D* M = createMatrix(V);
    
    //랜덤 sparse 그래프 생성 
    srand(time(NULL));
    for (int i = 0; i < V; i++) {
        addEdge(M, i, (i + 1) % V, 1);
        addEdge(M, i, (i + 2) % V, 1);
        if (rand() % 2 == 0) addEdge(M, i, rand() % V, 1);
    }
    
    printf("일반 행렬 곱셈 M^2..\n");
    clock_t start1 = clock();
    
    AdjacencyMatrix1D* normal_M2 = multiply(M, M);
    
    clock_t end1 = clock();
    double time1 = (double)(end1 - start1) / CLOCKS_PER_SEC;
    printf("   -> 소요 시간: %f 초\n\n", time1);
    
    printf("CSR 행렬 곱셈 M^2..\n");
    clock_t start2 = clock();
    
    // 변환 시간 고려를 해야죠~ 
    CSRMatrix* csrM = convertToCSR(M);
    AdjacencyMatrix1D* csr_M2 = multiplyCSR(csrM, csrM);
    
    clock_t end2 = clock();
    double time2 = (double)(end2 - start2) / CLOCKS_PER_SEC;
    printf("소요 시간: %f 초\n\n", time2);
    
    printf("약 %.2f 배\n", time1 / (time2 == 0 ? 0.0001 : time2));
    
    freeMatrix(M);
    freeMatrix(normal_M2);
    freeMatrix(csr_M2);
    freeCSR(csrM);
    
    return 0;
}
