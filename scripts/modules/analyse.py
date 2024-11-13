import numpy as np
import matplotlib.pyplot as plt

def fit_poly(data, n=3):
    cofs = np.polyfit(data[0], data[1], n)
    print(cofs)
    p = np.poly1d(cofs)

    fig, ax = plt.subplots(figsize=(12, 10))
    ax.scatter(data[0], data[1])
    t = np.linspace(data[0].min(), data[0].max(), 250)
    ax.plot(t, p(t), linewidth=2, c='r')
    plt.savefig('./output/images/fit_range.png', dpi=300)
