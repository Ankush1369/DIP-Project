#insert r infront of full file path name that we want to test it on

#getting only latitude and longitude
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
def pltcsv(raw_filename):    
    file = open(raw_filename,"r")
    # out_file = open("output.csv","w")
    R = 6371000
    out_trajectory = []
    lines = file.readlines()
    lines = lines[6:]
    
    for i in range(len(lines)):
        x = lines[i].split( ',')
        lat = float(x[0])
        lon = float(x[1])
        point = dict()
        lat, lon = np.deg2rad(lat), np.deg2rad(lon)
        point["x"] = R * np.cos(lat) * np.cos(lon)
        point["y"] = R * np.cos(lat) * np.sin(lon)
        out_trajectory.append(point)
        # out_file.write(x[0])
        # out_file.write(',')
        # out_file.write(x[1])
        # out_file.write('\n')

    df = pd.DataFrame(out_trajectory)
    df["x"] = df["x"] - df.iloc[0, 0]
    df["y"] = df["y"] - df.iloc[0, 1]
    df.to_csv(r"C:\Users\User\Desktop\project\DIP-Project\test_data\test_data_manik5.csv",index=False)
    # print(df["x"])
    # plt.plot(list(df["x"]), list(df["y"]))
    # plt.show()
    
    file.close()

pltcsv(r"C:\Users\User\Desktop\project\DIP-Project\datasets\002\Trajectory\20090223102533.plt")