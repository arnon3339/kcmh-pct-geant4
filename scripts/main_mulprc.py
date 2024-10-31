import os
from os import path
import multiprocessing
from modules.physics import gen_wepl_data, get_range

from modules.plots import *
import pandas as pd

def run_process(en_range):
    print(en_range)
    for en in range(en_range[0], en_range[1]):
        en_range = gen_wepl_data(en)

if __name__ == '__main__':
    processes = []
    ens = list(range(1, 231))
    num_processes = 8
    num_ens = len(ens)//8
    ens_processes = [[i*num_ens, (i+1)*num_ens] for i in range(len(ens)//num_ens)]
    ens_processes[0][0] = 1
    ens_processes[-1][-1] = 231

    for i in range(num_processes):
        p = multiprocessing.Process(target=run_process, args=(ens_processes[i],))
        processes.append(p)
        p.start()
    
    for p in processes:
        p.join()
    
    print('All workers finished')
    # for en in range(1, 230):