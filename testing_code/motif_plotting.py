import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

def find_motif_points(folder):
    filename = folder + "/motif.txt"
    file = open(filename, "r")
    lines = file.readlines()
    motif_points = [int(i.strip()) for i in lines]
    return motif_points

def continuous_plot(df):
    plt.xlabel("Latitude")
    plt.ylabel("Longitude") 
    plt.plot(df["x"], df["y"])

# test_file_number = input("Enter test file number: ")
#Just change this base path for input data

# for i in range(2, 11):
#     if(i==7):
#         continue
test_file_number = input()
base_test_data_path = './test_data'
test_data_path = base_test_data_path + "/test_data" + test_file_number + ".csv"

#Just change this output path to your base path
base_output_path = '.'
output_path = base_output_path + "/test_" + test_file_number
bounding_path = output_path + "/bounding_segments.csv"

df_data = pd.read_csv(test_data_path)
motif_points = find_motif_points(output_path)

first_trajectory = df_data.iloc[motif_points[0]:motif_points[1]+1]
second_trajectory = df_data.iloc[motif_points[2]:motif_points[3]]

def bb_points():
    points = pd.read_csv(bounding_path)
    points = points["start"]
    points = df_data.iloc[points]
    cnt = 1
    for index, row in points.iterrows():
        txt = "BB " + str(cnt)
        plt.text(row["x"], row["y"], txt)
        cnt += 1


continuous_plot(df_data)


continuous_plot(first_trajectory)
continuous_plot(second_trajectory)
bb_points()

# image_path = output_path + "/motif.svg"
# plt.savefig(image_path, format = 'svg' , dpi = 2000)

# image_path = output_path + "/motif.png"
# plt.savefig(image_path, format = 'png' , dpi = 2000)

# continuous_plot(df_data)

# image_path = output_path + "/graphic.svg"
# plt.savefig(image_path, format = 'svg' , dpi = 2000)

# image_path = output_path + "/graphic.png"
# plt.savefig(image_path, format = 'png' , dpi = 2000)

# image_path = output_path + "/graphic.eps"
# plt.savefig(image_path, format = 'eps' , dpi = 2000)

plt.show()
    # break

