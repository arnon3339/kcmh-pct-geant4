import os
from os import path
import uproot
import pandas as pd
import numpy as np
import modules.plots as plot
import warnings

OUTDIR = "/data/pct/sim/cal/output"

DENSITY_DICT = {
    'alpide': 2.33,
    'absorber': 2.70
}

def get_thickness(current_pos, next_pos, ref=False):
    abs_thickness = 1.5 #mm
    alpide_thickness = 50e-3 #mm
    dtc_distance = 25 #mm
    alpide_size_x = 30 #mm
    alpide_size_y = 15 #mm
    num_alpide_x = 9
    num_alpide_y = 12
    num_pixels_x_in_alpide = 1024
    num_pixels_y_in_alpide = 512
    pixel_size_x = alpide_size_x/num_pixels_x_in_alpide
    pixel_size_y = alpide_size_y/num_pixels_y_in_alpide

    pos = np.array((
        (next_pos[0] - current_pos[0])*pixel_size_x, 
        (next_pos[1] - current_pos[1])*pixel_size_y, 
        1*dtc_distance
        ))
    ref_pos = np.array((
        alpide_size_x*num_alpide_x/2, 
        alpide_size_y*num_alpide_y/2,
        1*dtc_distance
        ))
    
    dot_product = np.dot(pos, ref_pos)
    norm_a = np.linalg.norm(pos)
    norm_b = np.linalg.norm(ref_pos)

    if (norm_a * norm_b) < dot_product:
        angle = 0
    else:
        angle = np.arccos(dot_product / (norm_a * norm_b))

    return {
            'alpide': alpide_thickness/np.cos(angle), 
            'absorber': abs_thickness/np.cos(angle)
            }

def get_WEPL(data):
    wepl = 0.
    for d_k, d_v in data.items():
        wepl += DENSITY_DICT[d_k]*d_v
    
    return wepl

def get_range(data: pd.DataFrame):
    x = np.linspace(0, 200, 100)
    y = np.zeros(100)
    for i in range(100):
        y[i] = len(data[data.welp >= x[i]].index)
    
    return x, y

def get_proton_range(en, wepl=False):
    en_dir_name = f'eMeV_{"0"*(3 - len(str(en))) + str(en)}' 
    root_files = [
        f for f in os.listdir(
            path.join(
                OUTDIR, 
                en_dir_name,
                'projection_000'
                )
            ) if '_t' in f
        ]

    num_proton = 0
    thicknesses = []
    for f in root_files:
        print(f'Starting file {f}')
        with uproot.open(path.join(
                OUTDIR,
                en_dir_name, 
                'projection_000',
                f
            )) as root_data:

            tree = root_data['pCT;1']
            df = tree.arrays(library='pd')

            num_proton += len(df[df.layerID == 0].index)
            df_unique_sum = df.groupby('eventID',
                                       as_index=False)['thickness'].sum()
            thicknesses += df_unique_sum['thickness'].values.tolist()
        print(f'Finished file {f}')
    
    data = np.array(thicknesses)
    x_data = np.linspace(data.min(), data.max(), 200)
    y_data = np.array([np.sum(data >= x) for x in x_data])
    plot.plot_dtc_range([x_data, y_data])
    print(x_data[np.where(y_data <= num_proton/2.)[0][0]])