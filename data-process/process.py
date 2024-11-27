import pandas as pd
import os
import re
import numpy as np
import matplotlib.pyplot as plt

def main():
    lst = [['pitch', 'roll', 'yaw']]
    with open('F:/pid-proj/pid-proj/data-process/example.txt', 'r', encoding='utf-8') as file:
        for line in file:
            number = re.findall(r"[-+]?\d*\.?\d+", line) # get[pitch, yaw, roll]
            # print(number)
            lst.append([float(num) for num in number]) 
    df = pd.DataFrame(lst[1:], columns=lst[0])
    df.to_excel('output.xlsx', index=False)

    pitch = df['pitch'].to_numpy()
    roll = df['roll'].to_numpy()
    yaw = df['yaw'].to_numpy()
    print(pitch)
    fig, ax = plt.subplots(1, 3)
    ax[0].plot(pitch)
    ax[0].plot(np.zeros_like(pitch))
    ax[1].plot(roll)
    ax[1].plot(np.zeros_like(pitch))
    ax[2].plot(yaw)
    ax[2].plot(np.zeros_like(pitch))
    plt.show()

if __name__ == '__main__':
    main()
