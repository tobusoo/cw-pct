#!/usr/bin/env python3

import os

import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np


def draw(filenames, labels, filename: str):

    plt.rcParams["legend.markerscale"] = 2.0
    # plt.rcParams['font.family'] = 'sans-serif'
    # plt.rcParams['font.sans-serif'] = ['PT Sans']
    plt.rcParams['font.size'] = '14'
    # 'upper left', 'upper right', 'lower left', 'lower right', 'center'
    plt.rcParams["legend.loc"] = "upper left"
    plt.rcParams["legend.fontsize"] = "12"
    cm = 1/2.54  # centimeters in inches
    fig = plt.figure(figsize=(26*cm, 16*cm))
    ax = fig.add_subplot(111)
    ax.set_title("")
    ax.set(xlabel="Количество процессов", ylabel="Ускорение")
    ax.label_outer()
    ax.xaxis.set_tick_params(direction='in', which='both')
    ax.yaxis.set_tick_params(direction='in', which='both')
    ax.xaxis.set_ticks(np.arange(0, 33, 2))
    ax.yaxis.set_ticks(np.arange(0, 33, 4))

    i = 0
    for (fname, datalabel) in zip(filenames, labels):
        data = np.loadtxt(fname)
        marker = '-o'
        if datalabel == "n = 2500":
            marker = '-^'
        elif datalabel == "n = 5000":
            marker = '-s'

        x = data[:, 0]
        y = data[:, 1]

        # ax.plot(x,y,'-o',markersize=1,c="green")
        ax.plot(x, y, marker, markersize=6, linewidth=1.5, label=datalabel)
        # x.bar(x, y);
    ax.plot(range(2, 33), range(2, 33), '-', c="blue",
            linewidth=1.5, label="Линейное ускорение")
    labels = [item.get_text() for item in ax.get_xticklabels()]
    for i in range(17):
        labels[i] = ''
    labels[1] = '2\n(2x1)'
    labels[2] = '4\n(2x2)'
    labels[4] = '8\n(2x4)'
    labels[8] = '16\n(2x8)'
    labels[16] = '32\n(4x8)'
    ax.set_xticklabels(labels)


    plt.tight_layout()
    ax.legend()
    ax.grid(True)
    fig.savefig(filename, dpi=600)
    # fig.savefig('chart.pdf', dpi=300)


if __name__ == "__main__":
    draw(["results/n_2500/speedup.dat", "results/n_5000/speedup.dat"], ["n = 2500", "n = 5000"], 'chart.svg')
