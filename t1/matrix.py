class SparseMatrix():
    def __init__(self,elements=[]):
        self.elements = elements
    def rows(self):
        i=0
        max = 0
        while(i<len(self.elements)):
            if self.elements[i]>max:
                max = self.elements[i]
            i+=3
        return max+1
    def cols(self):
        i=1
        max = 0
        while(i<len(self.elements)):
            if self.elements[i]>max:
                max = self.elements[i]
            i+=3
        return max+1

    def get_item(r,c):
        i = 0
        while(i<len(self.elements)):
            if self.elements[i] == r:
                if self.elements[i+1]==c:
                    return self.elements[i+2]
            i+=3
        return 0


class Matrix():
    def __init__(self, row, col, elements=[]):
        
        self.row = row
        self.col = col
        if len(elements) == 0:
            print("Enter the elements of the matrix in order...")
            for i in range(self.row * self.col):
                x = input("")
                elements.append(int(x))

        self.elements = elements
        if self.row * self.col != len(elements):
            raise Exception("ERROR: Matrix entry invalid. Dimension mismatch!")

    def rows(self):
        return self.row

    def cols(self):
        return self.col
    
    def get_item(self,r,c):
        return self.elements[(self.row-1)*r + c+r]
def row(mat):
    return m.row

def col(mat):
    return m.col

def print_matrix(mat):
    rows = mat.rows()
    cols = mat.cols()
    for i in range(rows):
        print('| ', end='')
        for j in range(cols):
            print(" ",mat.get_item(i,j), end='')
            if j<cols-1:
                print(",",end='')
        print(' |')

def matrix_mul(a,b):
    if a.rows() != b.cols():
        raise Exception("ERROR. Matrix dimension not suitable for multiplication")
    result_mat_ele = []
    for i in range(a.rows()*b.cols()):
        result_mat_ele.append(0)
    a_row = a.rows()
    a_col = a.cols()
    b_row = b.rows()
    b_col = b.cols()
    
    for i in range(a_row):
        for j in range(b_col):
            for k in range(b_row):
                result_mat_ele[(a_row-1)*i+j+i] += a.get_item(i,k) * b.get_item(k,j)
    print(result_mat_ele)
    result_mat = Matrix(a_row,b_col, result_mat_ele)
    return result_mat

def sparse(mat):
    r = mat.rows()
    c = mat.cols()
    sparse_entry = []
    for i in range(r):
        for j in range(c):
            item = mat.get_item(i,j)
            if item != 0:
                sparse_entry.append(i)
                sparse_entry.append(j)
                sparse_entry.append(item)
    spar_mat = SparseMatrix(sparse_entry)
    return spar_mat

def parse_matrix(m,n):
    mat = []
    bar=0
    for i in range(m*n):
        n = input("")
        allowable = ['0','1','2','3','4','5','6','7','8','9','10','-','|']
        if n not in allowable:
            raise Exception("Invalid Entry Dectected")
        if n=='|':
            bar+=1
        else:
            mat.append(int(n))
        if bar/2 == m:
            break
    if len(mat) != m*n:
        raise Exception("Invalid entries")
    else:
        return mat


'''
G) Is Sparse Matrix always more efficient?
No. A sparse matrix representation uses 3 memory spaces for one entry whereas a concentional matrix representation uses only one memory location for each 
entry. This means that when a matrix has no zero entries, say in a 4x4 matrix, then conventional method uses 16 memory locations whereas a sparse representation 
will use 48 memory locations. Hence, sparse matrix representation is not ALWAYS more efficient. It is only efficient when the matrix has many number of 0 entries.
'''

'''
H) Can your function matrix_mul be used to multiply a Matrix and a SparseMatrix? Explain your answer.
Yes it can. This is because, in the matrix multiplication function, we are using class methods to access the elements. Despite the fact that the two are different
by representation, the methods return values in the same manner and hence it is possible.

'''