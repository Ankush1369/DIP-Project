import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

test_file_number = 7
inputpath = "/Users/ankushgarg/Desktop/DIP-Project/test_data/test_data" + str(test_file_number) + ".csv"
outpath = "/Users/ankushgarg/Desktop/DIP-Project/testing_code/test_" + str(test_file_number) + "/bounding_segments.csv"

df = pd.read_csv(inputpath)
points = pd.read_csv(outpath)
points = points["start"]
points = df.iloc[points]
cnt = 1
plt.plot(df["x"], df["y"])
plt.xlabel("Latitude")
plt.ylabel("Longitude")
plt.scatter(points["x"], points["y"], alpha = 0.5)
# for index, row in points.iterrows():
#     txt = "BB " + str(cnt)
#     plt.text(row["x"]+1, row["y"]+1, txt)
#     cnt += 1
#     if(cnt>35):
#         break

plt.show()

