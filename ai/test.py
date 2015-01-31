from learning import *
from helloworld import *
import time

def getTestStateLists():
    test = State()
    l =  test.toLists()
    return l

def runMinimax(l, depth):
    l[10][3] = depth
    l[10].append(1)
    a = time.time()
    out = minimaxSearch(*l)
    b = time.time()
    print out
    print 'time passed is ', b - a
    return out
