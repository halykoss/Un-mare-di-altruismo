import sys
import datetime as dt
import matplotlib
import matplotlib.pyplot as plt
import matplotlib.animation as animation
# Una figura con due sottografici
fig, axs = plt.subplots(2, tight_layout=True)
# Lista delle epoche
xs = []
# Lista numero dei pesci
ys = []
# Lista numero di cibo
ys2 = []
# Iteratore delle epoche
iter = 0

print("sono italo")
# Animazione dei grafici
def animate(i, xs, ys, ys2, list_of_fish):
    global iter
    # Leggo numero di pesci e di cibo dall'IPC
    curr_fish = sys.stdin.buffer.read(4)
    curr_food = sys.stdin.buffer.read(4)
    # Se il buffer viene chiuso allora fine programma
    if curr_fish == b'':
        exit(0)
        # Converto da bytes a interi e salvo
    curr_fish = int.from_bytes(curr_fish, "little")
    curr_food = int.from_bytes(curr_food, "little")
    ys.append(curr_fish)
    ys2.append(curr_food)
    # Aumento l'iteratore delle epoche
    iter += 1
    xs.append(iter)
    # Lista contenente il grado di altruismo dei pesci e la riempio
    list_of_fish = []
    for _ in range(curr_fish):
        list_of_fish.append(int.from_bytes(sys.stdin.buffer.read(4), "little"))
    # Limito a 1000 le epoche
    xs = xs[-1000:]
    ys = ys[-1000:]
    ys2 = ys2[-1000:]

    # Disegno il grafico con cibo e pesci
    axs[0].clear()
    axs[0].plot(xs, ys2, label="Food", color="green")
    axs[0].plot(xs, ys, label="Fish",color="maroon")
    axs[0].set_title("Fish and Food over epochs")
    # Disegno il grafico con l'altruismo
    axs[1].clear()
    num_of_bins = 20
    _, _, patches = axs[1].hist(list_of_fish, bins=num_of_bins, facecolor='blue',
                                alpha=1, edgecolor='black')
    # Imposto colore delle barre
    for idx in range(num_of_bins):
        patches[idx].set_fc('#%02x%02x%02x' % (int(((idx * (100/num_of_bins) / 100)) * 255), 0, 0))

    axs[1].set_title("Altruism degree distribution")


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


# Imposto animazione e posiziono l'immagine
ani = animation.FuncAnimation(
    fig, animate, fargs=(xs, ys, ys2, []), interval=(sys.argv[1]))
plt.title("Prova")
fig.canvas.set_window_title('Stats')
move_figure(fig, 950, 200)
plt.show()
