import customtkinter as ctk
import serial
import threading


SERIAL_PORT = 'COM3' 
BAUD_RATE = 9600

try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
except Exception as e:
    print(f"No se pudo conectar al puerto serial: {e}")
    ser = None

led_states = [0, 0, 0, 0]  # 0 = OFF, 1 = ON

def read_serial():
    while True:
        if ser and ser.in_waiting > 0:
            data = ser.readline().decode('utf-8').strip()
            process_data(data)

def process_data(data):
    try:
        parts = data.split(',')
        button_state = int(parts[0].split(':')[1])  
        adc_value = int(parts[1].split(':')[1])  

        button_indicator.configure(fg_color="green" if button_state else "gray")
        button_indicator.configure(text="ON" if button_state else "OFF")

        adc_gauge.set(adc_value / 1023 )
    except Exception as e:
        print(f"Error procesando datos: {e}")

def toggle_led(led_index):
    led_states[led_index] = 1 - led_states[led_index]
    state_text = "ON" if led_states[led_index] else "OFF"
    led_buttons[led_index].configure(text=f"LED {led_index + 1}: {state_text}")
    led_buttons[led_index].configure(fg_color="green" if led_states[led_index] else "gray")

    char = chr(97 + led_index)  
    send_char(char)

def send_char(char):
    if ser:
        ser.write(f"{char}\n".encode('utf-8'))

ctk.set_appearance_mode("Dark")
ctk.set_default_color_theme("blue")

root = ctk.CTk()
root.title("PIC Dashboard")
root.geometry("400x500")

button_indicator = ctk.CTkLabel(root, text="OFF", font=("Arial", 16), fg_color="gray", width=120, height=50, corner_radius=25)
button_indicator.pack(pady=20)

adc_gauge_label = ctk.CTkLabel(root, text="ADC Value", font=("Arial", 16))
adc_gauge_label.pack(pady=10)

adc_gauge = ctk.CTkProgressBar(root, orientation="horizontal", width=300, height=20)
adc_gauge.pack(pady=10)
adc_gauge.set(0)

led_frame = ctk.CTkFrame(root)
led_frame.pack(pady=20)

led_buttons = []
for i in range(4):
    btn = ctk.CTkButton(
        led_frame,
        text=f"LED {i + 1}: OFF",
        fg_color="gray",
        command=lambda i=i: toggle_led(i),
        width=120,
    )
    btn.grid(row=0, column=i, padx=10)
    led_buttons.append(btn)

if ser:
    thread = threading.Thread(target=read_serial, daemon=True)
    thread.start()

root.mainloop()

if ser:
    ser.close()
