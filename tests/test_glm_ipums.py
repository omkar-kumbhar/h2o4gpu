import time
import sys
import os
import numpy as np
import pandas as pd
import logging
import feather

print(sys.path)

try:
    from utils import find_file, runglm, elastic_net
except:
    from tests.utils import find_file, runglm, elastic_net

logging.basicConfig(level=logging.DEBUG)


def fun(nGPUs=1, nFolds=1, nLambdas=100, nAlphas=8, validFraction=0.2):
    t = time.time()

    print("cwd: %s" % (os.getcwd()))
    sys.stdout.flush()

    name = sys._getframe(1).f_code.co_name
    #    pipes = startfunnel(os.path.join(os.getcwd(), "tmp/"), name)

    print("Reading Data")
    # from numpy.random import randn
    #  m=1000
    #  n=100
    #  A=randn(m,n)
    #  x_true=(randn(n)/n)*float64(randn(n)<0.8)
    #  b=A.dot(x_true)+0.5*randn(m)

    # Rscript ipums.R  runs glmnet on ipums
    #
    df = pd.read_csv("./tests/data/ipums_1k.csv")
    print(df.shape)
    X = np.array(df.iloc[:, :df.shape[1] - 1], dtype='float32', order='C')
    y = np.array(df.iloc[:, df.shape[1] - 1], dtype='float32', order='C')

    t1 = time.time()
    rmse_train, rmse_test = elastic_net(X, y, nGPUs=nGPUs, nlambda=nLambdas, nfolds=nFolds, nalpha=nAlphas,
                                        validFraction=validFraction, verbose=0)

    # check rmse
    print(rmse_train[0, 0])
    print(rmse_train[0, 1])
    print(rmse_train[0, 2])
    print(rmse_test[0, 2])
    sys.stdout.flush()

    if validFraction==0.0:
        assert rmse_train[0, 0] < 54000
        assert rmse_train[0, 1] < 54000
        assert rmse_train[0, 2] < 54000
        assert rmse_test[0, 2] < 54000
    else:
        if nLambdas>20:
            assert rmse_train[0, 0] < 50000
            assert rmse_train[0, 1] < 50000
            assert rmse_train[0, 2] < 50000
            assert rmse_test[0, 2] < 50000
        else:
            assert rmse_train[0, 0] < 57000
            assert rmse_train[0, 1] < 57000
            assert rmse_train[0, 2] < 57000
            assert rmse_test[0, 2] < 57000

    print('/n Total execution time:%d' % (time.time() - t1))

    print("TEST PASSED")
    sys.stdout.flush()

    print("Time taken: {}".format(time.time() - t))
    #    endfunnel(pipes)
    print("DONE.")
    sys.stdout.flush()


def test_glm_ipums_gpu_fold1_quick_0(): fun(1, 1, 3, 3, validFraction=0)


def test_glm_ipums_gpu_fold1_0(): fun(1, 1, 20, 3, validFraction=0)


def test_glm_ipums_gpu_fold5_0(): fun(1, 5, 20, 3, validFraction=0)


def test_glm_ipums_gpu_fold1_quick(): fun(1, 1, 5, 3, validFraction=0.2)


def test_glm_ipums_gpu_fold1(): fun(1, 1, 20, 3, validFraction=0.2)


def test_glm_ipums_gpu_fold5(): fun(1, 5, 20, 3, validFraction=0.2)


def test_glm_ipums_gpu2_fold1_quick(): fun(2, 1, 3, 3, validFraction=0.2)


def test_glm_ipums_gpu2_fold1(): fun(2, 1, 20, 3, validFraction=0.2)


def test_glm_ipums_gpu2_fold5(): fun(3, 5, 20, 3, validFraction=0.2)


def test_glm_ipums_cpu_fold1_quick(): fun(0, 1, 3, 3, validFraction=0.2)


def test_glm_ipums_cpu_fold1(): fun(0, 1, 20, 3, validFraction=0.2)


def test_glm_ipums_cpu_fold5(): fun(0, 5, 20, 3, validFraction=0.2)


if __name__ == '__main__':
    test_glm_ipums_gpu_fold1_quick_0()
    test_glm_ipums_gpu_fold1_0()
    test_glm_ipums_gpu_fold5_0()

    test_glm_ipums_gpu_fold1_quick()
    test_glm_ipums_gpu_fold1()
    test_glm_ipums_gpu_fold5()

    test_glm_ipums_gpu2_fold1_quick()
    test_glm_ipums_gpu2_fold1()
    test_glm_ipums_gpu2_fold5()

    test_glm_ipums_cpu_fold1_quick()
    test_glm_ipums_cpu_fold1()
    test_glm_ipums_cpu_fold5()