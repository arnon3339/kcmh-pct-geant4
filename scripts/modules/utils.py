import pandas as pd
import numpy as np

def get_range(en, mat='water'):
    data = pd.read_csv(f'./data/proton_ranges/{mat}.csv', delimiter=' ')
    print(data)
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

print(get_range(166))