import pandas as pd
import numpy as np
import os
from os import path
from modules import analyse, utils
import uproot

def get_range(en, mat='water'):
    data = pd.read_csv(f'./data/proton_ranges/{mat}.csv', delimiter=' ')
    # print(data)
    matched_data = data[data.E==en]
    if len(matched_data.index):
        return matched_data['R'].values[0]
    else:
        low_data = data[data.E < en].sort_values(by=['E'])
        high_data = data[data.E > en].sort_values(by=['E'])

        m = (high_data['R'].values[0] - low_data['R'].values[-1])\
            /(high_data['E'].values[0] - low_data['E'].values[-1])
        
        b = low_data['R'].values[-1] - m*low_data['E'].values[-1]

        return m*en + b

def add_angle_data(data: pd.DataFrame, angle):
    return data.insert(0, 'angle', angle)

def gen_pct_data(en, original=False, format='csv'):
    data_path = f"/data/pct/sim/cpt404/eMeV_{en}"
    dirs = os.listdir(data_path)
    files = {}

    for d in dirs:
        files[d] = os.listdir(path.join(data_path, d))

    data_list = []
    for i, (k, v) in enumerate(files.items()):
        print(f"Starting {k}, file: {i}.")
        if not original:
            with uproot.open(path.join(data_path, k, v[0])) as f:
                tree = f['pCT;1']
                df = tree.arrays(library='pd')
                utils.add_angle_data(df, int(k.split('_')[-1]))
                data_list.append(analyse.get_pct_data(df, en))
        else:
            pass
        print(f"Finished {k}, file: {i}.")

    df = pd.concat(data_list, ignore_index=True)

    match format:
        case 'csv':
            df.to_csv(path.join(data_path, f'../e{en}MeV.csv'), index=False)
        case 'parquet':
            df.to_parquet(path.join(data_path, f'../e{en}MeV.parquet'),
                          engine='pyarrow')