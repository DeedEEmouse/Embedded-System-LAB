import socket
import json
from collections import deque
import time

import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server.bind(("0.0.0.0", 8002))
server.listen(1)

print("Waiting for STM32...")

conn, addr = server.accept()
conn.setblocking(False)

print("Connected:", addr)

x_values = deque(maxlen=100)
y_values = deque(maxlen=100)
z_values = deque(maxlen=100)

recv_buffer = b""
connection_open = True

fig, ax = plt.subplots()

line_x, = ax.plot([], [], label="X")
line_y, = ax.plot([], [], label="Y")
line_z, = ax.plot([], [], label="Z")

ax.set_xlim(0, 99)
ax.set_ylim(-2000, 2000)

ax.set_title("3D Accelerometer")
ax.set_xlabel("Sample")
ax.set_ylabel("Acceleration")
ax.legend()

motion_text = ax.text(
    0.5,
    0.95,
    "",
    transform=ax.transAxes,
    ha="center",
    va="top"
)

motion_time = None

def update(frame):
    global recv_buffer
    global connection_open
    global motion_time

    if connection_open:
        while True:
            try:
                chunk = conn.recv(1024)

                if not chunk:
                    connection_open = False
                    print("STM32 disconnected")
                    break

                recv_buffer += chunk

            except BlockingIOError:
                break

        while b"\n" in recv_buffer:
            line, recv_buffer = recv_buffer.split(b"\n", 1)
            line = line.strip()

            if not line:
                continue

            try:
                data = json.loads(line.decode())

                if data["type"] == "data":
                    x_values.append(data["x"])
                    y_values.append(data["y"])
                    z_values.append(data["z"])

                elif data["type"] == "motion":
                    print("Significant Motion!")
                    motion_time = time.perf_counter()
                    motion_text.set_text("Significant Motion Detected!")

            except (json.JSONDecodeError, UnicodeDecodeError, KeyError) as e:
                print("Invalid data:", line)
                print(e)

    if motion_time is not None:
        if time.perf_counter() - motion_time >= 1:
            motion_text.set_text("")
            motion_time = None

    t = range(len(x_values))

    line_x.set_data(t, list(x_values))
    line_y.set_data(t, list(y_values))
    line_z.set_data(t, list(z_values))

    return line_x, line_y, line_z, motion_text

ani = FuncAnimation(
    fig,
    update,
    interval=100,
    cache_frame_data=False
)

plt.show()

conn.close()
server.close()