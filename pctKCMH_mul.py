import argparse
import os
from os import path
from multiprocessing import Pool
import math

parser = argparse.ArgumentParser(description="Advanced argparse example")
parser.add_argument('--phantom', '-p', type=str, help='Phantom name', default='ctp404')
parser.add_argument('--beam', '-b', type=int, help='Number of proton beams', default=1000)
parser.add_argument('--thread', '-t', type=int, help='Number of running threads', default=1)
parser.add_argument('--macro', '-m', type=str, help='Macro file', default='beam_point.mac')
parser.add_argument('--process', '-pr', type=int, help='Number of processes', default=1)
parser.add_argument('--energy', '-e', nargs="*", type=int, 
                    help='Energy range', default=[200])

parser_args = parser.parse_args()

def run_pct_kcmh(en_range=[]):
    print(en_range)
    os.system(f'cd {path.join(os.getcwd(), "build")} && \
./pctKCMH -m {parser_args.macro} -p {parser_args.phantom} -b {parser_args.beam} \
-t {parser_args.thread} -e {" ".join([str(en) for en in en_range])}')

if __name__ == '__main__':
    en_args_len = len(parser_args.energy)
    en_step = 1 if en_args_len < 3 else parser_args.energy[2]
    if parser_args.process == 1 or  en_args_len == 1:
        os.system(f'cd {path.join(os.getcwd(), "build")} && \
./pctKCMH -m {parser_args.macro} -p {parser_args.phantom} -b {parser_args.beam} \
-t {parser_args.thread} -e {" ".join([str(en) for en in parser_args.energy])}')
    else:
        en_list = list(range(parser_args.energy[0], parser_args.energy[1], en_step))
        if len(en_list) < parser_args.process:
            with Pool(len(en_list)) as pool:
                pool.map(run_pct_kcmh, [[en] for en in en_list])
        else:
            num_en_split = math.ceil(len(en_list) / parser_args.process)
            en_process_list = []
            for i in range(parser_args.process):
                if i == parser_args.process - 1:
                    en_process_list.append(en_list[i * num_en_split:])
                else:
                    en_process_list.append(en_list[i * num_en_split:\
                        (i + 1) * num_en_split])
            with Pool(parser_args.process) as pool:
                pool.map(run_pct_kcmh, [[en[0], en[-1], en_step]\
                    for en in en_process_list])
