import os
from os import path
import uproot
import pandas as pd
import numpy as np
import modules.plots as plot
import warnings

OUTDIR = "../build/output"

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

def gen_wepl_data(en):
    en_dir = 'eMeV_' + '0'*(3 - len(str(en))) + str(en)
    root_files = [f for f in os.listdir(
        path.join(OUTDIR, en_dir,
                  'projection_000')) if '_t' in f]

    root_files.sort(key=lambda x: int(x.split('_t')[-1].split('.')[0]))
    dfs = []
    for f in root_files:
        print(f"Starting {f}")
        if len(root_files):
            with uproot.open(path.join(OUTDIR, en_dir, 
                                    'projection_000',
                                    f)) as root_data:
                tree = root_data['pCT;1']
                df_tmp = tree.arrays(library='pd')
                if len(df_tmp.index):
                    df_tmp['wepl'] = np.zeros(len(df_tmp.index))
                    evt_ids = np.sort(np.unique(df_tmp['eventID'].values))
                    for evt_id in evt_ids:
                        if evt_id == 0:
                            continue
                        df_tmp_id = df_tmp[df_tmp.eventID==evt_id]
                        for index, row in df_tmp_id.iterrows():
                            layer_ids = np.sort(np.unique(df_tmp['layerID'].values))
                            for layer_id in layer_ids:
                                welp = 0
                                current_df = df_tmp_id[df_tmp_id.layerID==layer_id]
                                next_df = df_tmp_id[df_tmp_id.layerID==(layer_id + 1)]
                                if len(current_df.index):
                                    if len(next_df.index):
                                        welp = get_WEPL(
                                            get_thickness(
                                                (current_df['pixelX'].values[0], current_df['pixelY'].values[0]),
                                                (next_df['pixelX'].values[0], next_df['pixelY'].values[0])
                                                )
                                            )
                                        df_tmp.loc[(df_tmp.eventID == evt_id) & (df_tmp.layerID == layer_id), 'welp'] = welp
        print(f"Finished {f}")
                    # wepls.append(wepl)
    pd.concat(dfs, ignore_index=True).\
        to_feather(f'./output/data/wepl/eMeV_{"0"*(3 - len(str(en))) + str(en)}.feather')
    # plot.plot_dtc_wepl_hist_1d(wepls) 