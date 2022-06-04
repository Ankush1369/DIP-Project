import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import plotting_functions as pf

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

test_file_number = input("Enter test file number: ")
#Just change this base path for input data
base_test_data_path = '/Users/ankushgarg/Desktop/DIP-Project/test_data'
test_data_path = base_test_data_path + "/test_data" + test_file_number + ".csv"

#Just change this output path to your base path
base_output_path = '/Users/ankushgarg/Desktop/DIP-Project/testing_code'
output_path = base_output_path + "/test_" + test_file_number

df_data = pd.read_csv(test_data_path)
motif_points = find_motif_points(output_path)

first_trajectory = df_data.iloc[motif_points[0]:motif_points[1]]
second_trajectory = df_data.iloc[motif_points[2]:motif_points[3]]

continuous_plot(df_data)
continuous_plot(first_trajectory)
continuous_plot(second_trajectory)


plt.show()
