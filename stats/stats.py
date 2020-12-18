import sys
import datetime as dt
import matplotlib
import sys
import signal
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
from threading import Thread, Lock
import time
import os
import struct
# Una figura con due sottografici
fig, axs = plt.subplots(3, 4)
# Lista delle epoche
xs = []
# Lista numero dei pesci
ys = []
# Lista numero di cibo
ys2 = []
# Iteratore delle epoche
iter = 0
# Animazione dei grafici

matrixVal = [[[] for i in range(3)] for j in range(3)]
matrixValFish = [[[] for i in range(3)] for j in range(3)]
matrixValMedium = [[[] for i in range(3)] for j in range(3)]
matrixValSpeed = [[[] for i in range(3)] for j in range(3)]
stop = False
stopThread = False
mutex = Lock()


def val_from_ch():
    global stop
    line = sys.stdin.buffer.readline()
    if line == b'':
        stop = True
        return None
    return list(map(lambda a: a.rstrip("\n"), line.decode("UTF-8").split(";")))


def animate(i, xs, ys, ys2, list_of_fish, mutex):
    global iter
    global stop
    global matrixVal
    mutex.acquire()
    for k in range(0, 3):
        for ky in range(0, 3):
            if len(matrixVal[k][ky]) > 0:
                matrixVal[k][ky] = matrixVal[k][ky][-600:]
                valX, valY, food = zip(*(matrixVal[k][ky]))
                axs[k][ky].clear()
                axs[k][ky].title.set_text(str(3 * k + ky + 1) + ' Field')
                axs[k][ky].plot(valX, valY, label="Fish", color="maroon")
                axs[k][ky].plot(valX, food, label="Food", color="green")
                axs[k][ky].set_xlabel('xlabel' + str(3 * k + ky))
                axs[k][ky].set_ylabel('ylabel' + str(3 * k + ky))
    extendedX = []
    extendedY = []
    for k in range(0, 3):
        for ky in range(0, 3):
            if len(matrixValMedium[k][ky]) <= 0:
                valX = []
                valY = []
            else:
                matrixValMedium[k][ky] = matrixValMedium[k][ky][-100:]
                valX, valY = zip(*(matrixValMedium[k][ky]))
            extendedX.extend(valX)
            extendedY.extend(valY)
    axs[0][3].clear()
    axs[0][3].scatter(extendedX, extendedY, alpha=0.3,
                      marker='o', facecolor='maroon')
    axs[0][3].set_xlabel('xlabel 0')
    axs[0][3].set_ylabel('ylabel 3')
    list_of_fish = []
    for k in range(0, 3):
        for ky in range(0, 3):
            list_of_fish.extend(matrixValFish[k][ky])
    extendedX = []
    extendedY = []
    for k in range(0, 3):
        for ky in range(0, 3):
            if len(matrixValSpeed[k][ky]) <= 0:
                valX = []
                valY = []
            else:
                matrixValSpeed[k][ky] = matrixValSpeed[k][ky][-100:]
                valX, valY = zip(*(matrixValSpeed[k][ky]))
            extendedX.extend(valX)
            extendedY.extend(valY)
    axs[1][3].clear()
    axs[1][3].scatter(extendedX, extendedY, alpha=0.3,
                      marker='o', facecolor='maroon')
    axs[1][3].set_xlabel('xlabel 1')
    axs[1][3].set_ylabel('ylabel 3')
    list_of_fish = []
    for k in range(0, 3):
        for ky in range(0, 3):
            list_of_fish.extend(matrixValFish[k][ky])
    axs[2][3].clear()
    _, _, patches = axs[2][3].hist(list_of_fish, bins=range(0, 100, 5), facecolor='blue',
                                   alpha=1, edgecolor='black')
    num_of_bins = len(patches)
    for idx in range(num_of_bins):
        patches[idx].set_fc('#%02x%02x%02x' % (
            int(((idx * (100/num_of_bins) / 100)) * 255), 0, 0))

    axs[2][3].set_xlabel('xlabel 2')
    axs[2][3].set_ylabel('ylabel 3')
    
    mutex.release()


def processData(stopThread, mutex):
    global stop
    global matrixVal
    while not stopThread:
        mutex.acquire()
        try:
            val = val_from_ch()
            i = int(val[0])
            j = int(val[1])
            curr_fish = int(val[2])
            curr_food = int(val[3])
            epoch = int(val[4])
            list_of_fish = []
            sum_kindness = 0
            sum_speed = 0
            sum_age = 0
            if curr_fish != 0:
                for _ in range(curr_fish):
                    from_out = val_from_ch()
                    val = int(from_out[0])
                    speed = int(from_out[1])
                    age = int(from_out[3])
                    sum_speed += speed
                    sum_kindness += val
                    sum_age += age
                    list_of_fish.append(val)
                    matrixValFish[i][j] = list_of_fish
                (matrixValSpeed[i][j]).extend(
                    [(curr_fish, curr_food)])
                (matrixValMedium[i][j]).extend(
                    [(sum_kindness / curr_fish / curr_food, sum_age / curr_fish / curr_food)])
                (matrixVal[i][j]).extend([(epoch, curr_fish, curr_food)])
        except IndexError:
            pass
        except Exception as e:
            print(e)
            stopThread = True
        finally:
            mutex.release()
            time.sleep(0.000001)


# Muovo la finestra


def move_figure(f, x, y):
    """Move figure's upper left corner to pixel (x, y)"""
    backend = matplotlib.get_backend()
    if backend == 'TkAgg':
        f.canvas.manager.window.wm_geometry("+%d+%d" % (x, y))
    elif backend == 'WXAgg':
        f.canvas.manager.window.SetPosition((x, y))
    else:
        # This works for QT and GTK
        # You can also use window.setGeometry
        f.canvas.manager.window.move(x, y)


t = Thread(target=processData, args=(stopThread, mutex))
t.start()


# Imposto animazione e posiziono l'immagine
ani = animation.FuncAnimation(fig, animate, fargs=(
    xs, ys, ys2, [], mutex), interval=(1000))
plt.title("Live Data")
fig.canvas.set_window_title('Stats')
move_figure(fig, 950, 200)

plt.show()
