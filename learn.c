#include <stdio.h>
#include <stdlib.h>
#include<string.h>

void printArray(int examples, int att, double** array){
    int i = 0;
    int j = 0;
    for( i = 0; i < examples;i++){
        for( j = 0; j <att;j++){
        }
        printf("\n");
    }
}

void freeArray(int rows, double** mat){
    int i =0;
    for(i = 0; i < rows;i++){
        free(mat[i]);
   }
    free(mat);
}

void easySplit(int col, char *row, double* colmatX){
    int i = 0;
    const char s[2] = ",";
    colmatX[i] = 1;

    char *token;
    token = strtok(row,s);

    while(token!=NULL){
        colmatX[i] = strtod(token,NULL);
        token = strtok(NULL,s);
        i++;
    }
}

double** fileToMatTest(int examples,int attributes, FILE * test){
    int i = 0;
    int row = 0;
    char single[150];
    double** matTest = (double**)malloc(examples * sizeof(double*));
    for( i = 0; i < examples;i++){
        matTest[i] = (double*)malloc(attributes*sizeof(double));
    }

    for(row = 0;row < examples;row++){
        fgets(single,150,test);
        easySplit(attributes,single,matTest[row]);
    }
    return matTest;
}

void arraySplit(int col, char *row, double *colmatX){
    int i = 0;
    const char s[2] = ",";
    colmatX[i] = 1;
    i++;

    char *token;
    token = strtok(row,s);

    while(token!= NULL && i < col){
        colmatX[i] = strtod(token, NULL);
        token = strtok(NULL,s);
        i++;
    }
}


/*
  iterates the training data and converts it to the corrsesponding
  X matrix.
*/

double ** fileToMatX(int examples,int attributes,FILE * train){
    int row = 0;
    int i = 0;
    char single[150];
    attributes++;

    double **matX = (double**)malloc(examples * sizeof(double*));
    for(i = 0; i < examples;i++){
        matX[i] = (double*)malloc(attributes * sizeof(double));
    }
        double temp = 0;

    for(row = 0; row < examples; row++){
        fgets(single,150,train);
        arraySplit(attributes,single,matX[row]);
    }
    return matX;
}

/*
  Gets the last number of a string seperated by commas
*/

void getLastCol(char *row, int attributes, double* matY){
    int i = 0;
    const char s[2] = ",";

    char *token;
    token = strtok(row,s);

    while(token!=NULL){
        if(i == attributes){
            matY[0] = strtod(token,NULL);
        }
        token = strtok(NULL,s);
        i++;
    }
}

/*
  iterates training file and captures the final number of each
  line in the final and converts it to the Y matrix.
*/

double** fileToMatY(int examples,int attributes, FILE * train){
    char single[150];
    int i = 0;
    int row = 0;

    double **matY = (double**)malloc(examples * sizeof(double*));
    for( i = 0; i < examples;i++){
        matY[i] = (double*)malloc(sizeof(double));
    }

    for( row = 0; row < examples;row++){
        fgets(single,150,train);
        getLastCol(single,attributes,matY[row]);
    }
    return matY;
}

double** matTranspose(int examples, int attributes,double ** matX){
    int i = 0;
    int j = 0;
    double ** matTranspose = (double**)malloc((attributes + 1) * sizeof(double*));
    for( i = 0; i < attributes + 1;i++){
        matTranspose[i] = (double*)malloc( examples * sizeof(double));
    }
    for( i = 0; i < examples;i++){
        for( j = 0; j < attributes + 1;j++){
            matTranspose[j][i] = matX[i][j];
        }
    }
    return matTranspose;
}

double** matMult(int xRow, int xCol,double** matX,int yRow,int yCol, double** matY){

    if(xCol != yRow){
        return NULL;
    }

    int i = 0;
    int j = 0;
    int k = 0;

    double **matResult = (double**)malloc(xRow * sizeof(double*));
    for( i = 0; i < xRow;i++){
        matResult[i] = (double*)malloc(yCol * sizeof(double));
    }
    for( i = 0; i < xRow; i++){
        for( j =0; j < yCol; j++){
            for(k =0; k < xCol; k++){
                matResult[i][j] += matX[i][k] * matY[k][j];

            }
        }
    }
    return matResult;
}

double** identityMat(int attributes, int examples){
    int i =0;
    int j = 0;
    double** mat = (double**)malloc(attributes * sizeof(double*));
    for( i = 0; i < attributes;i++){
        mat[i] = (double*)malloc(attributes*sizeof(double));
    }
    for( i = 0; i < attributes;i++){
        for( j = 0; j < examples;j++){
            mat[i][i] = 1;
            if(mat[i][j] != 0){
                mat[i][j] = 0;
            }
        }
    }
    return mat;
}

double compute(int size, double** matW, int testCase, int attributes, double**matTest){
    double constant = matW[0][0];
    int i = 0;
    double result = constant;
    for(i =0; i < attributes; i++){
        result += (matW[i+1][0] * matTest[testCase][i]);
    }
    return result;
}


double* rowDiv(int row, double val, double* mat,double*matI){
    int i = 0;
    for( i =0; i < row; i++){
        if(val!=0){
            mat[i] = mat[i]/val;
            matI[i] = matI[i]/val;
        }
    }
    return mat;
}

/*
  performs gaussian elimination on the target matrix
  and corrsesponding identity matrix
*/
double** REF(int rowP, int colF,int size,double** mat,double**matI){
    int i = 0;
    int j = 0;
    for( i =0; i < size;i++){
        if(i != rowP){
            double redVal = mat[i][colF];
            double blueVal = mat[i][colF];
            for( j = 0; j < size;j++){
                mat[i][j] -= redVal*(mat[rowP][j]);
                matI[i][j] -=blueVal*(matI[rowP][j]);
            }
        }
    }
    return matI;
}

double** copyArr(int att, double**mat){
    int i =0;
    int j = 0;
    double** copy =(double**)malloc(att* sizeof(double));
    for( i =0; i < att; i++){
        copy[i] = (double*)malloc(att * sizeof(double));
    }
    for( i = 0; i < att;i++){
        for( j = 0; j < att;j++){
            copy[i][j] = mat[i][j];
        }
    }
    return copy;
}

double** rowEchelon(int attributes,double** original, double** matI){
    double** matXT_X = copyArr(attributes,original);
  int i = 0;
  int j = 0;
  for( i = 0; i < attributes;i++){
        for( j =0; j < attributes;j++){
            if(matXT_X[i][i] != 1){
                double val = matXT_X[i][i];
                rowDiv(attributes,val,matXT_X[i],matI[i]);
                REF(i,i,attributes,matXT_X,matI);
            }
        }
    }
    return matI;
}

int main(int argc, char *argv[]){
    FILE *train,*test;
    int row = 0;
    int col = 0;
    int num = 0;
    char singleAtt [150];
    char singleEx [150];
    double** matX;
    double** matY;

    train = fopen(argv[1],"r");

    if(argc < 3){
        printf("error\n");
        return 0;
    }

    //get the number of attributes (columns)
    fgets(singleAtt,150,train);
    int attributes = atoi(singleAtt);

    //get the number of training examples (columns)
    fgets(singleEx,150,train);
    int examples = atoi(singleEx);

    matX = fileToMatX(examples,attributes,train);
    double ** transposeX = matTranspose(examples,attributes + 1,matX);

    //Created MatX, get the last col of the training data to get matrix Y
    fclose(train);
    train = fopen(argv[1],"r");
    fgets(singleEx,150,train);
    fgets(singleEx,150,train);

    matY = fileToMatY(examples,attributes,train);

    double** multResultXT_X = matMult(attributes+1,examples,transposeX,examples,attributes+1,matX);
    double ** multResultXT_Y = matMult(attributes + 1,examples,transposeX,examples,1,matY);
    double** identityX = identityMat(attributes + 1 ,examples);

    //Gaussian elimination to computer the inverse
    double** inverseXT = rowEchelon(attributes+1,multResultXT_X,identityX);

    freeArray(examples,matX);
    freeArray(attributes + 1,transposeX);
    freeArray(examples,matY);

    double** matW = matMult(attributes+1,attributes+1,inverseXT,attributes+1,1,multResultXT_Y);

    freeArray(attributes +1, multResultXT_X);
    freeArray(attributes + 1,multResultXT_Y);

    fclose(train);
    char singleTest[150];
    test = fopen(argv[2],"r");

    fgets(singleTest,150,test);
    int atTest = atoi(singleTest);

    double** matTest = fileToMatTest(atTest,attributes,test);
    int i = 0;

    for( i =0; i < atTest; i++){
       double row1 = compute(attributes+1,matW,i,attributes,matTest);
       printf("%0.0lf\n",row1);
    }

   freeArray(atTest,matTest);
   fclose(test);
   return 0;
}
