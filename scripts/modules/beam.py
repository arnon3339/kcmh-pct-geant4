import numpy as np
import matplotlib.pyplot as plt

# Distance from ISOCENTER(cm) -> -20, -10, 0, 10, 20
KCMH_DATA = {
    70: [7.10, 6.45, 5.95, 5.45, 5.00],
    200: [3.95, 3.75, 3.65, 3.60, 3.50]
}

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

fit_beam(200)