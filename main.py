import tkinter as tk
import json
import os
import subprocess

data = []


def close(entries, root):
    global data
    for key, value in entries.items():
        if(data[key]["type"] == "int"):
            data[key]["value"] = int(value.get())
        if(data[key]["type"] == "float"):
            data[key]["value"] = float(value.get())

    with open("settings.json", "w") as output:
        output.write(json.dumps(data, indent=4))

    proc = subprocess.Popen(["./bin/a.out"])
    root.withdraw()
    proc.wait()

    exit(0)


def makeform(root):
    global data
    entries = {}
    for key, field in data.items():
        if(field["display"]):
            row = tk.Frame(root)
            lab = tk.Label(row, width=22, text=field["name"]+": ", anchor='w')
            ent = tk.Entry(row)
            ent.insert(0, ""+str(field["value"]))
            row.pack(side=tk.TOP,
                     fill=tk.X,
                     padx=5,
                     pady=5)
            lab.pack(side=tk.LEFT)
            ent.pack(side=tk.RIGHT,
                     expand=tk.YES,
                     fill=tk.X)
            entries[key] = ent
    return entries


def open_json():
    global data
    with open("settings.json") as input:
        data = json.load(input)


if __name__ == '__main__':
    root = tk.Tk()
    root.title("Un mare di altruismo")
    open_json()
    ents = makeform(root)
    b3 = tk.Button(root, text='Play',
                   command=(lambda e=ents: close(e, root)))
    b3.pack(side=tk.RIGHT, padx=5, pady=5)
    root.mainloop()