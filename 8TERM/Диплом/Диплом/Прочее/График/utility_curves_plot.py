# -*- coding: utf-8 -*-
# Plots utility functions for 5 FSM states of the adaptive behavior module.
# Generates 4 variants:
#   v1_bw          — B/W, no numeric labels       (GOST-style strict)
#   v2_color       — colored, no numeric labels
#   v3_bw_numbers  — B/W, numeric labels 1..5 on curves + legend with numbers
#   v4_color_numbers — colored, numeric labels 1..5 on curves + legend with numbers
#
# Run:  python utility_curves_plot.py
# Requires: matplotlib, numpy

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.font_manager as fm
from matplotlib import rcParams

# ── Font setup: force Times New Roman if available ──────────────────────────
_available = {f.name for f in fm.fontManager.ttflist}
_preferred = 'Times New Roman' if 'Times New Roman' in _available else 'DejaVu Serif'
print(f'[font] using: {_preferred}')

rcParams['font.family']      = 'serif'
rcParams['font.serif']       = [_preferred, 'DejaVu Serif']
rcParams['mathtext.fontset'] = 'stix'  # serif-like math to match TNR
rcParams['font.size']        = 11
rcParams['axes.linewidth']   = 1.0
rcParams['axes.edgecolor']   = 'black'
rcParams['axes.labelcolor']  = 'black'
rcParams['xtick.color']      = 'black'
rcParams['ytick.color']      = 'black'
rcParams['grid.color']       = 'black'
rcParams['grid.linestyle']   = ':'
rcParams['grid.linewidth']   = 0.4
rcParams['legend.frameon']   = True
rcParams['legend.edgecolor'] = 'black'

# ── Math primitives (mirror DepthrunMath::BellCurve / Sigmoid) ──────────────
def bell(x, c, w):
    return np.exp(-((x - c) ** 2) / (2.0 * w * w))

def sigmoid(x, k, c):
    return 1.0 / (1.0 + np.exp(-k * (x - c)))

# ── Utility functions (parameters from AdaptiveConfig.h defaults) ───────────
T = np.linspace(0.0, 1.0, 400)

U_idle    = np.maximum(0.0, 1.0 - 6.0 * T * T)
U_chase   = bell(T, c=0.30, w=0.20)
U_attack  = bell(T, c=0.50, w=0.18)
U_flank   = bell(T, c=0.60, w=0.20) * 0.75   # FlankSoloBase = 0.75 (solo case)
U_retreat = sigmoid(T, k=12.0, c=0.75)

STATES = [
    ('Idle',    'бездействие',       U_idle),
    ('Chase',   'преследование',     U_chase),
    ('Attack',  'атака',             U_attack),
    ('Flank',   'фланговый обход',   U_flank),
    ('Retreat', 'отступление',       U_retreat),
]

# Simple standard linestyles (no custom dash tuples -> no legend dash artifacts)
BW_STYLES = [
    dict(color='black', linestyle='-',  linewidth=1.4),  # Idle
    dict(color='black', linestyle='--', linewidth=1.4),  # Chase
    dict(color='black', linestyle='-.', linewidth=1.6),  # Attack
    dict(color='black', linestyle=':',  linewidth=2.0),  # Flank
    dict(color='dimgray', linestyle='-', linewidth=1.4), # Retreat (grayscale contrast)
]

COLOR_STYLES = [
    dict(color='#1f77b4', linestyle='-',  linewidth=1.8),  # Idle    blue
    dict(color='#2ca02c', linestyle='-',  linewidth=1.8),  # Chase   green
    dict(color='#d62728', linestyle='-',  linewidth=2.0),  # Attack  red
    dict(color='#9467bd', linestyle='-',  linewidth=1.8),  # Flank   purple
    dict(color='#ff7f0e', linestyle='-',  linewidth=1.8),  # Retreat orange
]

# X positions for placing numeric labels on curves (near each curve's peak)
# Idle (1) сдвинут вправо с 0.00 -> 0.04 чтобы не налезать на ось Y.
# Flank (4) сдвинут влево с 0.60 -> 0.70 чтобы не налезать на красную Attack.
LABEL_X = [0.04, 0.30, 0.50, 0.70, 0.88]

def _render(ax, styles, with_numbers):
    labels = []
    for idx, ((name, ru, y), st) in enumerate(zip(STATES, styles), start=1):
        # Дефис вместо длинного тире
        lbl_prefix = f'{idx} - ' if with_numbers else ''
        labels.append(f'{lbl_prefix}{name} ({ru})')
        ax.plot(T, y, label=labels[-1], **st)

        if with_numbers:
            xi = LABEL_X[idx - 1]
            yi = np.interp(xi, T, y)
            # Offset label slightly above the curve
            yi_label = min(yi + 0.05, 1.02)
            ax.annotate(
                str(idx),
                xy=(xi, yi), xytext=(xi, yi_label),
                ha='center', va='bottom', fontsize=10, fontweight='bold',
                color=st['color'],
                bbox=dict(boxstyle='circle,pad=0.2',
                          facecolor='white', edgecolor=st['color'], linewidth=1.0),
            )

    # mathrm -> прямой шрифт, T_final не курсивом
    ax.set_xlabel(r'Значение итоговой угрозы $\mathrm{T_{final}}$')
    ax.set_ylabel(r'Значение функции полезности $\mathrm{U(T)}$')
    ax.set_xlim(0.0, 1.0)
    ax.set_ylim(0.0, 1.08)
    ax.set_xticks(np.arange(0.0, 1.01, 0.1))
    ax.set_yticks(np.arange(0.0, 1.01, 0.1))
    ax.grid(True)
    # handlelength=2.5 кратно dashes -> нет артефакта обрезанного штриха
    ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.13),
              ncol=3, fontsize=9, handlelength=2.5, handletextpad=0.8,
              columnspacing=1.2)

def make_figure(styles, with_numbers, fname):
    fig, ax = plt.subplots(figsize=(8.0, 5.0), dpi=150)
    _render(ax, styles, with_numbers)
    plt.tight_layout()
    plt.savefig(f'{fname}.png', dpi=300, bbox_inches='tight', facecolor='white')
    plt.savefig(f'{fname}.pdf',               bbox_inches='tight', facecolor='white')
    plt.close(fig)
    print(f'  saved: {fname}.png / .pdf')

print('Generating 4 variants...')
make_figure(BW_STYLES,    with_numbers=False, fname='utility_curves_v1_bw')
make_figure(COLOR_STYLES, with_numbers=False, fname='utility_curves_v2_color')
make_figure(BW_STYLES,    with_numbers=True,  fname='utility_curves_v3_bw_numbers')
make_figure(COLOR_STYLES, with_numbers=True,  fname='utility_curves_v4_color_numbers')
print('Done.')
