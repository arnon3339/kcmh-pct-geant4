import os
from os import path
import uproot
import pandas as pd
import numpy as np
import modules.plots as plot
from modules import utils
import warnings

OUTDIR = "/data/pct/sim/cal/output"

DENSITY_DICT = {
    'alpide': 2.33,
    'absorber': 2.70
}

WATER_DENS = 1
ALUMINIUM_DENS = 2.7
SILICON_DENS = 2.33
AIR_DENS = 0.0012

def get_thickness(current_pos, next_pos, ref=False):
    abs_thickness = 3.5 #mm
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

def get_WEPL(thick, en, mat='water'):
    water_range = utils.get_range(en, 'water')
    if mat.lower() == 'dtc':
        al_range = utils.get_range(en, 'aluminium')
        silicon_range = utils.get_range(en, 'silicon')
        air_range = utils.get_range(en, 'air')

        range_avg = 1./((1./silicon_range)*0.05 +
            (1./al_range)*3.5 + (1./air_range)*(25. - 3.5 - 0.05))
        
        print(f'water range: {water_range}')
        print(f'aluminium range: {al_range}')
        print(f'silicon range: {silicon_range}')
        print(f'air range: {air_range}')
        print(f'average range: {range_avg}')
        return thick*water_range/range_avg
    else:
        return thick*water_range/utils.get_range(en, mat)

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
            if wepl:
                thicknesses += (
                    get_WEPL(df_unique_sum['thickness'].values, en, 'dtc')
                    ).tolist()
            else:
                thicknesses += df_unique_sum['thickness'].values.tolist()
        print(f'Finished file {f}')
    
    data = np.array(thicknesses)
    x_data = np.linspace(data.min(), data.max(), 200)
    y_data = np.array([np.sum(data >= x) for x in x_data])
    plot.plot_dtc_range([x_data, y_data], 
                        name="dtc_range_plot_wepl" if wepl else "dtc_range_plot")
    print(x_data[np.where(y_data <= num_proton/2.)[0][0]])