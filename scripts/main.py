import os
from os import path
from modules.physics import *
from modules.plots import *
from modules.analyse import *
from modules.utils import *
import pandas as pd

if __name__ == '__main__':
    gen_pct_data(200, format='parquet')