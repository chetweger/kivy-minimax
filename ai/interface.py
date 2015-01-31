from ctypes import *

def call_main():
    lib = CDLL("./libmean.so.1.0.1")
    arglist = [b"asdf", b"hi"]
    options = (c_char_p * len(arglist))()
    options[:] = arglist
    lib.main(c_int(2), options)
