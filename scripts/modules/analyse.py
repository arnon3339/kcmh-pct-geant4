import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from modules import physics

def fit_poly(data, n=3):
    cofs = np.polyfit(data[0], data[1], n)
    print(cofs)
    p = np.poly1d(cofs)

    fig, ax = plt.subplots(figsize=(12, 10))
    ax.scatter(data[0], data[1])
    t = np.linspace(data[0].min(), data[0].max(), 250)
    ax.plot(t, p(t), linewidth=2, c='r')
    plt.savefig('./output/images/fit_range.png', dpi=300)

def rec_track(data: pd.DataFrame):
    return data

def get_pct_data(data: pd.DataFrame, en, experiment=False):
    if not experiment:
        max_track_id = data['trackID'].max()
        data['trackID'] = data.apply(
            lambda row: row['eventID']*max_track_id + row['trackID'],
            axis=1
            )

    rec_data = rec_track(data)

    allow_track_ids_0 = set(
        rec_data[rec_data.layerID == 0]['trackID'].values.flatten()
    )
    allow_track_ids_1 = set(
        rec_data[rec_data.layerID == 1]['trackID'].values.flatten()
    )
    rec_data = rec_data[
        rec_data.trackID.isin(allow_track_ids_0.intersection(allow_track_ids_1))
        ]
    data_ren: pd.DataFrame = rec_data.groupby(['trackID'], as_index=False)\
        ['thickness'].sum()
    data_ren['trackX0'] = data_ren.apply(
        lambda row: rec_data[(rec_data.trackID == row['trackID']) &\
            (rec_data.layerID == 0)]['pixelX'].values[0],
        axis=1)
    data_ren['trackY0'] = data_ren.apply(
        lambda row: rec_data[(rec_data.trackID == row['trackID']) &\
            (rec_data.layerID == 0)]['pixelY'].values[0],
        axis=1)
    data_ren['trackX1'] = data_ren.apply(
        lambda row: rec_data[(rec_data.trackID == row['trackID']) &\
            (rec_data.layerID == 1)]['pixelX'].values[0],
        axis=1)
    data_ren['trackY1'] = data_ren.apply(
        lambda row: rec_data[(rec_data.trackID == row['trackID']) &\
            (rec_data.layerID == 1)]['pixelY'].values[0],
        axis=1)
    
    data_ren['ren'] = data_ren['thickness'].\
        apply(lambda x: physics.get_ren_from_dtc(en, x))
    
    return data_ren 