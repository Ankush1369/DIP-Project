import matplotlib.pyplot as plt

def show_continuous_plot(df):
    plt.xlabel("Latitude")
    plt.ylabel("Longitude") 
    plt.plot(df["x"], df["y"])

def arrow_plot(df1, df2):
    plt.quiver(df1["x"], df1["y"], df2["x"], df2["y"])
    plt.xlabel("Latitude")
    plt.ylabel("Longitude")