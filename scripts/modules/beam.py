import numpy as np
import pandas as pd
import uproot
import os
from os import path
import matplotlib.pyplot as plt

DATA_DIR = "/data/pct/sim/cal/lynx/output"

# Distance from ISOCENTER(cm) -> -20, -10, 0, 10, 20
KCMH_DATA = {
    70: [7.10, 6.45, 5.95, 5.45, 5.00],
    200: [3.95, 3.75, 3.65, 3.60, 3.50]
}
LYNX_UNIT = 0.5 #mm

def fit_beam(en):
    data_x = np.array([-20, -10, 0, 10, 20])*10.
    data_y = np.array(KCMH_DATA[en])
    fit = np.polyfit(np.log(data_x),  data_y, 1)
    f = lambda x: fit[0]*np.log(x) + fit[1]
    fig, ax = plt.subplots(figsize=(12, 10))
    ax.scatter(data_x, data_y)
    ax.plot(f(data_x), data_y)
    plt.savefig(f'./output/images/beam_fit_{en}.png', dpi=300)

def get_beam_sigma(en, dis):
    pass

def get_beam_angle(en, dis):
    pass

def fit_sigma_data(en=200):
    files = os.listdir(DATA_DIR)
    sigerr_arr = np.zeros(len(files))
    for f_i, f in enumerate(files):
        with uproot.open(path.join(DATA_DIR, f)) as f:
            tree = f['LYNX;1']
            df: pd.DataFrame = tree.arrays(library='pd')
            df = df[df.layerID < 5]
            # df_posz = df.groupby(['layerID'], as_index=False)\
            #     ['posZ'].mean()
            # print(df_posz)
            df_std = df.groupby(['layerID', 'sigma'], as_index=False)\
                [['pixelX', 'pixelY']].std()
            df_std['pixel'] = df_std.apply(lambda row:
                (row['pixelX'] + row['pixelY'])/2, axis=1)
            df_std['sigerr'] = df_std.apply(lambda row: np.abs(row['pixel']*0.5 -\
                KCMH_DATA[en][int(4 - row['layerID'])])\
                    /KCMH_DATA[en][int(4 - row['layerID'])],
                                            axis=1)
            df_err = df_std.groupby('sigma', as_index=False)['sigerr'].mean()
            sigerr_arr[f_i] = df_err[df_err.sigerr <= df_err['sigerr'].min()]\
                ['sigma'].values[0]
    return sigerr_arr.mean()

print(fit_sigma_data())