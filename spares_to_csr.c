#include <stdio.h>
#include <stdlib.h>

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
    AdjacencyMatrix1D* M = createMatrix(4);
    addEdge(M, 0, 1, 1); addEdge(M, 0, 2, 1);
    addEdge(M, 1, 0, 1); addEdge(M, 1, 2, 1);
    addEdge(M, 2, 0, 1); addEdge(M, 2, 3, 1);
    addEdge(M, 3, 1, 1); addEdge(M, 3, 2, 1);
    
    printf("M^1\n");
    printMatrix(M);
    
    CSRMatrix* csrM = convertToCSR(M);
    printCSR(csrM);
    
	
    AdjacencyMatrix1D* M2_from_CSR = multiplyCSR(csrM, csrM);
    printf("CSR M^2\n");
    printMatrix(M2_from_CSR);
    
    CSRMatrix* csrM2 = convertToCSR(M2_from_CSR);
    
    AdjacencyMatrix1D* M3_from_CSR = multiplyCSR(csrM2, csrM); 
    printf("CSR M^3\n");
    printMatrix(M3_from_CSR);
    
    freeMatrix(M);
    freeCSR(csrM);
    freeMatrix(M2_from_CSR);
    freeCSR(csrM2);
    freeMatrix(M3_from_CSR);
    
    return 0;
}
