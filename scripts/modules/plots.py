import matplotlib.pyplot as plt

def plot_dtc_hits_2d(*args, **kwargs):
    if 'data' in kwargs.keys():
        data = kwargs['data']
    else:
        data = args[0]
    
    x_min = kwargs['x_min'] if 'x_min' in kwargs.keys() else int(min(data[:, 0]))
    x_max = kwargs['x_max'] if 'x_max' in kwargs.keys() else int(max(data[:, 0]))
    y_min = kwargs['y_min'] if 'y_min' in kwargs.keys() else int(min(data[:, 1]))
    y_max = kwargs['y_max'] if 'y_max' in kwargs.keys() else int(max(data[:, 1]))
    xbin = kwargs['xbin'] if 'xbin' in kwargs.keys() else 1
    ybin = kwargs['ybin'] if 'ybin' in kwargs.keys() else 1
    fig, ax = plt.subplots(figsize=(12, 10))
    ax.hist2d(data[:, 0], data[:, 1], 
              bins=(range(x_min, x_max, xbin), range(y_min, y_max, ybin)))
    plt.savefig(f"""./output/images/{kwargs["name"] 
                if "name" in kwargs.keys() else "dtc_hits_2d.png"}""", dpi=300)
    # plt.show()

def plot_dtc_wepl_hist_1d(*args, **kwargs):
    if 'data' in kwargs.keys():
        data = kwargs['data']
    else:
        data = args[0]

    x_min = kwargs['x_min'] if 'x_min' in kwargs.keys() else int(min(data))
    x_max = kwargs['x_max'] if 'x_max' in kwargs.keys() else int(max(data))
    xbin = kwargs['xbin'] if 'xbin' in kwargs.keys() else 1
    fig, ax = plt.subplots(figsize=(12, 10))
    plt.hist(data, bins=range(x_min, x_max, xbin))
    plt.savefig(f"""./output/images/{kwargs["name"] if "name"
        in kwargs.keys() else "dtc_wepl_hits_1d.png"}""", dpi=300)

def plot_dtc_range(*args, **kwargs):
    if 'data' in kwargs.keys():
        data = kwargs['data']
    else:
        data = args[0]

    fig, ax = plt.subplots(figsize=(12, 10))
    ax.plot(data[0], data[1])
    ax.set_xlim([0, 1.1*max(data[0])])
    plt.savefig(f"""./output/images/{kwargs["name"] if "name"
        in kwargs.keys() else "dtc_range.png"}""", dpi=300)

def plot_entries_thickness_hist_1d(*args, **kwargs):
    if 'data' in kwargs.keys():
        data = kwargs['data']
    else:
        data = args[0]

    x_min = kwargs['x_min'] if 'x_min' in kwargs.keys() else int(min(data))
    x_max = kwargs['x_max'] if 'x_max' in kwargs.keys() else int(max(data))
    xbin = kwargs['xbin'] if 'xbin' in kwargs.keys() else 1
    fig, ax = plt.subplots(figsize=(12, 10))
    plt.hist(data, bins=range(x_min, x_max, xbin))
    plt.savefig(f"""./output/images/{kwargs["name"] if "name"
        in kwargs.keys() else "entries_thickness_hits_1d.png"}""", dpi=300)