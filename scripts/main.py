import os
from os import path
from modules.physics import *

from modules.plots import *
import pandas as pd

def plot_wepl():
    data = pd.read_feather('./output/data/wepl/eMeV200.feather')
    df_group = data[data.layerID!=0].groupby(['eventID'])['welp'].sum().reset_index()
    df_group['welp'] = df_group['welp'] - 1.5
    plot_dtc_wepl_hist_1d(df_group['welp'], xbin=5)

def plot_range():
    data = pd.read_feather('./output/data/wepl/eMeV230.feather')
    df_group = data[data.layerID!=0].groupby(['eventID'])['welp'].sum().reset_index()
    df_group['welp'] = df_group['welp'] - 1.5
    x, y = get_range(df_group)
    plot_dtc_range((x, y)) 

if __name__ == '__main__':
    get_proton_range(202)