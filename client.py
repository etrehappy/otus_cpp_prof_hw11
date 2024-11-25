import socket

def send_command(sock, command):
    # Отправляем команду
    sock.sendall(command.encode())
    # Получаем ответ
    response = sock.recv(1024)
    # Выводим ответ в консоль
    print(response.decode())

# Список команд для отправки
commands = [
    "INSERT A 0 lean\n",
    "INSERT A 0 understand\n",
    "INSERT A 1 sweater\n",
    "INSERT A 2 frank\n",
    "INSERT A 3 violation\n",
    "INSERT A 4 quality\n",
    "INSERT A 5 precision\n",
    "INSERT B 3 proposal\n",
    "INSERT B 4 example\n",
    "INSERT B 5 lake\n",
    "INSERT B 6 flour\n",
    "INSERT B 7 wonder\n",
    "INSERT B 8 selection\n",
    "INTERSECTION\n",
    "SYMMETRIC_DIFFERENCE\n"
]

# Создаем TCP/IP сокет
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    # Подключаемся к серверу, работающему на локальном хосте и указанном порту
    s.connect(('localhost', 65500))

    # Отправляем команды на сервер
    for command in commands:
        send_command(s, command)
