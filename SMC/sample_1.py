import subprocess as sp
import os
import sys

def check(path_trace, path_property):
    try:
        program_output = sp.check_output(["./BLTL_monitor/checker", path_property , path_trace])
    except Exception as ex:
        raise Exception("Problem running BLTL2scLTL!")
        program_output = 0
        
    return program_output
