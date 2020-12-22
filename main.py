import tkinter as tk
import json
import os
import sys
import subprocess

data = []


def close(entries, root):
    global data
    for key, value in entries.items():
        if data[key]["type"] == "int":
            data[key]["value"] = int(value.get())
        if data[key]["type"] == "float":
            data[key]["value"] = float(value.get())
        if data[key]["type"] == "double":
            data[key]["value"] = float(value.get())
        if data[key]["type"] == "bool":
            data[key]["value"] = bool(value.get())
    with open("settings.json", "w") as output:
        output.write(json.dumps(data, indent=4))

    with open('bench/values.csv', 'w') as infile:
        proc = subprocess.Popen(['./bin/a.out'],
                                stdin=sys.stdin, stdout=infile, stderr=sys.stderr)
        root.withdraw()
        proc.wait()

    exit(0)


def makeform(root):
    global data
    entries = {}
    for key, field in data.items():
        if field["display"]:
            if field["type"] == "bool":
                row = tk.Frame(root)
                var = tk.BooleanVar(value=field["value"])
                val = tk.Checkbutton(row, text=field["name"],
                                     variable=var)
                val.pack()
                if field["value"]:
                    val.select()
                row.pack(side=tk.TOP,
                         fill=tk.X,
                         padx=5,
                         pady=5)
                val.pack(side=tk.LEFT)
                entries[key] = var
            else:
                row = tk.Frame(root)
                lab = tk.Label(
                    row, width=22, text=field["name"]+": ", anchor='w')
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
