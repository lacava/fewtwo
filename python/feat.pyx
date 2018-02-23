# distutils: language=c++
cimport numpy 
import numpy 
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp cimport bool
from eigency.core cimport *

cdef extern from "feat.h" namespace "FT":
    cdef cppclass Feat: 
        Feat() except +
        Feat(int pop_size, int gens, string ml, 
               bool classification, int verbosity, int max_stall,
               string sel, string surv, float cross_rate,
               char otype, string functions, 
               unsigned int max_depth, unsigned int max_dim, int random_state, 
               bool erc, string obj,bool shuffle, 
               double split, double fb) except + 
        void fit(double * X, int rowsX, int colsX, double*  y , int lenY)
        VectorXd predict(double * X, int rowsX,int colsY)
        #MatrixXd transform(Map[MatrixXd] & X)
        #VectorXd fit_predict(Map[MatrixXd] & X, Map[VectorXd] & y)
        #VectorXd fit_transform(Map[MatrixXd] & X, Map[VectorXd] & y)

cdef class PyFewtwo:
    cdef Feat ft  # hold a c++ instance which we're wrapping
    def __cinit__(self,int pop_size=100, int gens=100, string ml="LinearRidgeRegression", 
               bool classification=False, int verbosity=2, int max_stall=0,
               string sel="lexicase", string surv="pareto", float cross_rate=0.5,
               char otype='a', string functions="+,-,*,/,^2,^3,exp,log,and,or,not,=,<,>,ite", 
               unsigned int max_depth=3, unsigned int max_dim=10, int random_state=0, 
               bool erc = False, string obj="fitness,complexity",bool shuffle=False, double split=0.75, double fb=0.5 ):
        self.ft = Feat(pop_size,gens,ml,classification,verbosity,max_stall,sel,surv,cross_rate,
                otype, functions, max_depth, max_dim, random_state, erc, obj, shuffle, split, fb)

    def fit(self,numpy.ndarray X,numpy.ndarray y):

        cdef numpy.ndarray[numpy.double_t, ndim=2, mode="c"] arr_x
        cdef numpy.ndarray[numpy.double_t, ndim=1, mode="c"] arr_y
        
        arr_x = numpy.ascontiguousarray(X, dtype=numpy.double)
        arr_y = numpy.ascontiguousarray(y, dtype=numpy.double)
        
        rows_y = len(arr_y)
        rows_x = X.shape[0]
        cols_x = X.shape[1]
        print arr_x
        cdef int c_rows = rows_x
        cdef int c_cols = cols_x
        cdef int c_rows_y = rows_y
        self.ft.fit(&arr_x[0,0],c_rows,c_cols,&arr_y[0],c_rows_y)

    def predict(self,numpy.ndarray X):
        cdef numpy.ndarray[double,mode="c"] arr = numpy.array(X,dtype=float)
        rows = X.shape[0]
        cols = X.shape[1]
        cdef int c_rows = rows
        cdef int c_cols = cols
        res = ndarray(self.ft.predict(&arr[0],c_rows,c_cols))
        return res

    #def transform(self,np.ndarray X):
        #return ndarray(self.ft.transform(Map[MatrixXd](X)))

    #def fit_predict(self,np.ndarray X,np.ndarray y):
        #return ndarray(self.ft.fit_predict(Map[MatrixXd](X), Map[VectorXd](y)))

    #def fit_transform(self,np.ndarray X,np.ndarray y):
        #return ndarray(self.ft.fit_transform(Map[MatrixXd](X), Map[VectorXd](y)))