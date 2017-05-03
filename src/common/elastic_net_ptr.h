#ifndef __ELASTIC_NET_PTR_H__
#define __ELASTIC_NET_PTR_H__

#include <stddef.h>
#include <stdio.h>
#include <limits>
#include <vector>
#include <cassert>
#include <iostream>
#include <random>

#include "matrix/matrix_dense.h"
#include "h2oaiglm.h"
#include "timer.h"
#include <omp.h>

namespace h2oaiglm {


template<typename T>
T getRMSE(size_t len, const T *v1, const T *v2) {
  double rmse = 0;
  for (size_t i = 0; i < len; ++i) {
    double d = v1[i] - v2[i];
    rmse += d * d;
  }
  rmse /= (double) len;
  return static_cast<T>(std::sqrt(rmse));
 }

template<typename T>
  T getRMSE(const T*weights, size_t len, const T *v1, const T *v2) {
  double weightsum=0;
  for (size_t i = 0; i < len; ++i) {
    weightsum += weights[i];
  }

  double rmse = 0;
  for (size_t i = 0; i < len; ++i) {
    double d = v1[i] - v2[i];
    rmse += d * d * weights[i];
  }

  rmse /= weightsum;
  return static_cast<T>(std::sqrt(rmse));
 }
template<typename T>
  T getRMSE(const T offset, const T*weights, size_t len, const T *v1, const T *v2) {
  double weightsum=0;
  for (size_t i = 0; i < len; ++i) {
    weightsum += offset-weights[i];
  }

  double rmse = 0;
  for (size_t i = 0; i < len; ++i) {
    double d = v1[i] - v2[i];
    rmse += d * d * (offset-weights[i]);
  }

  rmse /= weightsum;
  return static_cast<T>(std::sqrt(rmse));
 }


// C++ program for implementation of Heap Sort
 #define mysize_t int
template<typename T>
  void swap(T arr[], mysize_t arrid[], mysize_t a,mysize_t b)
 {
   T t=arr[a];arr[a]=arr[b];arr[b]=t;
   mysize_t tid=arrid[a];arrid[a]=arrid[b];arrid[b]=tid;
 }
template<typename T>
  void heapify(mysize_t a,T arr[],mysize_t arrid[], mysize_t n)
 {
   mysize_t left=2*a+1;mysize_t right=2*a+2;mysize_t min=a;
   if(left<n){if(arr[min]>arr[left]) min=left;}
   if(right<n){if(arr[min]>arr[right]) min=right;}
   if(min!=a) {swap(arr,arrid,a,min);
     heapify(min,arr,arrid,n);}
 }
template<typename T>
  void heapSort(T arr[],mysize_t arrid[], mysize_t n)
 {mysize_t a;//cout<<"okk";
   for(a=(n-2)/2;a>=0;a--){heapify(a,arr,arrid,n);}

   for(a=n-1;a>=0;a--)
     {
       swap(arr,arrid,0,a);heapify(0,arr,arrid,a);
     }
 }
template<typename T>
  void printArray(T arr[], mysize_t arrid[], mysize_t k)
 {
   std::cout << "LARGEST: ";
   for (mysize_t i=0; i<k; i++){
     fprintf(stdout,"%d %21.15g ",arrid[i],arr[i]); fflush(stdout);
   }
   std::cout << "\n";
   
 }

template<typename T>
  void topk(T arr[], mysize_t arrid[], mysize_t n,mysize_t k, mysize_t *whichbeta, T *valuebeta)
 {
   T arr1[k];
   mysize_t arrid1[k];
   mysize_t a;
   for(a=0;a<k;a++)
     {
       arr1[a]=arr[a];
       arrid1[a]=arrid[a];
     }
   for(a=(k-2)/2;a>=0;a--){
     heapify(a,arr1,arrid1,k);
   }
   for(a=k;a<n;a++)
     {
       if(arr1[0]<arr[a]) {
         arr1[0]=arr[a]; arrid1[0]=arrid[a];
         heapify(0,arr1,arrid1,k);
       }
     }
   heapSort(arr1, arrid1,k);
#ifdef DEBUG
   printArray(arr1,arrid1,k); // DEBUG
#endif

   for (mysize_t i=0; i<k; i++){
     whichbeta[i] = arrid1[i];
     valuebeta[i] = arr1[i];
   }

 }
 /* A utility function to print array of size n */

 // Driver program
template<typename T>
  int topkwrap(mysize_t n, mysize_t k, T arr[], mysize_t *whichbeta, T *valuebeta)
 {
   mysize_t arrid[n];
   for(int i=0;i<n;i++) arrid[i]=i;
   //cout<<"okk";
   topk(arr,arrid,n,k,whichbeta,valuebeta);
   //   heapSort(arr, arrid,n);

   //   cout << "Sorted array is \n";
   //   printArray(arr, arrid,n);

   return(0);
 }

 

 template int topkwrap<double>(mysize_t n, mysize_t k, double arr[],mysize_t *whichbeta,double *valuebeta);
 template int topkwrap<float>(mysize_t n, mysize_t k, float arr[],mysize_t *whichbeta,float *valuebeta);

 

// Elastic Net
//   minimize    (1/2) ||Ax - b||_2^2 + \lambda \alpha ||x||_1 + \lambda 1-\alpha ||x||_2
//
// for many values of \lambda and multiple values of \alpha
// See <h2oaiglm>/matlab/examples/lasso_path.m for detailed description.
// m and n are training data size


   template<typename T>
     double ElasticNetptr(int sourceDev, int datatype, int sharedA, int nThreads, int nGPUs, const char ord,
                          size_t mTrain, size_t n, size_t mValid, int intercept, int standardize,
                          double lambda_min_ratio, int nLambdas, int nFolds, int nAlphas,
                          void *trainXptr, void *trainYptr, void *validXptr, void *validYptr, void *weightptr);

   
template <typename T>
  int makePtr_dense(int sharedA, int me, int wDev, size_t m, size_t n, size_t mValid, const char ord, const T *data, const T *datay, const T *vdata, const T *vdatay, const T *weight, void **_data, void **_datay, void **_vdata, void **_vdatay, void **_weight);
  
#ifdef __cplusplus
    extern "C" {
#endif
      double elastic_net_ptr_double(int sourceDev, int datatype, int sharedA, int nThreads, int nGPUs, const char ord,
                                  size_t mTrain, size_t n, size_t mValid, int intercept, int standardize,
                                  double lambda_min_ratio, int nLambdas, int nFolds, int nAlphas,
                                    void *trainXptr, void *trainYptr, void *validXptr, void *validYptr, void *weightptr);
      double elastic_net_ptr_float(int sourceDev, int datatype, int sharedA, int nThreads, int nGPUs, const char ord,
                                   size_t mTrain, size_t n, size_t mValid, int intercept, int standardize,
                                   double lambda_min_ratio, int nLambdas, int nFolds, int nAlphas,
                                   void *trainXptr, void *trainYptr, void *validXptr, void *validYptr, void *weightptr);

#ifdef __cplusplus
    }
#endif

}

#endif