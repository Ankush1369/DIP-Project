# from cgi import test
from email.mime import base
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
test_file_number = input("Enter test file number: ")
test_file_number = test_file_number.strip()

assert test_file_number == '7' or test_file_number =='9' , "Invalid file number, possible values are 7 or 9"


base_path = "./test_data" + test_file_number + "/"

def find_motif_points():
    filename = base_path + "BB_motif.txt"
    file = open(filename, "r")
    lines = file.readlines()
    motif_points = [int(i.strip()) for i in lines]
    return motif_points

def find_motif_points_DP():
    filename = base_path + "DP_motif.txt"
    file = open(filename, "r")
    lines = file.readlines()
    motif_points = [int(i.strip()) for i in lines]
    return motif_points

def continuous_plot(df):
    plt.xlabel("Latitude")
    plt.ylabel("Longitude")
    plt.plot(df["x"], df["y"])
    # plt.show()



def bb_points(bounding_path):
    points = pd.read_csv(bounding_path)
    points = points["start"]
    points = df_data.iloc[points]
    cnt = 1
    for index, row in points.iterrows():
        txt = "BB " + str(cnt)
        plt.text(row["x"], row["y"], txt)
        cnt += 1


test_data_path = base_path + "test_data.csv"
df_data = pd.read_csv(test_data_path)
continuous_plot(df_data)
image_path = base_path + "trajectory.png"
plt.savefig(image_path, format = 'png' , dpi = 2000)
plt.clf()
print("Saved trajectory image...")


motif_points_BB = find_motif_points()
first_trajectory = df_data.iloc[motif_points_BB[0]:motif_points_BB[1]+1]
second_trajectory = df_data.iloc[motif_points_BB[2]:motif_points_BB[3]]
continuous_plot(first_trajectory)
continuous_plot(second_trajectory)
image_path = base_path + "BB_motif.png"
plt.savefig(image_path, format = 'png' , dpi = 2000)
plt.clf()
print("Saved motif image from Bounding Box..")


continuous_plot(df_data)
bounding_path = base_path + "BB_segments.csv"
bb_points(bounding_path)
image_path = base_path + "BoundingBoxes.png";
plt.savefig(image_path, format = 'png', dpi = 2000)
plt.clf()
print("Saved Bounding Boxes..")


#Base Solution motif Pair
motif_points_DP = find_motif_points_DP()
first_trajectory_DP = df_data.iloc[motif_points_DP[0]:motif_points_DP[1]+1]
second_trajectory_DP = df_data.iloc[motif_points_DP[2]:motif_points_DP[3]]
continuous_plot(first_trajectory_DP)
continuous_plot(second_trajectory_DP)
image_path = base_path + "DP_motif.png"
plt.savefig(image_path, format = 'png' , dpi = 2000)
print("Saved Motif image from Recursive DP solution..")
plt.clf()
plt.close()

