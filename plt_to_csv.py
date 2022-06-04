#insert r infront of full file path name that we want to test it on

#getting only latitude and longitude
def pltcsv(raw_filename):    
    file = open(raw_filename,"r")
    out_file = open("output.csv","w")
    lines = file.readlines()

    lines = lines[6:]

    for i in range(len(lines)):
        x = lines[i].split(',')
        out_file.write(x[0])
        out_file.write(',')
        out_file.write(x[1])
        out_file.write('\n')


    out_file.close()
    file.close()

pltcsv(r"C:\Users\User\Desktop\project\DIP-Project\datasets\000\Trajectory\20081023025304.plt")