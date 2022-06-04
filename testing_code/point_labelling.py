import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import plotting_functions as pf
test_file_number = 7
inputpath = "/Users/ankushgarg/Desktop/DIP-Project/test_data/test_data" + str(test_file_number) + ".csv"
outpath = "/Users/ankushgarg/Desktop/DIP-Project/testing_code/test_" + str(test_file_number) + "/bounding_segments.csv"

df = pd.read_csv(inputpath)
points = pd.read_csv(outpath)
points = points["start"]
points = df.iloc[points]

arrow_points = [10*i for i in range(df.shape[0]//10)]
arrow_ends = [i+1 for i in arrow_points]
arrow_starts = df.iloc[arrow_points]
arrow_direction = df.iloc[arrow_ends];
arrow_direction["x"] = arrow_direction["x"] - arrow_starts["x"]
arrow_direction["y"] = arrow_direction["y"] - arrow_starts["y"]

pf.show_continuous_plot(df)
pf.arrow_plot(arrow_starts, arrow_direction)
# for index, row in points.iterrows():
#     txt = "BB " + str(cnt)
#     plt.text(row["x"]+1, row["y"]+1, txt)
#     cnt += 1
#     if(cnt>35):
#         break

plt.show()

