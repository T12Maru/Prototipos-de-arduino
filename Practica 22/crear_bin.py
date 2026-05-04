data = bytes([
    0b10000001,  # 192
    0b01000010,  # 24
    0b00100100   # 3
])

with open("b.bin", "wb") as f:
    f.write(data)

print("Archivo a.bin creado correctamente")