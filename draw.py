#!/usr/bin/env python3

import os

import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np


def draw(filenames, labels, filename: str):

    plt.rcParams["legend.markerscale"] = 2.0
    plt.rcParams['font.family'] = 'sans-serif'
    plt.rcParams['font.sans-serif'] = ['PT Sans']
    plt.rcParams['font.size'] = '14'
    # 'upper left', 'upper right', 'lower left', 'lower right', 'center'
    plt.rcParams["legend.loc"] = "upper left"
    plt.rcParams["legend.fontsize"] = "12"
    cm = 1/2.54  # centimeters in inches
    fig = plt.figure(figsize=(20*cm, 14*cm))
    ax = fig.add_subplot(111)
    ax.set_title("")
    ax.set(xlabel="Количество $p$ процессов", ylabel="Ускорение")
    ax.label_outer()
    # ax.xaxis.set_ticks(np.arange(min(x), max(x)+1, 1))
    # ax.yaxis.set_ticks(np.arange(min(y), max(y)+1, 1))
    ax.xaxis.set_tick_params(direction='in', which='both')
    ax.yaxis.set_tick_params(direction='in', which='both')
    # ax.loglog()
    ax.xaxis.set_ticks(np.arange(0, 33, 2))

    i = 0
    for (fname, datalabel) in zip(filenames, labels):
        data = np.loadtxt(fname)
        x = data[:, 0]
        y = data[:, 1]

        # ax.plot(x,y,'-o',markersize=1,c="green")
        ax.plot(x, y, '-o', markersize=5, linewidth=1, label=datalabel)
        # x.bar(x, y);
    ax.plot(range(2, 32), range(2, 32), '-', c="blue",
            linewidth=1, label="Linear speedup")

    plt.tight_layout()
    ax.legend()
    ax.grid(True)
    fig.savefig(filename, dpi=600)
    # fig.savefig('chart.pdf', dpi=300)


if __name__ == "__main__":
    draw(["results/n_2500/speedup.dat", "results/n_5000/speedup.dat",  "results/n_6000/speedup.dat"], ["n=2500", "n=5000", "n=6000"], 'chart.png')
